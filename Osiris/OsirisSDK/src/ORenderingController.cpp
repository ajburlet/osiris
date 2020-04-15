#include "shaders.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMesh.h"
#include "OsirisSDK/OText2D.h"
#include "OsirisSDK/OShaderObject.h"
#include "OsirisSDK/OShaderProgram.h"
#include "OsirisSDK/OGraphicsRenderCommandEncoder.h"
#include "OsirisSDK/OGraphicsResourceCommandEncoder.h"
#include "OsirisSDK/OGraphicsCommandBuffer.h"
#include "OsirisSDK/OGraphicsCommandQueue.h"
#include "OsirisSDK/ORenderingController.h"

// ----------------------------------------------------------------------------------------------
// OGra implementation structure
// ----------------------------------------------------------------------------------------------
struct ORenderingController::Impl {
	uint32_t shaderKey(ORenderable* aRenderable);
	OShaderProgram* createProgram(ORenderable* aRenderableType, const char* aVertexSrc, const char* aFragmentSrc);
	void loadMesh(OMesh* aMesh);
	void loadText2D(OText2D* aText);
	void renderMesh(OMesh* aMesh);
	void renderText2D(OText2D* aText);

	struct ShaderKey {
		ORenderable::Type	renderableType;
		uint32_t		typeKey;

		bool operator<(const ShaderKey& aOther) const {
			if (renderableType == aOther.renderableType) {
				return (typeKey < aOther.typeKey);
			}
			return renderableType < aOther.renderableType;
		}
	};
	using ShaderProgramCache = OMap<ShaderKey, OShaderProgram*>;

	OGraphicsEngine*	engine;
	OShaderSourceTable	shaderSourceTable;
	ShaderProgramCache	shaderProgramCache;
};

// ----------------------------------------------------------------------------------------------
// ORenderingController
// ----------------------------------------------------------------------------------------------
ORenderingController::ORenderingController(OGraphicsEngine* aEngine)
{
	OExceptionPointerCheck(_impl = new Impl);
	_impl->engine = aEngine;
	createShaderTable(_impl->shaderSourceTable);
}

ORenderingController::~ORenderingController()
{
	if (_impl) delete _impl;
}

void ORenderingController::load(ORenderable* aRenderable)
{
	auto shaderKey = _impl->shaderKey(aRenderable);
	auto shaderIt = _impl->shaderProgramCache.find({ aRenderable->type(), shaderKey });
	OShaderProgram* shader = nullptr;
	if (shaderIt == _impl->shaderProgramCache.end()) {
		auto vertSrcIt = _impl->shaderSourceTable.find({ _impl->engine->type(), 
								 OShaderObject::Type::Vertex, 
								 aRenderable->type() });
		auto fragSrcIt = _impl->shaderSourceTable.find({ _impl->engine->type(), 
								 OShaderObject::Type::Fragment, 
								 aRenderable->type() });
		shader = _impl->createProgram(aRenderable, vertSrcIt.value().c_str(), fragSrcIt.value().c_str());
		OExceptionForwardCb([&]() { delete shader; }, 
			_impl->shaderProgramCache.insert({ aRenderable->type(), shaderKey }, shader);
			_impl->engine->compile(shader);
		);
	} else {
		shader = shaderIt.value();
	}

	switch (aRenderable->type()) {
	case ORenderable::Type::Mesh:
		_impl->loadMesh(reinterpret_cast<OMesh*>(aRenderable));
		break;

	case ORenderable::Type::Text2D:
		_impl->loadText2D(reinterpret_cast<OText2D*>(aRenderable));
		break;
	default:
		throw OException("Invalid renderable.");
	}
}

void ORenderingController::render(ORenderable* aRenderable)
{
	if (aRenderable->shaderProgram() == nullptr) throw OException("Unable to find shader program.");
	switch (aRenderable->type()) {
	case ORenderable::Type::Mesh:
		_impl->renderMesh(reinterpret_cast<OMesh*>(aRenderable));
		break;
	case ORenderable::Type::Text2D:
		_impl->renderText2D(reinterpret_cast<OText2D*>(aRenderable));
		break;
	default:
		throw OException("Invalid renderable.");
	}
}

void ORenderingController::flush()
{

}

// ----------------------------------------------------------------------------------------------
// ORenderingController::Impl
// ----------------------------------------------------------------------------------------------
uint32_t ORenderingController::Impl::shaderKey(ORenderable* aRenderable)
{
	uint32_t key = 0;
	switch (aRenderable->type()) {
	case ORenderable::Type::Mesh:
		if (aRenderable->texture() != nullptr) {
			key |= (1 << 1);
		}
		break;
	case ORenderable::Type::Text2D:
		// no special option
		break;
	default:
		throw OException("Invalid renderable.");
	}
	return key;
}

OShaderProgram * ORenderingController::Impl::createProgram(ORenderable* aRenderable, 
								const char * aVertexSrc, const char * aFragmentSrc)
{
	OShaderProgram*	program		= nullptr;
	OShaderObject*	vertex		= nullptr;
	OShaderObject*	fragment	= nullptr;

	auto error_handler = [&]() {
		if (program) {
			delete program;
			if (vertex) delete vertex;
			if (fragment) delete fragment;
		}
	};

	OExceptionForwardCb(error_handler, 
		program = new OShaderProgram();
		OExceptionPointerCheck(program);

		if (aVertexSrc != nullptr) {
			vertex = new OShaderObject(OShaderObject::Type::Vertex, aVertexSrc);
			OExceptionPointerCheck(vertex);
			program->addObject(vertex);
		}

		if (aFragmentSrc != nullptr) {
			fragment = new OShaderObject(OShaderObject::Type::Fragment, aFragmentSrc);
			OExceptionPointerCheck(fragment);
			program->addObject(fragment);
		}
		
		switch (aRenderable->type()) {
		case ORenderable::Type::Mesh:
			if (aRenderable->texture() != nullptr) {
				program->addPreprocessorMacro("USE_TEXTURE");
			}
			break;
		case ORenderable::Type::Text2D:
			break;
		}
	);


	return program;
}

void ORenderingController::Impl::loadMesh(OMesh* aMesh)
{
}

void ORenderingController::Impl::loadText2D(OText2D* aText)
{
}

void ORenderingController::Impl::renderMesh(OMesh* aMesh)
{
}

void ORenderingController::Impl::renderText2D(OText2D* aText)
{
}
