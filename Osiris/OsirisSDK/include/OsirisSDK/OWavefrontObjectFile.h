#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OMeshFile.h"
#include "OsirisSDK/OWavefrontParser.h"

class OMeshGeometry;

/**
 @brief Mesh file handler for Wavefront object files. 
 */
class OAPI OWavefrontObjectFile : public OMeshFile, protected OWavefrontParser
{
public:
	/**
	 @brief Class constructor.
	 @param filename Object file name.
	 */
	OWavefrontObjectFile(const OString& aFilename);

	/**
	 @brief Class copy constructor.
	 */
	OWavefrontObjectFile(const OWavefrontObjectFile& aOther);

	/**
	 @brief Move constructor.
	 */
	OWavefrontObjectFile(OWavefrontObjectFile&& aOther);
	
	/**
	 @brief Class destructor.
	 */
	virtual ~OWavefrontObjectFile();

	/**
	 @brief Copy assignment operator.
	 */
	OWavefrontObjectFile& operator=(const OWavefrontObjectFile& aOther);

	/**
	 @brief Move assignment operator.
	 */
	OWavefrontObjectFile& operator=(OWavefrontObjectFile&& aOther);

	// OMeshFile interface overrides
	virtual void loadMesh(const OString& aObjName, RawData& aRawData) override;

protected:
	/**
	 @brief Scans the file for objecs.
	 */
	void loadObjectList();

	/**
	 @brief Loads materials.
	 */
	void loadMaterial(const OString& aFilename);

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OWavefrontObjectFile::OWavefrontObjectFile(OWavefrontObjectFile && aOther) : 
	OMeshFile(std::move(aOther)),
	OWavefrontParser(this)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

