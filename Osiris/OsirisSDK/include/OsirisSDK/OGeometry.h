#pragma once

#include <utility>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OShaderArrayBuffer.hpp"
#include "OsirisSDK/OResource.h"

/**
 @brief Resource containing the coupled vertex and index buffers for a draw.
 */
class OAPI OGeometry : public OMemoryManagedObject<OGraphicsAllocators::Default>, 
                       public OResource
{
public:
    /**
     * @brief Maps the material index for each triangle.
     */
    using MaterialIndexBuffer = OShaderArrayBuffer<std::uint32_t>;

public:
    /**
     * @copydoc OResource::OResource()
     */
    OGeometry() : OResource(OString()) {}

    /**
     * @brief Class constructor.
     * @param name Resource name.
     * @param drawMode Draw mode.
     * @param vertexBuffer Vertex buffer.
     * @param indexBuffer Index buffer if drawMode is ORenderMode::IndexedTriangles.
     * @param materialIndexBuffer Material index buffer.
     */
    OGeometry(OString&& name, ORenderMode drawMode,
              OVertexBuffer&& vertexBuffer, OIndexBuffer&& indexBuffer = {}, 
              MaterialIndexBuffer&& materialIndexBuffer={});

    /**
     * @brief Move constructor. 
     */
    OGeometry(OGeometry&& other);

    /**
     * @brief Move assignment operator.
     */
    OGeometry& operator=(OGeometry&&) = delete;

    /**
     * @brief Draw mode accessor.
     */
    ORenderMode drawMode() const { return _drawMode; }

    /**
     * @brief Vertex buffer accessor.
     */
    OVertexBuffer& vertexBuffer() { return _vertexBuffer; }

    /**
     * @brief Vertex buffer const accessor.
     */
    const OVertexBuffer& vertexBuffer() const { return _vertexBuffer; }

    /**
     * @brief Index buffer accessor.
     */
    OIndexBuffer& indexBuffer() { return _indexBuffer; }

    /**
     * @brief Index buffer const accessor.
     */
    const OIndexBuffer& indexBuffer() const { return _indexBuffer; }

    /**
     * @brief Material index buffer accessor.
     */
    MaterialIndexBuffer& materialIndexBuffer() { return _materialIndexBuffer; }

    /**
     * @brief MAterial index buffer const accessor.
     */
    const MaterialIndexBuffer& materialIndexBuffer() const { return _materialIndexBuffer; }

private:
    ORenderMode _drawMode = ORenderMode::Undefined;
    OVertexBuffer _vertexBuffer;
    OIndexBuffer _indexBuffer;
    MaterialIndexBuffer _materialIndexBuffer;
};

inline OGeometry::OGeometry(OString&& aName, ORenderMode aDrawMode,
                            OVertexBuffer&& aVertexBuffer, 
                            OIndexBuffer&& aIndexBuffer,
                            OGeometry::MaterialIndexBuffer&& aMaterialIndexBuffer)
    : OResource(std::move(aName))
    , _drawMode(aDrawMode)
    , _vertexBuffer(std::move(aVertexBuffer))
    , _indexBuffer(std::move(aIndexBuffer))
    , _materialIndexBuffer(std::move(aMaterialIndexBuffer))
{}

inline OGeometry::OGeometry(OGeometry&& aOther)
    : OResource(std::move(aOther))
    , _drawMode(aOther._drawMode)
    , _vertexBuffer(std::move(aOther)._vertexBuffer)
    , _indexBuffer(std::move(aOther)._indexBuffer)
    , _materialIndexBuffer(std::move(aOther)._materialIndexBuffer)
{}