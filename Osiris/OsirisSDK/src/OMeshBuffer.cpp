#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES
#include <GL\GL.h>

#include "OsirisSDK\OMeshBuffer.h"

template <class BType>
OMeshBuffer<BType>::OMeshBuffer() :
	buffer(NULL),
	size(0),
	itemCount(0),
	glBufferObject(0)
{

}

template<class BType>
OMeshBuffer<BType>::~OMeshBuffer()
{
	free(buffer);
}

template<class BType>
void OMeshBuffer<BType>::setSize(unsigned int new_size)
{
	size = new_size;
	buffer = realloc(buffer, new_size*sizeof(BType)*3);
}

template<class BType>
void OMeshBuffer<BType>::addData(BType x, BType y, BType z)
{
	unsigned int curPos = itemCount * 3;

	if (itemCount == size) setSize(size + OMESH_MALLOC_BLOCK);
	
	buffer[curPos] = x;
	buffer[curPos + 1] = y;
	buffer[curPos + 2] = z;

	itemCount++;
}

template<class BType>
const BType * OMeshBuffer<BType>::buffer()
{
	return buffer;
}

template<class BType>
int OMeshBuffer<BType>::count()
{
	return itemCount;
}

template<class BType>
GLuint OMeshBuffer<BType>::generateGLBufferObject()
{

}

template<class BType>
GLuint OMeshBuffer<BType>::glBufferObject()
{
	return glBufferObject;
}

