#ifndef _OMESHBUFFER_CPP_
#define _OMESHBUFFER_CPP_

#include <stdlib.h>

#include "OsirisSDK/OMeshBuffer.h"

template <class BType>
OMeshBuffer<BType>::OMeshBuffer() :
	_buffer(NULL),
	_size(0),
	_itemCount(0),
	_glBufferObject(0)
{

}

template<class BType>
OMeshBuffer<BType>::~OMeshBuffer()
{
	free(_buffer);
	if (_glBufferObject != 0) glDeleteBuffers(1, &_glBufferObject);
}

template<class BType>
void OMeshBuffer<BType>::setSize(unsigned int new_size)
{
	_size = new_size;
	_buffer = (BType*) realloc(_buffer, new_size*sizeof(BType));
}

template<class BType>
void OMeshBuffer<BType>::addData(BType x)
{
	if (_itemCount == _size) setSize(_size + OMESH_MALLOC_BLOCK);
	
	_buffer[_itemCount++] = x;
}

template<class BType>
void OMeshBuffer<BType>::addData(BType x, BType y)
{
	addData(x);
	addData(y);
}

template<class BType>
void OMeshBuffer<BType>::addData(BType x, BType y, BType z)
{
	addData(x);
	addData(y);
	addData(z);
}

template<class BType>
void OMeshBuffer<BType>::addData(BType x, BType y, BType z, BType w)
{
	addData(x);
	addData(y);
	addData(z);
	addData(w);
}

template<class BType>
const BType * OMeshBuffer<BType>::buffer() const
{
	return _buffer;
}

template<class BType>
int OMeshBuffer<BType>::count() const
{
	return _itemCount;
}

template<class BType>
GLuint OMeshBuffer<BType>::generateGLBufferObject(GLenum bufferType)
{
	glGenBuffers(1, &_glBufferObject);

	glBindBuffer(bufferType, _glBufferObject);
	glBufferData(bufferType, _itemCount*sizeof(BType) * 3, _buffer, GL_STATIC_DRAW);
	glBindBuffer(bufferType, 0);

	return _glBufferObject;
}

template<class BType>
GLuint OMeshBuffer<BType>::glReference() const
{
	return _glBufferObject;
}

#endif