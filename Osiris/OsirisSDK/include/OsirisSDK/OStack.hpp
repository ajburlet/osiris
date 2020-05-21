#pragma once

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"

template <typename T, class Allocator=OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
class OStack : OMemoryManagedObject<Allocator>
{
public:
	/**
	 @brief Default class constructor.
	 */
	OStack() = default;

	/**
	 @brief Class destructor.
	 */
	~OStack();

	/**
	 @brief Pushes an item to the top of the stack.
	 @param aItem Item to be added.
	 */
	void push(const T& aItem);

	/**
	 @brief Retrieves the top item in the stack.
	 @return A reference to the top item in the stack.
	 */
	T& top();

	/**
	 @brief Retrieves the top item in the stack (const).
	 @return A const reference to the top item in the stack.
	 */
	const T& top() const;

	/**
	 @brief Removes the top item in the stack.
	 */
	void pop();

	/**
	 @brief Returns true if the stack is empty.
	 */
	bool empty() const;

private:
	struct Node {
		T	_value;
		Node*	_next = nullptr;
	};

	Node*	_top	= nullptr;
};

template<typename T, class Allocator>
inline OStack<T, Allocator>::~OStack()
{
	while (!empty()) pop();
}

template<typename T, class Allocator>
inline void OStack<T, Allocator>::push(const T & aItem)
{
	auto node = new Node{ aItem, _top };
	OExceptionPointerCheck(node);
	_top = node;
}

template<typename T, class Allocator>
inline T & OStack<T, Allocator>::top()
{
	if (empty()) throw OException("Attempted to access the top element of an empty stack.");
	return _top->_value;
}

template<typename T, class Allocator>
inline const T & OStack<T, Allocator>::top() const
{
	if (empty()) throw OException("Attempted to access the top element of an empty stack.");
	return _top->_value;
}

template<typename T, class Allocator>
inline void OStack<T, Allocator>::pop()
{
	if (empty()) throw OException("Attempted to pop an element of an empty stack.");
	auto node = _top;
	_top = node->_next;
	delete node;
}

template<typename T, class Allocator>
inline bool OStack<T, Allocator>::empty() const
{
	return (_top == nullptr);
}

