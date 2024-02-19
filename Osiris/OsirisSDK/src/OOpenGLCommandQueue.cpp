#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OOpenGLCommandBuffer.h"
#include "OsirisSDK/OOpenGLCommandQueue.h"

struct OOpenGLCommandQueue::Impl {
	OList<OOpenGLCommandBuffer*> queue;
};

OOpenGLCommandQueue::OOpenGLCommandQueue()
{
	OExPointerCheck(_impl = new Impl);
}

OOpenGLCommandQueue::~OOpenGLCommandQueue()
{
	if (_impl != nullptr) delete _impl;
}

OOpenGLCommandQueue & OOpenGLCommandQueue::operator=(OOpenGLCommandQueue && aOther)
{
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;

	return *this;
}

OGraphicsCommandBuffer * OOpenGLCommandQueue::createCommandBuffer()
{
	return new OOpenGLCommandBuffer(this);
}

void OOpenGLCommandQueue::pushBuffer(OOpenGLCommandBuffer * aCommandBuffer)
{
	//_impl->queue.pushBack(aCommandBuffer);
	// for now, we run the commands right away. with multithreading, this should be done elsewhere.
	aCommandBuffer->run();
}
