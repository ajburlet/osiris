#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OIndexBuffer.h"

struct Point {
	uint32_t i, j, k;
};

struct OIndexBuffer::Impl 
{
	Impl(uint32_t aVertexCount) : array(aVertexCount, {0,0,0}) {}
	OArray<Point,OSystemMemoryAllocator<OMemoryManagerScope::Graphics>> array;
};

OIndexBuffer::OIndexBuffer(uint32_t aVertexCount)
{
	_impl = new Impl(aVertexCount);
	if (!_impl) {
		throw OEx("Unable to allocate memory for internal implementation.");
	}
}

OIndexBuffer::~OIndexBuffer()
{
	delete _impl;
}

OIndexBuffer & OIndexBuffer::operator=(OIndexBuffer && aOther)
{
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

void OIndexBuffer::setFaceCount(uint32_t aNewFaceCount)
{
	_impl->array.changeCapacity(aNewFaceCount);
}

void OIndexBuffer::setFace(uint32_t aFaceIndex, uint32_t aVertexIndex1, uint32_t aVertexIndex2, uint32_t aVertexIndex3)
{
	_impl->array[aFaceIndex] = { aVertexIndex1, aVertexIndex2, aVertexIndex3 };
}

void OIndexBuffer::addFace(uint32_t aVertexIndex1, uint32_t aVertexIndex2, uint32_t aVertexIndex3)
{
	_impl->array.append({ aVertexIndex1, aVertexIndex2, aVertexIndex3 });
}

const void * OIndexBuffer::buffer() const
{
	return &(_impl->array[0]);
}

uint32_t OIndexBuffer::faceCount() const
{
	return _impl->array.size();
}

uint32_t OIndexBuffer::size() const
{
	return faceCount() * sizeof(Point);
}
