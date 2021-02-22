#pragma once

#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OMemoryManagedObject.h"

template <typename CharT, typename Allocator=OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
class OBaseString;

/**
 @brief String wrapper.
 */
using OString = OBaseString<char>;

/**
 @brief String wrapper for wide characters.
 */
using OWString = OBaseString<wchar_t>;
