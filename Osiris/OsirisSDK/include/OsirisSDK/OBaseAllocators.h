#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"

class OBaseAllocators
{
public:
	using Default = OSystemMemoryAllocator<OMemoryManagerScope::Default>;

};