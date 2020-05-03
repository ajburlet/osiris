#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGraphicsCommandEncoder.h"

class OGraphicsCommandQueue;
class OShaderProgram;
class OVertexBuffer;
class OIndexBuffer;
class OTexture;
class OShaderArgumentInstanceList;

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
	virtual void setFaceCullingOptions(OCullFace aFace, OCullFront aFront) = 0;

	/**
	 @brief Sets the shader program to be used in this rendering run.
	 */
	virtual void setShaderProgram(OShaderProgram* aShaderProgram) = 0;

	/**
	 @brief Sets the vertex buffer cotaining all the vertices to be drawn.
	 */
	virtual void setVertexBuffer(OVertexBuffer* aVertexBuffer) = 0;

	/**
	 @brief Sets the index buffer, containing the indexes that references vertices on the vertex buffer.
	 */
	virtual void setIndexBuffer(OIndexBuffer* aIndexBuffer) = 0;

	/**
	 @brief Sets the texture resource.
	 @param aTexture The pointer to the texture object.
	 @param aIndex Texture index in the GPU.
	 */
	virtual void setTexture(OTexture* aTexture, uint32_t aIndex=0) = 0;

	/**
	 @brief Sets the shader uniform argument list.
	 */
	virtual void setUniformArgumentList(OShaderArgumentInstanceList* aUniformArguments) = 0;

	/**
	 @brief Returns a pointer to a shader program object.
	 */
	virtual OShaderProgram* shaderProgram() = 0;

	/**
	 @brief Returns a pointer to the vertex buffer used by this encoder.
	 */
	virtual OVertexBuffer* vertexBuffer() = 0;

	/**
	 @brief Returns a pointer to the index buffer used by this encoder.
	 */
	virtual OIndexBuffer* indexBuffer() = 0;

	/**
	 @brief Returns a pointer to the texture object.
	 @param aIndex Texture index in the GPU.
	 */
	virtual OTexture* texture(uint32_t aIndex = 0) = 0;

	/**
	 @brief Returns a pointer to the uniform argument list. 
	 */
	virtual OShaderArgumentInstanceList* uniformArgumentList() = 0;
	
	 /**
	 @brief Clear textures that were previously set.
	 */
	virtual void clearTextures() = 0;

	/**
	 @brief Issues a draw command.
	 */
	virtual void draw(ORenderMode aRenderType) = 0;
};

inline OGraphicsRenderCommandEncoder::OGraphicsRenderCommandEncoder(OGraphicsCommandQueue * aCommandQueue) :
	OGraphicsCommandEncoder(Type::Render, aCommandQueue)
{
}

