#pragma once

#include <memory>
#include <cstdint>
#include <utility>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OResource.h"
#include "OSirisSDK/OShaderStorageBuffer.h"
#include "OsirisSDK/OStringDefs.h"

class OMaterial;

/**
 @brief Resource containing the ordered materials referenced by one mesh.
 */
class OAPI OMaterialSet : public OMemoryManagedObject<OGraphicsAllocators::Default>, 
                          public OShaderStorageBuffer,
                          public OResource
{
public:
    using MaterialPtr = ORefCountPtr<OMaterial>;

private:
    using Super = OResource;

    using Allocator = OGraphicsAllocators::Default;

public:
    /**
     * @brief Class constructor.
     * @param name Material set name. 
     */
    OMaterialSet(OString&& name="");

    /**
     * @brief Move constructor.
     */
    OMaterialSet(OMaterialSet&& other);

    /**
     * @brief Class destructor.
     */
    ~OMaterialSet();

    /**
     * @brief Adds a material to the set.
     * @param material Pointer to the material object to be added.
     */
    void add(MaterialPtr material);

    /**
     * @brief Number of materials in the set.
     * @return Material count.
     */
    uint32_t count() const; 
    
    /**
     * @brief Acessor for a material for a given index.
     * @return Pointer to the material resource.
     */
    const MaterialPtr& at(uint32_t aIndex) const;

    // OGPUObject interface
    std::size_t size() const override;
    const std::uint8_t* data() const override;

private:
    /**
     * @cond INTERNAL
     */
    struct Impl;
    std::unique_ptr<Impl> _impl;
    /**
     * @endcond
     */
};
