#pragma once

#include "OsirisSDK/defs.h"

class OGraphicsCommandBuffer;

/**
 @brief Graphics command queue interface. 
 */
class OAPI OGraphicsCommandQueue
{
public:
	/**
	 @brief Creates a new command buffer linked to the queue.
	 */
	virtual OGraphicsCommandBuffer* createCommandBuffer() = 0;
};