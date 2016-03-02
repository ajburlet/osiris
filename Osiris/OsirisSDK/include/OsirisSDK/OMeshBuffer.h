#pragma once

#include <GL\freeglut.h>

#ifndef OMESH_MALLOC_BLOCK
#define OMESH_MALLOC_BLOCK	64
#endif

template <class BType> class OMeshBuffer
{
public:
	OMeshBuffer();
	virtual ~OMeshBuffer();

	void setSize(unsigned int size);
	void addData(BType x, BType y, BType z);

	const BType* buffer();
	int count();

	GLuint generateGLBufferObject();
	GLuint glBufferObject();

private:
	BType *buffer;
	unsigned int itemCount;
	unsigned int size;

	GLuint glBufferObject;
};

