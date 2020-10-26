#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/ORefCountObject.hpp"

class OVertexBuffer;
class OIndexBuffer;

/**
 @brief Mesh geometry holder, contains render mode, vertex and index buffers.
 */
class OAPI OMeshGeometry : public ORefCountObject<>
{
public:
	/**
	 @brief Class constructor.
	 @param aDrawMode Mesh draw mode.
	 @param aVertexBuffer Vertex buffer.
	 @param aIndexBuffer Index buffer (if applicable).
	 */
	OMeshGeometry(ORenderMode aDrawMode, OVertexBuffer* aVertexBuffer, OIndexBuffer* aIndexBuffer=nullptr);

	/**
	 @brief Class destructor.
	 */
	virtual ~OMeshGeometry() = default;

	/**
	 @brief Returns the draw mode.
	 */
	ORenderMode drawMode() const;

	/**
	 @brief Returns the vertex buffer.
	 */
	OVertexBuffer* vertexBuffer() const;

	/**
	 @brief Returns the index buffer.
	 */
	OIndexBuffer* indexBuffer() const;

private:
	ORenderMode	_drawMode	= ORenderMode::Undefined;
	OVertexBuffer*	_vertexBuffer	= nullptr;
	OIndexBuffer*	_indexBuffer	= nullptr;
};

inline OMeshGeometry::OMeshGeometry(ORenderMode aDrawMode, OVertexBuffer* aVertexBuffer,
	OIndexBuffer* aIndexBuffer) :
	_drawMode(aDrawMode),
	_vertexBuffer(aVertexBuffer),
	_indexBuffer(aIndexBuffer)
{
}

inline ORenderMode OMeshGeometry::drawMode() const
{
	return _drawMode;
}

inline OVertexBuffer * OMeshGeometry::vertexBuffer() const
{
	return _vertexBuffer;
}

inline OIndexBuffer * OMeshGeometry::indexBuffer() const
{
	return _indexBuffer;
}
