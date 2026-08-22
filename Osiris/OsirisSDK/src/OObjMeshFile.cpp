#include <charconv>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>

#include "OObjMeshFileTokenizer.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OObjMeshFile.h"
#include "OsirisSDK/OString.hpp"

struct OObjMeshFile::Impl
{
    using MaterialMap = OWavefrontMaterialFile::MaterialMap;

    MaterialMap materials;
};

namespace
{
using RawData = OMeshFile::RawData;
using Index = RawData::Index;

[[noreturn]] void parseError(const char* aMessage)
{
    throw OEx(aMessage);
}

bool parseIndex(std::string_view aToken, uint32_t& aIndex)
{
    int value = 0;
    auto result = std::from_chars(aToken.data(), aToken.data() + aToken.size(), value);
    if (result.ec != std::errc() || result.ptr != aToken.data() + aToken.size() || value <= 0) {
        return false;
    }

    aIndex = static_cast<uint32_t>(value - 1);
    return true;
}

Index parseFaceIndex(std::string_view aToken)
{
    Index result{};
    const size_t firstSlash = aToken.find('/');
    if (firstSlash == std::string_view::npos) {
        if (!parseIndex(aToken, result.vert)) parseError("Invalid vertex index");
        return result;
    }

    if (!parseIndex(aToken.substr(0, firstSlash), result.vert)) {
        parseError("Invalid vertex index");
    }

    const size_t secondSlash = aToken.find('/', firstSlash + 1);
    if (secondSlash == std::string_view::npos) {
        if (!parseIndex(aToken.substr(firstSlash + 1), result.tex)) {
            parseError("Invalid texture index");
        }
        return result;
    }

    if (secondSlash > firstSlash + 1 &&
        !parseIndex(aToken.substr(firstSlash + 1, secondSlash - firstSlash - 1), result.tex)) {
        parseError("Invalid texture index");
    }
    if (!parseIndex(aToken.substr(secondSlash + 1), result.norm)) {
        parseError("Invalid normal index");
    }
    return result;
}

void handleVertex(OObjMeshFileTokenizer& aTokenizer, RawData& aRawData)
{
    RawData::Position position{};
    if (!aTokenizer.nextFloat(position.x) || !aTokenizer.nextFloat(position.y) ||
        !aTokenizer.nextFloat(position.z)) {
        parseError("Vertex must have three numeric components");
    }
    position.w = 1.0f;
    aRawData.addPosition(position);
}

void handleTexCoord(OObjMeshFileTokenizer& aTokenizer, RawData& aRawData)
{
    RawData::TexCoord texCoord{};
    if (!aTokenizer.nextFloat(texCoord.u) || !aTokenizer.nextFloat(texCoord.v)) {
        parseError("Texture coordinate must have two numeric components");
    }
    aRawData.addTexCoordinate(texCoord);
}

void handleNormal(OObjMeshFileTokenizer& aTokenizer, RawData& aRawData)
{
    RawData::Normal normal{};
    if (!aTokenizer.nextFloat(normal.x) || !aTokenizer.nextFloat(normal.y) ||
        !aTokenizer.nextFloat(normal.z)) {
        parseError("Normal must have three numeric components");
    }
    aRawData.addNormal(normal);
}

void handleFace(OObjMeshFileTokenizer& aTokenizer, RawData& aRawData,
                std::vector<Index>& aFaceIndices)
{
    aFaceIndices.clear();
    while (aTokenizer.hasMore()) {
        const std::string_view token = aTokenizer.nextToken();
        if (!token.empty()) aFaceIndices.push_back(parseFaceIndex(token));
    }

    if (aFaceIndices.size() < 3) parseError("Face must have at least three vertices");
    for (size_t index = 1; index + 1 < aFaceIndices.size(); ++index) {
        aRawData.addFace({ aFaceIndices[0], aFaceIndices[index], aFaceIndices[index + 1] });
    }
}
}

OObjMeshFile::OObjMeshFile(const OString& aFilename)
    : OMeshFile(aFilename), _impl(new Impl)
{
}

OObjMeshFile::~OObjMeshFile()
{
    delete _impl;
}

void OObjMeshFile::loadMesh(const OString& aObjName, RawData& aRawData)
{
    open();
    aRawData.setPositionComponents(3);
    aRawData.setTextureComponents(2);
    _impl->materials.clear();

    constexpr uint32_t bufferSize = 4096;
    char lineBuffer[bufferSize];
    OObjMeshFileTokenizer tokenizer;
    std::vector<Index> faceIndices;
    bool targetObjectFound = aObjName.length() == 0;

    while (!eof()) {
        const uint32_t bytesRead = readLine(lineBuffer, bufferSize);
        if (bytesRead == 0) continue;

        tokenizer.setLine(std::string_view(lineBuffer, bytesRead));
        const std::string_view command = tokenizer.nextToken();
        if (command.empty() || command.front() == '#') continue;

        if (command == "o" || command == "g") {
            const std::string_view objectName = tokenizer.nextToken();
            targetObjectFound = aObjName.length() == 0 ||
                (objectName.size() == aObjName.length() &&
                 std::string_view(aObjName.cString(), aObjName.length()) == objectName);
            continue;
        }

        if (command == "mtllib") {
            const std::string_view materialFile = tokenizer.nextToken();
            if (materialFile.empty()) parseError("Expected material library filename");

            const auto basePath = std::filesystem::path(filename().cString()).parent_path();
            const auto materialFileFullPath = basePath / materialFile;
            OWavefrontMaterialFile materialFileParser(OString(materialFileFullPath.string().c_str()));
            materialFileParser.open();
            materialFileParser.loadMaterials(_impl->materials);
            materialFileParser.close();
            continue;
        }

        if (!targetObjectFound) continue;

        if (command == "usemtl") {
            const std::string_view materialName = tokenizer.nextToken();
            if (materialName.empty()) parseError("Expected material name");

            std::string materialNameString(materialName);
            OString name(materialNameString.c_str());
            auto material = _impl->materials.find(name);
            if (material == _impl->materials.end()) parseError("Material not found");
            aRawData.useMaterial(&material.value());
        } else if (command == "v") {
            handleVertex(tokenizer, aRawData);
        } else if (command == "vt") {
            handleTexCoord(tokenizer, aRawData);
        } else if (command == "vn") {
            handleNormal(tokenizer, aRawData);
        } else if (command == "f") {
            handleFace(tokenizer, aRawData, faceIndices);
        }
    }

    close();
}
