#include <cassert>

#include "shaders.h"
#include "OsirisSDK/OException.h"
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
#include "OsirisSDK/ORenderingController.h"

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
struct ORenderingController::Impl {
	// shaders
	uint32_t shaderKey(ORenderable* aRenderable);
	OShaderProgram* createProgram(ORenderable* aRenderable, const char* aVertexSrc, const char* aFragmentSrc);

	// load
	void load(ORenderable* aRenderable);
	void addUniformToRenderable(ORenderable* aRenderable, OVarType aType, OVarPrecision aPrecision, 
				    uint8_t aDim, const char* aName);
	void loadMeshUniforms(OMesh* aMesh);
	void loadGlyphUniforms(OGlyph* aText);

	// render
	void render(ORenderable* aRenderable);
	void setupMeshUniforms(OMesh* aMesh);
	void setupGlyphUniforms(OGlyph* aText);

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
	OGraphicsEngine*		_engine;
	OShaderSourceTable		_shaderSourceTable;
	ShaderProgramCache		_shaderProgramCache;
	OGraphicsCommandBuffer*		_currentBuffer		= nullptr;
	OGraphicsCommandQueue*		_currentQueue		= nullptr;
	OGraphicsCommandEncoder*	_currentEncoder		= nullptr;
	OMatrixStack*			_matrixStack		= nullptr;
};

// ----------------------------------------------------------------------------------------------
// ORenderingController
// ----------------------------------------------------------------------------------------------
ORenderingController::ORenderingController(OGraphicsEngine* aEngine)
{
	OExceptionPointerCheck(_impl = new Impl);
	_impl->_engine = aEngine;
	createShaderTable(_impl->_shaderSourceTable);
}

ORenderingController::~ORenderingController()
{
	if (_impl) delete _impl;
}

void ORenderingController::load(ORenderable* aRenderable)
{
	_impl->load(aRenderable);
}

void ORenderingController::render(ORenderable* aRenderable)
{
	_impl->render(aRenderable);
}

void ORenderingController::flush()
{

}

void ORenderingController::setMatrixStack(OMatrixStack * aMatrixStack)
{
	_impl->_matrixStack = aMatrixStack;
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
	case ORenderable::Type::Glyph:
		// no special option
		break;
	default:
		throw OException("Invalid renderable.");
	}
	return key;
}

OShaderProgram * ORenderingController::Impl::createProgram(ORenderable* aRenderable, const char * aVertexSrc, 
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
			if (aRenderable->texture() != nullptr) {
				program->addPreprocessorMacro("USE_TEXTURE");
			}
			break;
		case ORenderable::Type::Glyph:
			break;
		}
	);


	return program;
}

void ORenderingController::Impl::load(ORenderable * aRenderable)
{
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

	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);
	
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
	
	resourceEncoder->load(aRenderable->vertexBuffer());
	if (aRenderable->texture()) resourceEncoder->load(aRenderable->texture());
	if (aRenderable->indexBuffer()) resourceEncoder->load(aRenderable->indexBuffer());
}

void ORenderingController::Impl::addUniformToRenderable(ORenderable * aRenderable, 
							OVarType aType, OVarPrecision aPrecision, 
							uint8_t aDim, const char * aName)
{
	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);
	
	auto uniform = new OShaderArgumentInstance(aType, aPrecision, aDim);
	OExceptionPointerCheck(uniform);
	OExceptionForwardCb([&]() { delete uniform; }, {
		resourceEncoder->load(uniform, aRenderable->shaderProgram(), aName);
		aRenderable->uniformArgumentList()->pushBack(uniform);
	});
}

void ORenderingController::Impl::loadMeshUniforms(OMesh* aMesh)
{
	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);

	// uniforms
	addUniformToRenderable(aMesh, OVarType::Float4x4, OVarPrecision::High, 1, cMeshUniformMVPTransform);
}

void ORenderingController::Impl::loadGlyphUniforms(OGlyph* aText)
{
	auto resourceEncoder = reinterpret_cast<OGraphicsResourceCommandEncoder*>(_currentEncoder);

	// uniforms
	addUniformToRenderable(aText, OVarType::Float, OVarPrecision::High, 2, cGlyphUniformPosOffset);
	addUniformToRenderable(aText, OVarType::Float, OVarPrecision::High, 2, cGlyphUniformScale);
	addUniformToRenderable(aText, OVarType::Float, OVarPrecision::Low, 4, cGlyphUniformColor);
}

void ORenderingController::Impl::render(ORenderable * aRenderable)
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

	auto renderEncoder = reinterpret_cast<OGraphicsRenderCommandEncoder*>(_currentEncoder);

	if (aRenderable->shaderProgram() == nullptr) throw OException("Unable to find shader program.");
	switch (aRenderable->type()) {
	case ORenderable::Type::Mesh:
		setupMeshUniforms(reinterpret_cast<OMesh*>(aRenderable));
		break;
	case ORenderable::Type::Glyph:
		setupGlyphUniforms(reinterpret_cast<OGlyph*>(aRenderable));
		break;
	default:
		throw OException("Invalid renderable.");
	}

	renderEncoder->setShaderProgram(aRenderable->shaderProgram());
	renderEncoder->setUniformArgumentList(aRenderable->uniformArgumentList);
	renderEncoder->setVertexBuffer(aRenderable->vertexBuffer());
	if (aRenderable->indexBuffer()) renderEncoder->setIndexBuffer(aRenderable->indexBuffer());
	if (aRenderable->texture()) renderEncoder->setTexture(aRenderable->texture());
}

void ORenderingController::Impl::setupMeshUniforms(OMesh* aMesh)
{
}

void ORenderingController::Impl::setupGlyphUniforms(OGlyph* aText)
{
}
