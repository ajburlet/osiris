#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/ORefCountObject.hpp"
#include "OsirisSDK/OMeshGeometry.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OWavefrontObjectFile.h"
#include "OsirisSDK/OGeometryManager.h"

using Allocator = OGraphicsAllocators::Default;

struct OGeometryManager::Impl {
	using GeometryMap = OMap<OString, OMeshGeometry, Allocator>;
	using MeshFileMap = OMap<OString, OMeshFile*, Allocator>;

	enum class VertexDescrType {
		PositionsOnly	= 0,
		Normals		= 1 << 0,
		Texture		= 1 << 1,
		All		= Normals | Texture,
		Count
	};
	static constexpr uint32_t VertexDescrTypeCount = static_cast<uint32_t>(VertexDescrType::Count);

	GeometryMap		geometryMap;
	MeshFileMap		meshFileMap;
	OVertexBufferDescriptor	vertexDescr[VertexDescrTypeCount];
};

OGeometryManager::OGeometryManager()
{
	OExceptionPointerCheck(_impl = new Impl);
	for (uint32_t type = 0; type < Impl::VertexDescrTypeCount; type++) {
		auto& descr = _impl->vertexDescr[type];
		descr.addAttribute(OShaderVertexArgument(OVarType::Float3, 0));
		if ((type & static_cast<uint32_t>(Impl::VertexDescrType::Normals))) {
			descr.addAttribute(OShaderVertexArgument(OVarType::Float3, 1));
		}
		if ((type & static_cast<uint32_t>(Impl::VertexDescrType::Texture))) {
			descr.addAttribute(OShaderVertexArgument(OVarType::Float2, 2));
		}
	}
}

OGeometryManager::~OGeometryManager()
{
	if (_impl != nullptr) {
		for (auto it = _impl->meshFileMap.begin(); it != _impl->meshFileMap.end(); it++) {
			if (it.value() != nullptr) {
				delete it.value();
			}
		}
		delete _impl;
	}
}

OGeometryManager & OGeometryManager::operator=(OGeometryManager && aOther)
{
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

void OGeometryManager::registerFile(FileType aFileType, const OString& aFilename, const OString& aFileID)
{
	if (_impl->meshFileMap.find(aFileID) != _impl->meshFileMap.end()) {
		throw OException("File ID already exists");
	}

	OMeshFile* file = nullptr;
	switch (aFileType) {
	case FileType::WavefrontObjectFile:
		file = new OWavefrontObjectFile(aFilename);
		break;
	}
	OExceptionPointerCheck(file);

	try {
		_impl->meshFileMap.insert(aFileID, file);
	} catch (OException& e) {
		delete file;
		throw e;
	}
}

void OGeometryManager::unRegisterFile(const OString& aFileID)
{
	auto it = _impl->meshFileMap.find(aFileID);
	if (it == _impl->meshFileMap.end()) {
		throw OException("File ID not found.");
	}
	trashBin().trash(it.value());
	_impl->meshFileMap.remove(it);
}

OGeometryManager::ResourcePtr OGeometryManager::loadFromFile(const OString& aFileID, const OString& aObjectName, const OString& aKey)
{
	OMeshFile::RawData rawData;

	auto file_it = _impl->meshFileMap.find(aFileID);
	if (file_it == _impl->meshFileMap.end()) {
		throw OException("File ID not found.");
	}

	file_it.value()->loadMesh(aObjectName, rawData);

	if (rawData.positionComponents() != 3) {
		throw OException("Only three position components are currently supported by the geometry manager.");
	}

	uint32_t type = static_cast<uint32_t>(Impl::VertexDescrType::PositionsOnly);
	if (rawData.hasNormals()) {
		type |= static_cast<uint32_t>(Impl::VertexDescrType::Normals);
	}
	if (rawData.hasTexCoords()) {
		if (rawData.textureComponents() != 2) {
			throw OException("Only 2D textures are currently supported by the geometry manager");
		}
		type |= static_cast<uint32_t>(Impl::VertexDescrType::Texture);
	}

	OVertexBuffer vertexBuffer(_impl->vertexDescr[type], rawData.vertexCount());
	for (uint32_t i = 0; i < rawData.vertexCount(); i++) {
		auto& vertexData = rawData.vertexData(i);

		uint8_t attrIndex = 0;
		vertexBuffer.setAttributeValue(attrIndex++, i, vertexData.pos);
		if (rawData.hasNormals()) {
			vertexBuffer.setAttributeValue(attrIndex++, i, vertexData.normal);
		}
		if (rawData.hasTexCoords()) {
			vertexBuffer.setAttributeValue(attrIndex++, i, vertexData.texCoord);
		}
	}
	
	Impl::GeometryMap::Iterator it;
	OMeshGeometry geometry((rawData.hasIndices()) ? ORenderMode::IndexedTriangle : ORenderMode::Triangle,
						   std::move(vertexBuffer), std::move(rawData.indexedDrawInfoArray()));
	_impl->geometryMap.insert(aKey, std::move(geometry), &it);

	return ResourcePtr(&it.value());
}

OGeometryManager::ResourcePtr& OGeometryManager::fetchResource(const OString& aKey)
{
	OMeshGeometry* geometry = nullptr;
	auto it = _impl->geometryMap.find(aKey);
	if (it != _impl->geometryMap.end()) geometry = &it.value();
	return ResourcePtr(geometry);
}

void OGeometryManager::purge()
{
	OList<OString,Allocator> to_remove; 
	for (auto it=_impl->geometryMap.begin(); it != _impl->geometryMap.end(); ++it) {
		if (it.value().referenceCount() == 1) {
			to_remove.pushBack(it.key());
		}
	}

	for (auto& key : to_remove) {
		_impl->geometryMap.remove(key);
	}
}

void OGeometryManager::forEach(IterationCallbackFn aCallbackFn)
{
	for (auto it = _impl->geometryMap.begin(); it != _impl->geometryMap.end(); ++it) {
		aCallbackFn(it.key().cString(), it.value());
	}
}

void OGeometryManager::forEach(IterationConstCallbackFn aCallbackFn) const
{
	for (auto it = _impl->geometryMap.begin(); it != _impl->geometryMap.end(); ++it) {
		aCallbackFn(it.key().cString(), it.value());
	}
}


