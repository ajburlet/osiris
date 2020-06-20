#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OOpenGLCommandBuffer.h"
#include "OsirisSDK/OOpenGLCommandQueue.h"

struct OOpenGLCommandQueue::Impl {
	OList<OOpenGLCommandBuffer*> queue;
};

OOpenGLCommandQueue::OOpenGLCommandQueue()
{
	OExceptionPointerCheck(_impl = new Impl);
}

OOpenGLCommandQueue::~OOpenGLCommandQueue()
{
	if (_impl != nullptr) delete _impl;
}

OGraphicsCommandBuffer * OOpenGLCommandQueue::createCommandBuffer()
{
	return new OOpenGLCommandBuffer(this);
}

void OOpenGLCommandQueue::pushBuffer(OOpenGLCommandBuffer * aCommandBuffer)
{
	_impl->queue.pushBack(aCommandBuffer);
}
