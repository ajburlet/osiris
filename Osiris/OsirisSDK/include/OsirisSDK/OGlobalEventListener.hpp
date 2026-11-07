#pragma once

#include "OsirisSDK/OGlobalEventSystem.h"
#include "OsirisSDK/OEventListener.hpp"

/**
 * @brief Event listeners that are automatically subscribed to the global event system.
 */
template<typename... EventTypes>
class OGlobalEventListener : public OEventListener<EventTypes...>
{
public:
    /**
     * @brief Default class constructor.
     */
    OGlobalEventListener();

    /**
     * @brief Class destructor.
     */
    virtual ~OGlobalEventListener();
};

template<typename... EventTypes>
inline OGlobalEventListener<EventTypes...>::OGlobalEventListener()
{
    OGlobalEventSystem()->subscribe(*this);
}

template<typename... EventTypes>
inline OGlobalEventListener<EventTypes...>::~OGlobalEventListener()
= default;