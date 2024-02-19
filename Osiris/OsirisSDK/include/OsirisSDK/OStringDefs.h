#pragma once

#include "OsirisSDK/OMemoryManagerScope.h"

template <OMemoryManagerScope Scope>
class OSystemMemoryAllocator;

template <typename CharT, typename Allocator=OSystemMemoryAllocator<OMemoryManagerScope::Default>>
class OBaseString;

/**
 @brief String wrapper.
 */
using OString = OBaseString<char>;

/**
 @brief String wrapper for wide characters.
 */
using OWString = OBaseString<wchar_t>;
