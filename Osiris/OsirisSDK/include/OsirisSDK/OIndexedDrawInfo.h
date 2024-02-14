#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"

class OIndexBuffer;
class OMaterial;

/**
 @brief Indexed draw information class.

 This class holds information for index buffer and the related material properties of the group of faces,
 representing a single mesh draw call.
 */
class OAPI OIndexedDrawInfo : public OMemoryManagedObject<OGraphicsAllocators::Default>, public ONonCopiableT<OIndexedDrawInfo, false>
{
public:
	using Allocator = OGraphicsAllocators::Default;
	using Array = OArrayNC<OIndexedDrawInfo,Allocator>;

	/**
	 @brief Class constructor.
	 @param aMaterial Pointer to the material object, <code>nullptr</code> if none.
	 @note The object will have its own copy of the material object, so it won't assume ownership.
	 */
	OIndexedDrawInfo(const OMaterial* aMaterial=nullptr);

	/**
	 @brief Move constructor.
	 */
	OIndexedDrawInfo(OIndexedDrawInfo&& aOther);

	/**
	 @brief Move assignment operator.
	 */
	OIndexedDrawInfo& operator=(OIndexedDrawInfo&& aOther);

	/**
	 @brief Class destructor.
	 */
	~OIndexedDrawInfo();

	/**
	 @brief Pointer to the index buffer.
	 */
	OIndexBuffer* indexBuffer();

	/**
	 @brief Pointer to the material object.
	 */
	OMaterial* material();

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};