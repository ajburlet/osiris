#pragma once

#include "OsirisSDK/defs.h"

class OGraphicsCommandQueue;
class OGraphicsCommandEncoder;
class OGraphicsRenderCommandEncoder;
class OGraphicsResourceCommandEncoder;

/**
 @brief Graphics engine command buffer.
 */
class OGraphicsCommandBuffer
{
protected:
	/**
	 @brief Class constructor.
	 @param aCommandQueue Pointer to the command queue that owns the buffer.
	 */
	OGraphicsCommandBuffer(OGraphicsCommandQueue* aCommandQueue) :
		commandQueue_(aCommandQueue)
	{}

	/**
	 @brief Creates a new render command encoder.
	 */
	virtual OGraphicsRenderCommandEncoder* createRenderCommandEncoder() = 0;

	/**
	 @brief Creates a new resource command encoder.
	 */
	virtual OGraphicsResourceCommandEncoder* createResourceCommandEncoder() = 0;

	/**
	 @brief Commits the command buffer, clearing it to be processed on the next opportunity.
	 */
	virtual void commit() = 0;

private:
	OGraphicsCommandQueue* commandQueue_;

	friend class OGraphicsRenderCommandQueue;
};