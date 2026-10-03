#pragma once

#include <utility>

#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OShaderStorageBuffer.h"

/**
 * @brief A shader storage buffer defined as an array of types.
 */
template<typename T, OArrayReallocPolicy ReallocPolicy=OArrayNoResizePolicy>
class OShaderArrayBuffer : public OMemoryManagedObject<OGraphicsAllocators::Default>, 
                           public OShaderStorageBuffer
{
private:
    using Super = OShaderStorageBuffer;

public:
    using Array = OArray<T, ReallocPolicy, OGraphicsAllocators::Default>;

public:
    /**
     * @brief Default class constructor.
     */
    OShaderArrayBuffer() = default;

    /**
     * @brief Class constructor.
     * 
     * @param array R-value reference to the array to be owned by the buffer.
     */
    OShaderArrayBuffer(Array&& array);

    /**
     * @brief Class move constructor.
     */
    OShaderArrayBuffer(OShaderArrayBuffer&& other);

    /**
     * @brief Class destructor.
     */
    virtual ~OShaderArrayBuffer() = default;

    /**
     * @brief Assignemnt operator overload to assume ownership of the array.
     * 
     * @param array R-value reference to the array tobe owned by the buffer.
     */
    OShaderArrayBuffer& operator=(Array&& array);

    /**
     * @brief Assignment move operator.
     */
    OShaderArrayBuffer& operator=(OShaderArrayBuffer&& other);

    std::size_t size() const override;

    const std::uint8_t* data() const override;

private:
    Array _array;
};

template<typename T, OArrayReallocPolicy ReallocPolicy>
inline OShaderArrayBuffer<T, ReallocPolicy>::OShaderArrayBuffer(
    typename OShaderArrayBuffer<T, ReallocPolicy>::Array&& aArray)
    : _array(std::move(aArray))
{}

template<typename T, OArrayReallocPolicy ReallocPolicy>
inline OShaderArrayBuffer<T, ReallocPolicy>::OShaderArrayBuffer(OShaderArrayBuffer&& aOther)
    : _array(std::move(aOther)._array)
{}

template<typename T, OArrayReallocPolicy ReallocPolicy>
inline OShaderArrayBuffer<T, ReallocPolicy>& 
OShaderArrayBuffer<T, ReallocPolicy>::operator=(
    typename OShaderArrayBuffer<T, ReallocPolicy>::Array&& aArray)
{
    _array = std::move(aArray);
    return *this;
}

template<typename T, OArrayReallocPolicy ReallocPolicy>
inline OShaderArrayBuffer<T, ReallocPolicy>& 
OShaderArrayBuffer<T, ReallocPolicy>::operator=(OShaderArrayBuffer&& aOther)
{
    _array = std::move(aOther)._array;
}

template<typename T, OArrayReallocPolicy ReallocPolicy>
inline std::size_t OShaderArrayBuffer<T, ReallocPolicy>::size() const
{
    return _array.size()*sizeof(T);
}

template<typename T, OArrayReallocPolicy ReallocPolicy>
inline const std::uint8_t* OShaderArrayBuffer<T, ReallocPolicy>::data() const
{
    return reinterpret_cast<const std::uint8_t*>(&_array[0]);
}