#pragma once

#include "OsirisSDK/OOpenGLCommandEncoder.h"
#include "OsirisSDK/OGraphicsRenderCommandEncoder.h"
#include "OsirisSDK/defs.h"

/**
 @brief OpenGL render command encoder implementation.
 */
class OAPI OOpenGLRenderCommandEncoder : public OGraphicsRenderCommandEncoder, protected OOpenGLCommandEncoder
{
public:
	/**
	 @brief Class constructor.
	 @param aCommandBuffer The buffer where the commands will be encoded.
	 */
	OOpenGLRenderCommandEncoder(OOpenGLCommandBuffer* aCommandBuffer);

	/**
	 @brief Class destructor.
	 */
	~OOpenGLRenderCommandEncoder() = default;

	// OGraphicsRenderCommandEncoder interface
	virtual void setFaceCullingOptions(OCullFace aFace, OCullFront aFront) override;
	virtual void setShaderProgram(OShaderProgram * aShaderProgram) override;
	virtual void setVertexBuffer(OVertexBuffer * aVertexBuffer) override;
	virtual void setIndexBuffer(OIndexBuffer * aIndexBuffer) override;
	virtual void setTexture(OTexture * aTexture, uint32_t aIndex = 0) override;
	virtual void setUniformArgumentList(OShaderArgumentInstanceList * aUniformArguments) override;
	virtual OShaderProgram * shaderProgram() override;
	virtual OVertexBuffer * vertexBuffer() override;
	virtual OIndexBuffer * indexBuffer() override;
	virtual OTexture * texture(uint32_t aIndex = 0) override;
	virtual void clearTextures() override;
	virtual void draw(ORenderMode aRenderType) override;
	virtual void clearRenderTarget(const OVector4F& aColor) override;
	virtual void clearDepthBuffer(float aValue) override;

	// OGraphicsCommandEncoder interface
	virtual void end() override;

private:
	OShaderProgram*	_shaderProgram	= nullptr;
	OVertexBuffer*	_vertexBuffer	= nullptr;
	OIndexBuffer*	_indexBuffer	= nullptr;
	OTexture*	_texture[32];
};
