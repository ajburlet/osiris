#pragma once

#include <functional>

#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/ONonMovable.h"

class OEventSystem;

/**
 * @brief Defines the interface for receiving a single event.
 */
template<typename EventType>
class OEventReceiver {
public:
    virtual ~OEventReceiver() = default;
    
    /**
     * @brief Event handler for an individual event type.
     */
    virtual void onEvent(const EventType& event) = 0;
};

/**
 * @brief Defines the interface for receiving multiple events.
 */
template<typename... EventTypes>
class OEventListener : public OEventReceiver<EventTypes>..., 
                       public ONonCopiable,
                       public ONonMovable {
public:
    /**
     * @brief Class constructor.
     */
    OEventListener();

    /**
     * @brief Class destructor.
     */
    virtual ~OEventListener();

private:
    std::function<void()> _unsubscribe;

    friend class OEventSystem;
};

template<typename... EventTypes>
OEventListener<EventTypes...>::OEventListener()
= default;

template<typename... EventTypes>
OEventListener<EventTypes...>::~OEventListener()
{
    if (_unsubscribe) {
        _unsubscribe();
    }
}