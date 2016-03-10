#include <stdlib.h>

#include <glload/gl_3_3.h>

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
	_buffer = realloc(_buffer, new_size*sizeof(BType)*3);
}

template<class BType>
void OMeshBuffer<BType>::addData(BType x, BType y, BType z)
{
	unsigned int curPos = _itemCount * 3;

	if (_itemCount == _size) setSize(_size + OMESH_MALLOC_BLOCK);
	
	_buffer[curPos] = x;
	_buffer[curPos + 1] = y;
	_buffer[curPos + 2] = z;

	_itemCount++;
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

