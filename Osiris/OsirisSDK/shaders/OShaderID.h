#pragma once

#include <string>

#include "OsirisSDK/OGraphicsEngine.h"
#include "OsirisSDK/OShaderObject.h"
#include "OsirisSDK/ORenderable.h"
#include "OsirisSDK/OMap.hpp"

struct OShaderID {
	OGraphicsEngine::Type engineType;
	OShaderObject::Type shaderObjectType;
	ORenderable::Type renderObjectType;

	uint32_t value() const;
	bool operator<(const OShaderID& aOther) const;
};
using OShaderTable = OMap<OShaderID, 
			  std::string,
			  OSystemMemoryAllocator<OMemoryManager::Scope::Default>>;

inline uint32_t OShaderID::value() const
{
	return	static_cast<uint32_t>(engineType) << (sizeof(renderObjectType) + sizeof(shaderObjectType))*8 |
		static_cast<uint32_t>(shaderObjectType) << sizeof(renderObjectType)*8 |
		static_cast<uint32_t>(renderObjectType);
}

inline bool OShaderID::operator<(const OShaderID & aOther) const
{
	return (value() < aOther.value());
}
