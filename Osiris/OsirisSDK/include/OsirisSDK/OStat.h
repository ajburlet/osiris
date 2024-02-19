#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OStringDefs.h"

/**
 @brief Provides information about files or directories.
 */
class OAPI OStat : public OMemoryManagedObject<OSystemMemoryAllocator<OMemoryManagerScope::Default>> {
public:
    /**
     @brief Checks if file exists.
     @return true if file exists. 
     */
    static bool FileExists (const OString& aFilePath) noexcept;

    /**
     @brief Gets file information given a path. 
     */
    static OStat Get (const OString& aFilePath);

    /**
     @brief Class constructor.
     @note Use Get() static method instead.
     */
    OStat ();

    /**
     @brief Class destructor.
     */
    ~OStat();

    uint32_t uid;       /**< User owner identification. */
    uint32_t gid;       /**< Group owner identification. */
    uint64_t size;      /**< File size in bytes. */
    uint8_t mode;       /**< File mode. */
};

