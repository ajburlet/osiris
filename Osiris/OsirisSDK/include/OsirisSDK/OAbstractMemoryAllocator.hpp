#pragma once

#include <type_traits>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OMemoryManager.h"

/**
 @brief STL allocator interface.
 */
template <class Allocator, class T>
class OSTLAllocator {
public:
	using value_type	= T;
	using pointer		= T*;
	using const_pointer	= const T*;
	using reference		= T&;
	using const_reference	= const T&;
	using size_type		= std::size_t;
	using difference_type	= std::ptrdiff_t;

	OSTLAllocator() = default;
	~OSTLAllocator() = default;
	template <class U> OSTLAllocator(const OSTLAllocator<Allocator,U>&) {}

	// rebind allocator to type U
	template <class U>
	struct rebind {
		using other = OSTLAllocator<Allocator,U>;
	};

	T* address(T& aRef);
	const T* address(const T& aRef) const;

	size_t max_size() const;

	T* allocate(size_t aSize, const void* aHint=nullptr);
	void deallocate(T* aPtr, size_t aNum);

	void construct(T* aPtr, const T& aValue);
	void destroy(T* aPtr);

	template <class OtherAllocator, class OtherT>
	bool operator==(const OSTLAllocator<OtherAllocator, OtherT>&) {
		return (std::is_same<Allocator, OtherAllocator>::value && std::is_same<T, OtherT>::value);
	}
	template <class OtherAllocator, class OtherT>
	bool operator!=(const OSTLAllocator<OtherAllocator, OtherT>& aOther) {
		return !operator==(aOther);
	}

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
	return reinterpret_cast<T*>(Allocator().allocate(aSize*sizeof(T), aHint));
}

template<class Allocator, typename T>
inline void OSTLAllocator<Allocator, T>::deallocate(T* aPtr, size_t aNum)
{
	Allocator().deallocate(aPtr, aNum*sizeof(T));
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
 @param T The type to be allocated.
 @param Scope Allocation scope.
 */
template <OMemoryManagerScope Scope>
class OAbstractMemoryAllocator
{
public:
	/**
	 @brief Allocates memory.
	 @param aSize The amount of memory to be allocated.
	 @param aHint A previously allocated pointer and not freed, so it can be used in the new allocation.
	 @return Returns newly allocated area.
	 */
	void* allocate(size_t aSize, const void* aHint = nullptr);

	/**
	 @brief Frees memory.
	 @param aPtr The pointer to be freed.
	 @param aBytes Bytes freed. 
	 */
	void deallocate(void* aPtr, size_t aBytes);

	/**
	 @brief Reallocates memory.
	 @param aPtr Pointer to the memory area to be reallocated.
	 @param aCurrSize Current size.
	 @param aNewSize New size.
	 @return Returns newly allocated area.
	 */
	void* reallocate(void* aPtr, size_t aCurrSize, size_t aNewSize);

protected:
	/**
	 @brief Internal allocation method, to be implemented on derivation.
	 @param aSize The amount of memory to be allocated.
	 @param aHint A previously allocated T* and not freed, so it can be used in the new allocation.
	 @return Returns newly allocated area.
	 */
	virtual void* impl_allocate(size_t aSize, const void* aHint) = 0;

	/**
	 @brief Internal dealloction method, to be implemented on derivation.
	 @param aPtr The T* to be freed.
	 @param aBytes Bytes freed. 
	 */
	virtual void impl_deallocate(void* aPtr, size_t aBytes) = 0;

	/**
	 @brief Internal reallocatio nmethod, to be implemented on derivation.
	 @param aPtr Pointer to the memory area to be reallocated.
	 @param aCurrSize Current size.
	 @param aNewSize New size.
	 @return Returns newly allocated area.
	 */
	virtual void* impl_reallocate(void* aPtr, size_t aCurrSize, size_t aNewSize) = 0;

};

template<OMemoryManagerScope Scope>
inline void* OAbstractMemoryAllocator<Scope>::allocate(size_t aSize, const void * aHint)
{
	void* ptr = impl_allocate(aSize, aHint);
	OMemoryManagerSingleton()->increase(Scope, static_cast<uint64_t>(aSize));
	return ptr;
}

template<OMemoryManagerScope Scope>
inline void OAbstractMemoryAllocator<Scope>::deallocate(void* aPtr, size_t aBytes)
{
	impl_deallocate(aPtr, aBytes);
	OMemoryManagerSingleton()->decrease(Scope, static_cast<uint64_t>(aBytes));
}

template<OMemoryManagerScope Scope>
inline void* OAbstractMemoryAllocator<Scope>::reallocate(void* aPtr, size_t aCurrSize, size_t aNewSize)
{
	auto new_ptr = impl_reallocate(aPtr, aCurrSize, aNewSize);
	if (aCurrSize < aNewSize) {
		OMemoryManagerSingleton()->increase(Scope, aNewSize - aCurrSize);
	} else {
		OMemoryManagerSingleton()->decrease(Scope, aCurrSize - aNewSize);
	}
	return new_ptr;
}



