#pragma once

#include "OsirisSDK/OAbstractMemoryAllocator.hpp"

/**
 @brief System memory allocator implementation.
 */
class OSystemMemoryAllocatorImpl
{
public:
	static void* alloc(size_t aSize);
	static void dealloc(void* aPtr, size_t aSize);
	static void* realloc(void* aPtr, size_t aSize);
};


/**
 @brief Allocator that uses system <code>malloc()</code> and <code>free()</code> calls. 
 */
template <OMemoryManager::Scope Scope = OMemoryManager::Scope::Default>
class OSystemMemoryAllocator : public OAbstractMemoryAllocator<Scope>
{
public:
	/**
	 @brief Default class constructor.
	 */
	OSystemMemoryAllocator() = default;

	/**
	 @brief Class destructor.
	 */
	~OSystemMemoryAllocator() = default;

protected:
	/**
	 @copydoc OAbstractMemoryAllocator::impl_allocate(Internal::size_type, const void*)
	 */
	virtual void* impl_allocate(size_t aSize, const void* aHint) override;

	/**
	 @copydoc OAbstractMemoryAllocator::impl_deallocate(Internal::pointer, Internal::size_type)
	 */
	virtual void impl_deallocate(void* aPtr, size_t aNum) override;
	
	/**
	 @copydoc OAbstractMemoryAllocator::impl_reallocate(Internal::pointer, Internal::size_type, Internal::size_type)
	 */
	virtual void* impl_reallocate(void* aPtr, size_t aCurrSize, size_t aNewSize) override;
};

template<OMemoryManager::Scope Scope>
inline void* OSystemMemoryAllocator<Scope>::impl_allocate(size_t aSize, const void *)
{
	return OSystemMemoryAllocatorImpl::alloc(static_cast<uint32_t>(aSize));
}

template<OMemoryManager::Scope Scope>
inline void OSystemMemoryAllocator<Scope>::impl_deallocate(void* aPtr, size_t aBytes)
{
	OSystemMemoryAllocatorImpl::dealloc(aPtr, aBytes);
}

template<OMemoryManager::Scope Scope>
inline void* OSystemMemoryAllocator<Scope>::impl_reallocate(void* aPtr, size_t aCurrSize, size_t aNewSize)
{
	return OSystemMemoryAllocatorImpl::realloc(aPtr, static_cast<uint32_t>(aNewSize));
}
