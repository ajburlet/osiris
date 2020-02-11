#pragma once

#include "OsirisSDK/OAbstractMemoryAllocator.hpp"

/**
 @brief System memory allocator implementation.
 */
class OSystemMemoryAllocatorImpl
{
	static void* alloc(uint32_t aBytes);
	static void dealloc(void* aPtr);
	static void* realloc(void* aPtr, uint32_t aSize);
};


/**
 @brief Allocator that uses system <code>malloc()</code> and <code>free()</code> calls. 
 */
template <typename T, OMemoryManager::Scope Scope = OMemoryManager::Scope::Default>
class OSystemMemoryAllocator : OAbstractMemoryAllocator<OSystemMemoryAllocator<T,Scope>, T, Scope>
{
protected:
	/**
	 @copydoc OAbstractMemoryAllocator::impl_allocate(Internal::size_type, const void*)
	 */
	virtual T* impl_allocate(size_t aSize, const void* aHint) override;

	/**
	 @copydoc OAbstractMemoryAllocator::impl_deallocate(Internal::pointer, Internal::size_type)
	 */
	virtual void impl_deallocate(T* aPtr, size_t aNum) override;
	
	/**
	 @copydoc OAbstractMemoryAllocator::impl_reallocate(Internal::pointer, Internal::size_type, Internal::size_type)
	 */
	virtual void impl_reallocate(T* aPtr, size_t aCurrSize, size_t aNewSize) override;
};

template<typename T, OMemoryManager::Scope Scope>
inline T* OSystemMemoryAllocator<T, Scope>::impl_allocate(size_t aSize, const void *)
{
	return OSystemMemoryAllocatorImpl::alloc(static_cast<uint32_t>(aSize));
}

template<typename T, OMemoryManager::Scope Scope>
inline void OSystemMemoryAllocator<T, Scope>::impl_deallocate(T* aPtr, size_t)
{
	OSystemMemoryAllocatorImpl::dealloc(aPtr);
}

template<typename T, OMemoryManager::Scope Scope>
inline void OSystemMemoryAllocator<T, Scope>::impl_reallocate(T* aPtr, size_t aCurrSize, size_t aNewSize)
{
	return OSystemMemoryAllocatorImpl::realloc(aPtr, static_cast<uint32_t>(aNewSize));
}
