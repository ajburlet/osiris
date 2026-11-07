
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OEventSystem.hpp"

constexpr size_t kEventSystemPageSize = 16384; // 16K page size

// PIMPL structure storing the nested Osiris OArray containers
struct OEventSystem::Impl {
    using ReallocPolicy = OArrayLinearResizingPolicy<16>;
    using ReceiverList = OArray<void*, ReallocPolicy, Allocator>;
    using ReceiverTable = OArray<ReceiverList, ReallocPolicy, Allocator>;
   
    ReceiverTable receiverTable;
    
    struct PendingRegistration{
        enum class Action {
            add,
            remove,
        };

        Action action;
        uint32_t typeId;
        void* receiver;
    };
    OArray<PendingRegistration, ReallocPolicy, Allocator> pendingRegistrations;
    bool consuming = false;
};

OEventSystem::OEventSystem()
    : _buffer(kEventSystemPageSize)
    , _impl(std::make_unique<Impl>()) {}

OEventSystem::~OEventSystem() = default;

void OEventSystem::registerReceiverRaw(uint32_t typeId, void* receiver)
{
    if (_impl->consuming) {
        _impl->pendingRegistrations.append({
            .action = Impl::PendingRegistration::Action::add,
            .typeId = typeId, 
            .receiver = receiver,
        });
        return;
    }

    if (typeId >= _impl->receiverTable.size()) {
        while (typeId >= _impl->receiverTable.size()) {
            _impl->receiverTable.append(Impl::ReceiverList{});
        }
    }

    auto& list = _impl->receiverTable[typeId];

    // Prevent duplicate subscriptions
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i] == receiver) {
            return;
        }
    }

    list.append(receiver);
}

void OEventSystem::unregisterReceiverRaw(uint32_t typeId, void* receiver) 
{
    if (_impl->consuming) {
        _impl->pendingRegistrations.append({
            .action = Impl::PendingRegistration::Action::remove,
            .typeId = typeId, 
            .receiver = receiver,
        });
        return;
    }

    if (typeId >= _impl->receiverTable.size()) {
        return;
    }

    auto& list = _impl->receiverTable[typeId];
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i] == receiver) {
            list.remove(i);
            break;
        }
    }
}

void OEventSystem::forEachReceiver(uint32_t typeId, const std::function<void(void*)>& callback) const 
{
    if (typeId >= _impl->receiverTable.size()) {
        return;
    }

    const auto& list = _impl->receiverTable[typeId];
    
    // Cache-friendly sequential traversal over contiguous memory blocks of OArray
    for (size_t i = 0; i < list.size(); ++i) {
        callback(list[i]);
    }
}

void OEventSystem::startConsuming()
{
    _impl->consuming = true;
}

void OEventSystem::endConsuming()
{
    _impl->consuming = false;
    for (const auto& pending : _impl->pendingRegistrations) {
        switch(pending.action) {
        case Impl::PendingRegistration::Action::add:
            this->registerReceiverRaw(pending.typeId, pending.receiver);
            break;
        case Impl::PendingRegistration::Action::remove:
            this->unregisterReceiverRaw(pending.typeId, pending.receiver);
            break;
        }
    }
    _impl->pendingRegistrations.clear();
}