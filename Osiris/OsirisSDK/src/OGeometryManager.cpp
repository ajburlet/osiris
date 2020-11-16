#include <string>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/ORefCountObject.hpp"
#include "OsirisSDK/OMeshGeometry.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OWavefrontObjectFile.h"
#include "OsirisSDK/OGeometryManager.h"

struct OGeometryManager::Impl {
	using GeometryMap = OMap<std::string, OMeshGeometry>;
	using MeshFileMap = OMap<std::string, OMeshFile*>;

	enum class VertexDescrType {
		PositionsOnly = 0,
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
		descr.addAttribute(OShaderVertexArgument(OVarType::Float4));
		if ((type & static_cast<uint32_t>(Impl::VertexDescrType::Normals))) {
			descr.addAttribute(OShaderVertexArgument(OVarType::Float3));
		}
		if ((type & static_cast<uint32_t>(Impl::VertexDescrType::Texture))) {
			descr.addAttribute(OShaderVertexArgument(OVarType::Float3));
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

void OGeometryManager::registerFile(FileType aFileType, const char * aFilename, const char * aFileID)
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

void OGeometryManager::unRegisterFile(const char * aFileID)
{
	auto it = _impl->meshFileMap.find(aFileID);
	if (it == _impl->meshFileMap.end()) {
		throw OException("File ID not found.");
	}
	trashBin().trash(it.value());
	_impl->meshFileMap.remove(it);
}

OGeometryManager::ResourcePtr& OGeometryManager::loadFromFile(const char * aFileID, const char * aObjectName, const char * aKey)
{
	OMeshFile::RawData rawData;

	auto file_it = _impl->meshFileMap.find(aFileID);
	if (file_it != _impl->meshFileMap.end()) {
		throw OException("File ID not found.");
	}

	file_it.value()->loadMesh(aObjectName, rawData);

	uint32_t type = static_cast<uint32_t>(Impl::VertexDescrType::PositionsOnly);
	if (rawData.hasNormals()) type |= static_cast<uint32_t>(Impl::VertexDescrType::Normals);
	if (rawData.hasTexCoords()) type |= static_cast<uint32_t>(Impl::VertexDescrType::Texture);

	OVertexBuffer* vertexBuffer = nullptr;
	OIndexBuffer* indexBuffer = nullptr;
	try {
		vertexBuffer = new OVertexBuffer(_impl->vertexDescr[type], rawData.vertexArray().size());
		OExceptionPointerCheck(vertexBuffer);

		for (uint32_t i = 0; i < rawData.vertexArray().size(); i++) {
			auto& vertex = rawData.vertexArray()[i];
		}


	} catch (OException& e) {
		if (vertexBuffer) delete vertexBuffer;
		if (indexBuffer) delete indexBuffer;
	}
	
}

OGeometryManager::ResourcePtr& OGeometryManager::fetchResource(const char * aKey)
{
	OMeshGeometry* geometry = nullptr;
	auto it = _impl->geometryMap.find(aKey);
	if (it != _impl->geometryMap.end()) geometry = &it.value();
	return ResourcePtr(geometry);
}

void OGeometryManager::purge()
{
	while (_impl->geometryMap.size() > 0) {
		auto it = _impl->geometryMap.begin();
	}
}

void OGeometryManager::forEach(IterationCallbackFn aCallbackFn)
{
	for (auto it = _impl->geometryMap.begin(); it != _impl->geometryMap.end(); ++it) {
		aCallbackFn(it.key().c_str(), it.value());
	}
}

void OGeometryManager::forEach(IterationConstCallbackFn aCallbackFn) const
{
	for (auto it = _impl->geometryMap.cbegin(); it != _impl->geometryMap.cend(); ++it) {
		aCallbackFn(it.key().c_str(), it.value());
	}
}


