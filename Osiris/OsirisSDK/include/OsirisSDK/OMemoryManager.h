#pragma once

#include <stdint.h>

#include "OSingleton.hpp"

class OMemoryManager
{
public:
	/**
	 @brief Memory scope enumeration.
	 */
	enum class Scope {
		Default = 0,		/**< The default scope, unespecified use. */
		Simulation,		/**< Simulation related objects. */
		Graphics,		/**< Graphics/Rendering related objects. */
		COUNT
	};

public:
	/**
	 @brief Private class constructor.
	 */
	OMemoryManager() = default;

	/**
	 @brief Class destructor.
	 */
	virtual ~OMemoryManager() = default;

	/**
	 @brief Increase memory usage count for a given scope.
	 @param aScope The scoped to be increased.
	 @param aBytes The number of bytes to be increased.
	 */
	void increase(Scope aScope, uint64_t aBytes);

	/**
	 @brief Decrease memory usage count for a given scope.
	 @param aScope The scope to be decreased.
	 @param aBytes The number of bytes to be decreased.
	 */
	void decrease(Scope aScope, uint64_t aBytes);


protected:
	uint64_t _usage[static_cast<int>(Scope::COUNT)];
};

/**
 @brief Memory manager singleton.
 */
using OMemoryManagerSingleton = OSingleton<OMemoryManager>;