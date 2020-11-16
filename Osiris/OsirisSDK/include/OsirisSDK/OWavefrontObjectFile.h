#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OMeshFile.h"

class OMeshGeometry;

/**
 @brief Class that parses an Waveffront Object file and loads vertex information on a OMesh
        class object.
 */
class OAPI OWavefrontObjectFile : public OMeshFile
{
public:
	/**
	 @brief Class constructor.
	 @param filename Object file name.
	 */
	OWavefrontObjectFile(const char* aFilename);

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
	virtual void loadMesh(const char* aObjName, RawData& aRawData) override;

	virtual const ObjectNameArray & objectArray() const override;

protected:
	/**
	 @brief Scans the file for objecs.
	 */
	void loadObjectList();

	/**
	 @brief Reads the next line into the class internal buffer
	 */
	int readNextLine();
	
	/**
	 @brief Reads the next word on the line.
	 @return Returns the next word. If the end of line has been reached or the buffer overflowed, returns NULL.
	 */
	char* readNextWord();
	
	/**
	 @brief Returns the current line of the file that is being processed.
	 */
	unsigned int currLine();

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

inline OWavefrontObjectFile::OWavefrontObjectFile(OWavefrontObjectFile && aOther) : OMeshFile(std::move(aOther))
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

