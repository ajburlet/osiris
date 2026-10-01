#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OMaterialSet.h"

struct OMaterialSet::Impl
{
    using MaterialArray = ODynArray<MaterialPtr, Allocator>;

    struct alignas(16) MaterialBufferItem
    {
        OVector3F::GLMType color;
    };
    using MaterialBuffer = OArray<MaterialBufferItem, OArrayNoResizePolicy, Allocator>;

    MaterialArray materials;
    MaterialBuffer buffer;
};


OMaterialSet::OMaterialSet(OString&& aName)
    : Super(std::move(aName))
    , _impl(std::make_unique<OMaterialSet::Impl>())
{}

OMaterialSet::OMaterialSet(OMaterialSet&& aOther)
    : Super(std::move(aOther))
    , _impl(std::move(aOther)._impl)
{}

OMaterialSet::~OMaterialSet()
= default;

void OMaterialSet::add(OMaterialSet::MaterialPtr aMaterial)
{
   _impl->materials.append(std::move(aMaterial)); 
}

uint32_t OMaterialSet::count() const
{
    return static_cast<uint32_t>(_impl->materials.size()); 
}

const OMaterialSet::MaterialPtr& OMaterialSet::at(uint32_t aIndex) const
{
    return _impl->materials.get(aIndex);
}

std::size_t OMaterialSet::size() const
{
    return this->count()*sizeof(Impl::MaterialBufferItem);
}

const std::uint8_t* OMaterialSet::data() const
{
    _impl->buffer.clear();
    _impl->buffer.changeCapacity(_impl->materials.size());
    for (auto& mat : _impl->materials)
    {
        _impl->buffer.append(Impl::MaterialBufferItem{.color = mat->diffuseColor().glm()});
    }
    return reinterpret_cast<std::uint8_t*>(&_impl->buffer[0]);
}

