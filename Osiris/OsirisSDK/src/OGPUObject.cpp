
#include "OsirisSDK/OGPUObject.h"

OGPUObject::OGPUObject(void* aGPUHandle) : _gpuHandle(aGPUHandle)
{
}

void OGPUObject::setGpuHandle(void * aNewGpuHandle)
{
	_gpuHandle = aNewGpuHandle;
}

void * OGPUObject::gpuHandle()
{
	return _gpuHandle;
}
