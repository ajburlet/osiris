#pragma once

#include "OsirisSDK/defs.h"

class OFile;

/**
 @brief Utility class that helps Wavefront file parsing.
 */
class OAPI OWavefrontParser
{
public:
	/**
	 @brief Class constructor.
	 @param aFile The Wavefront object or material file.
	 */
	OWavefrontParser(OFile* aFile);

	/**
	 @brief Class destructor.
	 */
	virtual ~OWavefrontParser();
	
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
	 @brief Reads next float on the line.
	 @note If end of line has been reached or buffer overflowed, an exception will be thrown.
	 */
	float readNextFloat();

	/**
	 @brief Reads next signed integer on the line.
	 @note If end of line has been reached or buffer overflowed, an exception will be thrown.
	 */
	int32_t readNextInt();

	/**
	 @brief Reads next unsigned integer on the line.
	 @note If end of line has been reached or buffer overflowed, an exception will be thrown.
	 */
	uint32_t readNextUint();
	
	/**
	 @brief Returns the current line of the file that is being processed.
	 */
	unsigned int currLine() const;

	/**
	 @brief Returns true if end of line has been reached.
	 */
	bool endOfLine() const;

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