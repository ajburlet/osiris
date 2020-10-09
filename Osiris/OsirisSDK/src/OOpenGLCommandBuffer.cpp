#include <functional>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OOpenGLCommandQueue.h"
#include "OsirisSDK/OOpenGLRenderCommandEncoder.h"
#include "OsirisSDK/OOpenGLResourceCommandEncoder.h"
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

OOpenGLCommandBuffer::~OOpenGLCommandBuffer()
{
	if (_impl != nullptr) delete _impl;
}

OGraphicsRenderCommandEncoder * OOpenGLCommandBuffer::createRenderCommandEncoder()
{
	return new OOpenGLRenderCommandEncoder(this);
}

OGraphicsResourceCommandEncoder * OOpenGLCommandBuffer::createResourceCommandEncoder()
{
	return new OOpenGLResourceCommandEncoder(this);
}

void OOpenGLCommandBuffer::commit()
{
	_impl->commandQueue->pushBuffer(this);
}

void OOpenGLCommandBuffer::waitUntilCompleted()
{
	// must be completed when multithread approach is done
}

void OOpenGLCommandBuffer::run()
{
	for (auto& cmd : _impl->commandList) {
		cmd();
	}
}

void OOpenGLCommandBuffer::addCommandItem(CommandItem aCommandItem)
{
	_impl->commandList.pushBack(aCommandItem);
}
