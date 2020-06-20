#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OVertexBuffer.h"

// ****************************************************************************
// OVertexBufferDescriptor
// ****************************************************************************
struct OVertexBufferDescriptor::Impl {
	struct Attribute {
		OShaderArgument	attribute;
		uint32_t	offset		= 0;
	};
	ODynArray<Attribute>	attributes;
	uint32_t		stride		= 0;
};

OVertexBufferDescriptor::OVertexBufferDescriptor()
{
	_impl = new Impl;
	if (_impl == nullptr) {
		throw OException("Unable to allocate implementation structure.");
	}
}

OVertexBufferDescriptor::~OVertexBufferDescriptor()
{
	if (_impl != nullptr) {
		delete _impl;
	}
}

uint8_t OVertexBufferDescriptor::addAttribute(const OShaderArgument & aAttribute)
{
	auto idx = static_cast<uint8_t>(_impl->attributes.size());
	_impl->attributes.append({ aAttribute,  _impl->stride });
	_impl->stride += aAttribute.size();
	return idx;
}

const OShaderArgument& OVertexBufferDescriptor::attributeAtIndex(uint8_t aAttributeIndex) const
{
	return _impl->attributes[aAttributeIndex].attribute;
}

uint32_t OVertexBufferDescriptor::attributeCount() const
{
	return _impl->attributes.size();
}

uint32_t OVertexBufferDescriptor::offset(uint32_t aAttributeIndex) const
{
	return _impl->attributes[aAttributeIndex].offset;
}

uint32_t OVertexBufferDescriptor::stride() const
{
	return _impl->stride;
}

// ****************************************************************************
// OVertexBuffer
// ****************************************************************************
using OVertexBufferAllocator = OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>;

OVertexBuffer::OVertexBuffer(OVertexBufferDescriptor & aDescriptor, uint32_t aVertexCount) :
	_descriptor(&aDescriptor)
{
	resize(aVertexCount);
}

OVertexBuffer::~OVertexBuffer()
{
	if (_buffer != nullptr) {
		OVertexBufferAllocator().deallocate(_buffer, _vertexCount * _descriptor->stride());
	}
}

void OVertexBuffer::resize(uint32_t aNewVertexCount)
{
	size_t currSize = _vertexCount * _descriptor->stride();
	size_t newSize = aNewVertexCount * _descriptor->stride();
	auto new_buffer = static_cast<uint8_t*>(OVertexBufferAllocator().reallocate(_buffer, currSize, newSize));
	if (new_buffer == nullptr) {
		throw OException("Unable to reallocate vertex buffer.");
	}
	_buffer = new_buffer;
}

void OVertexBuffer::putValue(uint32_t aOffset, const void* aPtr, size_t aSize)
{
	memcpy(_buffer + aOffset, aPtr, aSize);
}
