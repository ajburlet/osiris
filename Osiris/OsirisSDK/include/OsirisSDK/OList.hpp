#pragma once

#include <list>

#include "OsirisSDK/OSystemMemoryAllocator.h"

/**
 @brief Linked list class.
 */
template <typename T, class Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
class OList : public std::list<T, OSTLAllocator<Allocator>, T>
{
public:
	/**
	 @brief Class default constructor.
	 */
	OList() = default;

	/**
	 @brief Class destructor.
	 */
	~OList() = default;
};