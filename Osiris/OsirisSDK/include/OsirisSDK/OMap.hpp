#pragma once

#include <map>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OMemoryManagedObject.h"

/**
 @brief The map, dictionary or binary tree implementation.
 */
template<class TKey, 
	 class TValue, 
	 class Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Default>,
	 class Compare = std::less<TKey>>
class OMap : public OMemoryManagedObject<Allocator>
{
protected:
	using MapType = std::map<TKey, TValue, Compare, OSTLAllocator<Allocator, std::pair<const TKey, TValue>>>;
	using MapIterator = typename MapType::iterator;
	using MapConstIterator = typename MapType::const_iterator;

public:
	template <class MapIteratorType>
	class BaseIterator : public OMemoryManagedObject<Allocator>
	{
	public:
		/**
		 @brief Default class constructor.
		 */
		BaseIterator() = default;
		
		/**
		 @brief Class constructor.
		 @param aOrigIterator The original iterator.
		 */
		BaseIterator(MapIteratorType& aOrigIterator) : _it(aOrigIterator) {}

		/**
		 @brief Class copy constructor.
		 @param aOther The iterator to be copied.
		 */
		BaseIterator(const BaseIterator& aOther) : _it(aOther._it) {}

		/**
		 @brief Class destructor.
		 */
		virtual ~BaseIterator() = default;

		/**
		 @brief Return the key value.
		 */
		const TKey& key() const { return _it->first; };

		/**
		 @brief Returns the value.
		 */
		TValue& value() { return _it->second; }

		/**
		 @brief Moves the iterator forward.
		 */
		BaseIterator& operator++() { ++_it; return *this; }

		/**
		 @brief Moves the iterator backwards.
		 */
		BaseIterator& operator--() { --_it; return *this; }
		
		/**
		 @brief Moves the iterator forward.
		 */
		BaseIterator operator++(int) { auto tmp = *this; ++tmp; return tmp; }

		/**
		 @brief Moves the iterator backwards.
		 */
		BaseIterator operator--(int) { auto tmp = *this; --tmp; return tmp; }

		/**
		 @brief Assignment operator.
		 */
		BaseIterator& operator=(const BaseIterator& aOther) { _it = aOther._it; return *this; }

		/**
		 @brief Equality comparison operator.
		 */
		bool operator==(const BaseIterator& aOther) const { return _it == aOther._it; }

		/**
		 @brief Inequality comparison operator.
		 */
		bool operator!=(const BaseIterator& aOther) const { return _it != aOther._it; }

	private:
		MapIteratorType _it;

		friend class OMap;
	};

	using Iterator = BaseIterator<MapIterator>;
	using ConstIterator = BaseIterator<MapConstIterator>;

	/**
	 @brief Class default constructor.
	 */
	OMap() = default;

	/**
	 @brief Deleted copy constructor.
	 */
	OMap(const OMap& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OMap(OMap&& aOther);

	/**
	 @brief Clones the map into another one.
	 @param aTarget The destination map.
	 */
	void cloneTo(OMap& aTarget) const;

	/**
	 @brief Clones the contents of the map to a newly allocated one.
	 @return The newly allocated map.
	 */
	OMap* clone() const;

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
	uint32_t size() const;

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
	 @copydoc insert(const TKey&, const TValue&, Iterator*)
	 */
	bool insert(const TKey& aKey, TValue&& aValue, Iterator* aItemAtMap = nullptr);

	/**
	 @brief Removes an item pointer to by an iterator.
	 */
	void remove(Iterator& aIterator);

	/**
	 @brief Removes an item by it's key.
	 */
	void remove(const TKey& aKey);

	/**
	 @brief Deleted assignment operator.
	 */
	OMap& operator=(const OMap& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OMap& operator=(OMap&& aOther);

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
// INLINE IMPLEMENTATION - OMap
// ------------------------------------------------------------------------------------------
template<class TKey, class TValue, class Allocator, class Compare>
inline OMap<TKey, TValue, Allocator, Compare>::OMap(OMap && aOther) :
	_map(std::move(aOther._map))
{
}

template<class TKey, class TValue, class Allocator, class Compare>
inline void OMap<TKey, TValue, Allocator, Compare>::cloneTo(OMap & aTarget) const
{
	aTarget._map = _map;
}

template<class TKey, class TValue, class Allocator, class Compare>
inline OMap<TKey, TValue, Allocator, Compare>* OMap<TKey, TValue, Allocator, Compare>::clone() const
{
	auto newClone = new OMap;
	OExceptionPointerCheck(newClone);
	cloneTo(*newClone);
	return newClone;
}

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
inline uint32_t OMap<TKey, TValue, Allocator, Compare>::size() const
{
	return _map.size();
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

template<class TKey, class TValue, class Allocator, class Compare>
inline bool OMap<TKey, TValue, Allocator, Compare>::insert(const TKey & aKey, TValue && aValue, Iterator * aItemAtMap)
{
	std::pair<MapIterator,bool> pair = _map.insert(std::pair<TKey,TValue>(aKey, std::move(aValue)));
	if (aItemAtMap != nullptr) {
		*aItemAtMap = pair.first;
	}
	return pair.second;
}

template<class TKey, class TValue, class Allocator, class Compare>
inline void OMap<TKey, TValue, Allocator, Compare>::remove(Iterator & aIterator)
{
	_map.erase(aIterator._it);
}

template<class TKey, class TValue, class Allocator, class Compare>
inline void OMap<TKey, TValue, Allocator, Compare>::remove(const TKey & aKey)
{
	_map.erase(aKey);
}

template<class TKey, class TValue, class Allocator, class Compare>
inline OMap<TKey, TValue, Allocator, Compare>& OMap<TKey, TValue, Allocator, Compare>::operator=(OMap && aOther)
{
	_map = std::move(aOther._map);
	return *this;
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

