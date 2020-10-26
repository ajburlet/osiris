#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/ORefCountObject.hpp"

class OTrashBin;

/**
 @brief Resource manager interface.
 @tparam ResourceKeyType Resource key type, used to identify the resource within the manager.
 @tparam ResourceType Resource data type.
 */
template <typename ResourceKeyType, class ResourceType>
class OResouceManagerInterface
{
public:
	/**
	 @brief Reference count pointer wrapper to the resource object.
	 */
	using ResourcePtr = ORefCountPtr<ResourceType>;

	/** 
	 @brief Returns a reference count pointer to the resource
	 */
	ResourcePtr fetchResource(const ResourceKeyType& aKey) = 0;

	/**
	 @brief Privides a reference to the manager trash bin.
	 */
	OTrashBin& trashBin() = 0;

	/**
	 @brief Clears any resources that may be no longer needed.
	 */
	void purge() = 0;
};