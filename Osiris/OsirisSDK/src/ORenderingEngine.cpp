#include <cassert>

#include "shaders.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OTrashBin.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMatrix.hpp"
#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/OMesh.h"
#include "OsirisSDK/OGlyph.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OShaderObject.h"
#include "OsirisSDK/OShaderProgram.h"
#include "OsirisSDK/OShaderArgumentInstanceList.h"
#include "OsirisSDK/OGraphicsRenderCommandEncoder.h"
#include "OsirisSDK/OGraphicsResourceCommandEncoder.h"
#include "OsirisSDK/OGraphicsCommandBuffer.h"
#include "OsirisSDK/OGraphicsCommandQueue.h"
#include "OsirisSDK/ORenderable.h"
#include "OsirisSDK/ORenderComponents.h"
#include "OsirisSDK/ORenderingEngine.h"

// ----------------------------------------------------------------------------------------------
// Constants 
// ----------------------------------------------------------------------------------------------
constexpr char	cMeshUniformMVPTransform[]	= "uMvpTransform";
constexpr char	cMeshUniformTexture[]		= "uTexture";
constexpr char	cGlyphUniformPosOffset[]	= "uPosOffset";
constexpr char	cGlyphUniformScale[]		= "uScale";
constexpr char	cGlyphUniformTexture[]		= "uTexture";
constexpr char	cGlyphUniformColor[]		= "uColor";


// ----------------------------------------------------------------------------------------------
// ORenderingController implementation structure
// ----------------------------------------------------------------------------------------------
struct ORenderingEngine::Impl {
	// shaders
	uint32_t shaderKey(ORenderable* aRenderable);
	OShaderProgram* createProgram(ORenderable* aRenderable, const char* aVertexSrc, const char* aFragmentSrc);

	// encoders
	OGraphicsRenderCommandEncoder* getRenderEncoder();
	OGraphicsResourceCommandEncoder* getResourceEncoder();

	// load
	void load(ORenderable* aRenderable);
	void addUniformToRenderable(ORenderable* aRenderable, OVarType aType, OVarPrecision aPrecision, 
				    uint8_t aArrayLength, const char* aName, 
				    OShaderArgumentInstance::UpdateCallbackFn aCallbackFn);
	void loadMeshUniforms(OMesh* aMesh);
	void loadGlyphUniforms(OGlyph* aText);
	void unload(ORenderComponents* aRenderComponents);

	// render
	void render(ORenderable* aRenderable);

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

	// class members
	OGraphicsAPI*			_engine;
	OShaderSourceTable		_shaderSourceTable;
	ShaderProgramCache		_shaderProgramCache;
	OGraphicsCommandBuffer*		_currentBuffer		= nullptr;
	OGraphicsCommandQueue*		_currentQueue		= nullptr;
	OGraphicsCommandEncoder*	_currentEncoder		= nullptr;
	OMatrixStack*			_matrixStack		= nullptr;
	OTrashBin			_trashBin;
};

// ----------------------------------------------------------------------------------------------
// ORenderingController
// ----------------------------------------------------------------------------------------------
ORenderingEngine::ORenderingEngine(OGraphicsAPI* aEngine)
{
	OExceptionPointerCheck(_impl = new Impl);
	_impl->_engine = aEngine;
	createShaderTable(_impl->_shaderSourceTable);
}

ORenderingEngine::~ORenderingEngine()
{
	if (_impl) delete _impl;
}

void ORenderingEngine::load(ORenderable* aRenderable)
{
	_impl->load(aRenderable);
}

void ORenderingEngine::unload(ORenderable * aRenderable)
{
	_impl->unload(aRenderable->renderComponents());
}

void ORenderingEngine::unload(ORenderComponents * aRenderComponents)
{
	_impl->unload(aRenderComponents);
}

void ORenderingEngine::render(ORenderable* aRenderable)
{
	_impl->render(aRenderable);
}

void ORenderingEngine::flush()
{

}

void ORenderingEngine::setMatrixStack(OMatrixStack * aMatrixStack)
{
	_impl->_matrixStack = aMatrixStack;
}

OTrashBin & ORenderingEngine::trashBin()
{
	return _impl->_trashBin;
}

void ORenderingEngine::clearRenderTarget(const OVector4F & aColor)
{
	auto renderingEncoder = _impl->getRenderEncoder();
	renderingEncoder->clearRenderTarget(aColor);
}

void ORenderingEngine::clearDepthBuffer(float aValue)
{
	auto renderingEncoder = _impl->getRenderEncoder();
	renderingEncoder->clearDepthBuffer(aValue);
}

// ----------------------------------------------------------------------------------------------
// ORenderingController::Impl
// ----------------------------------------------------------------------------------------------
uint32_t ORenderingEngine::Impl::shaderKey(ORenderable* aRenderable)
{
	uint32_t key = 0;
	switch (aRenderable->type()) {
	case ORenderable::Type::Mesh:
		if (aRenderable->renderComponents()->texture() != nullptr) {
			key |= (1 << 1);
		}
		break;
	case ORenderable::Type::Glyph:
		// no special option
		break;
	default:
		throw OException("Invalid renderable.");
	}
	return key;
}

OShaderProgram * ORenderingEngine::Impl::createProgram(ORenderable* aRenderable, const char * aVertexSrc, 
						       const char * aFragmentSrc)
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
			if (aRenderable->renderComponents()->texture() != nullptr) {
				program->addPreprocessorMacro("USE_TEXTURE");
			}
			break;
		case ORenderable::Type::Glyph:
			break;
		}
	);


	return program;
}

OGraphicsRenderCommandEncoder * ORenderingEngine::Impl::getRenderEncoder()
{
	if (!_currentBuffer) {
		OExceptionPointerCheck(_currentBuffer = _currentQueue->createCommandBuffer());
	}

	if (!_currentEncoder || _currentEncoder->type() != OGraphicsCommandEncoder::Type::Render) {
		if (_currentEncoder) {
			_currentEncoder->end();
			_currentEncoder = nullptr;
		}
		OExceptionPointerCheck(_currentEncoder = _currentBuffer->createRenderCommandEncoder());
	}

	return reinterpret_cast<OGraphicsRenderCommandEncoder*>(_currentEncoder);
}

OGraphicsResourceCommandEncoder * ORenderingEngine::Impl::getResourceEncoder()
{
	if (!_currentBuffer) {
		OExceptionPointerCheck(_currentBuffer = _currentQueue->createCommandBuffer());
	}

	if (!_currentEncoder || _currentEncoder->type() != OGraphicsCommandEncoder::Type::Resource) {
		if (_currentEncoder != nullptr) {
			_currentEncoder->end();
			_currentEncoder = nullptr;
		}
		OExceptionPointerCheck(_currentEncoder = _currentBuffer->createResourceCommandEncoder());
	}

	return reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);
}

void ORenderingEngine::Impl::load(ORenderable * aRenderable)
{
	// shader loading/referencing
	OShaderProgram* shader = nullptr;
	auto shader_key = shaderKey(aRenderable);
	
	auto shaderIt = _shaderProgramCache.find({ aRenderable->type(), shader_key });
	if (shaderIt == _shaderProgramCache.end()) {
		auto vertSrcIt = _shaderSourceTable.find({ _engine->type(), 
								 OShaderObject::Type::Vertex, 
								 aRenderable->type() });
		auto fragSrcIt = _shaderSourceTable.find({ _engine->type(), 
								 OShaderObject::Type::Fragment, 
								 aRenderable->type() });
		shader = createProgram(aRenderable, vertSrcIt.value().c_str(), fragSrcIt.value().c_str());
		OExceptionForwardCb([&]() { delete shader; }, 
			_shaderProgramCache.insert({ aRenderable->type(), shader_key }, shader);
			_engine->compile(shader);
		);
	} else {
		shader = shaderIt.value();
	}
	aRenderable->renderComponents()->setShaderProgram(shader);

	// make sure there is a buffer and a resource encoder.
	auto resourceEncoder = getResourceEncoder();

	// uniforms
	switch (aRenderable->type()) {
	case ORenderable::Type::Mesh:
		loadMeshUniforms(reinterpret_cast<OMesh*>(aRenderable));
		break;

	case ORenderable::Type::Glyph:
		loadGlyphUniforms(reinterpret_cast<OGlyph*>(aRenderable));
		break;
	default:
		throw OException("Invalid renderable.");
	}

	// resources (vertices and texture)
	resourceEncoder->load(aRenderable->renderComponents()->vertexBuffer());
	if (aRenderable->renderComponents()->texture()) 
		resourceEncoder->load(aRenderable->renderComponents()->texture());
	if (aRenderable->renderComponents()->indexBuffer()) 
		resourceEncoder->load(aRenderable->renderComponents()->indexBuffer());
}

void ORenderingEngine::Impl::addUniformToRenderable(ORenderable * aRenderable, 
						    OVarType aType, OVarPrecision aPrecision, 
						    uint8_t aArrayLength, const char * aName,
						    OShaderArgumentInstance::UpdateCallbackFn aCallbackFn)
{
	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);
	
	auto uniform = new OShaderArgumentInstance(aType, aPrecision, aArrayLength);
	OExceptionPointerCheck(uniform);
	OExceptionForwardCb([&]() { delete uniform; }, {
		uniform->setUpdateCallbackFunction(aCallbackFn);
		resourceEncoder->load(uniform, aRenderable->renderComponents()->shaderProgram(), aName);
		aRenderable->renderComponents()->uniformArgumentList()->pushBack(uniform);
	});
}

void ORenderingEngine::Impl::loadMeshUniforms(OMesh* aMesh)
{
	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);

	// uniforms
	addUniformToRenderable(aMesh, OVarType::Float4x4, OVarPrecision::High, 1, cMeshUniformMVPTransform,
		[aMesh](OShaderArgumentInstance& aArgumentInstance) {
			aArgumentInstance.copyFrom(aMesh->matrixStack()->top().glArea());
	});
}

void ORenderingEngine::Impl::loadGlyphUniforms(OGlyph* aText)
{
	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);

	// uniforms
	addUniformToRenderable(aText, OVarType::Float2, OVarPrecision::High, 1, cGlyphUniformPosOffset,
		[aText](OShaderArgumentInstance& aArgumentInstance) {
			aArgumentInstance.copyFrom(aText->positionOffset().glArea());
		});
	addUniformToRenderable(aText, OVarType::Float2, OVarPrecision::High, 1, cGlyphUniformScale, 
		[aText](OShaderArgumentInstance& aArgumentInstance) {
			aArgumentInstance.copyFrom(aText->scale().glArea());
		});
	addUniformToRenderable(aText, OVarType::Float4, OVarPrecision::Low, 1, cGlyphUniformColor,
		[aText](OShaderArgumentInstance& aArgumentInstance) {
			aArgumentInstance.copyFrom(aText->color().glArea());
		});
}

void ORenderingEngine::Impl::unload(ORenderComponents* aRenderComponents)
{
	auto resourceEncoder = getResourceEncoder();
	if (aRenderComponents->texture() != nullptr)  resourceEncoder->unload(aRenderComponents->texture());
	if (aRenderComponents->vertexBuffer() != nullptr) resourceEncoder->unload(aRenderComponents->vertexBuffer());
	if (aRenderComponents->indexBuffer() != nullptr) resourceEncoder->unload(aRenderComponents->indexBuffer());
	for (auto arg : *aRenderComponents->uniformArgumentList()) resourceEncoder->unload(arg);
}

void ORenderingEngine::Impl::render(ORenderable * aRenderable)
{
	auto renderEncoder = getRenderEncoder();

	if (aRenderable->renderComponents()->shaderProgram() == nullptr) 
		throw OException("Unable to find shader program.");

	renderEncoder->setShaderProgram(aRenderable->renderComponents()->shaderProgram());
	renderEncoder->setUniformArgumentList(aRenderable->renderComponents()->uniformArgumentList());
	renderEncoder->setVertexBuffer(aRenderable->renderComponents()->vertexBuffer());
	if (aRenderable->renderComponents()->indexBuffer()) 
		renderEncoder->setIndexBuffer(aRenderable->renderComponents()->indexBuffer());
	if (aRenderable->renderComponents()->texture()) 
		renderEncoder->setTexture(aRenderable->renderComponents()->texture());

	aRenderable->renderComponents()->uniformArgumentList()->update();

	renderEncoder->draw(aRenderable->renderComponents()->renderMode());
}

