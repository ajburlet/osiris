#pragma once

#include "GLdefs.h"
#include "defs.h"

#ifndef OMESH_MALLOC_BLOCK
#define OMESH_MALLOC_BLOCK	64
#endif

/**
 \brief Template class designed to handle OpenGL mesh buffers.
*/
template <class BType> class OAPI OMeshBuffer
{
public:
	/**
	 \brief Class constructor.
	*/
	OMeshBuffer();

	/**
	 \brief Class destructor.
	*/
	virtual ~OMeshBuffer();

	/**
	 \brief Set buffer size.
	 \param size New buffer size in bytes.
	*/
	void setSize(unsigned int size);

	/**
	 \brief Add a buffer data value.
	*/
	void addData(BType x);

	/**
	 \brief Add two-dimensional buffer data.
	*/
	void addData(BType x, BType y);

	/**
	 \brief Add three-dimensional buffer data.
	*/
	void addData(BType x, BType y, BType z);

	/**
	 \brief Add four-dimensional buffer data.
	*/
	void addData(BType x, BType y, BType z, BType w);

	/**
	 \brief Returns the mesh buffer.
	 \return Pointer to the memory area containing the buffer.
	*/
	const BType* buffer() const;

	/**
	 \brief Number of data items added to the buffer.
	*/
	int count() const;

	/**
	 \brief Generate and bind the buffer as an OpenGL object.
	 \return Buffer's OpenGL reference ID.
	*/
	GLuint generateGLBufferObject(GLenum bufferType);

	/**
	 \brief Retuns the buffer's OpenGL reference ID.
	*/
	GLuint glReference() const;

private:
	BType *_buffer;
	unsigned int _itemCount;
	unsigned int _size;

	GLuint _glBufferObject;
};

// begin template class implementation

#ifdef OSIRISSDK_EXPORTS

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
	glBufferData(bufferType, _itemCount*sizeof(BType), _buffer, GL_STATIC_DRAW);
	glBindBuffer(bufferType, 0);

	return _glBufferObject;
}

template<class BType>
GLuint OMeshBuffer<BType>::glReference() const
{
	return _glBufferObject;
}

#endif
