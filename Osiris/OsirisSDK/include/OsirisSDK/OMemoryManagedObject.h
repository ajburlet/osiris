#pragma once

#include "OsirisSDK/defs.h"

/**
 @brief Memory managed object.
 */
template <class Allocator>
class OAPI OMemoryManagedObject
{
public:
	/**
	 @brief New operator overload.
	 @param aSize The size of the newly allocated memory area.
	 */
	void* operator new(size_t aSize);

	/**
	 @brief New operator overload for arrays.
	 @param aSize The size of the newly allocated memory area.
	 */
	void* operator new[](size_t aSize);

	/**
	 @brief Delete operator overload.
	 @param aPtr The pointer to the target memory area.
	 @param aSize The size of the area to be freed.
	 */
	void operator delete(void* aPtr, size_t aSize);
	
	/**
	 @brief Delete operator overload for arrays.
	 @param aPtr The pointer to the target memory area.
	 @param aSize The size of the area to be freed.
	 */
	void operator delete[](void* aPtr, size_t aSize);
};

template<class Allocator>
inline void * OMemoryManagedObject<Allocator>::operator new(size_t aSize)
{
	return Allocator().allocate(aSize);
}

template<class Allocator>
inline void * OMemoryManagedObject<Allocator>::operator new[](size_t aSize)
{
	return Allocator().allocate(aSize);
}

template<class Allocator>
inline void OMemoryManagedObject<Allocator>::operator delete(void * aPtr, size_t aSize)
{
	return Allocator().deallocate(aPtr, aSize);
}

template<class Allocator>
inline void OMemoryManagedObject<Allocator>::operator delete[](void * aPtr, size_t aSize)
{
	return Allocator().deallocate(aPtr, aSize);
}
