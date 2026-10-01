#include <inttypes.h>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OWavefrontMaterialFile.h"

#define THROW_PARSE_EXCEPTION(aErrStr) throw OEx(OString::Fmt(aErrStr " (%s:%" PRIu32 ").", filename().cString(), currLine()));
#define THROW_PARSE_EXCEPTION_FMT(aErrStr, ...) throw OEx(OString::Fmt(aErrStr " (%s:%" PRIu32 ").", __VA_ARGS__, filename().cString(), currLine()));

OWavefrontMaterialFile::List OWavefrontMaterialFile::loadMaterials()
{
	List list;
	while (readNextLine() == 0) {
		auto firstWord = readNextWord();
		if (!firstWord || !strcmp(firstWord, "#") || *firstWord == 0) {
			// ignore empty lines or comments
			continue;
		}
		else if (!strcmp(firstWord, "newmtl")) {
			auto materialName = readNextWord();
			list.append(OMaterial(std::move(materialName)));
		}
		else {
			if (list.size() == 0) {
				THROW_PARSE_EXCEPTION("No material instanced");
			}
			auto& current = list.tail();
			if (!strcmp(firstWord, "Ka")) {
				current.setAmbientColor(OVector3F(readNextFloat(), readNextFloat(), readNextFloat()));
			}
			else if (!strcmp(firstWord, "Kd")) {
				current.setDiffuseColor(OVector3F(readNextFloat(), readNextFloat(), readNextFloat()));
			}
			else if (!strcmp(firstWord, "Ks")) {
				current.setSpectralColor(OVector3F(readNextFloat(), readNextFloat(), readNextFloat()));
			}
			else if (!strcmp(firstWord, "Ns")) {
				current.setSpectralExponent(readNextFloat());
			}
			else if (!strcmp(firstWord, "d")) {
				current.setDissolve(readNextFloat());
			}
			else if (!strcmp(firstWord, "Tr")) {
				current.setDissolve(1 - readNextFloat());
			}
			else if (!strcmp(firstWord, "Ni")) {
				current.setOpticalDensity(readNextFloat());
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
				current.setIllumModel(model);
			}
			else if (!strcmp(firstWord, "Tf")) {
				continue; // ignored for now.
			}
		}
	}
	return list;
}
