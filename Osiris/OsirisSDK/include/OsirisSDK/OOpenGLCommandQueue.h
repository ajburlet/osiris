#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsCommandQueue.h"

class OOpenGLCommandBuffer;

/**
 @brief OpenGL command queue implementation.
 */
class OAPI OOpenGLCommandQueue : public OGraphicsCommandQueue
{
public:
	/**
	 @brief Class default constructor.
	 */
	OOpenGLCommandQueue();

	/**
	 @brief Class destructor.
	 */
	~OOpenGLCommandQueue();

	virtual OGraphicsCommandBuffer * createCommandBuffer() override;

protected:
	/**
	 @brief Pushes a command buffer to the end of the queue.
	 @param aCommandBuffer The buffer to be pushed.
	 @note This method is intended for internal usage only.
	 */
	void pushBuffer(OOpenGLCommandBuffer* aCommandBuffer);

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */

	friend class OOpenGLCommandBuffer;
};