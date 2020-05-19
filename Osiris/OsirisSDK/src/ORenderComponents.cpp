#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OShaderArgumentInstanceList.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/ORenderComponents.h"


ORenderComponents::ORenderComponents()
{
	OExceptionPointerCheck(_argumentInstanceList = new OShaderArgumentInstanceList);
}

ORenderComponents::~ORenderComponents()
{
	if (_argumentInstanceList) {
		for (auto arg : *_argumentInstanceList) {
			delete arg;
		}
		delete _argumentInstanceList;
	}
}

ORenderMode ORenderComponents::renderMode() const
{
	return _renderMode;
}

