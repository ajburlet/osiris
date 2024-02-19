#pragma once

#include <stdint.h>

#include "OsirisSDK/OMemoryManagerScope.h"
#include "OsirisSDK/OSingleton.hpp"

class OAPI OMemoryManager
{
public:
	/**
	 @brief Private class constructor.
	 */
	OMemoryManager();

	/**
	 @brief Class destructor.
	 */
	virtual ~OMemoryManager() = default;

	/**
	 @brief Increase memory usage count for a given scope.
	 @param aScope The scoped to be increased.
	 @param aBytes The number of bytes to be increased.
	 */
	void increase(OMemoryManagerScope aScope, uint64_t aBytes);

	/**
	 @brief Decrease memory usage count for a given scope.
	 @param aScope The scope to be decreased.
	 @param aBytes The number of bytes to be decreased.
	 */
	void decrease(OMemoryManagerScope aScope, uint64_t aBytes);

	/**
	 @brief Memory usage at a given scope.
	 */
	uint64_t usageAt(OMemoryManagerScope aScope) const;

protected:
	uint64_t _usage[static_cast<int>(OMemoryManagerScope::COUNT)];
};

/**
 @brief Memory manager singleton.
 */
using OMemoryManagerSingleton = OSingleton<OMemoryManager>;