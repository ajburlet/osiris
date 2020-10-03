#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderObject.h"
#include "OsirisSDK/OShaderProgram.h"

using namespace std;

struct OShaderProgram::Implementation {
	using ShaderObjectList = OList<OShaderObject*, OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>>;
	ShaderObjectList objects;
};

OShaderProgram::OShaderProgram()
{
	OExceptionPointerCheck(_impl = new Implementation);
}

OShaderProgram::~OShaderProgram()
{
	for (auto object : _impl->objects) {
		delete object;
	}

	if (_impl) delete _impl;
}

void OShaderProgram::addObject(OShaderObject * aObject)
{
	_impl->objects.pushBack(aObject);
}

void OShaderProgram::addPreprocessorMacro(const char * aName, const char * aValue)
{
	for (auto object : _impl->objects) {
		object->addPreprocessorMacro(aName, aValue);
	}
}

void OShaderProgram::ForEachObject(ObjectIteratorCb aCallback)
{
	for (auto object : _impl->objects) aCallback(object);
}


