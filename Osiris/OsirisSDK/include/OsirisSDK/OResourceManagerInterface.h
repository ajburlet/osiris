#pragma once

#include <functional>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OTrashBinOwner.h"
#include "OsirisSDK/ORefCountObject.hpp"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OStringDefs.h"
#include "OsirisSDK/OGraphicsAllocators.h"

/**
 @brief Resource manager interface.
 @tparam ResourceType Resource data type.
 */
template <class ResourceType>
class OResouceManagerInterface : public OTrashBinOwner
{
public:
	/**
	 @brief Reference count pointer wrapper to the resource object.
	 */
	using ResourcePtr = ORefCountPtr<ResourceType>;

	/** 
	 @brief Returns a reference count pointer to the resource.
	 @param aKey The resource search key.
	 @return A reference countable pointer to the resource.
	 */
	virtual ResourcePtr& fetchResource(const OString& aKey) = 0;

	/**
	 @brief Iteration callback function type.
	 The callback function is in the form: <code>void callbackFn(const OString& aKey, ResourceType& aValue)</code>.
	 */
	using IterationCallbackFn = std::function<void(const OString&, ResourceType&)>;

	/**
	 @brief Iterate through all items.
	 @param aCallbackFn Iteration callback.
	 */
	virtual void forEach(IterationCallbackFn aCallbackFn) = 0;
	
	/**
	 @brief Iteration callback function type (const).
	 The callback function is in the form: <code>void callbackFn(const OString& aKey, const ResourceType& aValue)</code>.
	 */
	using IterationConstCallbackFn = std::function<void(const OString&, const ResourceType&)>;

	/**
	 @brief Iterate through all items.
	 @param aCallbackFn Const iteration callback.
	 */
	virtual void forEach(IterationConstCallbackFn aCallbackFn) const = 0;

	/**
	 @brief Clears any resources that may be no longer needed.
	 */
	virtual void purge() = 0;
};