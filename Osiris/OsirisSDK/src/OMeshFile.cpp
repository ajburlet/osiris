
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OMeshFile.h"

OMeshFile::RawData::RawData()
{
	OExceptionPointerCheck(_vertexArray = new Array<Vertex>);
}

OMeshFile::RawData::~RawData()
{
	if (_texCoordArray != nullptr) delete _texCoordArray;
	if (_normalArray != nullptr) delete _normalArray;
	if (_vertexArray != nullptr) delete _vertexArray;
	if (_indexArray != nullptr) delete _indexArray;
}

OMeshFile::RawData::TexCoordArray& OMeshFile::RawData::texCoordArray()
{
	if (_texCoordArray == nullptr) {
		OExceptionPointerCheck(_texCoordArray = new TexCoordArray);
	}
	return *_texCoordArray;
}

OMeshFile::RawData::NormalArray& OMeshFile::RawData::normalArray()
{
	if (_normalArray == nullptr) {
		OExceptionPointerCheck(_normalArray = new NormalArray);
	}
	return *_normalArray;
}

OMeshFile::RawData::VertexArray & OMeshFile::RawData::vertexArray()
{
	return *_vertexArray;
}

OMeshFile::RawData::IndexArray & OMeshFile::RawData::indexArray()
{
	if (_indexArray == nullptr) {
		OExceptionPointerCheck(_indexArray = new IndexArray);
	}
	return *_indexArray;
}

bool OMeshFile::RawData::hasTexCoords() const
{
	return (_texCoordArray != nullptr && _texCoordArray->size() > 0);
}

bool OMeshFile::RawData::hasNormals() const
{
	return (_normalArray != nullptr && _normalArray->size() > 0);
}

bool OMeshFile::RawData::hasIndices() const
{
	return (_indexArray != nullptr && _indexArray->size() > 0);
}
