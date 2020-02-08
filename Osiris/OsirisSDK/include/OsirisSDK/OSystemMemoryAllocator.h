#pragma once

#include "OsirisSDK/OAbstractMemoryAllocator.hpp"

/**
 @brief System memory allocator implementation.
 */
class OSystemMemoryAllocatorImpl
{
	static void* alloc(uint32_t aBytes);
	static void dealloc(void* aPtr);
};


/**
 @brief Allocator that uses system <code>malloc()</code> and <code>free()</code> calls. 
 */
template <typename T, OMemoryManager::Scope Scope = OMemoryManager::Scope::Default>
class OSystemMemoryAllocator : OAbstractMemoryAllocator<OSystemMemoryAllocator, T, Scope>
{
protected:
	/**
	 @copydoc OAbstractMemoryAllocator::impl_allocate(Internal::size_type, const void*)
	 */
	virtual Internal::pointer impl_allocate(Internal::size_type aSize, const void* aHint) override;

	/**
	 @copydoc OAbstractMemoryAllocator::impl_deallocate(Internal::pointer, Internal::size_type)
	 */
	virtual void impl_deallocate(Internal::pointer aPtr, Internal::size_type aNum) override;
};

template<typename T, OMemoryManager::Scope Scope>
inline OSystemMemoryAllocator<T,Scope>::Internal::pointer OSystemMemoryAllocator<T, Scope>::impl_allocate(Internal::size_type aSize, const void *)
{
	return OSystemMemoryAllocatorImpl::alloc(static_cast<uint32_t>(aSize));
}

template<typename T, OMemoryManager::Scope Scope>
inline void OSystemMemoryAllocator<T, Scope>::impl_deallocate(Internal::pointer aPtr, Internal::size_type)
{
	OSystemMemoryAllocator::deallocate(aPtr);
}
