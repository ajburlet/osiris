
#include <cinttypes>

#include "OsirisSDK/OError.h"

OError::OError(OString&& aMessage,
               OError::Location&& location)  
    : _message(std::move(aMessage))
    , _location(std::move(location))
{
}

OError::OError(OError&& aOther)
    : _message(std::move(aOther)._message)
    , _location(std::move(aOther)._location)
{
}

OError& OError::operator=(OError&& aOther)
{
    _message = std::move(aOther)._message;
    _location = std::move(aOther)._location;

    return *this;
}

const OString& OError::message() const noexcept
{
    return _message;
}

const OError::Location& OError::location() const noexcept
{
    return _location;
}

OString OError::toString(const OString& aPrefix) const noexcept
{
    auto msg = OString::Fmt("[%s:%" PRIu32 "] %s", aPrefix);
    for (const auto& err : _subErrors)
    {
        msg.append(OString::Fmt("\n%s", err.toString().cString()));
    }
    return msg;
}

OError& OError::with(ODynArray<OError>&& aSubErrors) noexcept
{
    _subErrors = std::move(aSubErrors);
    return *this;
}