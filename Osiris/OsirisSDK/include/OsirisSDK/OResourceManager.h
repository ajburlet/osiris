#pragma once

#include <functional>
#include <memory>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OTrashBinOwner.h"
#include "OsirisSDK/OTrashBin.h"
#include "OsirisSDK/ORefCountObject.hpp"
#include "OsirisSDK/OStringDefs.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OResourceManagerDefs.h"

class OResource;

/**
 @brief Resource manager interface.
 @tparam ResourceType Resource data type.
 */
template <OResourceType ResourceType>
class OResourceManager : public OMemoryManagedObject<OGraphicsAllocators::Default> 
{
private:
	using Allocator = OGraphicsAllocators::Default;
	using ResourceMap = OMap<OString, ResourceType, Allocator>;

public:
	/**
	 @brief Reference count pointer wrapper to the resource object.
	 */
	using ResourcePtr = ORefCountPtr<ResourceType>;

public:
	/**
	 * @brief Default class constructor.
	 */
	OResourceManager();

	/**
	 * @brief Class destructor.
	 */
	~OResourceManager();

	/**
	 * @brief Adds a resource.
	 * @param resource The resource object.
	 * @return A reference to the stored resource.
	 * @note This effectively transfers ownership of the resource to the manager.
	 */
	ResourceType& add(ResourceType&& resource);

	/** 
	 @brief Returns a reference count pointer to the resource.
	 @param key The resource search key.
	 @return A reference countable pointer to the resource, null if not found.
	 */
	ResourcePtr fetch(const OString& key);

	/**
	 @brief Iteration callback function type.
	 The callback function is in the form: <code>void callbackFn(const OString& aKey, ResourceType& aValue)</code>.
	 */
	using IterationCallbackFn = std::function<void(const OString&, ResourceType&)>;

	/**
	 @brief Iterate through all items.
	 @param callbackFn Iteration callback.
	 */
	void forEach(IterationCallbackFn callbackFn);
	
	/**
	 @brief Iteration callback function type (const).
	 The callback function is in the form: <code>void callbackFn(const OString& aKey, const ResourceType& aValue)</code>.
	 */
	using IterationConstCallbackFn = std::function<void(const OString&, const ResourceType&)>;

	/**
	 @brief Iterate through all items.
	 @param aCallbackFn Const iteration callback.
	 */
	virtual void forEach(IterationConstCallbackFn aCallbackFn) const;

	/**
	 @brief Clears any resources that may be no longer needed.
	 */
	virtual void purge();
	
private:
	ResourceMap _resources;
};

template <OResourceType ResourceType>
inline OResourceManager<ResourceType>::OResourceManager()
= default;

template <OResourceType ResourceType>
inline OResourceManager<ResourceType>::~OResourceManager()
= default;

template <OResourceType ResourceType>
inline ResourceType& OResourceManager<ResourceType>::add(ResourceType&& resource)
{
	typename ResourceMap::Iterator it;
	_resources.insert(resource.name(), std::move(resource), &it);
	return it.value();
} 

template <OResourceType ResourceType>
inline OResourceManager<ResourceType>::ResourcePtr 
OResourceManager<ResourceType>::fetch(const OString& aName)
{
	auto it = _resources.find(aName);
	if (it == _resources.end())
	{
		return nullptr;
	}

	return ResourcePtr(&(it.value()));
}

template <OResourceType ResourceType>
inline void OResourceManager<ResourceType>::forEach(OResourceManager::IterationCallbackFn aCallbackFn)
{
	for (auto it=_resources.begin(); it != _resources.end(); ++it){
		aCallbackFn(it.key(), it.value());
	}
}

template <OResourceType ResourceType>
inline void OResourceManager<ResourceType>::forEach(OResourceManager::IterationConstCallbackFn aCallbackFn) const
{
	for (auto it=_resources.begin(); it != _resources.end(); ++it){
		aCallbackFn(it.key(), it.value());
	}
}

template <OResourceType ResourceType>
inline void OResourceManager<ResourceType>::purge()
{
	OList<decltype(_resources)::Iterator, Allocator> to_remove;
	for (auto it=_resources.begin(); it != _resources.end(); ++it){
		to_remove.pushBack(it);
	}

	for (auto& it : to_remove){
		_resources.remove(it);
	}
}