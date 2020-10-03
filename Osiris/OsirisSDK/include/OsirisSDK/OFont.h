#pragma once

#include <string>

#include "OsirisSDK/defs.h"

class OGlyph;
class ORenderingEngine;

/**
 @brief Font handler.

 This class will serve as a reference for font usage, storing within it's object font data cache
 in order to avoid frequent accesses to the font files.
 */
class OAPI OFont
{
public:
	/**
	 @brief Class constructor.
	 @param aRenderingEngine Active rendering engine.
	 @param aFontName File name of the font.
	 
	 @note If running under windows without providing a path, the constructor will look on the default font 
	       directory (%WINDIR%/fonts).
	 */
	OFont(ORenderingEngine* aRenderingEngine, const char *aFontName);
	
	/**
	 @brief Class destructor.
	 */
	~OFont();

	/**
	 \brief Clean font cache: texture data and dimensions for each character and size.
	 */
	void cleanCache();

	/**
	 @brief Creates a new glyph object for a given character.
	 @param aCharCode Charecter code byte.
	 @param aSize Font size.
	 @param aPositionOffset Position offset (NDC).
	 @param aColor Font color.
	 @param aScale Scale vector.

	 */
	OGlyph* createGlyph(char aCharCode, uint8_t aSize, const OVector2F& aPosition, const OVector4FL& aColor,
			    const OVector2F& aScale);

	/**
	 @brief Default line spacing (baseline-to-baseline distance).
	 @return Line spacing in pixels.
	 */
	int lineSpacing() const;

	/**
	 @brief Initialize the required font library (freetype).
	 */
	static void init();

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */


	void loadToCache(uint8_t aSize);
};


