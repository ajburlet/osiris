#include <cassert>

#include "shaders.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
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
	void load(ORenderComponents* aRenderComponents);
	OShaderUniformArgument& addUniformToRenderable(ORenderable* aRenderable, OVarType aType,
							uint8_t aArrayLength, const char* aName, 
							OShaderUniformArgument::UpdateCallbackFn aCallbackFn);
	void loadMeshUniforms(OMesh* aMesh);
	void loadGlyphUniforms(OGlyph* aText);
	void load(OVertexBuffer* aVertexBuffer);
	void load(OIndexBuffer* aIndexBuffer);
	void load(OTexture* aTexture);
	void unload(ORenderable* aRenderable);
	void unload(ORenderComponents* aRenderComponents);
	void unload(OVertexBuffer* aVertexBuffer);
	void unload(OIndexBuffer* aIndexBuffer);
	void unload(OTexture* aTexture);

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

void ORenderingEngine::load(ORenderComponents * aRenderComponents)
{
	_impl->load(aRenderComponents);
}

void ORenderingEngine::load(OVertexBuffer * aVertexBuffer)
{
	_impl->load(aVertexBuffer);
}

void ORenderingEngine::load(OIndexBuffer * aIndexBuffer)
{
	_impl->load(aIndexBuffer);
}

void ORenderingEngine::load(OTexture * aTexture)
{
	load(aTexture);
}

void ORenderingEngine::unload(ORenderable * aRenderable, bool aUnloadAll)
{
	if (aUnloadAll) {
		unload(aRenderable->renderComponents(), true);
	}
	_impl->unload(aRenderable);
}

void ORenderingEngine::unload(ORenderComponents * aRenderComponents, bool aUnloadAll)
{
	if (aUnloadAll) {
		_impl->unload(aRenderComponents->vertexBuffer());
		_impl->unload(aRenderComponents->indexBuffer());
		_impl->unload(aRenderComponents->texture());
	}
	_impl->unload(aRenderComponents);
}

void ORenderingEngine::unload(OVertexBuffer * aVertexBuffer)
{
	_impl->unload(aVertexBuffer);
}

void ORenderingEngine::unload(OIndexBuffer * aIndexBuffer)
{
	_impl->unload(aIndexBuffer);
}

void ORenderingEngine::unload(OTexture * aTexture)
{
	_impl->unload(aTexture);
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


	// uniforms
	if (aRenderable->uniformsLoaded() == false) {
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
		aRenderable->setUniformsLoaded(true);
	}

	load(aRenderable->renderComponents());
}

void ORenderingEngine::Impl::load(ORenderComponents * aRenderComponents)
{
	load(aRenderComponents->vertexBuffer());
	load(aRenderComponents->indexBuffer());
	load(aRenderComponents->texture());
	if (aRenderComponents->needsLoading()) {
		getResourceEncoder()->load(aRenderComponents);
		aRenderComponents->setNeedsLoading(false);
	}
}

OShaderUniformArgument& ORenderingEngine::Impl::addUniformToRenderable(ORenderable * aRenderable, 
									OVarType aType, 
									uint8_t aArrayLength, 
									const char * aName,
						    OShaderUniformArgument::UpdateCallbackFn aCallbackFn)
{
	aRenderable->uniforms().pushBack(OShaderUniformArgument(aType, aArrayLength));
	auto& uniform = aRenderable->uniforms().tail();
	uniform.setUpdateCallbackFunction(aCallbackFn);
	getResourceEncoder()->load(&uniform, aRenderable->renderComponents()->shaderProgram(), aName);
	return uniform;
}

void ORenderingEngine::Impl::loadMeshUniforms(OMesh* aMesh)
{
	// uniforms
	addUniformToRenderable(aMesh, OVarType::Float4x4, 1, cMeshUniformMVPTransform,
		[](OShaderUniformArgument& aArgumentInstance, const ORenderable* aMesh) {
			aArgumentInstance.copyFrom(reinterpret_cast<const OMesh*>(aMesh)->matrixStack()->top().glArea());
	});
}

void ORenderingEngine::Impl::loadGlyphUniforms(OGlyph* aGlyph)
{
	// uniforms
	addUniformToRenderable(aGlyph, OVarType::Float2, 1, cGlyphUniformPosOffset,
		[](OShaderUniformArgument& aArgumentInstance, const ORenderable* aGlyph) {
			aArgumentInstance.copyFrom(static_cast<const OGlyph*>(aGlyph)->positionOffset().glArea());
		});
	addUniformToRenderable(aGlyph, OVarType::Float2, 1, cGlyphUniformScale, 
		[](OShaderUniformArgument& aArgumentInstance, const ORenderable* aGlyph) {
			aArgumentInstance.copyFrom(static_cast<const OGlyph*>(aGlyph)->scale().glArea());
		});
	addUniformToRenderable(aGlyph, OVarType::Float4, 1, cGlyphUniformColor,
		[](OShaderUniformArgument& aArgumentInstance, const ORenderable* aGlyph) {
			aArgumentInstance.copyFrom(static_cast<const OGlyph*>(aGlyph)->color().glArea());
		});
	
	auto& texUniform = addUniformToRenderable(aGlyph, OVarType::Int, 1, cGlyphUniformTexture, nullptr);
	texUniform.castTo<int32_t>() = 0; // texture index always set to zero
}

void ORenderingEngine::Impl::load(OVertexBuffer * aVertexBuffer)
{
	if (aVertexBuffer->needsLoading()) {
		getResourceEncoder()->load(aVertexBuffer);
		aVertexBuffer->setNeedsLoading(false);
	}
}

void ORenderingEngine::Impl::load(OIndexBuffer * aIndexBuffer)
{
	if (aIndexBuffer != nullptr && aIndexBuffer->needsLoading()) {
		getResourceEncoder()->load(aIndexBuffer);
		aIndexBuffer->setNeedsLoading(false);
	}
}

void ORenderingEngine::Impl::load(OTexture * aTexture)
{
	if (aTexture != nullptr && aTexture->needsLoading()) {
		getResourceEncoder()->load(aTexture);
		aTexture->setNeedsLoading(false);
	}
}

void ORenderingEngine::Impl::unload(ORenderable * aRenderable)
{
	auto encoder = getResourceEncoder();
	for (auto& uniform : aRenderable->uniforms()) {
		encoder->unload(&uniform);
	}
	aRenderable->setUniformsLoaded(false);
}

void ORenderingEngine::Impl::unload(ORenderComponents* aRenderComponents)
{
	getResourceEncoder()->unload(aRenderComponents);
	aRenderComponents->setNeedsLoading(true);
}

void ORenderingEngine::Impl::unload(OVertexBuffer * aVertexBuffer)
{
	if (aVertexBuffer != nullptr) {
		getResourceEncoder()->unload(aVertexBuffer);
		aVertexBuffer->setNeedsLoading(true);
	}
}

void ORenderingEngine::Impl::unload(OIndexBuffer * aIndexBuffer)
{
	if (aIndexBuffer != nullptr) {
		getResourceEncoder()->unload(aIndexBuffer);
		aIndexBuffer->setNeedsLoading(true);
	}
}

void ORenderingEngine::Impl::unload(OTexture * aTexture)
{
	if (aTexture != nullptr) {
		getResourceEncoder()->unload(aTexture);
		aTexture->setNeedsLoading(true);
	}
}

void ORenderingEngine::Impl::render(ORenderable * aRenderable)
{
	auto renderEncoder = getRenderEncoder();

	if (aRenderable->renderComponents()->shaderProgram() == nullptr) {
		throw OException("Unable to find shader program.");
	}

	// shader & arguments
	aRenderable->updateUniforms();
	renderEncoder->setShaderProgram(aRenderable->renderComponents()->shaderProgram());
	for (auto& arg : aRenderable->uniforms()) {
		renderEncoder->setUniformArgument(&arg);
	}

	// vertex, index & texture
	renderEncoder->setVertexBuffer(aRenderable->renderComponents()->vertexBuffer());
	if (aRenderable->renderComponents()->indexBuffer()) 
		renderEncoder->setIndexBuffer(aRenderable->renderComponents()->indexBuffer());
	if (aRenderable->renderComponents()->texture()) 
		renderEncoder->setTexture(aRenderable->renderComponents()->texture());
	renderEncoder->setRenderComponents(aRenderable->renderComponents());

	renderEncoder->draw(aRenderable->renderComponents()->renderMode());
}

