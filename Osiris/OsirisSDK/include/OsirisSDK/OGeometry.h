#pragma once

#include <utility>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OResource.h"

/**
 @brief Resource containing the coupled vertex and index buffers for a draw.
 */
class OAPI OGeometry : public OMemoryManagedObject<OGraphicsAllocators::Default>, 
                       public OResource
{
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
     * @param indexBuffer Indexbuffer if drawMode is ORenderMode::IndexedTriangles.
     */
    OGeometry(OString&& name, ORenderMode drawMode,
              OVertexBuffer&& vertexBuffer, OIndexBuffer&& indexBuffer = {});

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
     * @brief Vertex buffer accessor (const).
     */
    const OVertexBuffer& vertexBuffer() const { return _vertexBuffer; }

    /**
     * @brief Index buffer accessor.
     */
    OIndexBuffer& indexBuffer() { return _indexBuffer; }

    /**
     * @brief Index buffer accessor (const).
     */
    const OIndexBuffer& indexBuffer() const { return _indexBuffer; }

private:
    ORenderMode _drawMode = ORenderMode::Undefined;
    OVertexBuffer _vertexBuffer;
    OIndexBuffer _indexBuffer;
};

inline OGeometry::OGeometry(OString&& aName, ORenderMode aDrawMode,
                            OVertexBuffer&& aVertexBuffer, OIndexBuffer&& aIndexBuffer)
    : OResource(std::move(aName))
    , _drawMode(aDrawMode)
    , _vertexBuffer(std::move(aVertexBuffer))
    , _indexBuffer(std::move(aIndexBuffer))
{}

inline OGeometry::OGeometry(OGeometry&& aOther)
    : OResource(std::move(aOther))
    , _drawMode(aOther._drawMode)
    , _vertexBuffer(std::move(aOther)._vertexBuffer)
    , _indexBuffer(std::move(aOther)._indexBuffer)
{}