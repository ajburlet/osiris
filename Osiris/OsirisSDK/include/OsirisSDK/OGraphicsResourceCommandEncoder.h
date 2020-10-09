#pragma once

#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGraphicsCommandEncoder.h"

class OShaderProgram;
class OVertexBuffer;
class OVertexBufferDescriptor;
class OIndexBuffer;
class OShaderArgumentInstance;
class OTexture;
class ORenderComponents;

/**
 @brief Resource command encoder for the graphics engine.
 */
class OAPI OGraphicsResourceCommandEncoder : public OGraphicsCommandEncoder
{
protected:
	/**
	 @brief Class constructor.
	 @param aCommandQueue The queue on which the encoder writes the commands to.
	 */
	OGraphicsResourceCommandEncoder();

public:

	/**
	 @brief Class destructor.
	 */
	virtual ~OGraphicsResourceCommandEncoder() = default;

	/**
	 @brief Encodes commands to load general rendering definitions for all render components. 
	 */
	virtual void load(ORenderComponents* aRenderComponents) = 0;

	/**
	 @brief Encodes commands to unload general rendering definitions for all render components. 
	 */
	virtual void unload(ORenderComponents* aRenderComponents) = 0;

	/**
	 @brief Encodes commands to load a vertex buffer.
	 */
	virtual void load(OVertexBuffer* aVertexBuffer) = 0;

	/**
	 @brief Encodes commands to unload a vertex buffer.
	 */
	virtual void unload(OVertexBuffer* aVertexBuffer) = 0;

	/**
	 @brief Encodes commands to load an index buffer.
	 */
	virtual void load(OIndexBuffer* aIndexBuffer) = 0;

	/**
	 @brief Encodes commands to unload an index buffer.
	 */
	virtual void unload(OIndexBuffer* aIndexBuffer) = 0;

	/**
	 @brief Encodes commands to load a texture.
	 */
	virtual void load(OTexture* aTexture) = 0;

	/**
	 @brief Encodes commands to unload a texture.
	 */
	virtual void unload(OTexture* aTexture) = 0;

	/**
	 @brief Loads a shader uniform attribute instance.
	 @param aAttributeInstance The attribute instance to be loaded.
	 @param aShader The shader program to which the attribute relates.
	 @param aName The name the argument assumes in the shader.
	 */
	virtual void load(OShaderArgumentInstance* aAttributeInstance, OShaderProgram* aShader, const char* aName) = 0;

	/**
	 @brief Unloads a shader uniform attribute instance.
	 */
	virtual void unload(OShaderArgumentInstance* aAttributeInstance) = 0;
};

inline OGraphicsResourceCommandEncoder::OGraphicsResourceCommandEncoder() :
	OGraphicsCommandEncoder(Type::Resource)
{

}
