#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderArgumentInstanceList.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/ORenderComponents.h"


ORenderComponents::ORenderComponents()
{
	OExceptionPointerCheck(_argumentInstanceList = new OShaderArgumentInstanceList);
}

ORenderComponents::~ORenderComponents()
{
	if (_argumentInstanceList) {
		for (auto arg : *_argumentInstanceList) {
			delete arg;
		}
		delete _argumentInstanceList;
	}
}

ORenderMode ORenderComponents::renderMode() const
{
	return _renderMode;
}

bool ORenderComponents::loaded() const
{
	if (_vertexBuffer == nullptr || _vertexBuffer->needsLoading() || (_indexBuffer != nullptr && _indexBuffer->needsLoading()) ||
	    (_texture != nullptr && _texture->needsLoading())) {
		return false;
	}
	return true;
}

