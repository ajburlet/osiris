#pragma once

#include <list>
#include <stdint.h>

#include "OsirisSDK/defs.h"
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
	Ref_t operator*() const;

	/**
	 @brief Pointer access operator.
	 */
	Ptr_t operator->() const;

	/**
	 @brief Increment by one operator, moves the iterator to the next item.
	 */
	OBaseListIterator& operator++();

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
	Ptr_t node() const;

private:
	Node_t*	_nodePtr = nullptr;
};

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::OBaseListIterator(Node_t* aNodePtr) :
	_nodePtr(aNodePtr)
{
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline Ref_t OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator*() const
{
	return _nodePtr->_value;
}

template<typename Node_t, typename Ptr_t, typename Ref_t, class Allocator>
inline Ptr_t OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::operator->() const
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
inline Ptr_t OBaseListIterator<Node_t, Ptr_t, Ref_t, Allocator>::node() const
{
	return _node;
}


/**
 @brief Linked list class.
 */
template <typename T, class Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
class OAPI OList : public OMemoryManagedObject<Allocator> 
{
public:
	/**
	 @brief Class default constructor.
	 */
	OList() = default;

	/**
	 @brief Class destructor.
	 */
	~OList();

	/**
	 @brief Returns the list size.
	 */
	uint32_t size() const;

	/**
	 @brief Clears the list, removes all items.
	 */
	void clear();
	
	/**
	 @brief List node struct.
	 */
	struct Node : public OMemoryManagedObject<Allocator> {
		/**
		 @brief Constructor.
		 */
		Node(const T& aValue, Node* aPrev, Node* aNext) : _value(aValue), _prev(aPrev), _next(aNext) {}

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
	 @brief Push item to the back of the list.
	 @param aItem The item to be added to the list.
	 */
	void pushBack(const T& aItem);

	/**
	 @brief Inserts an item at a given position.
	 @param aItem The item to be added to the list.
	 @param aPositionIt 
	 */
	void insert(const T& aItem, Iterator& aPositionIt);

	/**
	 @brief Removes first element from the list.
	 */
	void popFront();

	/**
	 @brief Removes the last element from the list.
	 */
	void popBack();

private:
	Node*		_head	= nullptr;
	Node*		_tail	= nullptr;
	uint32_t	_count	= 0;
};

template<typename T, class Allocator>
inline OList<T, Allocator>::~OList()
{
	clear();
}

template<typename T, class Allocator>
inline uint32_t OList<T, Allocator>::size() const
{
	return _count;
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
	auto node = new Node(aItem, nullptr, _head);
	OExceptionPointerCheck(node);
	if (_head != nullptr) _head->_prev = node;
	if (_tail == nullptr) _tail = _head;
	_head = node;
	_count++;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::pushBack(const T & aItem)
{
	auto node = new Node(aItem, _tail, nullptr);
	OExceptionPointerCheck(node);
	if (_tail = nullptr) {
		_head = node;
	} else {
		_tail->_next = node;
	}
	_tail = node;
	_count++;
}

template<typename T, class Allocator>
inline void OList<T, Allocator>::insert(const T & aItem, Iterator & aPositionIt)
{
	if (aPositionIt.node() == nullptr || _count == 0) {
		// empty list or iterator to the end of the list
		pushBack(aItem);
		return;
	}

	auto node = new Node(aItem, , aPositionIt.node(), aPositionIt.node()->_next);
	OExceptionPointerCheck(node);
	if (aPositionIt.node()->_next != nullptr) aPositionIt.node()->_next->_prev = node;
	aPositionIt.node()->_next = node;
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
		_tail->-_prev->_next = nullptr;
	}
	_tail = _tail->_prev;
	delete node;
	_count--;
}

