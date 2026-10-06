#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OGeometry.h"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OMaterialSet.h"
#include "OsirisSDK/ORenderComponents.h"


ORenderComponents::ORenderComponents()
{
}

ORenderComponents::~ORenderComponents()
{
}

ORenderMode ORenderComponents::renderMode() const
{
	return _renderMode;
}

bool ORenderComponents::componentsLoaded() const
{
	if ( _geometry->vertexBuffer().needsLoading() || 
		(_geometry->drawMode() == ORenderMode::IndexedTriangle && _geometry->indexBuffer().needsLoading()) ||
		(_geometry->materialIndexBuffer().size() > 0 && _geometry->materialIndexBuffer().needsLoading()) ||
	    (!_texture.isNull() && _texture->needsLoading()) ||
		(!_materialSet.isNull() && _materialSet->needsLoading())) {
		return false;
	}
	return true;
}

void ORenderComponents::setGeometry(OGeometry& aGeometry)
{
	_geometry = &aGeometry;
	setRenderMode(aGeometry.drawMode());
}

