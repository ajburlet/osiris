#pragma once

#include <map>

#include "OsirisSDK/OSystemMemoryAllocator.h"

/**
 @brief The map, dictionary or binary tree implementation.
 */
template<typename TKey, 
	typename TValue, 
	class Compare = std::less<TKey>, 
	class Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
	class OMap : public std::map<TKey, TValue, Compare, OSTLAllocator<Allocator, std::pair<const TKey, TValue>>>
{

};