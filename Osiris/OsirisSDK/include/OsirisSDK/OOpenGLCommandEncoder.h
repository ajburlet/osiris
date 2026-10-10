#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OGPUObject.h"
#include "OsirisSDK/OOpenGLCommandBuffer.h"

/**
 @brief OpenGL command encoder common class.
 */
class OAPI OOpenGLCommandEncoder
{
protected:
	/**
	 @brief Class constructor.
	 @param aCommandBuffer The destination buffer where commands will be encoded.
	 */
	OOpenGLCommandEncoder(OOpenGLCommandBuffer* aCommandBuffer);

	/**
	 @brief Class destructor.
	 */
	~OOpenGLCommandEncoder() = default;

	/**
	 @brief Encodes command to the buffer.
	 @param aCommandItem Function to be added to the buffer.
	 */
	void encode(OOpenGLCommandBuffer::CommandItem aCommandItem);

	/**
	 @brief Returns the OpenGL handle.
	 @param aGPUObject The GPU object.
	 */
	GLuint& handle(OGPUObject& aGPUObject);

private:
	OOpenGLCommandBuffer* _commandBuffer;
};

inline OOpenGLCommandEncoder::OOpenGLCommandEncoder(OOpenGLCommandBuffer* aCommandBuffer) :
	_commandBuffer(aCommandBuffer)
{

}

inline void OOpenGLCommandEncoder::encode(OOpenGLCommandBuffer::CommandItem aCommandItem)
{
	if (!aCommandItem) throw OEx("Added non-callable command.");
	_commandBuffer->addCommandItem(aCommandItem);
}

inline GLuint& OOpenGLCommandEncoder::handle(OGPUObject& aGPUObject)
{
	return *reinterpret_cast<GLuint*>(aGPUObject.gpuHandle());
}
