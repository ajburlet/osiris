#pragma once

#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGraphicsCommandEncoder.h"

class OGraphicsCommandQueue;
class OVertexBuffer;
class OIndexBuffer;
class OTexture;

/**
 @brief Resource command encoder for the graphics engine.
 */
class OGraphicsResourceCommandEncoder : public OGraphicsCommandEncoder
{
public:
	/**
	 @brief Class constructor.
	 @param aCommandQueue The queue on which the encoder writes the commands to.
	 */
	OGraphicsResourceCommandEncoder(OGraphicsCommandQueue* aCommandQueue);

	/**
	 @brief Class destructor.
	 */
	~OGraphicsResourceCommandEncoder() = default;

	/**
	 @brief Encodes commands to load a vertex buffer.
	 */
	virtual void load(OVertexBuffer* aVertexBuffer) = 0;

	/**
	 @brief Encodes commadns to load an index buffer.
	 */
	virtual void load(OIndexBuffer* aIndexBuffer) = 0;

	/**
	 @brief Encodes commands to load a texture.
	 */
	virtual void load(OTexture* aTexture) = 0;
};

inline OGraphicsResourceCommandEncoder::OGraphicsResourceCommandEncoder(OGraphicsCommandQueue* aCommandQueue) :
	OGraphicsCommandEncoder(aCommandQueue)
{

}
