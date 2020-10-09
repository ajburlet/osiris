#include <cassert>

#include "shaders.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OTrashBin.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMatrix.hpp"
#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OTexture.h"
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
	Impl(OGraphicsAPI* aGraphicsAPI);
	~Impl();

	// shaders
	uint32_t shaderKey(ORenderable* aRenderable);
	OShaderProgram* createProgram(ORenderable* aRenderable, const char* aVertexSrc, const char* aFragmentSrc);

	// encoders
	void endEncoder();
	OGraphicsRenderCommandEncoder* getRenderEncoder();
	OGraphicsResourceCommandEncoder* getResourceEncoder();
	void flush();

	// load
	void load(ORenderable* aRenderable);
	void addUniformToRenderable(ORenderable* aRenderable, OVarType aType, uint8_t aArrayLength, 
				    const char* aName, OShaderArgumentInstance::UpdateCallbackFn aCallbackFn);
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
	using CommandBufferList = OList<OGraphicsCommandBuffer*>;

	// class members
	OGraphicsAPI*			_api			= nullptr;
	OShaderSourceTable		_shaderSourceTable;
	ShaderProgramCache		_shaderProgramCache;
	OGraphicsCommandBuffer*		_currentBuffer		= nullptr;
	OGraphicsCommandQueue*		_currentQueue		= nullptr;
	OGraphicsCommandEncoder*	_currentEncoder		= nullptr;
	OMatrixStack*			_matrixStack		= nullptr;
	CommandBufferList		_pendingBuffers;
	OTrashBin			_trashBin;
};

// ----------------------------------------------------------------------------------------------
// ORenderingController
// ----------------------------------------------------------------------------------------------
ORenderingEngine::ORenderingEngine(OGraphicsAPI* aGraphicsAPI)
{
	OExceptionPointerCheck(_impl = new Impl(aGraphicsAPI));
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
	_impl->flush();
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

void ORenderingEngine::waitUntilCompleted()
{
	while (_impl->_pendingBuffers.empty() == false) {
		auto buffer = _impl->_pendingBuffers.front();
		buffer->waitUntilCompleted();
		trashBin().trash(buffer);
		_impl->_pendingBuffers.popFront();
	}
}

// ----------------------------------------------------------------------------------------------
// ORenderingController::Impl
// ----------------------------------------------------------------------------------------------
ORenderingEngine::Impl::Impl(OGraphicsAPI* aGraphicsAPI) :
	_api(aGraphicsAPI)
{
	_currentQueue = aGraphicsAPI->createCommandQueue();
	createShaderTable(_shaderSourceTable);
}

ORenderingEngine::Impl::~Impl()
{
	if (_currentQueue) delete _currentQueue;
}

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

void ORenderingEngine::Impl::endEncoder()
{
	if (_currentEncoder) {
		_currentEncoder->end();
		_trashBin.trash(_currentEncoder);
		_currentEncoder = nullptr;
	}
}

OGraphicsRenderCommandEncoder * ORenderingEngine::Impl::getRenderEncoder()
{
	if (!_currentBuffer) {
		OExceptionPointerCheck(_currentBuffer = _currentQueue->createCommandBuffer());
	}

	if (!_currentEncoder || _currentEncoder->type() != OGraphicsCommandEncoder::Type::Render) {
		endEncoder();
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
		endEncoder();
		OExceptionPointerCheck(_currentEncoder = _currentBuffer->createResourceCommandEncoder());
	}

	return reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);
}

void ORenderingEngine::Impl::flush()
{
	endEncoder();
	_currentBuffer->commit();
	_pendingBuffers.pushBack(_currentBuffer);
	_currentBuffer = nullptr;
}

void ORenderingEngine::Impl::load(ORenderable * aRenderable)
{
	// shader loading/referencing
	OShaderProgram* shader = nullptr;
	auto shader_key = shaderKey(aRenderable);
	
	auto shaderIt = _shaderProgramCache.find({ aRenderable->type(), shader_key });
	if (shaderIt == _shaderProgramCache.end()) {
		auto vertSrcIt = _shaderSourceTable.find({ _api->type(), 
								 OShaderObject::Type::Vertex, 
								 aRenderable->type() });
		auto fragSrcIt = _shaderSourceTable.find({ _api->type(), 
								 OShaderObject::Type::Fragment, 
								 aRenderable->type() });
		shader = createProgram(aRenderable, vertSrcIt.value().c_str(), fragSrcIt.value().c_str());
		OExceptionForwardCb([&]() { delete shader; }, 
			_shaderProgramCache.insert({ aRenderable->type(), shader_key }, shader);
			_api->compile(shader);
		);
	} else {
		shader = shaderIt.value();
	}
	aRenderable->renderComponents()->setShaderProgram(shader);

	// make sure there is a buffer and a resource encoder.
	auto resourceEncoder = getResourceEncoder();

	// uniforms
	if (aRenderable->renderComponents()->uniformArgumentList()->needsLoading()) {
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
		aRenderable->renderComponents()->uniformArgumentList()->setNeedsLoading(false);
	}

	// resources (vertices and texture)
	auto vertexBuffer = aRenderable->renderComponents()->vertexBuffer();
	auto indexBuffer = aRenderable->renderComponents()->indexBuffer();
	auto texture = aRenderable->renderComponents()->texture();
	if (vertexBuffer->needsLoading()) {
		resourceEncoder->load(vertexBuffer);
		vertexBuffer->setNeedsLoading(false);
	}
	if (texture != nullptr && texture->needsLoading()) {
		resourceEncoder->load(texture);
		texture->setNeedsLoading(false);
	}
	if (indexBuffer != nullptr && indexBuffer->needsLoading()) {
		resourceEncoder->load(indexBuffer);
		indexBuffer->setNeedsLoading(false);
	}
	if (aRenderable->renderComponents()->needsLoading()) {
		resourceEncoder->load(aRenderable->renderComponents());
		aRenderable->renderComponents()->setNeedsLoading(false);
	}
}

void ORenderingEngine::Impl::addUniformToRenderable(ORenderable * aRenderable, OVarType aType, 
						    uint8_t aArrayLength, const char * aName,
						    OShaderArgumentInstance::UpdateCallbackFn aCallbackFn)
{
	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);
	
	auto uniform = new OShaderArgumentInstance(aType, aArrayLength);
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
	addUniformToRenderable(aMesh, OVarType::Float4x4, 1, cMeshUniformMVPTransform,
		[](OShaderArgumentInstance& aArgumentInstance, const ORenderable* aMesh) {
			aArgumentInstance.copyFrom(reinterpret_cast<const OMesh*>(aMesh)->matrixStack()->top().glArea());
	});
}

void ORenderingEngine::Impl::loadGlyphUniforms(OGlyph* aGlyph)
{
	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);

	// uniforms
	addUniformToRenderable(aGlyph, OVarType::Float2, 1, cGlyphUniformPosOffset,
		[](OShaderArgumentInstance& aArgumentInstance, const ORenderable* aGlyph) {
			aArgumentInstance.copyFrom(static_cast<const OGlyph*>(aGlyph)->positionOffset().glArea());
		});
	addUniformToRenderable(aGlyph, OVarType::Float2, 1, cGlyphUniformScale, 
		[](OShaderArgumentInstance& aArgumentInstance, const ORenderable* aGlyph) {
			aArgumentInstance.copyFrom(static_cast<const OGlyph*>(aGlyph)->scale().glArea());
		});
	addUniformToRenderable(aGlyph, OVarType::Float4, 1, cGlyphUniformColor,
		[](OShaderArgumentInstance& aArgumentInstance, const ORenderable* aGlyph) {
			aArgumentInstance.copyFrom(static_cast<const OGlyph*>(aGlyph)->color().glArea());
		});
}

void ORenderingEngine::Impl::unload(ORenderComponents* aRenderComponents)
{
	auto resourceEncoder = getResourceEncoder();
	if (aRenderComponents->texture() != nullptr) {
		resourceEncoder->unload(aRenderComponents->texture());
		aRenderComponents->texture()->setNeedsLoading(true);
	}
	if (aRenderComponents->vertexBuffer() != nullptr) {
		resourceEncoder->unload(aRenderComponents->vertexBuffer());
		aRenderComponents->vertexBuffer()->setNeedsLoading(true);
	}
	if (aRenderComponents->indexBuffer() != nullptr) {
		resourceEncoder->unload(aRenderComponents->indexBuffer());
		aRenderComponents->indexBuffer()->setNeedsLoading(true);
	}
	for (auto arg : *aRenderComponents->uniformArgumentList()) resourceEncoder->unload(arg);
	aRenderComponents->uniformArgumentList()->setNeedsLoading(true);
}

void ORenderingEngine::Impl::render(ORenderable * aRenderable)
{
	auto renderEncoder = getRenderEncoder();

	if (aRenderable->renderComponents()->shaderProgram() == nullptr) 
		throw OException("Unable to find shader program.");

	// shader & arguments
	renderEncoder->setShaderProgram(aRenderable->renderComponents()->shaderProgram());
	aRenderable->renderComponents()->uniformArgumentList()->update(aRenderable);
	renderEncoder->setUniformArgumentList(aRenderable->renderComponents()->uniformArgumentList());

	// vertex, index & texture
	renderEncoder->setVertexBuffer(aRenderable->renderComponents()->vertexBuffer());
	if (aRenderable->renderComponents()->indexBuffer()) 
		renderEncoder->setIndexBuffer(aRenderable->renderComponents()->indexBuffer());
	if (aRenderable->renderComponents()->texture()) 
		renderEncoder->setTexture(aRenderable->renderComponents()->texture());
	renderEncoder->setRenderComponents(aRenderable->renderComponents());

	renderEncoder->draw(aRenderable->renderComponents()->renderMode());
}

