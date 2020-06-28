#include <string.h>
#include "OsirisSDK/OMemoryManager.h"

OMemoryManager::OMemoryManager()
{
	memset(_usage, 0, sizeof(_usage));
}

void OMemoryManager::increase(Scope aScope, uint64_t aBytes)
{
	_usage[static_cast<int>(aScope)] += aBytes;
}

void OMemoryManager::decrease(Scope aScope, uint64_t aBytes)
{
	_usage[static_cast<int>(aScope)] -= aBytes;
}

uint64_t OMemoryManager::usageAt(Scope aScope) const
{
	return _usage[static_cast<uint32_t>(aScope)];
}
