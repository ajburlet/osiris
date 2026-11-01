#include <cstring>

#include <OsirisSDK/OPagedBuffer.hpp>

#include "OsirisTests.h"

struct TestA
{
    TestA()=default;
    TestA(int a, int b, float c) : a(a), b(b), c(c) {}
    int a=0;
    int b=0;
    float c=0.0f;
};

struct TestB
{
    TestB()=default;
    TestB(const char* inStr) { strncpy(str, inStr, 20); }
    char str[20];
};

struct TestC
{
    TestC()=default;
    TestC(bool a, int b) : a(a), b(b) {}
    bool a=0;
    int b=0;
};

struct Dispatcher
{
    Dispatcher(OPagedBuffer<>& aBuffer) : buffer(aBuffer) {}

    template <typename T, typename... Args>
    void add(Args&&... aArgs)
    {
        buffer.emplace<T>([](const void* aData, void* aContext) -> void {
            auto self = reinterpret_cast<Dispatcher*>(aContext);
            const auto& obj = *reinterpret_cast<const T*>(aData);
            self->onDispatch(obj);
        }, std::forward<Args>(aArgs)...);
    }

    void onDispatch(const TestA& obj)
    {
        recvA = obj;
    }

    void onDispatch(const TestB& obj)
    {
        memcpy(&recvB, &obj, sizeof(TestB));
    }

    void onDispatch(const TestC& obj)
    {
        recvC = obj;
    }

    OPagedBuffer<>& buffer;
    TestA recvA;
    TestB recvB;
    TestC recvC;
};

OTEST_START(OPagedBuffer, Emplace)
{
    OPagedBuffer buf(80);
    Dispatcher dispatcher(buf);

    dispatcher.add<TestA>(1, 2, 3.0f);
    dispatcher.add<TestB>("This is a test");
    dispatcher.add<TestC>(false, 5);
    dispatcher.add<TestA>(4, 5, 6.0f);

    ASSERT_EQ(buf.size(), 4);

    buf.forEach([&dispatcher](const OPagedBuffer<>::Header& aHeader, const void* aData)
    {
        aHeader.dispatcher(aData, &dispatcher);
    });

    buf.clear();

    ASSERT_EQ(dispatcher.recvA.a, 4);
    ASSERT_EQ(dispatcher.recvA.b, 5);
    ASSERT_EQ(dispatcher.recvA.c, 6.0f);
    
    ASSERT_STREQ(dispatcher.recvB.str, "This is a test");

    ASSERT_EQ(dispatcher.recvC.a, false);
    ASSERT_EQ(dispatcher.recvC.b, 5);
}
OTEST_END