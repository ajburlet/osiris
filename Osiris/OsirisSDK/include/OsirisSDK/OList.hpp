#pragma once

#include <stdint.h>
#include <functional>
#include <type_traits>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"

/**
 @brief Base template iterator class for lists.
 */
template <typename Node_t, typename Ptr_t, typename Ref_t, class Allocator >
class OAPI OBaseListIterator : public OMemoryManagedObject<Allocator>
{
public:
	/**
	 @brief Default class constructor.
	 */
	OBaseListIterator() = default;

	/**
	 @brief Class constructor.
	 @param aNodePtr Pointer to the node of interest. 
	 */
	OBaseListIterator(Node_t* aNodePtr);

	/**
	 @brief Class destructor.
	 */
	~OBaseListIterator() = default;
	
	/**
	 @brief Dereference operator.
	 */
	Ref_t operator*();

	/**
	 @brief Pointer access operator.
	 */
	Ptr_t operator->();

	/**
	 @brief Prefix increment by one operator, moves the iterator to the next item.
	 */
	OBaseListIterator& operator++();
	
	/**
	 @brief Postfix increment by one operator, moves the iterator to the next item.
	 */
	OBaseListIterator operator++(int);

	/**
	 @brief Subtract by one operator, moves the iterator to the previous item.
	 */
	OBaseListIterator& operator--();

	/**
	 @brief Equality comparison operator.
	 @param aOther The object to be compared against.
	 */
	bool operator==(const OBaseListIterator& aOther) const;

	/**
	 @brief Inequeality comparison operator.
	 @param aOther The object to be compared against.
	 */
	bool operator!=(const OBaseListIterator& aOther) const;

	/**
	 @brief Pointer to the list node.
	 */
	Node_t* node();

private:
	Node_t*	_nodePtr = nullptr;
};

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::OBaseListIterator(Node_t* aNodePtr) :
	_nodePtr(aNodePtr)
{
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline Ref_t OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator*()
{
	return _nodePtr->_value;
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline Ptr_t OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator->()
{
	return &(_nodePtr->_value);
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>& OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator++()
{
	_nodePtr = _nodePtr->_next;
	return *this;
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator> OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator++(int)
{
	OBaseListIterator it(*this);
	++(*this);
	return it;
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>& OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator--()
{
	_nodePtr = _nodePtr->_prev;
	return *this;
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline bool OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator==(const OBaseListIterator & aOther) const
{
	return (_nodePtr == aOther._nodePtr);
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline bool OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator!=(const OBaseListIterator & aOther) const
{
	return !(*this == aOther);
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline Node_t* OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::node()
{
	return _nodePtr;
}


/**
 @brief Linked list class.
 */
template <typename T, class Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
class OAPI OList : public OMemoryManagedObject<Allocator>, ONonCopiableT<OList<T,Allocator>> 
{
public:
	/**
	 @brief Class default constructor.
	 */
	OList() = default;

	/**
	 @brief Deleted copy constructor.
	 */
	OList(const OList& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OList(OList&& aOther);

	/**
	 @brief Class destructor.
	 */
	~OList();

	/**
	 @brief Deleted assignment operator.
	 */
	OList& operator=(const OList& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OList& operator=(OList&& aOther);

	/**
	 @brief Clones the list into another one.
	 @param aTarget The destination list.
	 */
	void cloneTo(OList& aTarget) const override;

	/**
	 @brief Returns the list size.
	 */
	uint32_t size() const;

	/**
	 @brief Returns true if there are no items on the list.
	 */
	bool empty() const;

	/**
	 @brief Clears the list, removes all items.
	 */
	void clear();
	
	/**
	 @brief List node struct.
	 */
	struct Node : public OMemoryManagedObject<Allocator> {
		Node(const T& aValue, Node* aPrev = nullptr, Node* aNext=nullptr)  
		{
			if constexpr(std::is_base_of<T, ONonCopiable>::value) {
				OException("Attempted to copy a non-copiable item. Use move semantics instead.");
			} else {
				_value = aValue;
			}
			_prev = aPrev;
			_next = aNext;
		}
		Node(T&& aValue, Node* aPrev=nullptr, Node* aNext=nullptr) : 
			_value(std::move(aValue)), _prev(aPrev), _next(aNext) {}

		T	_value;
		Node*	_prev = nullptr;
		Node*	_next = nullptr;
	};

	/**
	 @brief Non-const iterator class.
	 */
	using Iterator = OBaseListIterator<Node, T*, T&, Allocator>;

	/**
	 @brief Const iterator class.
	 */
	using ConstIterator = OBaseListIterator<Node, const T*, const T&, Allocator>;

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
	 @brief Push item to the front of the list.
	 @param aItem The item to be added to the list.
	 */
	void pushFront(const T& aItem);

	/**
	 @copydoc pushFront(const T&)
	 */
	void pushFront(T&& aItem);

	/**
	 @brief Push node to the front of the list.
	 @param aNode Node to be added to the list.
	 */
	void pushFront(Node* aNode);

	/**
	 @brief Push item to the back of the list.
	 @param aItem The item to be added to the list.
	 */
	void pushBack(const T& aItem);

	/**
	 @copydoc pushBack(const T&)
	 */
	void pushBack(T&& aItem);

	/**
	 @brief Push node to the back of the list.
	 @param aNode Node to be added to the list.
	 */
	void pushBack(Node* aNode);

	/**
	 @brief Inserts an item after a given iterator marking position.
	 @param aItem The item to be added to the list.
	 @param aPositionIt The iterator used as reference to the insertion point.
	 */
	void insertAfter(const T& aItem, Iterator& aPositionIt);

	/**
	 @copydoc insertAfter(const T&, Iterator&)
	 */
	void insertAfter(T&& aItem, Iterator& aPositionIt);

	/**
	 @brief Inserts a node after a given iterator marking position.
	 @param aNode The node to be added to the list.
	 @param aPositionIt The iterator used as reference to the insertion point.
	 */
	void insertAfter(Node* aNode, Iterator& aPositionIt);

	/**
	 @brief Inserts an item before a given iterator marking position.
	 @param aItem The item to be added to the list.
	 @param aPositionIt The iterator used as reference to the insertion point.
	 */
	void insertBefore(const T& aItem, Iterator& aPositionIt);

	/**
	 @copydoc insertBefore(const T&, Iterator&)
	 */
	void insertBefore(T&& aItem, Iterator& aPositionIt);

	/**
	 @brief Inserts a node before a given iterator marking position.
	 @param aNode The node to be added to the list.
	 @param aPositionIt The iterator used as reference to the insertion point.
	 */
	void insertBefore(Node* aNode, Iterator& aPositionIt);

	/**
	 @brief Removes first element from the list.
	 */
	void popFront();

	/**
	 @brief Removes the last element from the list.
	 */
	void popBack();

	/**
	 @brief Remos an item from the list using the iterator.
	 @param aIterator The iterator pointing to the item to be removed.
	 */
	void remove(Iterator& aIterator);

	/**
	 @brief Removes the first occurence of an item from the list.
	 @param aItem The item to be removed from the list.
	 @return true if an item was removed.
	 */
	bool remove(const T& aItem);

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

	/**
	 @brief Comparison function to be used in find operation.
	 */
	using FindComparisonFn = std::function<bool(const T&)>;

	/**
	 @brief Constant find operation.
	 @param aFn Comparison function.
	 @return A constant iterator to the element found, or end() if none is found.
	 */
	ConstIterator find (FindComparisonFn aFn) const;
	
	/**
	 @brief Find operation.
	 @param aFn Comparison function.
	 @return A constant iterator to the element found, or end() if none is found.
	 */
	Iterator find (FindComparisonFn aFn);

private:
	void moveFrom(OList&& aOther);

	Node*		_head	= nullptr;
	Node*		_tail	= nullptr;
	uint32_t	_count	= 0;
};

template<typename T, class Allocator>
inline OList<T, Allocator>::OList(OList<T,Allocator>&& aOther)
{
	moveFrom(std::move(aOther));
}

template<typename T, class Allocator>
inline OList<T, Allocator>::~OList()
{
	clear();
}

template<typename T, class Allocator>
inline OList<T, Allocator>& OList<T, Allocator>::operator=(OList<T,Allocator>&& aOther)
{
	clear();
	moveFrom(std::move(aOther));
	return *this;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::cloneTo(OList & aTarget) const
{
	aTarget.clear();
	if constexpr(std::is_base_of<ONonCopiable, T>::value) {
		for (auto& item : *this) {
			aTarget.pushBack(T());
			item.cloneTo(aTarget.tail());
		}
	} else {
		for (auto& item : *this) aTarget.pushBack(item);
	}
}

template<typename T, class Allocator>
inline uint32_t OList<T, Allocator>::size() const
{
	return _count;
}

template<typename T, class Allocator>
inline bool OList<T, Allocator>::empty() const
{
	return (_count == 0);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::clear()
{
	auto node = _head;
	while (node != nullptr) {
		auto curr = node;
		node = node->_next;
		Allocator().deallocate(curr, sizeof(*curr));
	}
	_head = nullptr;
	_tail = nullptr;
	_count = 0;
}

template<typename T, class Allocator>
inline typename OList<T,Allocator>::Iterator OList<T, Allocator>::begin()
{
	return Iterator(_head);
}

template<typename T, class Allocator>
inline typename OList<T,Allocator>::ConstIterator OList<T, Allocator>::begin() const
{
	return ConstIterator(_head);
}

template<typename T, class Allocator>
inline typename OList<T,Allocator>::Iterator OList<T, Allocator>::end()
{
	return Iterator(nullptr);
}

template<typename T, class Allocator>
inline typename OList<T,Allocator>::ConstIterator OList<T, Allocator>::end() const
{
	return ConstIterator(nullptr);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::pushFront(const T & aItem)
{
	auto node = new Node{ aItem, nullptr, _head };
	OExceptionPointerCheck(node);
	pushFront(node);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::pushFront(T && aItem)
{
	auto node = new Node{ std::move(aItem), nullptr, _head };
	OExceptionPointerCheck(node);
	pushFront(node);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::pushFront(Node * aNode)
{
	if (_head != nullptr) _head->_prev = aNode;
	if (_tail == nullptr) _tail = _head;
	_head = aNode;
	_count++;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::pushBack(const T & aItem)
{
	auto node = new Node{ aItem, _tail, nullptr };
	OExceptionPointerCheck(node);
	pushBack(node);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::pushBack(T && aItem)
{
	auto node = new Node{ std::move(aItem), _tail, nullptr };
	OExceptionPointerCheck(node);
	pushBack(node);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::pushBack(Node * aNode)
{
	if (_tail == nullptr) {
		_head = aNode;
	} else {
		_tail->_next = aNode;
	}
	_tail = aNode;
	_count++;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::insertAfter(const T & aItem, Iterator & aPositionIt)
{
	auto node = new Node{ aItem };
	OExceptionPointerCheck(node);
	insertAfter(node, aPositionIt);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::insertAfter(T && aItem, Iterator & aPositionIt)
{
	auto node = new Node{ std::move(aItem) };
	OExceptionPointerCheck(node);
	insertAfter(node, aPositionIt);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::insertAfter(Node * aNode, Iterator & aPositionIt)
{
	aNode->_prev = aPositionIt.node();
	if (aPositionIt.node() == nullptr || _count == 0) {
		// empty list or iterator to the end of the list
		pushBack(aNode);
		return;
	}
	aNode->_next = aPositionIt.node()->_next;
	if (aPositionIt.node()->_next != nullptr) aPositionIt.node()->_next->_prev = aNode;
	aPositionIt.node()->_next = aNode;
	_count++;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::insertBefore(const T & aItem, Iterator & aPositionIt)
{
	auto node = new Node{ aItem, aPositionIt.node()->_prev, aPositionIt.node() };
	OExceptionPointerCheck(node);
	insertBefore(node, aPositionIt);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::insertBefore(T && aItem, Iterator & aPositionIt)
{
	auto node = new Node{ std::move(aItem) };
	OExceptionPointerCheck(node);
	insertBefore(node, aPositionIt);
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::insertBefore(Node * aNode, Iterator & aPositionIt)
{
	aNode->_next = aPositionIt.node();
	if (aPositionIt.node() == nullptr || _count == 0) {
		// empty list or iterator to the end of the list
		pushBack(aNode);
		return;
	} else if (aPositionIt.node()->_prev == nullptr) {
		pushFront(aNode);
		return;
	}
	aNode->_prev = aPositionIt.node()->_prev;
	if (aPositionIt.node()->_prev != nullptr) aPositionIt.node()->_prev->_next = aNode;
	aPositionIt.node()->_prev = aNode;
	_count++;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::popFront()
{
	if (_head == nullptr) throw OException("Removing item from an empty list.");
	auto node = _head;
	_head = _head->_next;
	if (_head != nullptr) _head->_prev = nullptr;
	if (_tail == node) _tail = nullptr;
	delete node;
	_count--;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::popBack()
{
	if (_tail == nullptr) throw OException("Removing item from an empty list.");
	auto node = _tail;
	if (_head == _tail) {
		_head = nullptr;
	} else {
		_tail->_prev->_next = nullptr;
	}
	_tail = _tail->_prev;
	delete node;
	_count--;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::remove(Iterator & aIterator)
{
	auto node = aIterator.node();
	++aIterator;
	if (node->_prev == nullptr) {
		_head = node->_next;
	} else {
		node->_prev->_next = node->_next;
	}
	if (node->_next == nullptr) {
		_tail = node->_prev;
	} else {
		node->_next->_prev = node->_prev;
	}
	delete node;
	_count--;
}

template<typename T, class Allocator>
inline bool OList<T, Allocator>::remove(const T & aItem)
{
	bool removed = false;
	for (Iterator it = begin(); it != end(); ++it) {
		if (*it == aItem) {
			remove(it);
			removed = true;
			break;
		}
	}
	return removed;
}

template<typename T, class Allocator>
inline T & OList<T, Allocator>::front()
{
	return _head->_value;
}

template<typename T, class Allocator>
inline const T & OList<T, Allocator>::front() const
{
	return _head->_value;
}

template<typename T, class Allocator>
inline T & OList<T, Allocator>::tail()
{
	return _tail->_value;
}

template<typename T, class Allocator>
inline const T & OList<T, Allocator>::tail() const
{
	return _tail->_value;
}

template<typename T, class Allocator>
inline typename OList<T, Allocator>::ConstIterator OList<T, Allocator>::find(OList<T, Allocator>::FindComparisonFn aFn) const
{
	auto res = end();
	for (auto it=begin(); it != end(); ++it) {
		if (aFn(*it)) {
			res = it;
			break;
		}
	}
	return res;
}

template<typename T, class Allocator>
inline typename OList<T, Allocator>::Iterator OList<T, Allocator>::find(OList<T, Allocator>::FindComparisonFn aFn)
{
	auto res = end();
	for (auto it=begin(); it != end(); ++it) {
		if (aFn(*it)) {
			res = it;
			break;
		}
	}
	return res;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::moveFrom(OList && aOther)
{
	_head = aOther._head;
	_tail = aOther._tail;
	_count = aOther._count;

	aOther._head = nullptr;
	aOther._tail = nullptr;
	aOther._count = 0;
}

