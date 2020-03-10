
#include "shaders.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMesh.h"
#include "OsirisSDK/OText2D.h"
#include "OsirisSDK/OGraphicsRenderCommandEncoder.h"
#include "OsirisSDK/OGraphicsResourceCommandEncoder.h"
#include "OsirisSDK/OGraphicsCommandBuffer.h"
#include "OsirisSDK/OGraphicsCommandQueue.h"
#include "OsirisSDK/OGraphicsEngine.h"

// ----------------------------------------------------------------------------------------------
// OGraphicsEngine implementation structure
// ----------------------------------------------------------------------------------------------
struct OGraphicsEngine::Implementation {
	OShaderProgram* createProgram(const char* aVertexSrc, const char* aFragmentSrc);
	OShaderProgram* createOrFetchShader(OMesh* aMesh);
	OShaderProgram* createOrFetchShader(OText2D* aText);
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

	Type			type;
	OShaderSourceTable	shaderSourceTable;
	ShaderProgramCache	shaderProgramCache;
};

// ----------------------------------------------------------------------------------------------
// OGraphicsEngine
// ----------------------------------------------------------------------------------------------
OGraphicsEngine::OGraphicsEngine(Type aType)
{
	_impl = new Implementation;
	if (_impl == nullptr) throw OException("Failed to create graphics engine implementation.");
	_impl->type = aType;
	createShaderTable(_impl->shaderSourceTable);
}

OGraphicsEngine::~OGraphicsEngine()
{
	if (_impl) delete _impl;
}

void OGraphicsEngine::load(ORenderable* aRenderable)
{
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

void OGraphicsEngine::render(ORenderable* aRenderable)
{
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

// ----------------------------------------------------------------------------------------------
// OGraphicsEngine::Implementation
// ----------------------------------------------------------------------------------------------
OShaderProgram * OGraphicsEngine::Implementation::createProgram(const char * aVertexSrc, const char * aFragmentSrc)
{
	OShaderProgram*	program = nullptr;
	OShaderObject*	obj	= nullptr;

	// TODO -- complete this thing

	return nullptr;
}

OShaderProgram * OGraphicsEngine::Implementation::createOrFetchShader(OMesh* aMesh)
{
	uint32_t key = 0;
	if (aMesh->texture() != nullptr) {
		key |= (1 << 1);
	}
	auto shaderIt = shaderProgramCache.find({ aMesh->type(), key });
	OShaderProgram* shader = nullptr;
	if (shaderIt == shaderProgramCache.end()) {
		auto vertSrcIt = shaderSourceTable.find({ type, OShaderObject::Type::Vertex, aMesh->type() });
		auto fragSrcIt = shaderSourceTable.find({ type, OShaderObject::Type::Fragment, aMesh->type() });
		shader = createProgram(vertSrcIt.value().c_str(), fragSrcIt.value().c_str());
		shaderProgramCache.insert({ aMesh->type(), key }, shader);
	} else {
		shader = shaderIt.value();
	}
	return shader;
}

OShaderProgram * OGraphicsEngine::Implementation::createOrFetchShader(OText2D* aText)
{
	auto shaderIt = shaderProgramCache.find({ aText->type(), 0 });
	OShaderProgram* shader = nullptr;
	if (shaderIt == shaderProgramCache.end()) {
		auto vertSrcIt = shaderSourceTable.find({ type, OShaderObject::Type::Vertex, aText->type() });
		auto fragSrcIt = shaderSourceTable.find({ type, OShaderObject::Type::Fragment, aText->type() });
		shader = createProgram(vertSrcIt.value().c_str(), fragSrcIt.value().c_str());
		shaderProgramCache.insert({ aText->type(), 0 }, shader);
	} else {
		shader = shaderIt.value();
	}
	return shader;
}

void OGraphicsEngine::Implementation::loadMesh(OMesh* aMesh)
{
	auto shader = createOrFetchShader(aMesh);
	if (shader == nullptr) throw OException("Unable to find shader program.");

}

void OGraphicsEngine::Implementation::loadText2D(OText2D* aText)
{
	auto shader = createOrFetchShader(aText);
	if (shader == nullptr) throw OException("Unable to find shader program.");
}

void OGraphicsEngine::Implementation::renderMesh(OMesh* aMesh)
{
}

void OGraphicsEngine::Implementation::renderText2D(OText2D* aText)
{
}
