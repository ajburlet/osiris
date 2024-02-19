#pragma once

#include <type_traits>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMemoryManager.h"
#include "OsirisSDK/OAbstractMemoryAllocator.hpp"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OMemoryManagedObject.h"


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
 @brief Array of non-copyable items.

 This array implementation avoids all object copy operations. Insertions must be done via r-value references.
 */
template <typename T, class Allocator=OSystemMemoryAllocator<OMemoryManagerScope::Default>>
class OArrayNC : public OMemoryManagedObject<Allocator>, public ONonCopiableT<OArrayNC<T,Allocator>>
{
public:
	/**
	 @brief Class constructor.
	 @param aCapacity Array capacity, to be allocated right away.
	 @param aSizeToCapacity The array size is set to capacity.
	 */
	OArrayNC(uint32_t aCapacity=0, bool aSizeToCapacity=false);

	/**
	 @brief Move constructor.
	 */
	OArrayNC(OArrayNC&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OArrayNC();

	/**
	 @brief Clones the array into another one.
	 @param aTarget The destination array.
	 */
	void cloneTo(OArrayNC& aTarget) const override;

	/**
	 @brief Returns the array capacity.
	 */
	virtual uint32_t capacity() const;

	/**
	 @brief Returns the array size.
	 */
	virtual uint32_t size() const;

	/**
	 @brief Changes the array item count.
	 @param aSize The new array item count.
	 */
	virtual void resize(uint32_t aSize);

	/**
	 @brief Change the capacity of the array.
	 @param aNewCapacity New array capacity.
	 @param aSizeToCapacity The array size is set to capacity.
	 */
	virtual void changeCapacity(uint32_t aNewCapacity);

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
	virtual void set(uint32_t aIndex, T&& aValue);

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
	 @brief Clears the array, maintaining the allocated memory.
	 */
	virtual void clear();

	/**
	 @brief Deleted assignment operator.
	 */
	OArrayNC& operator=(const OArrayNC& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OArrayNC& operator=(OArrayNC&& aOther);

	/**
	 @brief Subscript operator override.
	 @param aIndex Index of the item on the array.
	 */
	virtual T& operator[](uint32_t aIndex);

	/**
	 @brief Const subscript operator override.
	 @param aIndex Index of the item on the array.
	 */
	virtual const T& operator[](uint32_t aIndex) const;

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
	T*		_array		= nullptr;
	uint32_t	_capacity	= 0;
	uint32_t	_size		= 0;
};

template<typename T, class Allocator>
inline OArrayNC<T, Allocator>::OArrayNC(uint32_t aCapacity, bool aSizeToCapacity)
{
	if (aSizeToCapacity) {
		resize(aCapacity);
	} else {
		changeCapacity(aCapacity);
	}
}

template<typename T, class Allocator>
inline OArrayNC<T, Allocator>::OArrayNC(OArrayNC && aOther)
{
	_size = aOther._size;
	_capacity = aOther._capacity;
	_array = aOther._array;

	aOther._array = nullptr;
	aOther._capacity = 0;
	aOther._size = 0;
}

template<typename T, class Allocator>
inline OArrayNC<T, Allocator>::~OArrayNC()
{
	if (_array != nullptr) delete[] _array;
}

template<typename T, class Allocator>
inline void OArrayNC<T, Allocator>::cloneTo(OArrayNC & aTarget) const
{
	aTarget.changeCapacity(_size);
	if constexpr(std::is_base_of<ONonCopiable,T>::value) {
		for (uint32_t i=0; i<_size;i++) {
			get(i).cloneTo(aTarget[i]);
		}
	} else {
		for (uint32_t i = 0; i < _size; i++) aTarget[i] = get(i);
	}
}

template<typename T, class Allocator>
inline uint32_t OArrayNC<T, Allocator>::capacity() const
{
	return _capacity;
}

template<typename T, class Allocator>
inline uint32_t OArrayNC<T, Allocator>::size() const
{
	return _size;
}

template<typename T, class Allocator>
inline void OArrayNC<T, Allocator>::resize(uint32_t aSize)
{
	if (aSize > _capacity) changeCapacity(aSize);
	_size = aSize;
}

template<typename T, class Allocator>
inline void OArrayNC<T, Allocator>::changeCapacity(uint32_t aNewCapacity)
{
	if (aNewCapacity == _capacity) return;

	T* new_array = nullptr;
	if (aNewCapacity > 0) {
		new_array = new T[aNewCapacity];
		OExPointerCheck(new_array);
	} 

	uint32_t itemCount = (aNewCapacity > _size) ? _size : aNewCapacity;
	if (_array) {
		for (uint32_t i = 0; i < itemCount; i++) {
			new_array[i] = std::move(_array[i]);
		}
		delete[] _array;
	}

	_array = new_array;
	_capacity = aNewCapacity;
	if (_size > _capacity) _size = _capacity;
}

template<typename T, class Allocator>
inline void OArrayNC<T, Allocator>::append(T && aItemValue)
{
	if (_size == _capacity) {
		throw OEx("Array overflow.");
	}
	_array[_size++] = std::move(aItemValue);
}

template<typename T, class Allocator>
inline void OArrayNC<T, Allocator>::set(uint32_t aIndex, T&& aValue)
{
	get(aIndex) = std::move(aValue);
}

template<typename T, class Allocator>
inline T& OArrayNC<T, Allocator>::get(uint32_t aIndex)
{
	if (aIndex >= _capacity) {
		throw OException(__FILE__, __LINE__, "Invalid array index.");
	}
	if (aIndex >= _size) {
		_size = aIndex + 1;
	}
	return _array[aIndex];
}

template<typename T, class Allocator>
inline const T & OArrayNC<T, Allocator>::get(uint32_t aIndex) const
{
	if (aIndex >= _size) {
		throw OEx("Invalid array index.");
	}
	return _array[aIndex];
}

template<typename T, class Allocator>
inline void OArrayNC<T, Allocator>::remove(uint32_t aIndex)
{
	if (aIndex >= _size) {
		throw OEx("Invalid array index.");
	}
	for (uint32_t i = aIndex + 1; i < _size; i++) {
		_array[i - 1] = std::move(_array[i]);
	}
	_size--;
}

template<typename T, class Allocator>
inline void OArrayNC<T, Allocator>::clear()
{
	_size = 0;
}

template<typename T, class Allocator>
inline OArrayNC<T, Allocator>& OArrayNC<T, Allocator>::operator=(OArrayNC && aOther)
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

template<typename T, class Allocator>
inline T & OArrayNC<T, Allocator>::operator[](uint32_t aIndex)
{
	return get(aIndex);
}

template<typename T, class Allocator>
inline const T & OArrayNC<T, Allocator>::operator[](uint32_t aIndex) const
{
	return get(aIndex);
}

template<typename T, class Allocator>
inline typename OArrayNC<T,Allocator>::Iterator OArrayNC<T, Allocator>::begin()
{
	return Iterator(0, _array);
}

template<typename T, class Allocator>
inline typename OArrayNC<T, Allocator>::ConstIterator OArrayNC<T, Allocator>::begin() const
{
	return ConstIterator(0, _array);
}

template<typename T, class Allocator>
inline typename OArrayNC<T,Allocator>::Iterator OArrayNC<T, Allocator>::end()
{
	return Iterator(_size, _array);
}

template<typename T, class Allocator>
inline typename OArrayNC<T, Allocator>::ConstIterator OArrayNC<T, Allocator>::end() const
{
	return ConstIterator(_size, _array);
}

template<typename T, class Allocator>
inline T & OArrayNC<T, Allocator>::front()
{
	return _array[0];
}

template<typename T, class Allocator>
inline const T & OArrayNC<T, Allocator>::front() const
{
	return _array[0];
}

template<typename T, class Allocator>
inline T & OArrayNC<T, Allocator>::tail()
{
	return _array[_size - 1];
}

template<typename T, class Allocator>
inline const T & OArrayNC<T, Allocator>::tail() const
{
	return _array[_size - 1];
}

/**
 @brief Array of copyable items.
 */
template <typename T, class Allocator=OSystemMemoryAllocator<OMemoryManagerScope::Default>>
class OArray : public OArrayNC<T, Allocator>
{
private:
	using Super = OArrayNC<T, Allocator>;
public:
	/**
	 @brief Class constructor.
	 @param aCapacity Array capacity, to be allocated right away.
	 @param aSizeToCapacity The array size is set to capacity.
	 */
	OArray(uint32_t aCapacity = 0, bool aSizeToCapacity = false) : Super(aCapacity, aSizeToCapacity) {}

	/**
	 @brief Class constructor, sets array size to capacity and all allocated initializes members.
	 @param aCapacity Array capacity, to be allocated right away.
	 @param aInitValue Initial value of the items.
	 @param aSizeToCapacity The array size is set to capacity.
	 */
	OArray(uint32_t aCapacity, const T& aInitValue, bool aSizeToCapacity=false);

	/**
	 @brief Deleted copy constructor.
	 */
	OArray(const OArray& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OArray(OArray&& aOther) : Super(std::move(aOther)) {}

	/**
	 @brief Deleted copy assignment operator.
	 */
	OArray& operator=(const OArray& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OArray& operator=(OArray&& aOther) { Super::operator=(std::move(aOther)); return *this; }
	
	/**
	 @brief Changes the array item count and initializes any new items.
	 @param aSize The new array item count.
	 @param aInitValue The initial value of the new items.
	 */
	virtual void resizeInit(uint32_t aSize, const T& aInitValue);

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

};

template<typename T, class Allocator>
inline OArray<T, Allocator>::OArray(uint32_t aCapacity, const T & aInitValue, bool aSizeToCapacity)
{
	if (aSizeToCapacity) {
		resizeInit(aCapacity, aInitValue);
	} else {
		changeCapacity(aCapacity);
		for (auto& item : *this) item = aInitValue;
	}
}

template<typename T, class Allocator>
inline void OArray<T, Allocator>::resizeInit(uint32_t aSize, const T & aInitValue)
{
	auto currSize = _size;
	resize(aSize);
	for (uint32_t i = currSize; i < _size; i++) _array[i] = aInitValue;
}

template<typename T, class Allocator>
inline void OArray<T, Allocator>::append(const T& aItemValue)
{
	if (_size == _capacity) {
		throw OEx("Array overflow.");
	}
	_array[_size++] = aItemValue;
}

template<typename T, class Allocator>
inline void OArray<T, Allocator>::set(uint32_t aIndex, const T & aValue)
{
	get(aIndex) = aValue;
}


/**
 @brief Dynamic array handler class.

 The difference from OArray is that on the <code>append()</code> method the array will increase
 in size if necessary. The same wont apply to <code>get()</code> or <code>set()</code> methods.
 */
template<typename T, 
	 class Allocator=OSystemMemoryAllocator<OMemoryManagerScope::Default>, 
	 size_t BlockSize=1>
class ODynArray : public OArray<T, Allocator>
{
private:
	using Super = OArray<T, Allocator>;

public:
	/**
	 @copydoc OArray(uint32_t,bool)
	 */
	ODynArray(uint32_t aCapacity=0, bool aSizeToCapacity=false);
	
	/**
	 @copydoc OArray(uint32_t, const T&, bool)
	 */
	ODynArray(uint32_t aCapacity, const T& aInitValue, bool aSizeToCapacity=false);

	/**
	 @copydoc OArray(OArray&&)
	 */
	ODynArray(ODynArray&& aOther);

	/**
	 @brief Class destructor.
	 */
	~ODynArray() = default;

	/**
	 @brief Deleted copy assignment operator.
	 */
	ODynArray& operator=(const ODynArray& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	ODynArray& operator=(ODynArray&& aOther) { return Super::operator=(std::move(aOther)); }

	using Super::resize;
	using Super::resizeInit;

	virtual void changeCapacity(uint32_t aNewCapacity) override;

	virtual void append(const T& aItemValue) override;
	
	T& operator[](uint32_t aIndex) override;

	const T& operator[](uint32_t aIndex) const override;
};

template<typename T, class Allocator, size_t BlockSize>
inline ODynArray<T, Allocator, BlockSize>::ODynArray(uint32_t aCapacity, bool aSizeToCapacity)
{
	if (aSizeToCapacity) {
		resize(aCapacity);
	} else {
		changeCapacity(aCapacity);
	}
}

template<typename T, class Allocator, size_t BlockSize>
inline ODynArray<T, Allocator, BlockSize>::ODynArray(uint32_t aCapacity, const T & aInitValue, bool aSizeToCapacity) 
{
	if (aSizeToCapacity) {
		resizeInit(aCapacity, aInitValue);
	} else {
		changeCapacity(aCapacity);
		for (auto& item : *this) item = aInitValue;
	}
}

template<typename T, class Allocator, size_t BlockSize>
inline ODynArray<T, Allocator, BlockSize>::ODynArray(ODynArray && aOther) :
	Super(std::move(aOther))
{
}

template<typename T, class Allocator, size_t BlockSize>
inline void ODynArray<T, Allocator, BlockSize>::changeCapacity(uint32_t aNewCapacity)
{
	auto origSize = _size;
	Super::changeCapacity(aNewCapacity + (BlockSize - (aNewCapacity % BlockSize)));
	if (_size > aNewCapacity) _size = aNewCapacity;
}

template<typename T, class Allocator, size_t BlockSize>
inline void ODynArray<T, Allocator, BlockSize>::append(const T & aItemValue)
{
	if (_capacity == _size) {
		changeCapacity(_capacity + 1);
	}
	Super::append(aItemValue);
}

template<typename T, class Allocator, size_t BlockSize>
inline T & ODynArray<T, Allocator, BlockSize>::operator[](uint32_t aIndex)
{
	if (aIndex >= _capacity) changeCapacity(aIndex + 1);
	return Super::operator[](aIndex);
}

template<typename T, class Allocator, size_t BlockSize>
inline const T & ODynArray<T, Allocator, BlockSize>::operator[](uint32_t aIndex) const
{
	return const_cast<ODynArray*>(this)->operator[](aIndex);
}

