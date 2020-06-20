#pragma once

#include "OsirisSDK/defs.h"
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
	uint32_t& handle(OGPUObject* aGPUObject);

	/**
	 @brief Allocates a new OpenGL handle and assigns to the GPU object.
	 @param aGPUObject The GPU object.
	 @return A reference to the newly created handle.
	 */
	uint32_t& createHandle(OGPUObject* aGPUObject);

	/**
	 @brief Encodes the destruction of the OpenGL handle.
	 @param aGPUObject The GPU object.
	 */
	void destroyHandle(OGPUObject* aGPUObject);

private:
	OOpenGLCommandBuffer* _commandBuffer;
};

inline OOpenGLCommandEncoder::OOpenGLCommandEncoder(OOpenGLCommandBuffer* aCommandBuffer) :
	_commandBuffer(aCommandBuffer)
{

}

inline void OOpenGLCommandEncoder::encode(OOpenGLCommandBuffer::CommandItem aCommandItem)
{
	_commandBuffer->addCommandItem(aCommandItem);
}

inline uint32_t& OOpenGLCommandEncoder::handle(OGPUObject* aGPUObject)
{
	return * reinterpret_cast<uint32_t*>(aGPUObject->gpuHandle());
}

inline uint32_t & OOpenGLCommandEncoder::createHandle(OGPUObject * aGPUObject)
{
	uint32_t *newHandle = nullptr;
	OExceptionPointerCheck(newHandle = new uint32_t);
	aGPUObject->setGpuHandle(newHandle);
	return *newHandle;
}

inline void OOpenGLCommandEncoder::destroyHandle(OGPUObject * aGPUObject)
{
	encode([aGPUObject]() {
		delete reinterpret_cast<uint32_t*>(aGPUObject->gpuHandle());
		aGPUObject->setGpuHandle(nullptr);
	});
}
