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

	enum class Usage {
		GPU,		/**< Will be used by the GPU only. No need to keep local copy after loaded. */
		Mixed,		/**< Used by GPU and CPU, so a local copy is kept for read/write. */
		Default=GPU	/**< Default usage: GPU. */
	};

	/**
	 @brief Sets the resource usage type.
	 */
	void setUsage(Usage aUsage);

	/**
	 @brief Access to the pointer to the GPU handle (use meant for graphics engine only).
	 */
	void* gpuHandle();

	/**
	 @brief Returns a cast to the GPU handler (use meant for graphics engine only).
	 */
	template <typename T>
	T& gpuHandleCastTo();

	/**
	 @brief Returns true if the object needs loading to the GPU.
	*/
	bool needsLoading() const;

	/**
	 @brief Returns the resource usage type.
	 */
	Usage usage() const;

private:
	void*	_gpuHandle	= nullptr;
	bool	_needsLoading	= true;
	Usage	_usage		= Usage::Default;
};

inline OGPUObject::OGPUObject(void* aGPUHandle) : _gpuHandle(aGPUHandle)
{
}

inline void OGPUObject::setGpuHandle(void * aNewGpuHandle)
{
	_gpuHandle = aNewGpuHandle;
}

inline void OGPUObject::setNeedsLoading(bool aFlag)
{
	_needsLoading = aFlag;
}

inline void OGPUObject::setUsage(Usage aUsage)
{
	_usage = aUsage;
}

inline void * OGPUObject::gpuHandle()
{
	return _gpuHandle;
}

template<typename T>
inline T & OGPUObject::gpuHandleCastTo()
{
	return *reinterpret_cast<T*>(_gpuHandle);
}

inline bool OGPUObject::needsLoading() const
{
	return (_gpuHandle == nullptr || _needsLoading);
}

inline OGPUObject::Usage OGPUObject::usage() const
{
	return _usage;
}

