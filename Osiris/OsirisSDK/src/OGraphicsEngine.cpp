
#include "shaders.h"
#include "OsirisSDK/OGraphicsEngine.h"

struct OGraphicsEngine::Implementation {
	Type type;
	OShaderTable shaderTable;
};

OGraphicsEngine::OGraphicsEngine(Type aType)
{
	_impl = new Implementation;
	_impl->type = aType;
	createShaderTable(_impl->shaderTable);
}

OGraphicsEngine::~OGraphicsEngine()
{
	if (_impl) delete _impl;
}

void OGraphicsEngine::load(const ORenderable * aMesh)
{
	// TODO
}

void OGraphicsEngine::render(const ORenderable * aMesh)
{
	// TODO
}


