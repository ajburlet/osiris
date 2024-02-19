#include <string.h>
#include "OsirisSDK/OMemoryManager.h"

OMemoryManager::OMemoryManager()
{
	memset(_usage, 0, sizeof(_usage));
}

void OMemoryManager::increase(OMemoryManagerScope aScope, uint64_t aBytes)
{
	_usage[static_cast<int>(aScope)] += aBytes;
}

void OMemoryManager::decrease(OMemoryManagerScope aScope, uint64_t aBytes)
{
	_usage[static_cast<int>(aScope)] -= aBytes;
}

uint64_t OMemoryManager::usageAt(OMemoryManagerScope aScope) const
{
	return _usage[static_cast<uint32_t>(aScope)];
}
