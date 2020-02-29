#pragma once

#include <map>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"

/**
 @brief The map, dictionary or binary tree implementation.
 */
template<class TKey, 
	 class TValue, 
	 class Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Default>,
	 class Compare = std::less<TKey>>
class OMap
{
protected:
	using MapType = std::map<TKey, TValue, Compare, OSTLAllocator<Allocator, std::pair<const TKey, TValue>>>;
	using MapIterator = typename MapType::iterator;
	using MapConstIterator = typename MapType::const_iterator;

public:
	/**
	 @brief Iterator class for OMap.
	 */
	class Iterator {
	public:
		/**
		 @brief Class default constructor.
		 */
		Iterator() = default;

		/**
		 @brief Class constructor.
		 @param aOrigIterator The original iterator.
		 */
		Iterator(MapIterator& aOrigIterator);

		/**
		 @brief Class copy constructor.
		 @param aOther The iterator to be copied.
		 */
		Iterator(const Iterator& aOther);

		/**
		 @brief Class destructor.
		 */
		virtual ~Iterator() = default;

		/**
		 @brief Return the key value.
		 */
		TKey& key();

		/**
		 @brief Returns the value.
		 */
		TValue& value();

		/**
		 @brief Moves the iterator forward.
		 */
		Iterator& operator++();

		/**
		 @brief Moves the iterator backwards.
		 */
		Iterator& operator--();

		/**
		 @brief Assignment operator.
		 */
		Iterator& operator=(const Iterator& aOther);

		/**
		 @brief Equal comparison operator.
		 */
		bool operator==(const Iterator& aOther) const;

	protected:
		MapIterator _it;
	};

	/**
	 @brief Constant iterator for OMap.
	 */
	class ConstIterator {
	public:
		/**
		 @brief Class default constructor.
		 */
		ConstIterator() = default;

		/**
		 @brief Class constructor.
		 @param aOrigIterator The original iterator.
		 */
		ConstIterator(MapConstIterator& aOrigIterator);

		/**
		 @brief Class copy constructor.
		 @param aOther The iterator to be copied.
		 */
		ConstIterator(const ConstIterator& aOther);

		/**
		 @brief Class destructor.
		 */
		virtual ~ConstIterator() = default;

		/**
		 @brief Return the key value.
		 */
		const TKey& key() const;

		/**
		 @brief Returns the value.
		 */
		const TValue& value() const;

		/**
		 @brief Moves the iterator forward.
		 */
		ConstIterator& operator++();

		/**
		 @brief Moves the iterator backwards.
		 */
		ConstIterator& operator--();

		/**
		 @brief Assignment operator.
		 */
		ConstIterator& operator=(const ConstIterator& aOther);

		/**
		 @brief Equal comparison operator.
		 */
		bool operator==(const ConstIterator& aOther) const;

	private:
		MapConstIterator _it;
	};

	/**
	 @brief Class default constructor.
	 */
	OMap() = default;

	/**
	 @brief Class destructor.
	 */
	virtual ~OMap() = default;

	/**
	 @brief Returns an iterator that points to the beginning of the collection.
	 */
	Iterator begin();

	/**
	 @brief Returns an iterator to the end of the collection. 
	 */
	Iterator end();

	/**
	 @brief Returns a constant iterator that points to the beginning of the collection.
	 */
	ConstIterator cbegin() const;

	/**
	 @brief Returns a constant iterator to the end of the collection. 
	 */
	ConstIterator cend() const;
	
	/**
	 @brief Returns the number of items in the collection.
	 */
	uint32_t count() const;

	/**
	 @brief Find an element by it's key.
	 @param aKey The element key.
	 @return An iterator pointing to the element related to the given key, or iterator 
		 pointing to the end.
	 */
	Iterator find(const TKey& aKey);

	/**
	 @brief Find an element by it's key (const).
	 @param aKey The element key.
	 @return An iterator pointing to the element related to the given key, or iterator 
		 pointing to the end.
	 */
	ConstIterator find(const TKey& aKey) const;

	/**
	 @brief Inserts a value in the map.
	 @param aKey The key related to the value given.
	 @param aValue The value to be inserted.
	 @param aItemAtMap Pointer to the iterator related to the inserted value (if not NULL).
	 @return True if the value is added, false if there was a value previously assigned to the key.
	 */
	bool insert(const TKey& aKey, const TValue& aValue, Iterator* aItemAtMap = nullptr);

	/**
	 @brief Subscript operators, gives access to the value given by a key.
	 @param aKey The key related to the value we are interested in.
	 @return A reference to the value related to the key.
	 */
	TValue& operator[](const TKey& aKey);

protected:
	MapType _map;
};

// ------------------------------------------------------------------------------------------
// INLINE IMPLEMENTATION - Iterator
// ------------------------------------------------------------------------------------------
template<typename TKey, typename TValue, class Allocator, class Compare>
OMap<TKey, TValue, Allocator, Compare>::Iterator::Iterator(MapIterator& aOrigIterator) :
	_it(aOrigIterator)
{

}

template<class TKey, class TValue, class Allocator, class Compare>
inline OMap<TKey, TValue, Allocator, Compare>::Iterator::Iterator(const Iterator & aOther) :
	_it(aOther._it)
{
}

template<typename TKey, typename TValue, class Allocator, class Compare>
TKey& OMap<TKey, TValue, Allocator, Compare>::Iterator::key()
{
	return _it->first;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline TValue & OMap<TKey, TValue, Allocator, Compare>::Iterator::value()
{
	return _it->second;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::Iterator & OMap<TKey, TValue, Allocator, Compare>::Iterator::operator++()
{
	_it++;
	return *this;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::Iterator & OMap<TKey, TValue, Allocator, Compare>::Iterator::operator--()
{
	_it--;
	return *this;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::Iterator & OMap<TKey, TValue, Allocator, Compare>::Iterator::operator=(const Iterator& aOther)
{
	_it = aOther._it;
	return *this;
}

template<class TKey, class TValue, class Allocator, class Compare>
inline bool OMap<TKey, TValue, Allocator, Compare>::Iterator::operator==(const Iterator & aOther) const
{
	return (_it == aOther._it);
}

// ------------------------------------------------------------------------------------------
// INLINE IMPLEMENTATION - ConstIterator
// ------------------------------------------------------------------------------------------
template<typename TKey, typename TValue, class Allocator, class Compare>
inline OMap<TKey, TValue, Allocator, Compare>::ConstIterator::ConstIterator(MapConstIterator & aOrigIterator) :
	_it(aOrigIterator)
{
}

template<class TKey, class TValue, class Allocator, class Compare>
inline OMap<TKey, TValue, Allocator, Compare>::ConstIterator::ConstIterator(const ConstIterator & aOther) :
	_it(aOther._it)
{
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline const TKey & OMap<TKey, TValue, Allocator, Compare>::ConstIterator::key() const
{
	return _it->first;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline const TValue & OMap<TKey, TValue, Allocator, Compare>::ConstIterator::value() const
{
	return _it->second;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::ConstIterator & OMap<TKey, TValue, Allocator, Compare>::ConstIterator::operator++()
{
	_it++;
	return *this;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::ConstIterator & OMap<TKey, TValue, Allocator, Compare>::ConstIterator::operator--()
{
	_it--;
	return *this;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::ConstIterator & OMap<TKey, TValue, Allocator, Compare>::ConstIterator::operator=(const ConstIterator & aOther)
{
	_it = aOther._it;
	return *this;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline bool OMap<TKey, TValue, Allocator, Compare>::ConstIterator::operator==(const ConstIterator & aOther) const
{
	return (_it == aOther._it);
}

// ------------------------------------------------------------------------------------------
// INLINE IMPLEMENTATION - OMap
// ------------------------------------------------------------------------------------------
template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::Iterator OMap<TKey, TValue, Allocator, Compare>::begin()
{
	return Iterator(_map.begin());
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::Iterator OMap<TKey, TValue, Allocator, Compare>::end()
{
	return Iterator(_map.end());
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::ConstIterator OMap<TKey, TValue, Allocator, Compare>::cbegin() const
{
	return ConstIterator(_map.cbegin());
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::ConstIterator OMap<TKey, TValue, Allocator, Compare>::cend() const
{
	return ConstIterator(_map.cend());
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline uint32_t OMap<TKey, TValue, Allocator, Compare>::count() const
{
	return _map.count();
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::Iterator OMap<TKey, TValue, Allocator, Compare>::find(const TKey & aKey)
{
	return Iterator(_map.find(aKey));
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline typename OMap<TKey, TValue, Allocator, Compare>::ConstIterator OMap<TKey, TValue, Allocator, Compare>::find(const TKey & aKey) const
{
	return ConstIterator(_map.find(aKey));
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline bool OMap<TKey, TValue, Allocator, Compare>::insert(const TKey & aKey, const TValue & aValue, Iterator * aItemAtMap)
{
	std::pair<MapIterator,bool> pair = _map.insert(std::pair<TKey,TValue>(aKey, aValue));
	if (aItemAtMap != nullptr) {
		*aItemAtMap = pair.first;
	}
	return pair.second;
}

template<typename TKey, typename TValue, class Allocator, class Compare>
inline TValue & OMap<TKey, TValue, Allocator, Compare>::operator[](const TKey & aKey)
{
	Iterator it = find(aKey);
	if (it == end()) {
		insert(aKey, TValue(), &it);
	}
	return it.value();
}
