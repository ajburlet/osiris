#include "OsirisSDK/OException.h"
#include "OsirisSDK/ORenderingEngine.h"
#include "OsirisSDK/OMatrix.hpp"
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
	OMatrix4x4F					mvp{1.0f};
};

OMesh::OMesh() 
	: ORenderable(ORenderable::Type::Mesh)
	, _impl(std::make_unique<OMesh::Impl>())
{
	auto renderComponents = new ORenderComponents;
	OExPointerCheck(renderComponents);
	setRenderComponents(renderComponents);
}

OMesh::OMesh(OMesh && aOther) 
	: ORenderable(std::move(aOther))
	, _impl(std::move(aOther)._impl)
{
}

OMesh::~OMesh() = default;

OMesh & OMesh::operator=(OMesh&& aOther)
{
	if (renderComponents() != nullptr) {
		delete renderComponents();
		setRenderComponents(nullptr);
	}

	_impl = std::move(aOther)._impl;

	return *this;
}


const OMatrix4x4F& OMesh::mvp() const
{
	return _impl->mvp;
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
	_impl->mvp = aMatrixStack->top();
	aRenderingEngine->render(this);
}
