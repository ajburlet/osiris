#include <utility>
#include <string>

#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OShaderObject.h"

using namespace std;

struct OShaderObject::Implementation
{
	using PreprocessorPair = pair<string, string>;
	using PreprocessorList = OList<PreprocessorPair>;

	Implementation(Type aType, const char* aSource) : type(aType), source(aSource) {}

	Type			type;
	std::string		source;
	PreprocessorList	preprocessorList;
};


OShaderObject::OShaderObject(Type aType, const char * aSource)
{
	OExceptionPointerCheck(_impl = new Implementation(aType, aSource));
}

OShaderObject::~OShaderObject()
{
	if (_impl != nullptr) delete _impl;
}

void OShaderObject::setSource(const char* source)
{
	_impl->source = source;
}

const char * OShaderObject::source() const
{
	return _impl->source.c_str();
}

OShaderObject::Type OShaderObject::type() const
{
	return _impl->type;
}

void OShaderObject::addPreprocessorMacro(const char * aName, const char * aValue)
{
	_impl->preprocessorList.pushBack(Implementation::PreprocessorPair(aName, aValue));
}

void OShaderObject::ForEachPreprocessorMacro(PreprocessorIteratorCb aCallback)
{
	for (auto& item : _impl->preprocessorList) aCallback(item.first.c_str(), item.second.c_str());
}

