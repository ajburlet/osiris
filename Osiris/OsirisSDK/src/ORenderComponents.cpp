#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OTexture.h"
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
	if (_vertexBuffer == nullptr || _vertexBuffer->needsLoading() || (_indexBuffer != nullptr && _indexBuffer->needsLoading()) ||
	    (_texture != nullptr && _texture->needsLoading())) {
		return false;
	}
	return true;
}

