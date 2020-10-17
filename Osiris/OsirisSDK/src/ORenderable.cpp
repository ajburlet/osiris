#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/ORenderable.h"

ORenderable::ORenderable(Type aType) :
	_type(aType)
{
	OExceptionPointerCheck(_uniforms = new UniformList);
}

ORenderable::~ORenderable()
{
	if (_uniforms) delete _uniforms;
}

void ORenderable::updateUniforms()
{
	for (auto& uniform : *_uniforms) uniform.update(this);
}

void ORenderable::setUniformsLoaded(bool aLoaded)
{
	_uniformsLoaded = aLoaded;
}

bool ORenderable::uniformsLoaded() const
{
	return _uniformsLoaded;
}

