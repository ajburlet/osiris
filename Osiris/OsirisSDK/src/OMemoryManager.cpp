
#include "OsirisSDK/OMemoryManager.h"


void OMemoryManager::increase(Scope aScope, uint64_t aBytes)
{
	_usage[static_cast<int>(aScope)] += aBytes;
}

void OMemoryManager::decrease(Scope aScope, uint64_t aBytes)
{
	_usage[static_cast<int>(aScope)] -= aBytes;
}
