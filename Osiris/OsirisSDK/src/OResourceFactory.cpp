#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/ORefCountObject.hpp"
#include "OsirisSDK/OMeshBuilder.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OObjMeshFile.h"
#include "OsirisSDK/OMesh.h"
#include "OsirisSDK/ORenderComponents.h"
#include "OsirisSDK/OResourceFactory.h"

using Allocator = OGraphicsAllocators::Default;

struct OResourceFactory::Impl {
	using MeshFileMap = OMap<OString, OMeshFile*, Allocator>;

	enum class VertexDescrType {
		PositionsOnly	= 0,
		Normals		= 1 << 0,
		Texture		= 1 << 1,
		Material		= 1 << 2,
		All		= Normals | Texture | Material,
		Count
	};
	static constexpr uint32_t VertexDescrTypeCount = static_cast<uint32_t>(VertexDescrType::Count);

	GeometryManager geometryManager;
	TextureManager textureManager;
	MaterialManager materialManager;
	MaterialSetManager materialSetManager;

	MeshFileMap		meshFileMap;
	OVertexBufferDescriptor	vertexDescr[VertexDescrTypeCount];
};

OResourceFactory::OResourceFactory()
	: _impl(std::make_unique<OResourceFactory::Impl>())
{
	for (uint32_t type = 0; type < Impl::VertexDescrTypeCount; type++) {
		auto& descr = _impl->vertexDescr[type];
		descr.addAttribute(OShaderVertexArgument(OVarType::Float3, 0));
		if ((type & static_cast<uint32_t>(Impl::VertexDescrType::Normals))) {
			descr.addAttribute(OShaderVertexArgument(OVarType::Float3, 1));
		}
		if ((type & static_cast<uint32_t>(Impl::VertexDescrType::Texture))) {
			descr.addAttribute(OShaderVertexArgument(OVarType::Float2, 2));
		}
		descr.addAttribute(OShaderVertexArgument(OVarType::UnsignedInt, 3));
	}
}

OResourceFactory::OResourceFactory(OResourceFactory&& aOther) 
	: _impl(std::move(aOther)._impl)
{
}

OResourceFactory::~OResourceFactory()
{
	if (_impl != nullptr) {
		for (auto it = _impl->meshFileMap.begin(); it != _impl->meshFileMap.end(); it++) {
			if (it.value() != nullptr) {
				delete it.value();
			}
		}
	}
}

OResourceFactory & OResourceFactory::operator=(OResourceFactory && aOther)
{
	aOther._impl = std::move(aOther)._impl;
	return *this;
}

OResourceFactory::GeometryManager& OResourceFactory::geometryManager()
{
	return _impl->geometryManager;
}

OResourceFactory::TextureManager& OResourceFactory::textureManager()
{
	return _impl->textureManager;
}

OResourceFactory::MaterialManager& OResourceFactory::materialManager()
{
	return _impl->materialManager;
}

OResourceFactory::MaterialSetManager& OResourceFactory::materialSetManager()
{
	return _impl->materialSetManager;
}

void OResourceFactory::registerFile(FileType aFileType, const OString& aFilename, const OString& aFileID)
{
	if (_impl->meshFileMap.find(aFileID) != _impl->meshFileMap.end()) {
		throw OEx("File ID already exists");
	}

	OMeshFile* file = nullptr;
	switch (aFileType) {
	case FileType::WavefrontObjectFile:
		file = new OObjMeshFile(aFilename);
		break;
	}
	OExPointerCheck(file);

	try {
		_impl->meshFileMap.insert(aFileID, file);
	} catch (OException& e) {
		delete file;
		throw e;
	}
}

void OResourceFactory::unRegisterFile(const OString& aFileID)
{
	auto it = _impl->meshFileMap.find(aFileID);
	if (it == _impl->meshFileMap.end()) {
		throw OEx("File ID not found.");
	}
	delete it.value();
	_impl->meshFileMap.remove(it);
}

OResourceFactory::MeshResources OResourceFactory::loadMeshResources(const OString& aFileID, const OString& aObjectName, const OString& aKey)
{
	auto file_it = _impl->meshFileMap.find(aFileID);
	if (file_it == _impl->meshFileMap.end()) {
		throw OEx("File ID not found.");
	}

	OMeshFile::RawData rawData;
	file_it.value()->loadMesh(aObjectName, rawData);
	if (rawData.positionComponents() != 3) {
		throw OEx("Only three position components are currently supported by the geometry manager.");
	}

	uint32_t type = static_cast<uint32_t>(Impl::VertexDescrType::PositionsOnly);
	if (rawData.hasNormals()) {
		type |= static_cast<uint32_t>(Impl::VertexDescrType::Normals);
	}
	if (rawData.hasTexCoords()) {
		if (rawData.textureComponents() != 2) {
			throw OEx("Only 2D textures are currently supported by the geometry manager");
		}
		type |= static_cast<uint32_t>(Impl::VertexDescrType::Texture);
	}
	type |= static_cast<uint32_t>(Impl::VertexDescrType::Material);

	OMeshBuilder builder(_impl->geometryManager, _impl->materialManager, _impl->materialSetManager, _impl->vertexDescr[type]);
	auto built = builder.build(aKey, aObjectName, rawData);
	return { std::move(built.geometry), std::move(built.materials) };
}

void OResourceFactory::loadFromFile(const OString& aFileID, const OString& aObjectName,
	const OString& aKey, ORenderComponents& aRenderComponents)
{
	auto resources = loadMeshResources(aFileID, aObjectName, aKey);
	aRenderComponents.setGeometry(*resources.geometry);
	if (resources.materials != nullptr) {
		aRenderComponents.setMaterialSet(*resources.materials);
	}
}

void OResourceFactory::loadFromFile(const OString& aFileID, const OString& aObjectName,
	const OString& aKey, OMesh& aMesh)
{
	loadFromFile(aFileID, aObjectName, aKey, aMesh.renderComponents());
}


