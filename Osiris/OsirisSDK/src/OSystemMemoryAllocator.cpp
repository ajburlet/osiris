#include "OsirisSDK/OSystemMemoryAllocator.h"

void * OSystemMemoryAllocatorImpl::alloc(uint32_t aBytes)
{
	return malloc(aBytes);
}

void OSystemMemoryAllocatorImpl::dealloc(void * aPtr)
{
	free(aPtr);
}
