#pragma once

#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OShaderArgument.h"

class OShaderArgumentInstance;
class ORenderable;

/**
 @brief Shader argument instance list.
 */
class OShaderArgumentInstanceList :
	public OList<OShaderArgumentInstance*, OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>>
{
public:
	/**
	 @brief Returns true if the arguments still must be loaded.
	 */
	bool needsLoading() const;

	/**
	 @brief Sets the update status of the argument list.
	 */
	void setNeedsLoading(bool aFlag);

	/**
	 @brief Calls the update callback on all argument instances.
	 */
	void update(const ORenderable* aRenderable);

private:
	bool _needsLoading = true;
};

inline bool OShaderArgumentInstanceList::needsLoading() const
{
	return _needsLoading;
}

inline void OShaderArgumentInstanceList::setNeedsLoading(bool aFlag)
{
	_needsLoading = aFlag;
}

inline void OShaderArgumentInstanceList::update(const ORenderable* aRenderable)
{
	for (auto arg : *this) {
		arg->update(aRenderable);
	}
}