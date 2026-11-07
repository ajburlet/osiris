#pragma once
#include <utility>
#include <concepts>

/**
 * @brief Executes a given function when going out of scope.
 * 
 * Use this to ensure that a given code is executed whenever going out of scope.
 */
template <typename Fn>
requires std::invocable<Fn>
class OScopeGuard 
{
public:
    /**
     * @brief Class constructor.
     * @param fn The function to be used when going out of scope.
     */
    explicit OScopeGuard(Fn&& fn) 
        : m_fn(std::forward<Fn>(fn)), m_active(true) {}

    /**
     * @brief Class destructor,.
     */
    ~OScopeGuard() {
        if (m_active) {
            m_fn();
        }
    }

    // Prevent copying
    OScopeGuard(const OScopeGuard&) = delete;
    OScopeGuard& operator=(const OScopeGuard&) = delete;

    // Allow moving if needed
    OScopeGuard(OScopeGuard&& other) noexcept 
        : m_fn(std::move(other.m_fn)), m_active(other.m_active) {
        other.m_active = false;
    }

    OScopeGuard& operator=(OScopeGuard&& other) noexcept {
        if (this != &other) {
            if (m_active) {
                m_fn();
            }
            m_fn = std::move(other.m_fn);
            m_active = other.m_active;
            other.m_active = false;
        }
        return m_active; // (or handle assignment)
    }

    // Disarm the guard so it doesn't execute on destruction
    void Dismiss() {
        m_active = false;
    }

private:
    Fn m_fn;
    bool m_active;
};

// Deduction guide for cleaner syntax in C++17/20
template <typename Fn>
OScopeGuard(Fn) -> OScopeGuard<Fn>;