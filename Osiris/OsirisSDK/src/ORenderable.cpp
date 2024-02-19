#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/ORenderable.h"

ORenderable::ORenderable(Type aType) :
	_type(aType)
{
	OExPointerCheck(_uniforms = new UniformList);
}

ORenderable::~ORenderable()
{
	if (_uniforms) delete _uniforms;
}

ORenderable & ORenderable::operator=(ORenderable && aOther)
{
	if (_uniformsLoaded) throw OEx("Cannot overwrite, since the uniforms must be unloaded first.");
	if (_uniforms != nullptr) delete _uniforms;

	_type = aOther._type;
	_renderComponents = aOther._renderComponents;
	_uniformsLoaded = aOther._uniformsLoaded;
	_uniforms = aOther._uniforms;

	aOther._renderComponents = nullptr;
	aOther._uniforms = nullptr;
	aOther._uniformsLoaded = false;

	return *this;
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

