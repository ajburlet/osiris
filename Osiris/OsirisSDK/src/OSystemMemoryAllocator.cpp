#include <stdlib.h>

#include "OsirisSDK/OSystemMemoryAllocator.h"

void * OSystemMemoryAllocatorImpl::alloc(uint32_t aSize)
{
	return malloc(aSize);
}

void OSystemMemoryAllocatorImpl::dealloc(void * aPtr, size_t)
{
	free(aPtr);
}

void* OSystemMemoryAllocatorImpl::realloc(void * aPtr, size_t aSize)
{
	return ::realloc(aPtr, aSize);
}
