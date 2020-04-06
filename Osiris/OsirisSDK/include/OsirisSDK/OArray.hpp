#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMemoryManager.h"
#include "OsirisSDK/OAbstractMemoryAllocator.hpp"
#include "OsirisSDK/OSystemMemoryAllocator.h"


/**
 @brief Array base iterator class.
 */
template <typename Ptr_t, typename Ref_t>
class OAPI OBaseArrayIterator {
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
	OBaseArrayIterator(uint32_t aindex, Ptr_t aArray);

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
	OBaseArrayIterator& operator+(uint32_t aPositions);
	
	/**
	 @brief Increment by one operator, moves the iterator to the next item.
	 */
	OBaseArrayIterator& operator--();

	/**
	 @brief Increment operator, moves the iterator fowards by a given number of positions.
	 @param aPositions Number of positions to move the iterator.
	 */
	OBaseArrayIterator& operator-(uint32_t aPositions);

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
	uint32_t	_index		= 0;
	Ptr_t		_arrayPtr	= nullptr;
};

template<typename Ptr_t, typename Ref_t>
inline OBaseArrayIterator<Ptr_t, Ref_t>::OBaseArrayIterator(uint32_t aIndex, Ptr_t aArray) :
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
inline OBaseArrayIterator<Ptr_t, Ref_t>& OBaseArrayIterator<Ptr_t, Ref_t>::operator+(uint32_t aPositions)
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
inline OBaseArrayIterator<Ptr_t, Ref_t>& OBaseArrayIterator<Ptr_t, Ref_t>::operator-(uint32_t aPositions)
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
 @brief Array handling class.
 */
template <typename T, class Allocator=OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
class OAPI OArray 
{
public:
	/**
	 @brief Class constructor.
	 @brief aCapacity Array capacity, to be allocated right away.
	 @brief aSizeToCapacity If true sets array size to capacity.
	 */
	OArray(uint32_t aCapacity=0, bool aSizeToCapacity=false);

	/**
	 @brief Class destructor.
	 */
	virtual ~OArray();

	/**
	 @brief Returns the array capacity.
	 */
	virtual uint32_t capacity() const;

	/**
	 @brief Returns the array size.
	 */
	virtual uint32_t size() const;

	/**
	 @brief Change the capacity of the array.
	 @param aNewCapacity New array capacity.
	 */
	virtual void resize(uint32_t aNewCapacity);

	/**
	 @brief Add item to the end of the array.
	 @param aItemValue of the item to be added.
	 */
	virtual void append(const T& aItemValue);

	/**
	 @brief Set item value.
	 @param aIndex Index of the item on the array.
	 @param aValue Value to be set.
	 */
	virtual void set(uint32_t aIndex, const T& aValue);

	/**
	 @brief Gets the non-const reference to an item on the array.
	 @param aIndex Index of the item on the array.
	 */
	virtual T& get(uint32_t aIndex);

	/**
	 @brief Gets the const reference to an item on the array.
	 @param aIndex Index of the item on the array.
	 */
	virtual const T& get(uint32_t aIndex) const;

	/**
	 @brief Remove item from the array.
	 */
	virtual void remove(uint32_t aIndex);

	/**
	 @brief Subscript operator override.
	 @param aIndex Index of the item on the array.
	 */
	T& operator[](uint32_t aIndex);

	/**
	 @brief Const subscript operator override.
	 @param aIndex Index of the item on the array.
	 */
	const T& operator[](uint32_t aIndex) const;

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

protected:
	T*		_array		= nullptr;
	uint32_t	_capacity	= 0;
	uint32_t	_size		= 0;
};

template<typename T, class Allocator>
inline OArray<T, Allocator>::OArray(uint32_t aCapacity, bool aSizeToCapacity)
{
	if ((_array = static_cast<T*>(Allocator().allocate(aCapacity))) == nullptr) {
		throw OException("Unable to allocate memory for array.");
	}
	_capacity = aCapacity;
	_size = (aSizeToCapacity) ? _capacity : 0;
}

template<typename T, class Allocator>
inline OArray<T, Allocator>::~OArray()
{
	if (_array != nullptr) {
		Allocator().deallocate(_array, _capacity*sizeof(T));
	}
}

template<typename T, class Allocator>
inline uint32_t OArray<T, Allocator>::capacity() const
{
	return _capacity;
}

template<typename T, class Allocator>
inline uint32_t OArray<T, Allocator>::size() const
{
	return _size;
}

template<typename T, class Allocator>
inline void OArray<T, Allocator>::resize(uint32_t aNewCapacity)
{
	auto new_array = static_cast<T*>(Allocator().reallocate(_array,
								_capacity*sizeof(T),
								aNewCapacity*sizeof(T)));
	if (new_array == nullptr) {
		throw OException("Failed to reallocate array.");
	}
	_array = new_array;
	_capacity = aNewCapacity;
	if (aNewCapacity < _size) {
		_size = aNewCapacity;
	}
}

template<typename T, class Allocator>
inline void OArray<T, Allocator>::append(const T& aItemValue)
{
	if (_size == _capacity) {
		throw OException("Array overflow.");
	}
	_array[_size++] = aItemValue;
}

template<typename T, class Allocator>
inline void OArray<T, Allocator>::set(uint32_t aIndex, const T& aValue)
{
	get(aIndex) = aValue;
}

template<typename T, class Allocator>
inline T& OArray<T, Allocator>::get(uint32_t aIndex)
{
	if (aIndex >= _capacity) {
		throw OException("Invalid array index.");
	}
	if (aIndex >= _size) {
		_size = aIndex + 1;
	}
	return _array[aIndex];
}

template<typename T, class Allocator>
inline const T & OArray<T, Allocator>::get(uint32_t aIndex) const
{
	if (aIndex >= _size) {
		throw OException("Invalid array index.");
	}
	return _array[aIndex];
}

template<typename T, class Allocator>
inline void OArray<T, Allocator>::remove(uint32_t aIndex)
{
	if (aIndex >= _size) {
		throw OException("Invalid array index.");
	}
	for (uint32_t i = aIndex + 1; i < _size; i++) {
		_array[aIndex - 1] = _array[aIndex];
	}
	_size--;
}

template<typename T, class Allocator>
inline T & OArray<T, Allocator>::operator[](uint32_t aIndex)
{
	return get(aIndex);
}

template<typename T, class Allocator>
inline const T & OArray<T, Allocator>::operator[](uint32_t aIndex) const
{
	return get(aIndex);
}

template<typename T, class Allocator>
inline typename OArray<T,Allocator>::Iterator OArray<T, Allocator>::begin()
{
	return Iterator(0, _array);
}

template<typename T, class Allocator>
inline typename OArray<T, Allocator>::ConstIterator OArray<T, Allocator>::begin() const
{
	return ConstIterator(0, _array);
}

template<typename T, class Allocator>
inline typename OArray<T,Allocator>::Iterator OArray<T, Allocator>::end()
{
	return Iterator(_size, _array);
}

template<typename T, class Allocator>
inline typename OArray<T, Allocator>::ConstIterator OArray<T, Allocator>::end() const
{
	return ConstIterator(_size, _array);
}

/**
 @brief Dynamic array handler class.

 The difference from OArray is that on the <code>append()</code> method the array will increase
 in size if necessary. The same wont apply to <code>get()</code> or <code>set()</code> methods.
 */
template<typename T, 
	 class Allocator=OSystemMemoryAllocator<OMemoryManager::Scope::Default>, 
	 size_t BlockSize=1>
class ODynArray : public OArray<T, Allocator>
{
private:
	using Super = OArray<T, Allocator>;

public:
	/**
	 @copydoc OArray(uint32_t, bool)
	 */
	ODynArray(uint32_t aCapacity=0, bool aSizeToCapacity = false);

	/**
	 @brief Class destructor.
	 */
	~ODynArray() = default;

	/**
	 @copydoc OArray::append(const T&)
	 */
	virtual void append(const T& aItemValue) override;
};

template<typename T, class Allocator, size_t BlockSize>
inline ODynArray<T, Allocator, BlockSize>::ODynArray(uint32_t aCapacity, bool aSizeToCapacity) :
	Super(aCapacity, aSizeToCapacity)
{
}

template<typename T, class Allocator, size_t BlockSize>
inline void ODynArray<T, Allocator, BlockSize>::append(const T & aItemValue)
{
	if (_capacity == _size) {
		resize(_capacity + BlockSize);
	}
	Super::append(aItemValue);
}

