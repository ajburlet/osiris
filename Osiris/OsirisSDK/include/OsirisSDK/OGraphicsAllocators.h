#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"

/**
 @brief References memory allocators to be used in the graphics module.
 */
class OGraphicsAllocators {
public:
	/**
	 @brief Default graphics allocator.
	 */
	using Default = OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>;
};