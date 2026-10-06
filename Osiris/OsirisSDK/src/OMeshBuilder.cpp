#include <map>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OResourceManager.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OGeometry.h"
#include "OsirisSDK/OMaterialSet.h"
#include "OsirisSDK/OMeshRawData.h"
#include "OsirisSDK/OMeshBuilder.h"
#include "OsirisSDK/OVertexBuffer.h"

namespace
{
struct RenderVertex
{
    OMeshRawData::Position position{};
    OMeshRawData::Normal normal{};
    OMeshRawData::TexCoord texCoord{};
};
}

OMeshBuilder::OMeshBuilder(GeometryManager& aGeometryManager,
                           MaterialManager& aMaterialManager,
                           MaterialSetManager& aMaterialSetManager,
                           OVertexBufferDescriptor& aVertexDescriptor)
    : _geometryManager(aGeometryManager),
      _materialManager(aMaterialManager),
      _materialSetManager(aMaterialSetManager),
      _vertexDescriptor(aVertexDescriptor)
{
}

OMeshBuilder::Result OMeshBuilder::build(const OString& aMeshName,
                                         const OString& aMaterialFileStem,
                                         const OMeshRawData& aRawData)
{
    using MaterialIndexType = OGeometry::MaterialIndexBuffer::ItemType;

    const size_t maxMaterialCount =
        static_cast<size_t>(std::numeric_limits<MaterialIndexType>::max()) + 1;
    if (aRawData.materialCount() > maxMaterialCount) {
        throw OEx("Mesh has more materials than its material index buffer can represent.");
    }

    size_t triangleCount = 0;
    const size_t maxTriangleCount = std::numeric_limits<uint32_t>::max();
    for (uint32_t faceIndex = 0; faceIndex < aRawData.faceCount(); ++faceIndex) {
        const size_t cornerCount = aRawData.face(faceIndex).corners.size();
        if (cornerCount < 3) throw OEx("Mesh face must have at least three corners.");
        const size_t faceTriangleCount = cornerCount - 2;
        if (faceTriangleCount > maxTriangleCount - triangleCount) {
            throw OEx("Mesh has too many triangles.");
        }
        triangleCount += faceTriangleCount;
    }

    auto materialSetName = OString::Fmt("%s:%s", aMeshName.cString(), aMaterialFileStem.cString());

    OMaterialSet materialSet(std::move(materialSetName));
    for (uint32_t index = 0; index < aRawData.materialCount(); ++index) {
        const OMaterial& sourceMaterial = aRawData.material(index);
        auto material = _materialManager.fetch(sourceMaterial.name());
        if (material.isNull()) {
            _materialManager.add(OMaterial(sourceMaterial));
            material = _materialManager.fetch(sourceMaterial.name());
        }
        if (material.isNull()) throw OEx("Unable to register mesh material.");
        materialSet.add(std::move(material));
    }

    auto& materials = _materialSetManager.add(std::move(materialSet));

    std::map<OMeshRawData::Index, uint32_t> vertexMap;
    std::vector<RenderVertex> vertices;
    if (triangleCount > vertices.max_size() / 3) throw OEx("Mesh has too many vertices.");
    vertices.reserve(triangleCount * 3);

    OIndexBuffer indexBuffer(static_cast<uint32_t>(triangleCount));
    OGeometry::MaterialIndexBuffer::Array materialIndices(triangleCount, true);
    size_t materialIndex = 0;

    auto getVertexIndex = [&](OMeshRawData::Index aSourceIndex) {
        if (!aRawData.hasNormals()) aSourceIndex.norm = 0;
        if (!aRawData.hasTexCoords()) aSourceIndex.tex = 0;
        const auto found = vertexMap.find(aSourceIndex);
        if (found != vertexMap.end()) return found->second;

        RenderVertex renderVertex;
        renderVertex.position = aRawData.position(aSourceIndex.vert);
        if (aRawData.hasNormals()) renderVertex.normal = aRawData.normal(aSourceIndex.norm);
        if (aRawData.hasTexCoords()) renderVertex.texCoord = aRawData.texCoord(aSourceIndex.tex);
        if (vertices.size() >= std::numeric_limits<uint32_t>::max()) {
            throw OEx("Mesh has too many vertices.");
        }
        const uint32_t index = static_cast<uint32_t>(vertices.size());
        vertices.push_back(renderVertex);
        vertexMap.emplace(aSourceIndex, index);
        return index;
    };

    for (uint32_t faceIndex = 0; faceIndex < aRawData.faceCount(); ++faceIndex) {
        const auto& polygon = aRawData.face(faceIndex);
        for (size_t corner = 1; corner + 1 < polygon.corners.size(); ++corner) {
            const OMeshRawData::Index triangle[3] = {
                polygon.corners[0], polygon.corners[corner], polygon.corners[corner + 1]
            };
            const uint32_t indices[3] = {
                getVertexIndex(triangle[0]),
                getVertexIndex(triangle[1]),
                getVertexIndex(triangle[2])
            };
            indexBuffer.addFace(indices[0], indices[1], indices[2]);
            materialIndices[materialIndex++] = static_cast<MaterialIndexType>(polygon.materialIndex);
        }
    }

    OVertexBuffer vertexBuffer(_vertexDescriptor, static_cast<uint32_t>(vertices.size()));
    for (uint32_t index = 0; index < vertices.size(); ++index) {
        uint8_t attribute = 0;
        vertexBuffer.setAttributeValue(attribute++, index, &vertices[index].position.x);
        if (aRawData.hasNormals()) vertexBuffer.setAttributeValue(attribute++, index, &vertices[index].normal.x);
        if (aRawData.hasTexCoords()) vertexBuffer.setAttributeValue(attribute++, index, &vertices[index].texCoord.u);
    }

    OGeometry geometry(OString(aMeshName), 
                       ORenderMode::IndexedTriangle, 
                       std::move(vertexBuffer), 
                       std::move(indexBuffer),
                       OGeometry::MaterialIndexBuffer(std::move(materialIndices)));
    auto& geometryResource = _geometryManager.add(std::move(geometry));

    return {.geometry = &geometryResource, .materials = &materials};
}
