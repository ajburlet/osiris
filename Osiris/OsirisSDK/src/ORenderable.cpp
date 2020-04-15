#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/ORenderable.h"

using ArgumentList = ODynArray<OShaderArgumentInstance*, OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>>;

struct ORenderable::Impl {
	ArgumentList argument_list;
};

ORenderable::ORenderable(Type aType) : _type(aType) 
{
	OExceptionPointerCheck(_impl = new Impl);
}

ORenderable::~ORenderable()
{
	if (_impl) {
		for (auto arg : _impl->argument_list) {
			delete arg;
		}
		delete _impl;
	}
}

void ORenderable::addUniformArgument(OShaderArgumentInstance* aNewArgument)
{
	_impl->argument_list.append(aNewArgument);
}
