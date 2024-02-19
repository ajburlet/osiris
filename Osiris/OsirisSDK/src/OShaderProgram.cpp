#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderObject.h"
#include "OsirisSDK/OShaderProgram.h"

struct OShaderProgram::Impl {
	using ShaderObjectList = OList<OShaderObject*, OSystemMemoryAllocator<OMemoryManagerScope::Graphics>>;
	ShaderObjectList objects;
};

OShaderProgram::OShaderProgram()
{
	OExPointerCheck(_impl = new Impl);
}

OShaderProgram::~OShaderProgram()
{
	for (auto object : _impl->objects) {
		delete object;
	}

	if (_impl) delete _impl;
}

OShaderProgram & OShaderProgram::operator=(OShaderProgram && aOther)
{
	if (_impl != nullptr) delete _impl;

	_impl = aOther._impl;
	aOther._impl = nullptr;

	return *this;
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


