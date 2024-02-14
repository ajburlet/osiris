#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/Omap.hpp"
#include "OsirisSDK/OStringDefs.h"
#include "OsirisSDK/OFile.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OWavefrontParser.h"

class OMaterial;

/**
 @brief Material file handler for Wavefront material files.
 */
class OAPI OWavefrontMaterialFile : public OFile, protected OWavefrontParser
{
public:
	using Allocator = OGraphicsAllocators::Default;

	/**
	 @brief Class constructor.
	 */
	OWavefrontMaterialFile(const OString& aFilename) : 
		OFile(aFilename, OFile::Mode::Read),
		OWavefrontParser(this)
	{}

	/**
	 @brief Class destructor.
	 */
	virtual ~OWavefrontMaterialFile() = default;
	
	/**
	 @brief Material dictionary, name is the key.
	 */
	using MaterialMap = OMap<OString, OMaterial, Allocator>;

	/**
	 @brief Loads material list.
	 */
	void loadMaterials(MaterialMap& aMaterialList);
};