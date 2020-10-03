#pragma once

#include <functional>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsCommandBuffer.h"

class OOpenGLCommandQueue;

/**
 @brief OpenGL command buffer implementation.
 */
class OAPI OOpenGLCommandBuffer : public OGraphicsCommandBuffer
{
protected:
	/**
	 @brief Class constructor.
	 @param aCommandQueue The command queue that created the buffer
	 */
	OOpenGLCommandBuffer(OOpenGLCommandQueue* aCommandQueue);

public:
	/**
	 @brief Class destructor.
	 */
	virtual ~OOpenGLCommandBuffer();

	virtual OGraphicsRenderCommandEncoder * createRenderCommandEncoder() override;

	virtual OGraphicsResourceCommandEncoder * createResourceCommandEncoder() override;

	virtual void commit() override;

	virtual void waitUntilCompleted() override;

public:
	/**
	 @brief Runs all commands in the buffer.
	 */
	void run();

protected:
	/**
	 @brief A command item to be executed.
	 */
	using CommandItem = std::function<void()>;

	/**
	 @brief Adds a command item to the buffer.
	 */
	void addCommandItem(CommandItem aCommandItem);

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
	
	friend class OOpenGLCommandQueue;
	friend class OOpenGLCommandEncoder;
};