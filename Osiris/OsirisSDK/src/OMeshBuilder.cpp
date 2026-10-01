#include <map>
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
struct RenderVertexKey
{
    OMeshRawData::Index source;
    uint32_t materialIndex = 0;

    bool operator<(const RenderVertexKey& aOther) const
    {
        if (source < aOther.source) return true;
        if (aOther.source < source) return false;
        return materialIndex < aOther.materialIndex;
    }
};

struct SourceVertexKey
{
    OMeshRawData::Index source;

    bool operator<(const SourceVertexKey& aOther) const
    {
        return source < aOther.source;
    }
};

struct RenderVertex
{
    OMeshRawData::Position position{};
    OMeshRawData::Normal normal{};
    OMeshRawData::TexCoord texCoord{};
    uint32_t materialIndex = 0;
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
                                         const OMeshRawData& aRawData,
                                         const Options& aOptions)
{
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

    std::map<RenderVertexKey, uint32_t> materialVertexMap;
    std::map<SourceVertexKey, uint32_t> sourceVertexMap;
    std::vector<RenderVertex> vertices;
    OIndexBuffer indexBuffer;

    auto addVertex = [&](const OMeshRawData::Index& aSourceIndex, uint32_t aMaterialIndex) {
        RenderVertex renderVertex;
        renderVertex.position = aRawData.position(aSourceIndex.vert);
        if (aRawData.hasNormals()) renderVertex.normal = aRawData.normal(aSourceIndex.norm);
        if (aRawData.hasTexCoords()) renderVertex.texCoord = aRawData.texCoord(aSourceIndex.tex);
        renderVertex.materialIndex = aMaterialIndex;
        const uint32_t index = static_cast<uint32_t>(vertices.size());
        vertices.push_back(renderVertex);
        materialVertexMap.emplace(RenderVertexKey{aSourceIndex, aMaterialIndex}, index);
        return index;
    };

    for (uint32_t faceIndex = 0; faceIndex < aRawData.faceCount(); ++faceIndex) {
        const auto& polygon = aRawData.face(faceIndex);
        if (polygon.corners.size() < 3) throw OEx("Mesh face must have at least three corners.");
        for (size_t corner = 1; corner + 1 < polygon.corners.size(); ++corner) {
            const OMeshRawData::Index triangle[3] = {
                polygon.corners[0], polygon.corners[corner], polygon.corners[corner + 1]
            };
            uint32_t indices[3]{};
            if (aOptions.vertexMaterialMode == Options::VertexMaterialMode::Duplicated) {
                for (uint32_t vertex = 0; vertex < 3; ++vertex) {
                    const RenderVertexKey key{triangle[vertex], polygon.materialIndex};
                    auto found = materialVertexMap.find(key);
                    indices[vertex] = found == materialVertexMap.end()
                        ? addVertex(triangle[vertex], polygon.materialIndex)
                        : found->second;
                }
            } else {
                uint32_t provokingCorner = 0;
                bool foundCompatibleCorner = false;
                for (uint32_t vertex = 0; vertex < 3; ++vertex) {
                    const auto found = sourceVertexMap.find(SourceVertexKey{triangle[vertex]});
                    if (found != sourceVertexMap.end() &&
                        vertices[found->second].materialIndex == polygon.materialIndex) {
                        provokingCorner = vertex;
                        foundCompatibleCorner = true;
                        break;
                    }
                }
                if (!foundCompatibleCorner) {
                    for (uint32_t vertex = 0; vertex < 3; ++vertex) {
                        if (sourceVertexMap.find(SourceVertexKey{triangle[vertex]}) == sourceVertexMap.end()) {
                            provokingCorner = vertex;
                            foundCompatibleCorner = true;
                            break;
                        }
                    }
                }

                for (uint32_t vertex = 0; vertex < 3; ++vertex) {
                    const uint32_t corner = (provokingCorner + vertex + 1) % 3;
                    const SourceVertexKey sourceKey{triangle[corner]};
                    auto found = sourceVertexMap.find(sourceKey);
                    if (corner == provokingCorner) {
                        if (found != sourceVertexMap.end() &&
                            vertices[found->second].materialIndex == polygon.materialIndex) {
                            indices[vertex] = found->second;
                        } else {
                            const RenderVertexKey materialKey{triangle[corner], polygon.materialIndex};
                            auto materialFound = materialVertexMap.find(materialKey);
                            indices[vertex] = materialFound == materialVertexMap.end()
                                ? addVertex(triangle[corner], polygon.materialIndex)
                                : materialFound->second;
                            if (found == sourceVertexMap.end()) sourceVertexMap.emplace(sourceKey, indices[vertex]);
                        }
                    } else if (found == sourceVertexMap.end()) {
                        indices[vertex] = addVertex(triangle[corner], polygon.materialIndex);
                        sourceVertexMap.emplace(sourceKey, indices[vertex]);
                    } else {
                        indices[vertex] = found->second;
                    }
                }
            }
            indexBuffer.addFace(indices[0], indices[1], indices[2]);
        }
    }

    OVertexBuffer vertexBuffer(_vertexDescriptor, static_cast<uint32_t>(vertices.size()));
    for (uint32_t index = 0; index < vertices.size(); ++index) {
        uint8_t attribute = 0;
        vertexBuffer.setAttributeValue(attribute++, index, &vertices[index].position.x);
        if (aRawData.hasNormals()) vertexBuffer.setAttributeValue(attribute++, index, &vertices[index].normal.x);
        if (aRawData.hasTexCoords()) vertexBuffer.setAttributeValue(attribute++, index, &vertices[index].texCoord.u);
        vertexBuffer.setAttributeValue(attribute, index, &vertices[index].materialIndex);
    }

    OGeometry geometry(OString(aMeshName), 
                       ORenderMode::IndexedTriangle, 
                       std::move(vertexBuffer), 
                       std::move(indexBuffer));
    auto& geometryResource = _geometryManager.add(std::move(geometry));

    return {.geometry = &geometryResource, .materials = &materials};
}
