#pragma once

#include <cstdint>

/**
 * @brief Compile-time unique event identifier provider.
 */
class OEventType {
public:
    template<typename T>
    static uint32_t getId() {
        static const uint32_t id = s_counter++;
        return id;
    }

private:
    inline static uint32_t s_counter = 0;
};
