#pragma once

#include "OsirisSDK/OSingleton.hpp"
#include "OsirisSDK/OEventSystem.hpp"

/**
 * @brief Global event system singleton.
 * 
 * The event system used by global event listeners and emitters.
 */
using OGlobalEventSystem = OSingleton<OEventSystem>;