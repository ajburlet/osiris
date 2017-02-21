#pragma once

#include "defs.h"
#include "OMemoryPool.h"

template <size_t blockSize, size_t segmentSize>
class OAPI OMemoryPoolObject 
{
private:
	static OMemoryPool* _memoryPool;

	static void Init();

public:
	OMemoryPool();
	virtual ~OMemoryPool();

	void* operator new(size_t sz);
	void operator delete(void *ptr);

	static OMemoryPool* memoryPool();
};

template<size_t blockSize, size_t segmentSize>
inline void OMemoryPoolObject<blockSize, segmentSize>::Init()
{
	if (_memoryPool == NULL) _memoryPool = new OMemoryPool(blockSize, segmentSize);
}

template<size_t blockSize, size_t segmentSize>
inline OMemoryPoolObject<blockSize, segmentSize>::OMemoryPool()
{
	Init();
}

template<size_t blockSize, size_t segmentSize>
inline OMemoryPoolObject<blockSize, segmentSize>::~OMemoryPool()
{
}

template<size_t blockSize, size_t segmentSize>
inline void * OMemoryPoolObject<blockSize, segmentSize>::operator new(size_t sz)
{
	Init();
	return _memoryPool->alloc(sz);
}

template<size_t blockSize, size_t segmentSize>
inline void OMemoryPoolObject<blockSize, segmentSize>::operator delete(void * ptr)
{
	_memoryPool->free(ptr);
}

template<size_t blockSize, size_t segmentSize>
inline OMemoryPool * OMemoryPoolObject<blockSize, segmentSize>::memoryPool()
{
	return _memoryPool;
}
