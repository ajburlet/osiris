#pragma once

#include <map>

/**
 @brief Template class to manage collection of objects.
 */
template<class T> class OCollection {
public:
	/**
	 @brief Class constructor.
	 */
	OCollection() : _lastID(0) { }

	/**
	 @brief Class destructor.
	 */
	virtual ~OCollection() { }

	/**
	 @brief Unsigned integer that represents a collection object ID.

	 The collection object ID must be higher than zero.
	 */
	typedef unsigned long long ID;
	
	/**
	 @brief Add a new item to the collection.
	 @param item Item to be added into the collection.
	 @return The assigned collection ID. If item is already present in the collection, returns 0.
	 */
	ID add(T* item) 
	{
		/* checking of object already exists in the collection */
		if (_ptrMap.find(item) != _ptrMap.end()) return 0;

		/* finding next valid ID */
		ID newID = ++_lastID;
		std::map<ID, T*>::iterator it;
		for (it = _idMap.find(newID); it != _idMap.end(); it++) {
			newID++;
			if (it->first - newID > 0) break;
		}

		/* adding to internal maps */
		_ptrMap[item] = newID;
		_idMap[newID] = item;

		return newID;
	}
	
	/**
	 @brief Add a new item to the collection.
	 @param item Item to be added into the collection.
	 @return The assigned collection ID. If ID is already assigned, then returns 0.
	 */
	ID add(T* item, ID id) 
	{
		/* checking of object already exists in the collection */
		if (_ptrMap.find(item) != _ptrMap.end()) return 0;
		if (_idMap.find(id) != _idMap.end()) return 0;
		
		/* adding to internal maps */
		_ptrMap[item] = newID;
		_idMap[newID] = item;
		
		return newID;
	}
	
	/**
	 @brief Remove item from the collection.
	 @param item Object memory address.
	 */
	void remove(T* item)
	{
		std::map<T*, ID>::iterator it = _ptrMap.find(item);
		if (it == _ptrMap.end()) return;
		_idMap.erase(it->second);
		_ptrMap.erase(item);
	}

	/**
	 @brief Remove item from the collection.
	 @param id Object collection ID. 
	 */
	void remove(ID id)
	{
		std::map<ID, T*>::iterator it = _idMap.find(id);
		if (it == _idMap.end()) return;
		_ptrMap.erase(it->second);
		_idMap.erase(id);
	}

	/**
	 @brief Retrieve item from the collection.
	 @param id Object collection ID.
	 @return Pointer to object related to the given ID. Returns NULL if ID is not found.
	 */
	T* get(ID id)
	{
		std::map<ID, T*>::iterator it = _idMap.find(id);
		if (it == _idMap.end()) return NULL;
		return it->second;
	}

	/**
	 @brief Collection iterator for OCollection.
	 */
	class Iterator {
	public:
		/**
		 @brief Assignment operator.
		 */
		Iterator& operator=(const Iterator& in)
		{
			_it = in._it;
			return *this;
		}

		/**
		 @brief Increment operator, moves the iterator forward.
		 */
		Iterator operator++(int c)
		{
			Iterator copy(*this);
			_it++;
			return copy;
		}

		/**
		 @brief Decrement operator, moves the iterator backward.
		 */
		Iterator operator--(int c)
		{
			Iterator copy(*this);
			_it--;
			return copy;
		}

		/**
		 @brief Addition operation, moves the iterator forward by c positions.
		 @param c Number of positions to move the iterator.
		 */
		Iterator operator+(int c) const
		{
			Iterator ret;
			ret._it = _it + c;
			return ret;
		}

		/**
		 @brief Subtraction operator, moves the iterator backward by c positions.
		 @param c Number of positions to move the iterator.
		 */
		Iterator operator-(int c) const
		{
			Iterator ret;
			ret_it = _it - c;
			return ret;
		}

		/**
		 @brief Addition operation, moves the iterator forward by c positions.
		 @param c Number of positions to move the iterator.
		 */
		Iterator& operator+=(int c)
		{
			_it += c;
			return *this;
		}

		/**
		 @brief Subtraction operator, moves the iterator backward by c positions.
		 @param c Number of positions to move the iterator.
		 */
		Iterator& operator-=(int c)
		{
			_it -= c;
			return *this;
		}

		/**
		 @brief Equal-to comparison operator.
		 */
		bool operator==(const Iterator& in) { return (_it == in._it); }
		
		/**
		 @brief Not equal-to comparison operator.
		 */
		bool operator!=(const Iterator& in) { return (_it != in._it); }

		/**
		 @brief Returns object collection ID.
		 */
		ID id() const { return _it->first; }

		/**
		 @brief Returns object memory address.
		 */
		T* object() const { return _it->second;  }

	private:
		typename std::map<ID, T*>::iterator _it;
		Iterator(typename std::map<ID, T*>::iterator& it) : _it(it) { };
		friend OCollection;
	};

	/**
	 @brief Returns iterator to pointing to the beggining of the collection.
	 */
	Iterator begin() { return Iterator(_idMap.begin()); }

	/**
	 @brief Returns iterator to pointing to the end of the collection.
	 */
	Iterator end() { return Iterator(_idMap.end()); }

	/**
	 @brief Get the iterator for an object with a given ID.
	 @param id Object collection ID.
	 @return Iterator pointing to the object related to the given ID. If object is not found, 
	         then returns end() iterator.
	 */
	Iterator find(ID id) { return Iterator(_idMap.find(id)); }

	/**
	 @brief Object count.
	 */
	size_t count() { return _idMap.size(); }

private:
	ID _lastID;
	std::map<ID, T*> _idMap;
	std::map<T*, ID> _ptrMap;
};