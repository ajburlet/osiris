
#include <OsirisSDK/OEventSystem.hpp>

#include "OsirisTests.h"

struct ClickEvent
{
    int x;
    int y;
    int btn;

    bool operator==(const ClickEvent&) const = default;
};

struct KeyEvent
{
    char c;

    bool operator==(const KeyEvent&) const = default;
};

class Receiver : public OEventListener<ClickEvent, KeyEvent>
{
public:
    void onEvent(const ClickEvent& evt)
    {
        click = evt;
    }

    void onEvent(const KeyEvent& evt)
    {
        key = evt;
    }

    ClickEvent click;
    KeyEvent key;
};

OTEST_START(OEventSystem, Emit)
{
    OEventSystem eventSystem;
    Receiver rec;

    eventSystem.subscribe(rec);

    eventSystem.emit<KeyEvent>('X');
    eventSystem.emit<ClickEvent>(10, 20, 1);

    eventSystem.consume();

    ASSERT_EQ(rec.click, (ClickEvent{.x = 10, .y = 20, .btn = 1}));
    ASSERT_EQ(rec.key, (KeyEvent{.c = 'X'}));

}
OTEST_END

struct EmitDuringConsumeTriggerEvent {};
struct EmitDuringConsumeFollowupEvent {};

class EmitDuringConsumeReceiver : public OEventListener<EmitDuringConsumeTriggerEvent, EmitDuringConsumeFollowupEvent>
{
public:
    explicit EmitDuringConsumeReceiver(OEventSystem& eventSystem)
        : _eventSystem(eventSystem)
    {}

    void onEvent(const EmitDuringConsumeTriggerEvent&)
    {
        _eventSystem.emit<EmitDuringConsumeFollowupEvent>();
    }

    void onEvent(const EmitDuringConsumeFollowupEvent&)
    {
        ++followupCount;
    }

    int followupCount = 0;

private:
    OEventSystem& _eventSystem;
};

OTEST_START(OEventSystem, EmitDuringConsume)
{
    OEventSystem eventSystem;
    EmitDuringConsumeReceiver receiver(eventSystem);
    eventSystem.subscribe(receiver);

    eventSystem.emit<EmitDuringConsumeTriggerEvent>();
    eventSystem.consume();

    ASSERT_EQ(receiver.followupCount, 1);
}
OTEST_END

struct SubscribeDuringConsumeTriggerEvent {};
struct SubscribeDuringConsumeTargetEvent {};

class SubscribeDuringConsumeTargetReceiver : public OEventListener<SubscribeDuringConsumeTargetEvent>
{
public:
    void onEvent(const SubscribeDuringConsumeTargetEvent&)
    {
        ++eventCount;
    }

    int eventCount = 0;
};

class SubscribeDuringConsumeReceiver : public OEventListener<SubscribeDuringConsumeTriggerEvent>
{
public:
    SubscribeDuringConsumeReceiver(OEventSystem& eventSystem, SubscribeDuringConsumeTargetReceiver& target)
        : _eventSystem(eventSystem)
        , _target(target)
    {}

    void onEvent(const SubscribeDuringConsumeTriggerEvent&)
    {
        _eventSystem.subscribe(_target);
    }

private:
    OEventSystem& _eventSystem;
    SubscribeDuringConsumeTargetReceiver& _target;
};

OTEST_START(OEventSystem, SubscribeDuringConsume)
{
    OEventSystem eventSystem;
    SubscribeDuringConsumeTargetReceiver target;
    SubscribeDuringConsumeReceiver receiver(eventSystem, target);
    eventSystem.subscribe(receiver);

    eventSystem.emit<SubscribeDuringConsumeTriggerEvent>();
    eventSystem.emit<SubscribeDuringConsumeTargetEvent>();
    eventSystem.consume();

    ASSERT_EQ(target.eventCount, 0);

    eventSystem.emit<SubscribeDuringConsumeTargetEvent>();
    eventSystem.consume();

    ASSERT_EQ(target.eventCount, 1);
}
OTEST_END

struct UnsubscribeDuringConsumeTriggerEvent {};
struct UnsubscribeDuringConsumeTargetEvent {};

class UnsubscribeDuringConsumeTargetReceiver : public OEventListener<UnsubscribeDuringConsumeTargetEvent>
{
public:
    void onEvent(const UnsubscribeDuringConsumeTargetEvent&)
    {
        ++eventCount;
    }

    int eventCount = 0;
};

class UnsubscribeDuringConsumeReceiver : public OEventListener<UnsubscribeDuringConsumeTriggerEvent>
{
public:
    UnsubscribeDuringConsumeReceiver(OEventSystem& eventSystem, UnsubscribeDuringConsumeTargetReceiver& target)
        : _eventSystem(eventSystem)
        , _target(target)
    {}

    void onEvent(const UnsubscribeDuringConsumeTriggerEvent&)
    {
        _eventSystem.unsubscribe(_target);
    }

private:
    OEventSystem& _eventSystem;
    UnsubscribeDuringConsumeTargetReceiver& _target;
};

OTEST_START(OEventSystem, UnsubscribeDuringConsume)
{
    OEventSystem eventSystem;
    UnsubscribeDuringConsumeTargetReceiver target;
    UnsubscribeDuringConsumeReceiver receiver(eventSystem, target);
    eventSystem.subscribe(target);
    eventSystem.subscribe(receiver);

    eventSystem.emit<UnsubscribeDuringConsumeTriggerEvent>();
    eventSystem.emit<UnsubscribeDuringConsumeTargetEvent>();
    eventSystem.consume();

    ASSERT_EQ(target.eventCount, 1);

    eventSystem.emit<UnsubscribeDuringConsumeTargetEvent>();
    eventSystem.consume();

    ASSERT_EQ(target.eventCount, 1);
}
OTEST_END