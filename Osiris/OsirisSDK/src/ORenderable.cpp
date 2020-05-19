#include "OsirisSDK/OException.h"
#include "OsirisSDK/ORenderComponents.h"
#include "OsirisSDK/ORenderable.h"

ORenderable::ORenderable(Type aType) :
	_type(aType)
{
	OExceptionPointerCheck(_renderComponents = new ORenderComponents);
}

ORenderable::~ORenderable()
{
	if (_renderComponents) delete _renderComponents;
}

