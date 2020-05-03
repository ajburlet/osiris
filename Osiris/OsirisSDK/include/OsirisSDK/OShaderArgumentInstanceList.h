#pragma once

#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OList.hpp"

class OShaderArgumentInstance;

/**
 @brief Shader argument instance list.
 */
class OShaderArgumentInstanceList :
	public OList<OShaderArgumentInstance*, OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>>
{

};