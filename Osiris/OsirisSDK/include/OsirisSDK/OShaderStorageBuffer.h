#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGPUObject.h"
#include "OsirisSDK/ONonCopiable.h"

class OAPI OShaderStorageBuffer : public OGPUObject
{
private:
    using Super = OGPUObject;

public:
    /**
     * @brief Default class constructor.
     */
    OShaderStorageBuffer() = default;

    /**
     * @brief Class destructor.
     */
    virtual ~OShaderStorageBuffer() = default;

    /**
     * @brief Size of the storage buffer. 
     */
    virtual std::size_t size() const = 0;

    /**
     * @brief Pointer to the memory area.
     */
    virtual const std::uint8_t* data() const = 0;
};
