#pragma once

/**
 * @brief Estabilishes a non-movable class.
 */
class ONonMovable
{
public:
    /**
     * @brief Default class constructor.
     */
    ONonMovable() = default;

    /**
     * @brief Deleted move constructor.
     */
    ONonMovable(ONonMovable&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    ONonMovable& operator=(ONonMovable&&) = delete;
};