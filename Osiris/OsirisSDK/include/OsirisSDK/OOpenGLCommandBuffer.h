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
	 @brief Deleted copy constructor.
	 */
	OOpenGLCommandBuffer(const OOpenGLCommandBuffer& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OOpenGLCommandBuffer(OOpenGLCommandBuffer&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OOpenGLCommandBuffer();

	/**
	 @bried Deleted copy assignment operator.
	 */
	OOpenGLCommandBuffer& operator=(const OOpenGLCommandBuffer& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OOpenGLCommandBuffer& operator=(OOpenGLCommandBuffer&& aOther);

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

inline OOpenGLCommandBuffer::OOpenGLCommandBuffer(OOpenGLCommandBuffer&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}