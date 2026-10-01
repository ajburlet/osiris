#pragma once

#include "OsirisSDK/ORefCountObject.hpp"
#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/OString.hpp"

/**
 * @brief Graphics resource base class.
 */
class OAPI OResource : public ORefCountObject<>, public ONonCopiable
{
public:
    /**
     * @brief Class constructor.
     * @param resourceName Resource unique name.
     */
    OResource(OString&& resourceName="");

    /**
     * @brief Move constructor.
     */
    OResource(OResource&& other);

    /**
     * @brief Class destructor.
     */
    ~OResource();

    OResource& operator=(OResource&& other);

    /**
     * @brief Provides resource unique name.
     */
    const OString& name() const;

private:
    OString _name;
};

inline OResource::OResource(OString&& aResourceName)
    : _name(std::move(aResourceName))
{}

inline OResource::OResource(OResource&& aOther)
    : _name(std::move(aOther)._name)
{}

inline OResource::~OResource() = default;

inline OResource& OResource::operator=(OResource&& aOther)
{
    _name = std::move(aOther)._name;
    return *this;
}

inline const OString& OResource::name() const
{
    return _name;
}