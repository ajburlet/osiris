#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OWavefrontObjectFile.h"

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef OWAVEFRONTOBJECTFILE_LINEBUFFER
#define OWAVEFRONTOBJECTFILE_LINEBUFFER		128
#endif

#ifndef OWAVEFRONTOBJECTFILE_WORDBUFFER
#define OWAVEFRONTOBJECTFILE_WORDBUFFER		64
#endif

#ifndef OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH
#define OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH	128
#endif

using namespace std;

struct OWavefrontObjectFile::Impl : public OMemoryManagedObject<OMeshFile::Allocator> {
	Impl() = default;
	Impl(const Impl& aOther);
	~Impl();

	void copyFrom(const Impl& aOther);

	OMap<std::string, uint32_t>	objIndex;
	ObjectNameArray			objNames;

	uint32_t	currLine	= 0;
	char*		wordptr		= nullptr;
	bool		endOfLine	= false;
	char lineBuffer[OWAVEFRONTOBJECTFILE_LINEBUFFER];
	char wordBuffer[OWAVEFRONTOBJECTFILE_WORDBUFFER];
};

OWavefrontObjectFile::Impl::Impl(const Impl & aOther)
{
	copyFrom(aOther);
}

OWavefrontObjectFile::Impl::~Impl()
{
	for (auto& name : objNames) {
		if (name != nullptr) delete name;
	}
}

void OWavefrontObjectFile::Impl::copyFrom(const Impl & aOther)
{
	aOther.objIndex.cloneTo(objIndex);
	aOther.objNames.cloneTo(objNames);
	currLine = 0;
	wordptr = nullptr;
	endOfLine = false;
}

OWavefrontObjectFile::OWavefrontObjectFile(const char* aFilename) : OMeshFile(aFilename)
{
	OExceptionPointerCheck(_impl = new Impl);
	memset(_impl->lineBuffer, 0, OWAVEFRONTOBJECTFILE_LINEBUFFER);
	loadObjectList();
}

OWavefrontObjectFile::OWavefrontObjectFile(const OWavefrontObjectFile & aOther) :
	OMeshFile(aOther),
	_impl(aOther._impl)
{
}

OWavefrontObjectFile::~OWavefrontObjectFile()
{
	if (_impl != nullptr) delete _impl;
}

OWavefrontObjectFile & OWavefrontObjectFile::operator=(const OWavefrontObjectFile & aOther)
{
	OMeshFile::operator=(static_cast<const OMeshFile&>(aOther));
	_impl->copyFrom(*aOther._impl);
	return *this;
}

OWavefrontObjectFile & OWavefrontObjectFile::operator=(OWavefrontObjectFile && aOther)
{
	OMeshFile::operator=(static_cast<OMeshFile&&>(aOther));
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

void OWavefrontObjectFile::loadMesh(const char* aObjName, RawData& aRawData)
{
	char errString[OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH];

	// try and find the object 
	auto it = _impl->objIndex.find(aObjName);
	if (it == _impl->objIndex.end()) {
		snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, 
			 "Unable to locate object '%s' in file '%s'.", 
			 aObjName, filename());
		throw OException(errString);
	}

	seekTo(it.value());
	while (readNextLine() == 0) {
		auto firstWord = readNextWord();
		if (!firstWord || !strcmp(firstWord, "#")) {
			// ignore empty lines or comments
			continue;
		}
		else if (!strcmp(firstWord, "o")) {
			// reached next object, time to stop
			break;
		}
		else if (!strcmp(firstWord, "v")) {
			// reading vertices
			aRawData.vertexArray().append(RawData::Vertex());
			auto& newVertex = aRawData.vertexArray().tail();
			for (uint32_t i = 0; i < 4; i++) {
				auto value = readNextWord();
				if (value == nullptr) break;
				switch (i) {
				case 0: newVertex.x = atof(value);	break;
				case 1:	newVertex.y = atof(value);	break;
				case 2:	newVertex.z = atof(value);	break;
				case 3:	newVertex.w = atof(value);	break;
				}
				if (i + 1 > aRawData.vertexComponents()) {
					aRawData.setVertexComponents(i + 1);
				}
			}
		}
		else if (!strcmp(firstWord, "vn")) {
			// reading normals
			aRawData.normalArray().append(RawData::Normal());
			auto& newNormal = aRawData.normalArray().tail();
			for (uint32_t i = 0; i < 3; i++) {
				auto value = readNextWord();
				if (value == nullptr) {
					snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, 
						 "Normal vector must be three-dimensional, at %s:%d",
						 filename(), currLine());
					throw OException(errString);
				}
				switch (i) {
				case 0:	newNormal.x = atof(value);	break;
				case 1:	newNormal.y = atof(value);	break;
				case 2:	newNormal.z = atof(value);	break;
				}
			}
		}
		else if (!strcmp(firstWord, "vt")) {
			// reading textures
			aRawData.texCoordArray().append(RawData::TexCoord());
			auto& newTexCoord = aRawData.texCoordArray().tail();
			for (uint32_t i = 0; i < 3; i++) {
				auto value = readNextWord();
				if (value == nullptr) break;
				switch (i) {
				case 0:	newTexCoord.u = atof(value);	break;
				case 1:	newTexCoord.v = atof(value);	break;
				case 2:	newTexCoord.w = atof(value);	break;
				}
				if (i + 1 > aRawData.textureComponents()) {
					aRawData.setTextureComponents(i + 1);
				}
			}
		}
		else if (!strcmp(firstWord, "f")) {
			// reading indexes, finalizing the vertex array compositio, finalizing 
			// the vertex array compositionn
			uint32_t i = 0;
			ODynArray<uint32_t> faceIndexes;
			while (_impl->endOfLine == false) {
				auto value = readNextWord();
				if (value == nullptr) {
					snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, 
						 "Missing index values on %s:%d",
						 filename(), currLine());
					throw OException(errString);
				}

				char* pVertex = value;
				char* pTex = strchr(pVertex, '/');
				char* pNormal = nullptr;
				if (pTex != nullptr) {
					*(pTex++) = '\0';
					pNormal = strchr(pTex, '/');
					if (pNormal != nullptr) {
						*(pNormal++) = '\0';
					}
				}

				auto iVertex = static_cast<uint32_t>(atoi(pVertex))-1;
				
				if (aRawData.hasTexCoords()) {
					uint32_t iTex;
					if (pTex == nullptr || *pTex == '\0') {
						// if it doesnt have a specific index, follows the same as the
						// vertex index
						iTex = iVertex;
					} else {
						iTex = static_cast<uint32_t>(atoi(pTex))-1;
					}

					if (iTex >= aRawData.texCoordArray().size()) {
						snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, 
							 "Invalid texture index %lu (%s:%d)",
							 iTex, filename(), currLine());
						throw OException(errString);
					}
					if (aRawData.vertexArray()[iVertex].texCoord != nullptr) {
						aRawData.vertexArray().append(RawData::Vertex());
						iVertex = aRawData.vertexArray().size() - 1;
					}
					aRawData.vertexArray()[iVertex].texCoord = &aRawData.texCoordArray()[iTex];
				}

				if (aRawData.hasNormals()) {
					uint32_t iNormal;
					if (pNormal == nullptr || *pNormal == '\0') {
						// if it doesnt have a specific index, follows the same as the
						// vertex index
						iNormal = iVertex;
					} else {
						iNormal = static_cast<uint32_t>(atoi(pNormal))-1;
					}

					if (iNormal >= aRawData.normalArray().size()) {
						snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, 
							 "Invalid normal index %lu (%s:%d)",
							 iNormal, filename(), currLine());
						throw OException(errString);
					}

					if (aRawData.vertexArray()[iVertex].normal != nullptr) {
						aRawData.vertexArray().append(RawData::Vertex());
						iVertex = aRawData.vertexArray().size() - 1;
					}
					aRawData.vertexArray()[iVertex].normal = &aRawData.normalArray()[iNormal];
				}
				faceIndexes.append(iVertex);
			}
			if (faceIndexes.size() < 3) throw OException("Face must have at least 3 vertices.");
			// transforming into triangles
			for (uint32_t i = 1; i < faceIndexes.size() - 1; ++i) {
				RawData::Index idx;
				idx.i = faceIndexes[0];
				idx.j = faceIndexes[i];
				idx.k = faceIndexes[i + 1];
				aRawData.indexArray().append(idx);
			}
		}
	}
}

const OMeshFile::ObjectNameArray & OWavefrontObjectFile::objectArray() const
{
	return _impl->objNames;
}

void OWavefrontObjectFile::loadObjectList()
{
	const char *firstWord, *objectName;
	char errString[OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH];

	_impl->objIndex.clear();

	while (!eof() && readNextLine() == 0) {
		firstWord = readNextWord();
		if (strcmp(firstWord, "o") != 0) continue;

		if ((objectName = readNextWord()) == NULL) {
			snprintf(errString, OWAVEFRONTOBJECTFILE_ERRSTR_LENGTH, 
				"Parse error at line %u.", currLine());
			throw OException(errString);
		}

		_impl->objIndex[objectName] = currentPosition();
		_impl->objNames.append(strdup(objectName));
	}
}

int OWavefrontObjectFile::readNextLine()
{
	if (readLine(_impl->lineBuffer, OWAVEFRONTOBJECTFILE_LINEBUFFER) == 0) {
		_impl->wordptr = NULL;
		return 1;
	}
	_impl->wordptr = _impl->lineBuffer;
	_impl->endOfLine = false;
	_impl->currLine++;
	return 0;
}

char * OWavefrontObjectFile::readNextWord()
{
	char *p = _impl->wordptr;

	/* if end of line has been reached, point to the beginning of the line and return NULL. */
	if (_impl->endOfLine) {
		_impl->wordptr = _impl->lineBuffer;
		_impl->endOfLine = false;
		return NULL;
	}

	/* if no space is found or end of line chars, means end of line */
	if ((p = strchr(_impl->wordptr, ' ')) == NULL && ((p = strchr(_impl->wordptr, '\n')) != NULL || 
	    (p = strchr(_impl->wordptr, '\r')) != NULL || (p = strchr(_impl->wordptr, '\0')) != NULL) ) {
		_impl->endOfLine = true;
	}

	*_impl->wordBuffer = '\0';

#ifdef WIN32
	strncpy_s(_impl->wordBuffer, (rsize_t)OWAVEFRONTOBJECTFILE_WORDBUFFER, _impl->wordptr, (size_t)(p-_impl->wordptr));
#else
	strncpy(_impl->wordBuffer, _impl->wordptr, (size_t)(p - _impl->wordptr));
#endif
	if (!_impl->endOfLine) _impl->wordptr = p + 1;
	return _impl->wordBuffer;
}

unsigned int OWavefrontObjectFile::currLine()
{
	return _impl->currLine;
}

