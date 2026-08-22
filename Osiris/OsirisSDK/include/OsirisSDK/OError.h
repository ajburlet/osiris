#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/ONonCopiable.h"

#include <expected>
#include <cstdint>

/**
 * @brief Error description class.
 */
class OAPI OError : public ONonCopiable
{
public:
    /**
     * @brief Provides the location that originated the error.
     */
    struct Location
    {
        const char* file = nullptr;
        const char* function = nullptr;
        uint32_t    line = 0;
    };

    /**
     * @brief Class constructor.
     */
    OError(OString&& message="", Location&& location = {});

    /**
     * @brief Class move constructor.
     */
    OError(OError&& other);

    /**
     * @brief Move assignment operator.
     */
    OError& operator=(OError&& other);

    /**
     * @brief Error message.
     */
    [[nodiscard]]
    const OString& message() const noexcept;

    /**
     * @brief Error location.
     */
    [[nodiscard]]
    const Location& location() const noexcept;

    /**
     * @brief Converts error to string.
     */
    [[nodiscard]]
    OString toString(const OString& prefix="") const noexcept;

    /**
     * @brief Adds sub-errors to the error.
     */
    OError& with(ODynArray<OError>&& subErrors) noexcept;

private:
    OString           _message;
    Location          _location;
    ODynArray<OError> _subErrors;
};

template<typename T>
using OExpected = std::expected<T, OError>;

#define OERROR_LOCATION \
    ::OError::Location{ __FILE__, __func__, static_cast<uint32_t>(__LINE__) }

#define OERROR_CREATE(code, message) \
    ::OError{ code, message, OERROR_LOCATION }