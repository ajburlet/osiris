#pragma once

#include "OsirisSDK/OResource.h"
#include "OsirisSDK/OGeometry.h"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OMaterialSet.h"
#include "OsirisSDK/OStringDefs.h"
#include "OsirisSDK/OResourceManager.h"

class OMeshRawData;

/**
 @brief Converts parsed mesh data into manager-owned render resources.
 */
class OAPI OMeshBuilder
{
public:
    using GeometryManager = OResourceManager<OGeometry>;
    using MaterialManager = OResourceManager<OMaterial>;
    using MaterialSetManager = OResourceManager<OMaterialSet>;

    struct Options {
        enum class VertexMaterialMode {
            Duplicated,
            ProvokingVertex
        };

        VertexMaterialMode vertexMaterialMode = VertexMaterialMode::Duplicated;
    };

    struct Result {
        GeometryManager::ResourcePtr geometry;
        MaterialSetManager::ResourcePtr materials;
    };

    OMeshBuilder(GeometryManager& aGeometryManager,
                 MaterialManager& aMaterialManager,
                 MaterialSetManager& aMaterialSetManager,
                 OVertexBufferDescriptor& aVertexDescriptor);

    Result build(const OString& aMeshName, const OString& aMaterialFileStem,
                 const OMeshRawData& aRawData, const Options& aOptions = {});

private:
    GeometryManager& _geometryManager;
    MaterialManager& _materialManager;
    MaterialSetManager& _materialSetManager;
    OVertexBufferDescriptor& _vertexDescriptor;
};
