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
	 @brief Deleted copy constructor.
	 */
	OFont(const OFont& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OFont(OFont&& aOther);

	/**
	 @brief Class destructor.
	 */
	~OFont();
	
	/**
	 @brief Deleted copy assignment.
	 */
	OFont& operator=(const OFont& aOther) = delete;

	/**
	 @brief Move assignment.
	 */
	OFont& operator=(OFont&& aOther);

	/**
	 @brief Clean font cache: texture data and dimensions for each character and size.
	 */
	void cleanCache();

	/**
	 @brief Creates a new glyph object for a given character.
	 @param aGlyph Glyph to be loaded.
	 @param aCharCode Charecter code byte.
	 @param aSize Font size.
	 @param aColor Font color.
	 */
	void loadGlyph(OGlyph& aGlyph, char aCharCode, uint8_t aSize, const OVector4FL& aColor);

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

inline OFont::OFont(OFont&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

