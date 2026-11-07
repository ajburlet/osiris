#pragma once

#include <utility>

#include "OGlobalEventSystem.h"

class OGlobalEventEmitter
{
public:
    /**
     * @brief Default class constructor.
     */
    OGlobalEventEmitter();

    /**
     * @brief Class destructor.
     */
    virtual ~OGlobalEventEmitter();

    template <typename T, typename... Args>
    void emit(Args&&... args);
};

inline OGlobalEventEmitter::OGlobalEventEmitter()
= default;

inline OGlobalEventEmitter::~OGlobalEventEmitter()
= default;

template <typename T, typename... Args>
inline void OGlobalEventEmitter::emit(Args&&... args)
{
    OGlobalEventSystem()->emit<T>(std::forward<Args>(args...));
}