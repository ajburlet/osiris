#include <inttypes.h>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OWavefrontMaterialFile.h"

#define THROW_PARSE_EXCEPTION(aErrStr) throw OException(OString::Fmt(aErrStr " (%s:%" PRIu32 ").", filename(), currLine()).cString());
#define THROW_PARSE_EXCEPTION_FMT(aErrStr, ...) throw OException(OString::Fmt(aErrStr " (%s:%" PRIu32 ").", __VA_ARGS__, filename(), currLine()).cString());

void OWavefrontMaterialFile::loadMaterials(MaterialMap & aMaterialList)
{
	OMaterial* current_mat = nullptr;
	while (readNextLine() == 0) {
		auto firstWord = readNextWord();
		if (!firstWord || !strcmp(firstWord, "#")) {
			// ignore empty lines or comments
			continue;
		}
		else if (!strcmp(firstWord, "newmtl")) {
			MaterialMap::Iterator it;
			auto materialName = readNextWord();
			aMaterialList.insert(materialName, OMaterial(materialName), &it);
			current_mat = &(it.value());
		}
		else {
			if (current_mat == nullptr) THROW_PARSE_EXCEPTION("No material instanced");
			if (!strcmp(firstWord, "Ka")) {
				current_mat->setAmbientColor(OVector3F(readNextFloat(), readNextFloat(), readNextFloat()));
			}
			else if (!strcmp(firstWord, "Kd")) {
				current_mat->setDiffuseColor(OVector3F(readNextFloat(), readNextFloat(), readNextFloat()));
			}
			else if (!strcmp(firstWord, "Ks")) {
				current_mat->setSpectralColor(OVector3F(readNextFloat(), readNextFloat(), readNextFloat()));
			}
			else if (!strcmp(firstWord, "Ns")) {
				current_mat->setSpectralExponent(readNextFloat());
			}
			else if (!strcmp(firstWord, "d")) {
				current_mat->setDissolve(readNextFloat());
			}
			else if (!strcmp(firstWord, "Tr")) {
				current_mat->setDissolve(1 - readNextFloat());
			}
			else if (!strcmp(firstWord, "Ni")) {
				current_mat->setOpticalDensity(readNextFloat());
			}
			else if (!strcmp(firstWord, "illum")) {
				OMaterial::IllumModel model = OMaterial::IllumModel::NotSet;
				switch (readNextUint()) {
				case 0:
					model = OMaterial::IllumModel::ColorOnAmbientOff;
					break;
				case 1:
					model = OMaterial::IllumModel::ColorOnAmbientOn;
					break;
				case 2:
					model = OMaterial::IllumModel::Highlight;
					break;
				case 3:
					model = OMaterial::IllumModel::ReflectionOnRayTraceOn;
					break;
				case 4:
					model = OMaterial::IllumModel::TranspRefractionReflectFresnelRayTrace;
					break;
				case 5:
					model = OMaterial::IllumModel::ReflectFresnelRayTrace;
					break;
				case 6:
					model = OMaterial::IllumModel::TranspRefractionReflectRayTrace;
					break;
				case 7:
					model = OMaterial::IllumModel::TranspRefractionReflectFresnelRayTrace;
					break;
				case 8:
					model = OMaterial::IllumModel::ReflectionOnRayTraceOff;
					break;
				case 9:
					model = OMaterial::IllumModel::TranspGlassReflectionRayTraceOff;
					break;
				}
				current_mat->setIllumModel(model);
			}
			else if (!strcmp(firstWord, "Tf")) {
				continue; // ignored for now.
			}
		}
	}
}
