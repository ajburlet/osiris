#pragma once

#include "OsirisSDK/defs.h"

/**
 @brief Base GPU object class.
 */
class OAPI OGPUObject 
{
public:
	/**
	 @brief Class constructor.
	 */
	OGPUObject(void* aGpuHandle = nullptr);

	/**
	 @brief Class destructor.
	 */
	virtual ~OGPUObject() = default;

	/**
	 @brief Sets the GPU handle (use meant for graphics engine only).
	 */
	void setGpuHandle(void* aNewGpuHandle);

	/**
	 @brief Access to the pointer to the GPU handle (use meant for graphics engine only).
	 */
	void* gpuHandle();

private:
	void* _gpuHandle = nullptr;
};

