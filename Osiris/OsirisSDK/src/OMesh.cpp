#include "OsirisSDK/OException.h"
#include "OsirisSDK/ORenderingEngine.h"
#include "OsirisSDK/OMatrix.hpp"
#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/ORenderComponents.h"
#include "OsirisSDK/OMesh.h"

#include <stdio.h>

struct OMesh::Impl {
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
	if (ORenderable::renderComponents() != nullptr) {
		delete ORenderable::renderComponents();
		setRenderComponents(nullptr);
	}

	_impl = std::move(aOther)._impl;

	return *this;
}


const OMatrix4x4F& OMesh::mvp() const
{
	return _impl->mvp;
}

ORenderComponents& OMesh::renderComponents()
{
	return *ORenderable::renderComponents();
}

inline void OMesh::render(ORenderingEngine * aRenderingEngine, OMatrixStack * aMatrixStack)
{
	_impl->mvp = aMatrixStack->top();
	aRenderingEngine->render(this);
}
