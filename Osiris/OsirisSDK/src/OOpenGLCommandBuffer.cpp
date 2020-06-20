#include <functional>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OOpenGLCommandQueue.h"
#include "OsirisSDK/OOpenGLCommandBuffer.h"

struct OOpenGLCommandBuffer::Impl {
	using CommandList = OList<CommandItem>;

	OOpenGLCommandQueue*	commandQueue = nullptr;
	CommandList		commandList;
};

OOpenGLCommandBuffer::OOpenGLCommandBuffer(OOpenGLCommandQueue * aCommandQueue)
{
	OExceptionPointerCheck(_impl = new Impl);
	_impl->commandQueue = aCommandQueue;
}

OOpenGLCommandBuffer::OOpenGLCommandBuffer()
{
	if (_impl != nullptr) delete _impl;
}

OGraphicsRenderCommandEncoder * OOpenGLCommandBuffer::createRenderCommandEncoder()
{
	return nullptr;
}

OGraphicsResourceCommandEncoder * OOpenGLCommandBuffer::createResourceCommandEncoder()
{
	return nullptr;
}

void OOpenGLCommandBuffer::commit()
{
	_impl->commandQueue->pushBuffer(this);
}

void OOpenGLCommandBuffer::addCommandItem(CommandItem aCommandItem)
{
	_impl->commandList.pushBack(aCommandItem);
}
