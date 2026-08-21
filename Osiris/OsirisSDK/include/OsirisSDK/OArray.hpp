#pragma once

#include <initializer_list>
#include <type_traits>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMemoryManager.h"
#include "OsirisSDK/OAbstractMemoryAllocator.hpp"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OMemoryManagedObject.h"


/**
 * @brief Concept describes how the array capacity should be increased
 */
template <typename T>
concept OArrayReallocPolicy = requires(T policy, std::size_t currentCapacity) {
    { policy(currentCapacity) } -> std::convertible_to<std::size_t>;
};

/**
 * @brief No resizing policy for OArray.
 */
struct OArrayNoResizePolicy
{
	std::size_t operator()(std::size_t) { return 0; }
};

/**
 * @brief Linear resizing policy for OArray.
 */
template <std::size_t ReallocBlock>
struct OArrayLinearResizingPolicy
{
	std::size_t operator()(std::size_t) { return ReallocBlock; }
};

/**
 * @brief Exponential resizing policy for OArray.
 */
struct OArrayExponentialResizingPolicy
{
	std::size_t operator()(std::size_t currentCapacity) { return currentCapacity == 0 ? 8 : currentCapacity; }
};

/**
 @brief Array base iterator class.
 */
template <typename Ptr_t, typename Ref_t>
class OBaseArrayIterator {
public:
	/**
	 @brief Default class constructor.
	 */
	OBaseArrayIterator() = default;

	/**
	 @brief Class constructor (meant to be used by parent class).
	 @param aIndex Array index.
	 @param aArrayPtr Pointer to the array.
	 */
	OBaseArrayIterator(std::size_t aindex, Ptr_t aArray);

	/**
	 @brief Class destructor.
	 */
	~OBaseArrayIterator() = default;

	/**
	 @brief Dereference operator.
	 */
	Ref_t operator*() const;

	/**
	 @brief Pointer access operator.
	 */
	Ptr_t operator->() const;

	/**
	 @brief Increment by one operator, moves the iterator to the next item.
	 */
	OBaseArrayIterator& operator++();

	/**
	 @brief Increment operator, moves the iterator fowards by a given number of positions.
	 @param aPositions Number of positions to move the iterator.
	 */
	OBaseArrayIterator& operator+(std::size_t aPositions);
	
	/**
	 @brief Increment by one operator, moves the iterator to the next item.
	 */
	OBaseArrayIterator& operator--();

	/**
	 @brief Increment operator, moves the iterator fowards by a given number of positions.
	 @param aPositions Number of positions to move the iterator.
	 */
	OBaseArrayIterator& operator-(std::size_t aPositions);

	/**
	 @brief Equality comparison operator.
	 @param aOther The object to be compared against.
	 */
	bool operator==(const OBaseArrayIterator& aOther) const;

	/**
	 @brief Inequeality comparison operator.
	 @param aOther The object to be compared against.
	 */
	bool operator!=(const OBaseArrayIterator& aOther) const;

private:
	std::size_t	_index		= 0;
	Ptr_t		_arrayPtr	= nullptr;
};

template<typename Ptr_t, typename Ref_t>
inline OBaseArrayIterator<Ptr_t, Ref_t>::OBaseArrayIterator(std::size_t aIndex, Ptr_t aArray) :
	_index(aIndex),
	_arrayPtr(aArray)
{
}

template<typename Ptr_t, typename Ref_t>
inline Ref_t OBaseArrayIterator<Ptr_t, Ref_t>::operator*() const
{
	return _arrayPtr[_index];
}

template<typename Ptr_t, typename Ref_t>
inline Ptr_t OBaseArrayIterator<Ptr_t, Ref_t>::operator->() const
{
	return &_arrayPtr[_index];
}

template<typename Ptr_t, typename Ref_t>
inline OBaseArrayIterator<Ptr_t, Ref_t>& OBaseArrayIterator<Ptr_t, Ref_t>::operator++()
{
	_index++;
	return *this;
}

template<typename Ptr_t, typename Ref_t>
inline OBaseArrayIterator<Ptr_t, Ref_t>& OBaseArrayIterator<Ptr_t, Ref_t>::operator+(std::size_t aPositions)
{
	_index += aPositions;
	return *this;
}

template<typename Ptr_t, typename Ref_t>
inline OBaseArrayIterator<Ptr_t, Ref_t>& OBaseArrayIterator<Ptr_t, Ref_t>::operator--()
{
	_index--;
	return *this;
}

template<typename Ptr_t, typename Ref_t>
inline OBaseArrayIterator<Ptr_t, Ref_t>& OBaseArrayIterator<Ptr_t, Ref_t>::operator-(std::size_t aPositions)
{
	_index -= aPositions;
	return *this;
}

template<typename Ptr_t, typename Ref_t>
inline bool OBaseArrayIterator<Ptr_t, Ref_t>::operator==(const OBaseArrayIterator & aOther) const
{
	return (_arrayPtr == aOther._arrayPtr && _index == aOther._index);
}

template<typename Ptr_t, typename Ref_t>
inline bool OBaseArrayIterator<Ptr_t, Ref_t>::operator!=(const OBaseArrayIterator & aOther) const
{
	return !(*this == aOther);
}

/**
 * @brief Array container.
 * 
 * @tparam T The item type.
 * @tparam ReallocPolicy Policy for automatic reallocations.
 * @tparam Allocator Memory allocator.
 */
template <typename T, 
		  OArrayReallocPolicy ReallocPolicy = OArrayNoResizePolicy, 
		  class Allocator=OSystemMemoryAllocator<OMemoryManagerScope::Default>>
class OArray : public OMemoryManagedObject<Allocator>, 
			   public ONonCopiable
{
public:
	/**
	 @brief Class constructor.
	 @param aCapacity Array capacity, to be allocated right away.
	 @param aSizeToCapacity The array size is set to capacity.
	 */
	OArray(std::size_t aCapacity = 0, bool aSizeToCapacity = false); 

	/**
	 @brief Class constructor from an initialization list.
	 @param aItems Items to initialize the array with.
	 */
	OArray(std::initializer_list<T> aItems)
	requires OCopiable<T>;

	/**
	 @brief Class constructor, sets array size to capacity and all allocated initializes members.
	 @param aCapacity Array capacity, to be allocated right away.
	 @param aInitValue Initial value of the items.
	 @param aSizeToCapacity The array size is set to capacity.
	 */
	OArray(std::size_t aCapacity, const T& aInitValue, bool aSizeToCapacity=false) 
	requires (OCopiable<T> || OClonable<T>);

	/**
	 @brief Deleted copy constructor.
	 */
	OArray(const OArray& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OArray(OArray&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OArray();

	/**
	 @brief Clones the array into another one.
	 @param aTarget The destination array.
	 */
	void cloneTo(OArray& aTarget) const
	requires (OCopiable<T> || OClonable<T>);

	/**
	 @brief Returns the array capacity.
	 */
	virtual std::size_t capacity() const;

	/**
	 @brief Returns the array size.
	 */
	virtual std::size_t size() const;

	/**
	 @brief Changes the array item count.
	 @param aSize The new array item count.
	 */
	virtual void resize(std::size_t aSize);

	/**
	 @brief Changes the array item count and initializes any new items.
	 @param aSize The new array item count.
	 @param aInitValue The initial value of the new items.
	 */
	void resizeInit(std::size_t aSize, const T& aInitValue) 
	requires (OCopiable<T> || OClonable<T>);

	/**
	 @brief Change the capacity of the array.
	 @param aNewCapacity New array capacity.
	 @param aSizeToCapacity The array size is set to capacity.
	 */
	virtual void changeCapacity(std::size_t aNewCapacity);
	
	/**
	 @brief Move assignment operator.
	 */
	OArray& operator=(OArray&& aOther);
	
	/**
	 @brief Subscript operator override.
	 @param aIndex Index of the item on the array.
	 */
	virtual T& operator[](std::size_t aIndex);

	/**
	 @brief Const subscript operator override.
	 @param aIndex Index of the item on the array.
	 */
	virtual const T& operator[](std::size_t aIndex) const;

	/**
	 @brief Add item to the end of the array.
	 @param aItemValue of the item to be added.
	 */
	void append(const T& aItemValue) requires OCopiable<T>;
	
	/**
	 @brief Add item to the end of the array.
	 @param aItemValue of the item to be added.
	 */
	virtual void append(T&& aItemValue);
	
	/**
	 @brief Set item value.
	 @param aIndex Index of the item on the array.
	 @param aValue Value to be set.
	 */
	void set(std::size_t aIndex, const T& aValue) requires OCopiable<T>;
	
	/**
	 @brief Set item value.
	 @param aIndex Index of the item on the array.
	 @param aValue Value to be set.
	 */
	virtual void set(std::size_t aIndex, T&& aValue);
	
	/**
	 @brief Gets the non-const reference to an item on the array.
	 @param aIndex Index of the item on the array.
	 */
	virtual T& get(std::size_t aIndex);

	/**
	 @brief Gets the const reference to an item on the array.
	 @param aIndex Index of the item on the array.
	 */
	virtual const T& get(std::size_t aIndex) const;

	/**
	 @brief Remove item from the array.
	 */
	virtual void remove(std::size_t aIndex);

	/**
	 @brief Clears the array, maintaining the allocated memory.
	 */
	virtual void clear();
	
	/**
	 @brief Non-const iterator class.
	 */
	using Iterator = OBaseArrayIterator<T*, T&>;

	/**
	 @brief Const iterator class.
	 */
	using ConstIterator = OBaseArrayIterator<const T*, const T&>;

	/**
	 @brief Returns an iterator pointing to the first element.
	 */
	Iterator begin();

	/**
	 @brief Retuns a constant iterator pointing the first element.
	 */
	ConstIterator begin() const;

	/**
	 @brief Returns the end iterator, past the last item.
	 */
	Iterator end();

	/**
	 @brief Retuns the end const iterator, past the last item.
	 */
	ConstIterator end() const;

	/**
	 @brief Returns a reference to the first element.
	 */
	T& front();
	
	/**
	 @brief Returns a reference to the first element.
	 */
	const T& front() const;
	
	/**
	 @brief Returns a reference to the last element.
	 */
	T& tail();
	
	/**
	 @brief Returns a reference to the last element.
	 */
	const T& tail() const;

protected:
	void onFullCapacity();

protected:
	T*				_array		= nullptr;
	std::size_t		_capacity	= 0;
	std::size_t	  	_size		= 0;
	ReallocPolicy 	_reallocPolicy;
};

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline OArray<T, ReallocPolicy, Allocator>::OArray(std::size_t aCapacity, bool aSizeToCapacity)
{
	if (aSizeToCapacity) {
		resize(aCapacity);
	} else {
		changeCapacity(aCapacity);
	}
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline OArray<T, ReallocPolicy, Allocator>::OArray(std::initializer_list<T> aItems)
requires OCopiable<T>
{
	resize(aItems.size());
	std::size_t index = 0;
	for (const auto& item : aItems) {
		_array[index++] = item;
	}
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline OArray<T, ReallocPolicy, Allocator>::OArray(std::size_t aCapacity, const T &aInitValue, bool aSizeToCapacity)
requires (OCopiable<T> || OClonable<T>)
{
	if (aSizeToCapacity) {
		resizeInit(aCapacity, aInitValue);
	} else {
		changeCapacity(aCapacity);
		if constexpr(OClonable<T>) {
			for (auto& item : *this) {
				aInitValue.cloneTo(item);
			}
		} else if constexpr (OCopiable<T>) {
			for (auto& item : *this) {
				item = aInitValue;
			}
		}
	}
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline OArray<T, ReallocPolicy, Allocator>::OArray(OArray&& aOther)
{
	_size = aOther._size;
	_capacity = aOther._capacity;
	_array = aOther._array;

	aOther._array = nullptr;
	aOther._capacity = 0;
	aOther._size = 0;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline OArray<T, ReallocPolicy, Allocator>::~OArray()
{
	if (_array != nullptr) {
		delete[] _array;
	}
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::cloneTo(OArray& aTarget) const
requires (OCopiable<T> || OClonable<T>)
{
	aTarget.resize(_size);
	if constexpr(OClonable<T>) {
		for (std::size_t i=0; i<_size;i++) {
			get(i).cloneTo(aTarget[i]);
		}
	} else if constexpr (OCopiable<T>){
		for (std::size_t i = 0; i < _size; i++) aTarget[i] = get(i);
	}
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline std::size_t OArray<T, ReallocPolicy, Allocator>::capacity() const
{
    return _capacity;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline std::size_t OArray<T, ReallocPolicy, Allocator>::size() const
{
    return _size;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::resize(std::size_t aSize)
{
	if (aSize > _capacity) changeCapacity(aSize);
	_size = aSize;
}

template<typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::resizeInit(std::size_t aSize, const T & aInitValue)
requires (OCopiable<T> || OClonable<T>)
{
	auto currSize = _size;
	resize(aSize);
	if constexpr(OClonable<T>) {
		for (std::size_t i = currSize; i < _size; i++) {
			aInitValue.cloneTo(_array[i]);
		}
	} else if constexpr(std::assignable_from<T&, const T&>) {
		for (std::size_t i = currSize; i < _size; i++) {
			_array[i] = aInitValue;
		}
	} 
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::changeCapacity(std::size_t aNewCapacity)
{
	if (aNewCapacity == _capacity) return;

	T* new_array = nullptr;
	if (aNewCapacity > 0) {
		new_array = new T[aNewCapacity];
		OExPointerCheck(new_array);
	} 

	std::size_t itemCount = (aNewCapacity > _size) ? _size : aNewCapacity;
	if (_array) {
		for (std::size_t i = 0; i < itemCount; i++) {
			new_array[i] = std::move(_array[i]);
		}
		delete[] _array;
	}

	_array = new_array;
	_capacity = aNewCapacity;
	if (_size > _capacity) _size = _capacity;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline OArray<T, ReallocPolicy, Allocator> &OArray<T, ReallocPolicy, Allocator>::operator=(OArray&& aOther)
{
	if (_array != nullptr) {
		delete[] _array;
	}
	_array = aOther._array;
	_capacity = aOther._capacity;
	_size = aOther._size;

	aOther._array = nullptr;
	aOther._capacity = 0;
	aOther._size = 0;

	return *this;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline T& OArray<T, ReallocPolicy, Allocator>::operator[](std::size_t aIndex)
{
	if (aIndex > _size) {
		throw OEx("Invalid index");
	}
	return _array[aIndex];
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline const T& OArray<T, ReallocPolicy, Allocator>::operator[](std::size_t aIndex) const
{
	if (aIndex > _size) {
		throw OEx("Invalid index");
	}
	return _array[aIndex];
}

template<typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::append(const T& aItemValue)
requires OCopiable<T>
{
	if (_size == _capacity) {
		onFullCapacity();
	}
	_array[_size++] = aItemValue;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::append(T &&aItemValue)
{
	if (_size == _capacity) {
		onFullCapacity();
	}
	_array[_size++] = std::move(aItemValue);
}

template<typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::set(std::size_t aIndex, const T & aValue)
requires OCopiable<T>
{
	get(aIndex) = aValue;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::set(std::size_t aIndex, T &&aValue)
{
	get(aIndex) = std::move(aValue);
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline T &OArray<T, ReallocPolicy, Allocator>::get(std::size_t aIndex)
{
	if (aIndex >= _capacity) {
		throw OEx("Invalid array index.");
	}
	if (aIndex >= _size) {
		_size = aIndex + 1;
	}
	return _array[aIndex];
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline const T &OArray<T, ReallocPolicy, Allocator>::get(std::size_t aIndex) const
{
	if (aIndex >= _size) {
		throw OEx("Invalid array index.");
	}
	return _array[aIndex];
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::remove(std::size_t aIndex)
{
	if (aIndex >= _size) {
		throw OEx("Invalid array index.");
	}
	for (std::size_t i = aIndex + 1; i < _size; i++) {
		_array[i - 1] = std::move(_array[i]);
	}
	_size--;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::clear()
{
	_size = 0;
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline typename OArray<T, ReallocPolicy, Allocator>::Iterator
OArray<T, ReallocPolicy, Allocator>::begin()
{
	return Iterator(0, _array);
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline typename OArray<T, ReallocPolicy,Allocator>::ConstIterator 
OArray<T, ReallocPolicy, Allocator>::begin() const
{
	return ConstIterator(0, _array);
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline typename OArray<T, ReallocPolicy, Allocator>::Iterator 
OArray<T, ReallocPolicy, Allocator>::end()
{
	return Iterator(_size, _array);
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline typename OArray<T, ReallocPolicy,Allocator>::ConstIterator 
OArray<T, ReallocPolicy, Allocator>::end() const
{
	return ConstIterator(_size, _array);
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline T& OArray<T, ReallocPolicy, Allocator>::front()
{
	return _array[0];
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline const T& OArray<T, ReallocPolicy, Allocator>::front() const
{
	return _array[0];
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline T& OArray<T, ReallocPolicy, Allocator>::tail()
{
	return _array[_size - 1];
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline const T& OArray<T, ReallocPolicy, Allocator>::tail() const
{
	return _array[_size - 1];
}

template <typename T, OArrayReallocPolicy ReallocPolicy, class Allocator>
inline void OArray<T, ReallocPolicy, Allocator>::onFullCapacity()
{
	auto addedCapacity = _reallocPolicy(_capacity);
	if (addedCapacity == 0)
	{
		throw OEx("Array overflow");
	}
	changeCapacity(_capacity + addedCapacity);
}

/**
 * @brief Alias for a dynamicly allocated array with exponential growth policy.
 */
template <typename T, class Allocator=OSystemMemoryAllocator<OMemoryManagerScope::Default>>
using ODynArray = OArray<T, OArrayExponentialResizingPolicy, Allocator>;


