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
public:
	/**
	 @brief Calls the update callback on all argument instances.
	 */
	void update();
};

inline void OShaderArgumentInstanceList::update()
{
	for (auto arg : *this) {
		arg->update();
	}
}