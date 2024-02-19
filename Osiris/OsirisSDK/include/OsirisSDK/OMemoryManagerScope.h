#pragma once

/**
 @brief Memory scope enumeration.
 */
enum class OMemoryManagerScope {
		Default = 0,		/**< The default scope, unespecified use. */
		Simulation,		/**< Simulation related objects. */
		Graphics,		/**< Graphics/Rendering related objects. */
		COUNT
};