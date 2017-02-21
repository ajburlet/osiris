#pragma once

#include "defs.h"
#include "OMemoryPool.h"


/**
 \brief Template class for objects that use memory pools.
 
 Classes of this type can take advantage of memory pools to avoid frequent malloc() and free() calls,
 avoiding heap fragmentation. Inheriting this template class will overload new and delete operators.

 \tparam blockSize Block size in bytes.
 \tparam segmentSize Number of blocks in a segment.

 */
template <size_t blockSize, size_t segmentSize>
class OAPI OMemoryPoolObject 
{
private:
	static OMemoryPool* _memoryPool;

	/**
	 \brief Initializes the memory pool. Used by the new operator.
	 */
	static void Init();

public:
	/**
	 \brief Class constructor.
	 */
	OMemoryPool();

	/**
	 \brief Class destructor.
	 */
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
