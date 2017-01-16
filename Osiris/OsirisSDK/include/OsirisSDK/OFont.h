#pragma once

#include <string>
#include <utility>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "defs.h"
#include "GLdefs.h"

class OAPI OFont
{
public:
	OFont(const char *fontName);
	~OFont();

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

	const CacheEntry* entry(char character, int size);


private:
	std::string _fontName;
	FT_Face _face;
	int _lastSize;

	std::map<std::pair<char,char>, CacheEntry> _cache; /* key will be (size, character) */

	static FT_Library _library;
	static void _init();
};


