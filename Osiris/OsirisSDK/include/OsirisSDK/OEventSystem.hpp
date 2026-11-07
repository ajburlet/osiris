#pragma once

#include <memory>
#include <functional>

#include "OsirisSDK/OScopeGuard.hpp" 
#include "OsirisSDK/OPagedBuffer.hpp"
#include "OsirisSDK/OEventListener.hpp"
#include "OsirisSDK/OEventType.hpp"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/OBaseAllocators.h"

/**
 * @brief Queues events and dispatches them to subscribed listeners.
 */
class OAPI OEventSystem : public OMemoryManagedObject<OBaseAllocators::Default>, 
                          public ONonCopiable 
{
private:
    using Allocator = OBaseAllocators::Default;
    using PagedBuffer = OPagedBuffer<Allocator>;

public:
    /**
     * @brief Class constructor.
     */
    OEventSystem();

    /**
     * @brief Class destructor.
     */
    ~OEventSystem();

    /**
     * @brief Queues an event for dispatch by consume().
     * 
     * @tparam T Event type.
     * @tparam Args Event construction argument types.
     * 
     * @param args Variadic list of event constructor arguments.
     */
    template<typename T, typename... Args>
    void emit(Args&&... args); 

    /**
     * @brief Subscribes a listener to its declared event types.
     *
     * A listener may belong to only one event system, which must outlive it.
     * Changes made during consume() take effect after the current cycle.
     * 
     * @param listener The listener to be subscribed.
     */
    template<typename... EventTypes>
    void subscribe(OEventListener<EventTypes...>& listener); 

    /**
     * @brief Unsubscribes a listener from this event system.
     * 
     * @param listener The listener to be unsubscribed.
     */
    template<typename... EventTypes>
    void unsubscribe(OEventListener<EventTypes...>& listener); 

    /**
     * @brief Dispatches queued events, including events emitted during dispatch, then clears the queue.
     */
    void consume();

    /**
     * @cond INTERNAL
     */
private:
    template<typename T>
    void registerReceiver(OEventReceiver<T>* receiver) 
    {
        registerReceiverRaw(OEventType::getId<T>(), receiver);
    }

    template<typename T>
    void unregisterReceiver(OEventReceiver<T>* receiver) 
    {
        unregisterReceiverRaw(OEventType::getId<T>(), receiver);
    }

    void registerReceiverRaw(uint32_t typeId, void* receiver);
    void unregisterReceiverRaw(uint32_t typeId, void* receiver);
    void forEachReceiver(uint32_t typeId, const std::function<void(void*)>& callback) const;
    void startConsuming();
    void endConsuming();

private:
    PagedBuffer _buffer;
    struct Impl;
    std::unique_ptr<Impl> _impl;
    /**
     * @endcond
     */
};

template<typename T, typename... Args>
inline void OEventSystem::emit(Args&&... aArgs) 
{
    _buffer.emplace<T>([](const void* aData, void* aContext) -> void {
        auto& self = *reinterpret_cast<OEventSystem*>(aContext);
        const auto& event = *reinterpret_cast<const T*>(aData);
        self.forEachReceiver(OEventType::getId<T>(), [&event](void* aReceiverPtr){
            auto& receiver = *reinterpret_cast<OEventReceiver<T>*>(aReceiverPtr);
            receiver.onEvent(event);
        });
    }, std::forward<Args>(aArgs)...);
}
    
template<typename... EventTypes>
void OEventSystem::subscribe(OEventListener<EventTypes...>& listener) 
{
    if (listener._unsubscribe) [[unlikely]] {
        throw OEx("Listener is already subscribed to another event system");
    }
    (registerReceiver<EventTypes>(&listener), ...);
    listener._unsubscribe = [this, &listener] {
        this->unsubscribe(listener);
    };
}

template<typename... EventTypes>
void OEventSystem::unsubscribe(OEventListener<EventTypes...>& listener) 
{
    (unregisterReceiver<EventTypes>(&listener), ...);
    listener._unsubscribe = {};
}
    
void OEventSystem::consume()
{
    OScopeGuard guard([&](){
        _buffer.clear();
        endConsuming();
    });        

    startConsuming();
    _buffer.forEach([&](const OPagedBuffer<>::Header& aHeader, const void* aData) {
        aHeader.dispatcher(aData, this);
    });
}