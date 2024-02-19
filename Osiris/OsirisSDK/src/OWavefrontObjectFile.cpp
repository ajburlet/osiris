#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OWavefrontMaterialFile.h"
#include "OsirisSDK/OWavefrontObjectFile.h"

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#define THROW_PARSE_EXCEPTION(aErrStr) throw OEx(OString::Fmt(aErrStr " (%s:%" PRIu32 ").", filename(), currLine()));
#define THROW_PARSE_EXCEPTION_FMT(aErrStr, ...) throw OEx(OString::Fmt(aErrStr " (%s:%" PRIu32 ").", __VA_ARGS__, filename(), currLine()));

using namespace std;

struct OWavefrontObjectFile::Impl : public OMemoryManagedObject<OMeshFile::Allocator> {
	Impl() = default;
	Impl(const Impl& aOther);
	~Impl() = default;

	using MaterialMap = OWavefrontMaterialFile::MaterialMap;
	
	void copyFrom(const Impl& aOther);

	struct ObjectSection {
		uint32_t offset = 0;
		uint32_t triangleCount = 0;
		OMaterial* material = nullptr;
	};
	using ObjectSectionList = OList<ObjectSection, OMeshFile::Allocator>;

	struct ObjectData : public ONonCopiableT<ObjectData> {
		ObjectData (uint32_t aOffset=0) : offset(aOffset) {}
		ObjectData(ObjectData&& aOther) : 
			offset(aOther.offset), 
			vertexCount(aOther.vertexCount),
			sections(std::move(aOther.sections))
		{}
		~ObjectData () = default;

		ObjectData& operator= (ObjectData&& aOther) {
			offset = aOther.offset;
			vertexCount = aOther.vertexCount;
			sections = std::move(aOther.sections);
			return *this;
		}

		uint32_t			offset;
		uint32_t			vertexCount = 0;
		ObjectSectionList	sections;
	};
	using ObjectIndex = OMap<OString, ObjectData, OMeshFile::Allocator>;

	ObjectIndex	objIndex;
	MaterialMap	matMap;
};

OWavefrontObjectFile::Impl::Impl(const Impl & aOther)
{
	copyFrom(aOther);
}

void OWavefrontObjectFile::Impl::copyFrom(const Impl & aOther)
{
	aOther.objIndex.cloneTo(objIndex);
}

OWavefrontObjectFile::OWavefrontObjectFile(const OString& aFilename) : 
	OMeshFile(aFilename), 
	OWavefrontParser(this)
{
	OExPointerCheck(_impl = new Impl);
	loadObjectList();
}

OWavefrontObjectFile::OWavefrontObjectFile(const OWavefrontObjectFile & aOther) :
	OMeshFile(aOther),
	OWavefrontParser(this),
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

void OWavefrontObjectFile::loadMesh(const OString& aObjName, RawData& aRawData)
{
	// try and find the object 
	auto it = _impl->objIndex.find(aObjName);
	if (it == _impl->objIndex.end()) {
		THROW_PARSE_EXCEPTION_FMT("Unable to locate object '%s' in file '%s'", aObjName, filename());
	}

	seekTo(it.value().offset);
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
			RawData::Position newPos;
			for (uint32_t i = 0; i < 4; i++) {
				auto value = readNextWord();
				if (value == nullptr) break;
				switch (i) {
				case 0:	newPos.x = static_cast<float>(atof(value));	break;
				case 1:	newPos.y = static_cast<float>(atof(value));	break;
				case 2: newPos.z = static_cast<float>(atof(value));	break;
				}
				if (i + 1 > aRawData.positionComponents()) {
					aRawData.setPositionComponents(i + 1);
				}
			}
			aRawData.addPosition(newPos);
		}
		else if (!strcmp(firstWord, "vn")) {
			// reading normals
			RawData::Normal newNormal;
			for (uint32_t i = 0; i < 3; i++) {
				auto value = readNextWord();
				if (value == nullptr) {
					THROW_PARSE_EXCEPTION("Normal vector must be three-dimensional");
				}
				switch(i) {
				case 0:	newNormal.x = static_cast<float>(atof(value));	break;
				case 1:	newNormal.y = static_cast<float>(atof(value));	break;
				case 2:	newNormal.z = static_cast<float>(atof(value));	break;
				}
			}
			aRawData.addNormal(newNormal);
		}
		else if (!strcmp(firstWord, "vt")) {
			// reading textures
			RawData::TexCoord newTexCoord;
			for (uint32_t i = 0; i < 3; i++) {
				auto value = readNextWord();
				if (value == nullptr) break;
				switch(i) {
				case 0:	newTexCoord.u = static_cast<float>(atof(value));	break;
				case 1:	newTexCoord.v = static_cast<float>(atof(value));	break;
				case 2:	newTexCoord.w = static_cast<float>(atof(value));	break;
				}
				if (i + 1 > aRawData.textureComponents()) {
					aRawData.setTextureComponents(i + 1);
				}
			}
			aRawData.addTexCoordinate(newTexCoord);
		}
		else if (!strcmp(firstWord, "f")) {
			// reading indexes, finalizing the vertex array compositio, finalizing 
			// the vertex array compositionn
			uint32_t i = 0;
			ODynArray<RawData::Index> faceIndexes;
			while (endOfLine() == false) {
				auto value = readNextWord();
				if (value == nullptr) THROW_PARSE_EXCEPTION("Missing index values");

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

				uint32_t iVertex = static_cast<uint32_t>(atoi(pVertex))-1;
				uint32_t iTex = 0;
				uint32_t iNormal = 0;
				if (pTex != nullptr && *pTex != '\0') {
					iTex = static_cast<uint32_t>(atoi(pTex))-1;
				}
				if (pNormal != nullptr && *pNormal != '\0') {
					iNormal = static_cast<uint32_t>(atoi(pNormal))-1;
				}
				
				faceIndexes.append({iVertex, iNormal, iTex});
			}
			
			if (faceIndexes.size() < 3) THROW_PARSE_EXCEPTION("Face must have at least 3 vertices.");
			// transforming into triangles
			for (uint32_t i = 1; i < faceIndexes.size() - 1; ++i) {
				aRawData.addFace({faceIndexes[0], faceIndexes[i], faceIndexes[i+1]});
			}
		}
	}
}

void OWavefrontObjectFile::loadObjectList()
{
	_impl->objIndex.clear();

	auto remove_line_end = [](char* aStr) -> void {
		char* p;
		while ((p = strchr(aStr, '\r')) != nullptr || (p = strchr(aStr, '\n')) != nullptr) {
			*p = '\0';
		}
	};

	Impl::ObjectData* curr_object = nullptr;
	Impl::ObjectSection* curr_section = nullptr;
	while (!eof() && readNextLine() == 0) {
		auto firstWord = readNextWord();
		if (!strcmp(firstWord, "o") != 0) {
			auto objectName = readNextWord();
			if (objectName == NULL) THROW_PARSE_EXCEPTION("Expected object name");

			remove_line_end(objectName);

			char* p;
			while ((p = strchr(objectName, '\r')) != nullptr || (p = strchr(objectName, '\n')) != nullptr) {
				*p = '\0';
			}

			Impl::ObjectIndex::Iterator itPos;
			_impl->objIndex.insert(objectName, Impl::ObjectData(currentPosition()), &itPos);
			curr_object = &itPos.value();
			curr_section = nullptr;
		}
		else if (!strcmp(firstWord, "mtllib")) {
			auto matfile_name = readNextWord();
			if (matfile_name == nullptr) THROW_PARSE_EXCEPTION("Expected material filename");
			
			remove_line_end(matfile_name);

			auto matfile_path = OString(filename());
			auto it = matfile_path.findLastOf("\\/");
			if (it == matfile_path.end()) {
				it = matfile_path.begin();
			} else {
				it++;
			}
			matfile_path.erase(it);
			matfile_path += matfile_name;

			OWavefrontMaterialFile matfile(matfile_path.cString());
			matfile.loadMaterials(_impl->matMap);
		}
		else if (!strcmp(firstWord, "v")) {
			if (curr_object == nullptr) THROW_PARSE_EXCEPTION("Vertex defined with no active object");
			
			curr_object->vertexCount++;
		}
		else if (!strcmp(firstWord, "f")) {
			if (curr_object == nullptr) THROW_PARSE_EXCEPTION("Face defined with no active object");
			
			if (curr_section == nullptr) {
				curr_object->sections.pushBack(Impl::ObjectSection());
				curr_section = &curr_object->sections.tail();
			}

			uint32_t faceSegments = 0;
			while (readNextWord() != nullptr) ++faceSegments;
			if (faceSegments < 3) THROW_PARSE_EXCEPTION("Face defined with less than three segments");
			curr_section->triangleCount += faceSegments - 2;
		}
		else if (!strcmp(firstWord, "usemtl")) {
			if (curr_object == nullptr) THROW_PARSE_EXCEPTION("Material assignment with no active object");

			auto material_name = readNextWord();
			if (material_name == nullptr) THROW_PARSE_EXCEPTION("Expected material name");
			
			auto it = curr_object->sections.find([material_name] (const Impl::ObjectSection& a_sec) {
				return strcmp(a_sec.material->name(), material_name) == 0;
			});

			if (it != curr_object->sections.end()) {
				curr_section = &(*it);				
			} else {
				auto matIt = _impl->matMap.find(material_name);
				if (matIt == _impl->matMap.end()) THROW_PARSE_EXCEPTION_FMT("Material '%s' not found", material_name);

				curr_object->sections.pushBack(Impl::ObjectSection());
				curr_section = &curr_object->sections.tail();
				curr_section->material = &matIt.value();
			}
		}
	}
}

void OWavefrontObjectFile::loadMaterial(const OString& aFilename)
{
	OWavefrontMaterialFile matFile(aFilename);
	matFile.loadMaterials(_impl->matMap);
}

