#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OShaderArgumentInstanceList.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/ORenderable.h"


ORenderable::ORenderable(Type aType) : _type(aType) 
{
	OExceptionPointerCheck(_argumentInstanceList = new OShaderArgumentInstanceList);
}

ORenderable::~ORenderable()
{
	if (_argumentInstanceList) {
		for (auto arg : *_argumentInstanceList) {
			delete arg;
		}
		delete _argumentInstanceList;
	}
}

