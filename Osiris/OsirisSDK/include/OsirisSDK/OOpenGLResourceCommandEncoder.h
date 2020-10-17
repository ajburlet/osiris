#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsResourceCommandEncoder.h"
#include "OsirisSDK/OOpenGLCommandEncoder.h"

/**
 @brief OpenGL resource command encoder implementation.
 */
class OAPI OOpenGLResourceCommandEncoder : public OGraphicsResourceCommandEncoder, protected OOpenGLCommandEncoder
{
public:
	/**
	 @brief Class constructor.
	 @param aCommandBuffer The buffer where the commands will be encoded.
	 */
	OOpenGLResourceCommandEncoder(OOpenGLCommandBuffer* aCommandBuffer);

	/**
	 @brief Class destructor.
	 */
	~OOpenGLResourceCommandEncoder() = default;


	// OGraphicsResourceCommandEncoder interface
	virtual void load(ORenderComponents* aRenderComponents) override;
	virtual void unload(ORenderComponents* aVertexBufferDescriptor) override;
	virtual void load(OVertexBuffer * aVertexBuffer) override;
	virtual void unload(OVertexBuffer * aVertexBuffer) override;
	virtual void load(OIndexBuffer * aIndexBuffer) override;
	virtual void unload(OIndexBuffer * aIndexBuffer) override;
	virtual void load(OTexture * aTexture) override;
	virtual void unload(OTexture * aTexture) override;
	virtual void load(OShaderUniformArgument * aAttributeInstance, OShaderProgram * aShader, const char * aName) override;
	virtual void unload(OShaderUniformArgument* aAttributeInstance) override;

	// Inherited via OGraphicsCommandEncoder interface
	virtual void end() override;

};
