#pragma once

#include <string>
#include <utility>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "defs.h"
#include "GLdefs.h"

/**
 \brief Font handler.

 This class will serve as a reference for font usage, storing within it's object font data cache
 in order to avoid frequent accesses to the font files.
 */
class OAPI OFont
{
public:
	/**
	 \brief Class constructor.
	 \param fontName File name of the font. If running under windows without providing a path, the
			 constructor will look on the default font directory (%WINDIR%/fonts).
	 */
	OFont(const char *fontName);
	
	/**
	 \brief Class destructor.
	 */
	~OFont();

	/**
	 \brief Clean font cache: texture data and dimensions for each character and size.
	 */
	void cleanCache();
	
	struct CacheEntry {
		GLuint texId;
		int top;
		int left;
		int width;
		int rows;
		int advance_x;
		int advance_y;
	};

	/**
	 \brief Returns the font cache entry related to a given character and size.
	 */
	const CacheEntry* entry(char character, int size);

	/**
	 @brief Default line spacing (baseline-to-baseline distance).
	 @return Line spacing in pixels.
	 */
	int lineSpacing() const;

private:
	std::string _fontName;
	FT_Face _face;
	int _lastSize;

	std::map<std::pair<char,char>, CacheEntry> _cache; /* key will be (size, character) */

	static FT_Library _library;

	/**
	 \brief Initialize the required font library (freetype).
	 */
	static void _init();
};


