
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OIndexedDrawInfo.h"
#include "OsirisSDK/OMeshFile.h"

#ifndef OMESHFILE_BLOCKSIZE
#define OMESHFILE_BLOCKSIZE	128
#endif

// ****************************************************************************
// Internal implementation
// ****************************************************************************
struct OMeshFile::RawData::Impl {
	template <typename T>
	using Array = ODynArray<T, Allocator, OMESHFILE_BLOCKSIZE>;

	using TexCoordArray = Array<TexCoord>;
	using NormalArray = Array<Normal>;
	using PositionArray = Array<Position>;
	using IndexMap = OMap<Index, uint32_t, Allocator>;
	using VertexDataArray = Array<VertexData>;

	TexCoordArray		texCoordArray;
	NormalArray		normalArray;
	PositionArray		positionArray;
	IndexMap		indexMap;
	VertexDataArray		vertexDataArray;
	OIndexedDrawInfo::Array	indexDrawInfoArray;

	uint32_t getIndex(const Index& aIndex);
};

uint32_t OMeshFile::RawData::Impl::getIndex(const Index& aIndex)
{
	auto it = indexMap.find(aIndex);
	if (it == indexMap.end()) {
		auto index = vertexDataArray.size();
		if (aIndex.vert >= positionArray.size()) {
			throw OEx("Invalid position index.");
		}

		Position* position = &(positionArray[aIndex.vert]);

		Normal* normal = nullptr;
		if (normalArray.size() > 0) {
			if (aIndex.norm >= normalArray.size()) {
				throw OEx("Invalid normal index.");
			}
			normal = &(normalArray[aIndex.norm]);
		}

		TexCoord* texCoord = nullptr;
		if (texCoordArray.size() > 0) {
			if (aIndex.tex >= texCoordArray.size()) {
				throw OEx("Invalid tex coordinate.");
			}
			texCoord = &(texCoordArray[aIndex.tex]);
		}

		vertexDataArray.append({position, normal, texCoord});
		indexMap.insert(aIndex, index, &it);
	}
	return it.value();
}


// ****************************************************************************
// Rawdata Implementation
// ****************************************************************************
OMeshFile::RawData::RawData()
{
	OExPointerCheck(_impl = new Impl);
}

OMeshFile::RawData::~RawData()
{
	if (_impl != nullptr) delete _impl;
}

void OMeshFile::RawData::addTexCoordinate(const TexCoord& aTexCoord)
{
	_impl->texCoordArray.append(aTexCoord);
}

void OMeshFile::RawData::addNormal(const Normal & aNormal)
{
	_impl->normalArray.append(aNormal);
}

void OMeshFile::RawData::addPosition(const Position & aPosition)
{
	_impl->positionArray.append(aPosition);
}

void OMeshFile::RawData::addFace(const Face& aFace)
{
	auto v1 = _impl->getIndex(aFace.v1);
	auto v2 = _impl->getIndex(aFace.v2);
	auto v3 = _impl->getIndex(aFace.v3);

	if (_impl->indexDrawInfoArray.size() == 0) {
		_impl->indexDrawInfoArray.append(OIndexedDrawInfo());
	}

	_impl->indexDrawInfoArray.tail().indexBuffer()->addFace(v1, v2, v3);
}

void OMeshFile::RawData::useMaterial(OMaterial* aMaterial)
{
	_impl->indexDrawInfoArray.append(OIndexedDrawInfo(aMaterial));
}

const OString& OMeshFile::RawData::currentMaterial() const
{
	if (_impl->indexDrawInfoArray.size() == 0 || _impl->indexDrawInfoArray.tail().material() == nullptr) {
		return nullptr;
	}
	return _impl->indexDrawInfoArray.tail().material()->name();
}

OMeshFile::RawData::TexCoord& OMeshFile::RawData::texCoord(uint32_t aIndex)
{
	return _impl->texCoordArray[aIndex];
}

OMeshFile::RawData::Normal& OMeshFile::RawData::normal(uint32_t aIndex)
{
	return _impl->normalArray[aIndex];
}

OMeshFile::RawData::Position& OMeshFile::RawData::position(uint32_t aIndex)
{
	return _impl->positionArray[aIndex];
}

OMeshFile::RawData::VertexData OMeshFile::RawData::vertexData(uint32_t aIndex) const
{
	if (hasIndices()) {
		if (aIndex >= _impl->vertexDataArray.size()) throw OEx("Indexed array member out of range.");
		return _impl->vertexDataArray[aIndex];
	}
	if (aIndex >= _impl->positionArray.size()) throw OEx("Array member of out range.");
	return { &_impl->positionArray[aIndex],
		 (hasNormals()) ? &_impl->normalArray[aIndex] : nullptr,
		 (hasTexCoords()) ? &_impl->texCoordArray[aIndex] : nullptr};
}

uint32_t OMeshFile::RawData::vertexCount () const
{
	return _impl->vertexDataArray.size();
}

OIndexedDrawInfo::Array&& OMeshFile::RawData::indexedDrawInfoArray()
{
	return std::move(_impl->indexDrawInfoArray);
}

bool OMeshFile::RawData::hasTexCoords() const
{
	return (_impl->texCoordArray.size() > 0);
}

bool OMeshFile::RawData::hasNormals() const
{
	return (_impl->normalArray.size() > 0);
}

bool OMeshFile::RawData::hasIndices() const
{
	return (_impl->indexDrawInfoArray.size() > 0);
}

