#pragma once

#include "GLdefs.h"

#ifndef OMESH_MALLOC_BLOCK
#define OMESH_MALLOC_BLOCK	64
#endif

template <class BType> class OMeshBuffer
{
public:
	OMeshBuffer();
	virtual ~OMeshBuffer();

	void setSize(unsigned int size);
	void addData(BType x);
	void addData(BType x, BType y);
	void addData(BType x, BType y, BType z);
	void addData(BType x, BType y, BType z, BType w);

	const BType* buffer() const;
	int count() const;

	GLuint generateGLBufferObject(GLenum bufferType);
	GLuint glReference() const;

private:
	BType *_buffer;
	unsigned int _itemCount;
	unsigned int _size;

	GLuint _glBufferObject;
};

#include "OMeshBuffer.cpp"

