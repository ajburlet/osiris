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
	 @brief Set true if the object needs to be loaded to the GPU.
	 The object is constructed with this option set to true.
	 */
	void setNeedsLoading(bool aFlag);

	/**
	 @brief Access to the pointer to the GPU handle (use meant for graphics engine only).
	 */
	void* gpuHandle();

	/**
	 @brief Returns true if the object needs loading to the GPU.
	*/
	bool needsLoading() const;

private:
	void*	_gpuHandle	= nullptr;
	bool	_needsLoading	= true;
};

