#pragma once

#include "OsirisSDK/OMemoryManager.h"

/**
 @brief STL allocator interface.
 */
template <class Allocator, typename T>
class OSTLAllocator {
public:
	using value_type	= T;
	using pointer		= T*;
	using const_pointer	= const T*;
	using reference		= T&;
	using const_reference	= const T&;
	using size_type		= size_t;
	using difference_type	= std::ptrdiff_t;

	pointer address(reference aRef);
	const_pointer address(const_reference aRef) const;

	size_type max_size() const;

	pointer allocate(size_type aSize, const void* aHint=nullptr);
	void deallocate(pointer aPtr, size_type aNum);

	void construct(pointer aPtr, const_reference aValue);
	void destroy(pointer aPtr);
};

template<class Allocator, typename T>
inline OSTLAllocator<Allocator,T>::pointer OSTLAllocator<Allocator, T>::address(reference aRef)
{
	return &aRef;
}

template<class Allocator, typename T>
inline OSTLAllocator<Allocator, T>::const_pointer OSTLAllocator<Allocator, T>::address(const_reference aRef) const
{
	return &aRef;
}

template<class Allocator, typename T>
inline OSTLAllocator<Allocator,T>::size_type OSTLAllocator<Allocator, T>::max_size() const
{
	return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template<class Allocator, typename T>
inline OSTLAllocator<Allocator,T>::pointer OSTLAllocator<Allocator, T>::allocate(size_type aSize, const void* aHint)
{
	return _allocator.allocate(aSize, aHint);
}

template<class Allocator, typename T>
inline void OSTLAllocator<Allocator, T>::deallocate(pointer aPtr, size_type aNum)
{
	_allocator.deallocate(aPtr, aNum);
}

template<class Allocator, typename T>
inline void OSTLAllocator<Allocator, T>::construct(pointer aPtr, const_reference aValue)
{
	new((void*)aPtr)T(aValue);
}

template<class Allocator, typename T>
inline void OSTLAllocator<Allocator, T>::destroy(pointer aPtr)
{
	aPtr->~T();
}


/**
 @brief Abstract allocator, serves as a base for allocator implementation.
 @param Allocator The allocator class, derived of this class.
 @param T The type to be allocated.
 @param Scope Allocation scope.
 */
template <class Allocator, typename T, OMemoryManager::Scope Scope>
class OAbstractMemoryAllocator
{
public:
	using Internal = OSTLAllocator<Allocator, T>;

	/**
	 @brief Allocates memory.
	 @param aSize The amount of memory to be allocated.
	 @param aHint A previously allocated pointer and not freed, so it can be used in the new allocation.
	 */
	Internal::pointer allocate(Internal::size_type aSize, const void* aHint = nullptr);

	/**
	 @brief Frees memory.
	 @param aPtr The pointer to be freed.
	 @param aNum Number of elements used in the allocation call.
	 */
	void deallocate(Internal::pointer aPtr, Internal::size_type aNum);

protected:
	/**
	 @brief Internal allocation method, to be implemented on derivation.
	 @param aSize The amount of memory to be allocated.
	 @param aHint A previously allocated pointer and not freed, so it can be used in the new allocation.
	 */
	virtual Internal::pointer impl_allocate(Internal::size_type aSize, const void* aHint) = 0;

	/**
	 @brief Internal dealloction method, to be implemented on derivation.
	 @param aPtr The pointer to be freed.
	 @param aNum Number of elements used in the allocation call.
	 */
	virtual void impl_deallocate(Internal::pointer aPtr, Internal::size_type aHint) = 0;

protected:
	OMemoryManagerSingleton _memory_manager;
};

template<class Allocator, typename T, OMemoryManager::Scope Scope>
inline OAbstractMemoryAllocator<Allocator,T,Scope>::Internal::pointer 
OAbstractMemoryAllocator<Allocator, T, Scope>::allocate(Internal::size_type aSize, const void * aHint)
{
	impl_allocate(aSize, aHint);
	_memory_manager->increase(Scope, static_cast<uint64_t>(aSize));
}

template<class Allocator, typename T, OMemoryManager::Scope Scope>
inline void OAbstractMemoryAllocator<Allocator, T, Scope>::deallocate(Internal::pointer aPtr, Internal::size_type aNum)
{
	impl_deallocate(aSize, aHint);
	_memory_manager->decrease(Scope, static_cast<uint64_t>(aSize));
}

