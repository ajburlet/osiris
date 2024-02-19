#include "OsirisSDK/OException.h"
#include "OsirisSDK/ORenderingEngine.h"
#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/ORenderComponents.h"
#include "OsirisSDK/ORefCountObject.hpp"
#include "OsirisSDK/OMeshGeometry.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OMesh.h"

#include <stdio.h>

struct OMesh::Impl {
	ORefCountPtr<OMeshGeometry>	geometry	= nullptr;
	ORefCountPtr<OTexture>		texture		= nullptr;
	OMatrixStack*			matrixStack	= nullptr;
};

OMesh::OMesh() : ORenderable(ORenderable::Type::Mesh)
{
	OExPointerCheck(_impl = new Impl);
	auto renderComponents = new ORenderComponents;
	OExPointerCheck(renderComponents);
	setRenderComponents(renderComponents);
}

OMesh::~OMesh()
{
	if (_impl != nullptr) delete _impl;
}

OMesh & OMesh::operator=(OMesh && aOther)
{
	if (renderComponents() != nullptr) {
		delete renderComponents();
		setRenderComponents(nullptr);
	}

	if (_impl) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

void OMesh::setMatrixStack(OMatrixStack* aMatrixStack)
{
	_impl->matrixStack = aMatrixStack;
}

OMatrixStack* OMesh::matrixStack() const
{
	return _impl->matrixStack;
}

void OMesh::setGeometry(ORefCountPtr<OMeshGeometry>& aGeometry, uint32_t aIndex)
{
	if (!aGeometry.isNull()) {
		renderComponents()->setRenderMode(aGeometry->drawMode());
		renderComponents()->setVertexBuffer(&aGeometry->vertexBuffer());
		renderComponents()->setIndexBuffer(aGeometry->indexedDrawInfoArray()[aIndex].indexBuffer());
	} else {
		renderComponents()->setRenderMode(ORenderMode::Undefined);
		renderComponents()->setVertexBuffer(nullptr);
		renderComponents()->setIndexBuffer(nullptr);
	}
	_impl->geometry = aGeometry;
}

void OMesh::setTexture(ORefCountPtr<OTexture>& aTexture)
{
	if (!aTexture.isNull()) {
		renderComponents()->setTexture(aTexture.getPtr());
	} else {
		renderComponents()->setTexture(nullptr);
	}
	_impl->texture = aTexture;
}

inline void OMesh::render(ORenderingEngine * aRenderingEngine, OMatrixStack * aMatrixStack)
{
	setMatrixStack(aMatrixStack);
	aRenderingEngine->render(this);
}
