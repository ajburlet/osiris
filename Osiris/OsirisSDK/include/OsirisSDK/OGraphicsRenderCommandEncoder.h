#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGraphicsCommandEncoder.h"

class OGraphicsCommandQueue;
class OShaderProgram;
class OVertexBuffer;
class OIndexBuffer;

/**
 @brief Resource command encoder for the graphics engine.
 */
class OAPI OGraphicsRenderCommandEncoder : public OGraphicsCommandEncoder
{
public:
	/**
	 @brief Class constructor.
	 @param aCommandQueue The queue on which the encoder writes the commands to.
	 */
	OGraphicsRenderCommandEncoder(OGraphicsCommandQueue* aCommandQueue);

	/**
	 @brief Class destructor.
	 */
	virtual ~OGraphicsRenderCommandEncoder() = default;

	/**
	 @brief Sets face culling options.
	 @param aFace Which face is considered by the rasterizer.
	 @param aFront How a front face is defined (i.e. when vertices are ordered CW or CCW).
	 */
	void setFaceCullingOptions(OCullFace aFace, OCullFront aFront);

	/**
	 @brief Sets the shader program to be used in this rendering run.
	 */
	void setShaderProgram(OShaderProgram* aShaderProgram);

	/**
	 @brief Sets the vertex buffer cotaining all the vertices to be drawn.
	 */
	void setVertexBuffer(OVertexBuffer* aVertexBuffer);

	/**
	 @brief Sets the index buffer, containing the indexes that references vertices on the vertex buffer.
	 */
	void setIndexBuffer(OIndexBuffer* aIndexBuffer);

	/**
	 @brief Returns a pointer to a shader program object.
	 */
	OShaderProgram* shaderProgram();

	/**
	 @brief Returns a pointer to the vertex buffer used by this encoder.
	 */
	OVertexBuffer* vertexBuffer();

	/**
	 @brief Returns a pointer to the index buffer used by this encoder.
	 */
	OIndexBuffer* indexBuffer();

	/**
	 @brief Issues a draw command.
	 */
	virtual void draw(ORenderType aRenderType) = 0;
	
protected:
	OGraphicsCommandQueue*	_commandQueue	= nullptr;
	OShaderProgram*		_shaderProgram	= nullptr;
	OVertexBuffer*		_vertexBuffer	= nullptr;
	OIndexBuffer*		_indexBuffer	= nullptr;
	OCullFace		_faceCulling	= OCullFace::Undefined;
	OCullFront		_faceFront	= OCullFront::Undefined;
};

inline OGraphicsRenderCommandEncoder::OGraphicsRenderCommandEncoder(OGraphicsCommandQueue * aCommandQueue) :
	OGraphicsCommandEncoder(aCommandQueue)
{
}

inline void OGraphicsRenderCommandEncoder::setFaceCullingOptions(OCullFace aFace, OCullFront aFront)
{
	_faceCulling = aFace;
	_faceFront = aFront;
}

inline void OGraphicsRenderCommandEncoder::setShaderProgram(OShaderProgram* aShaderProgram)
{
	_shaderProgram = aShaderProgram;
}

inline void OGraphicsRenderCommandEncoder::setVertexBuffer(OVertexBuffer* aVertexBuffer)
{
	_vertexBuffer = aVertexBuffer;
}

inline void OGraphicsRenderCommandEncoder::setIndexBuffer(OIndexBuffer* aIndexBuffer)
{
	_indexBuffer = aIndexBuffer;
}

inline OShaderProgram* OGraphicsRenderCommandEncoder::shaderProgram()
{
	return _shaderProgram;
}

inline OVertexBuffer* OGraphicsRenderCommandEncoder::vertexBuffer()
{
	return _vertexBuffer;
}

inline OIndexBuffer* OGraphicsRenderCommandEncoder::indexBuffer()
{
	return _indexBuffer;
}

