#include <stdlib.h>

#include "OsirisSDK/OSystemMemoryAllocator.h"

void * OSystemMemoryAllocatorImpl::alloc(uint32_t aBytes)
{
	return malloc(aBytes);
}

void OSystemMemoryAllocatorImpl::dealloc(void * aPtr)
{
	free(aPtr);
}

void* OSystemMemoryAllocatorImpl::realloc(void * aPtr, uint32_t aSize)
{
	return ::realloc(aPtr, aSize);
}
