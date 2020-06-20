#pragma once

#include "OsirisSDK/defs.h"

class OGraphicsRenderCommandEncoder;
class OGraphicsResourceCommandEncoder;

/**
 @brief Graphics engine command buffer.
 */
class OAPI OGraphicsCommandBuffer
{
public:
	/**
	 @brief Class default constructor.
	 */
	OGraphicsCommandBuffer() = default;

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
};