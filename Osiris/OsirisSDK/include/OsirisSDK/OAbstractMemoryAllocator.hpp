#pragma once

#include "OsirisSDK/OMemoryManager.h"

/**
 @brief STL allocator interface.
 */
template <class Allocator, typename T>
class OSTLAllocator {
public:
	T* address(T& aRef);
	const T* address(const T& aRef) const;

	size_t max_size() const;

	T* allocate(size_t aSize, const void* aHint=nullptr);
	void deallocate(T* aPtr, size_t aNum);

	void construct(T* aPtr, const T& aValue);
	void destroy(T* aPtr);
};

template<class Allocator, typename T>
inline T* OSTLAllocator<Allocator, T>::address(T& aRef)
{
	return &aRef;
}

template<class Allocator, typename T>
inline const T* OSTLAllocator<Allocator, T>::address(const T& aRef) const
{
	return &aRef;
}

template<class Allocator, typename T>
inline size_t OSTLAllocator<Allocator, T>::max_size() const
{
	return std::numeric_limits<size_t>::max() / sizeof(value_type);
}

template<class Allocator, typename T>
inline T* OSTLAllocator<Allocator, T>::allocate(size_t aSize, const void* aHint)
{
	return _allocator.allocate(aSize, aHint);
}

template<class Allocator, typename T>
inline void OSTLAllocator<Allocator, T>::deallocate(T* aPtr, size_t aNum)
{
	_allocator.deallocate(aPtr, aNum);
}

template<class Allocator, typename T>
inline void OSTLAllocator<Allocator, T>::construct(T* aPtr, const T& aValue)
{
	new((void*)aPtr)T(aValue);
}

template<class Allocator, typename T>
inline void OSTLAllocator<Allocator, T>::destroy(T* aPtr)
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
	 @param aHint A previously allocated T* and not freed, so it can be used in the new allocation.
	 @return Returns newly allocated area.
	 */
	T* allocate(size_t aSize, const void* aHint = nullptr);

	/**
	 @brief Frees memory.
	 @param aPtr The T* to be freed.
	 @param aNum Number of elements used in the allocation call.
	 */
	void deallocate(T* aPtr, size_t aNum = 1);

	/**
	 @brief Reallocates memory.
	 @param aPtr Pointer to the memory area to be reallocated.
	 @param aCurrSize Current size.
	 @param aNewSize New size.
	 @return Returns newly allocated area.
	 */
	T* reallocate(T* aPtr, size_t aCurrSize, size_t aNewSize);

protected:
	/**
	 @brief Internal allocation method, to be implemented on derivation.
	 @param aSize The amount of memory to be allocated.
	 @param aHint A previously allocated T* and not freed, so it can be used in the new allocation.
	 @return Returns newly allocated area.
	 */
	virtual T* impl_allocate(size_t aSize, const void* aHint) = 0;

	/**
	 @brief Internal dealloction method, to be implemented on derivation.
	 @param aPtr The T* to be freed.
	 @param aNum Number of elements used in the allocation call.
	 */
	virtual void impl_deallocate(T* aPtr, size_t aHint) = 0;

	/**
	 @brief Internal reallocatio nmethod, to be implemented on derivation.
	 @param aPtr Pointer to the memory area to be reallocated.
	 @param aCurrSize Current size.
	 @param aNewSize New size.
	 @return Returns newly allocated area.
	 */
	T* impl_reallocate(T* aPtr, size_t aCurrSize, size_t aNewSize) = 0;

protected:
	OMemoryManagerSingleton _memory_manager;
};

template<class Allocator, typename T, OMemoryManager::Scope Scope>
inline T* OAbstractMemoryAllocator<Allocator, T, Scope>::allocate(size_t aSize, const void * aHint)
{
	impl_allocate(aSize, aHint);
	_memory_manager->increase(Scope, static_cast<uint64_t>(aSize));
}

template<class Allocator, typename T, OMemoryManager::Scope Scope>
inline void OAbstractMemoryAllocator<Allocator, T, Scope>::deallocate(T* aPtr, size_t aNum)
{
	impl_deallocate(aSize, aHint);
	_memory_manager->decrease(Scope, static_cast<uint64_t>(aSize));
}

template<class Allocator, typename T, OMemoryManager::Scope Scope>
inline T* OAbstractMemoryAllocator<Allocator, T, Scope>::reallocate(T* aPtr, size_t aCurrSize, size_t aNewSize)
{
	auto new_ptr = impl_reallocate(aPtr, aCurrSize, aNewSize);
	if (aCurrSize < aNewSize) {
		_memory_manager->increase(Scope, aNewSize - aCurrSize);
	} else {
		_memory_manager->decrease(Scope, aCurrSize - aNewSize);
	}
	return new_ptr;
}



