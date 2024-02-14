#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OMemoryManagedObject.h"

#include "OsirisSDK/OMap.hpp"

template <typename T, 
	  class Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Default>, 
	  class Compare = std::less<T>>
class OSet : protected OMap<T, bool, Allocator, Compare>
{
private:
	using Super = OMap<T, bool, Allocator, Compare>;

public:
	template <class MapIteratorType, typename Ptr_t, typename Ref_t>
	class BaseIterator : protected MapIteratorType
	{
	private:
		using ItSuper = MapIteratorType;

	public:
		/**
		 @brief Default class constructor.
		 */
		BaseIterator() = default;
		
		/**
		 @brief Class constructor.
		 @param aOrigIterator The original iterator.
		 */
		BaseIterator(const ItSuper& aOrigIterator) : ItSuper(aOrigIterator) {}

		/**
		 @brief Class copy constructor.
		 @param aOther The iterator to be copied.
		 */
		BaseIterator(const BaseIterator& aOther) : ItSuper(aOther) {}

		/**
		 @brief Class destructor.
		 */
		virtual ~BaseIterator() = default;

		/**
		 @brief Returns the value.
		 */
		Ref_t value() { return ItSuper::key(); }

		/**
		 @brief Moves the iterator forward.
		 */
		BaseIterator& operator++() { ItSuper::operator++(); return *this; }

		/**
		 @brief Moves the iterator backwards.
		 */
		BaseIterator& operator--() { ItSuper::operator--(); return *this; }
		
		/**
		 @brief Moves the iterator forward.
		 */
		BaseIterator operator++(int) { return ItSuper::operator++(); }

		/**
		 @brief Moves the iterator backwards.
		 */
		BaseIterator operator--(int) { return ItSuper::operator--(); }

		/**
		 @brief Assignment operator.
		 */
		BaseIterator& operator=(const BaseIterator& aOther) { return ItSuper::operator=(aOther); }

		/**
		 @brief Equality comparison operator.
		 */
		bool operator==(const BaseIterator& aOther) const { return ItSuper::operator==(aOther); }

		/**
		 @brief Inequality comparison operator.
		 */
		bool operator!=(const BaseIterator& aOther) const { return ItSuper::operator!=(aOther); }
		
		/**
		 @brief Dereference operator.
		 */
		Ref_t operator*() { return value(); }

		/**
		 @brief Pointer access operator.
		 */
		Ptr_t operator->() { return &(value()); }

		friend class OSet;
	};

	using Iterator = BaseIterator<typename Super::Iterator, T*, const T&>;
	using ConstIterator = BaseIterator<typename Super::ConstIterator, const T&, const T*>;

	/**
	 @brief Class default constructor.
	 */
	OSet() = default;

	/**
	 @brief Deleted copy constructor.
	 */
	OSet(const OSet& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OSet(OSet&& aOther) : Super(std::move(aOther)) {}

	/**
	 @brief Class destructor.
	 */
	virtual ~OSet() = default;

	/**
	 @brief Deleted copy assignment operator.
	 */
	OSet& operator=(const OSet& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OSet& operator=(OSet&& aOther) { return Super::operator=(std::move(aOther)); }

	/**
	 @brief Clones the set into another one.
	 @param aTarget The destination set.
	 */
	void cloneTo(OSet& aTarget) const { return Super::cloneTo(aTarget); }

	/**
	 @brief Clones the contents of the set to a newly allocated one.
	 @return The newly allocated set.
	 */
	OSet* clone() const;

	/**
	 @brief Returns an iterator that points to the beginning of the collection.
	 */
	Iterator begin() { return Super::begin(); }

	/**
	 @brief Returns an iterator to the end of the collection. 
	 */
	Iterator end() { return Super::end(); }

	/**
	 @brief Returns a constant iterator that points to the beginning of the collection.
	 */
	ConstIterator cbegin() const { return Super::cbegin(); }

	/**
	 @brief Returns a constant iterator to the end of the collection. 
	 */
	ConstIterator cend() const { return Super::cend(); }
	
	/**
	 @brief Returns the number of items in the collection.
	 */
	uint32_t size() const { return Super::size(); }
	
	/**
	 @brief Inserts a value in the map.
	 @param aKey The value to be added. 
	 @return True if the value is added, false if there was a value previously assigned to the key.
	 */
	bool insert(const T& aValue) { return Super::insert(aValue, true);  }

	/**
	 @brief Removes an item by it's key.
	 */
	void remove(const T& aKey) { return Super::remove(aKey); }

	/**
	 @brief Returns true if set has a value.
	 */
	bool has(const T& aKey) const { return Super::find(aKey);  }
};

template<typename T, class Allocator, class Compare>
inline OSet<T, Allocator, Compare> * OSet<T, Allocator, Compare>::clone() const
{
	auto newClone = new OSet;
	OExceptionPointerCheck(newClone);
	cloneTo(newClone);
	return newClone;
}
