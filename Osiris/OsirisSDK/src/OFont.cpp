#include "OsirisSDK/OFont.h"
#include "OsirisSDK/OException.h"

using namespace std;

FT_Library OFont::_library = NULL;

OFont::OFont(const char * fontName)
{
	_init();

#ifdef WIN32
	char *windir;
	struct stat st;

	_dupenv_s(&windir, NULL, "WINDIR");

	/* if the local file exists, or the fontName refers to a full path or no WINDIR variable is set */
	if (stat(fontName, &st) == 0 || (strchr(fontName, '/') != 0 && strchr(fontName, '\\') != 0) || !windir) {
		_fontName = fontName;
	} else {
		_fontName = string(windir) + "/fonts/" + fontName;
	}
#else
	_fontName = fontName;
#endif
	
	if (FT_New_Face(_library, _fontName.c_str(), 0, &_face) != 0) {
		throw OException("Unable to open font.");
	}
	_lastSize = 0;
}

OFont::~OFont()
{
	if (&_face != NULL) FT_Done_Face(_face);
	cleanCache();
}

void OFont::cleanCache()
{
	map<pair<char,char>, CacheEntry>::iterator it;
	for (it = _cache.begin(); it != _cache.end(); it++) {
		glDeleteTextures(1, (const GLuint*)&it->second.texId);
		glDeleteBuffers(1, (const GLuint*)&it->second.arrBufId);
	}
	_cache.clear();
}

const OFont::CacheEntry * OFont::entry(char character, int size)
{
	pair<char, char> key = std::make_pair(size, character);

	map<pair<char,char>, CacheEntry>::iterator it;
	if ((it = _cache.find(key)) == _cache.end()) {
		CacheEntry ent;

		if (size != _lastSize && FT_Set_Pixel_Sizes(_face, 0, size) != 0) throw OException("Unable to set font size.");
		_lastSize = size;

		if (FT_Load_Char(_face, character, FT_LOAD_RENDER) != 0) return NULL;

		ent.advance_x = _face->glyph->advance.x;
		ent.advance_y = _face->glyph->advance.y;
	
		/* Activating texture unit and binding texture ID */
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &ent.texId);
		glBindTexture(GL_TEXTURE_2D, ent.texId);
		
		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _face->glyph->bitmap.width, _face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, _face->glyph->bitmap.buffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		/* Now we estabilish the vertices that will delimeter the character box */
		glGenBuffers(1, &ent.arrBufId);
		glBindBuffer(GL_ARRAY_BUFFER, ent.arrBufId);
		float x2 = (float)_face->glyph->bitmap_left;
		float y2 = (float)-_face->glyph->bitmap_top;
		float w = (float)_face->glyph->bitmap.width;
		float h = (float)_face->glyph->bitmap.rows;

		GLfloat boxVertices[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};
		
		glBufferData(GL_ARRAY_BUFFER, 4*4*sizeof(GLfloat), boxVertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		_cache[key] = ent;
	}

	return &_cache[key];
}

int OFont::lineSpacing() const
{
	return _face->size->metrics.height / 64;
}

void OFont::_init()
{
	if (!_library && FT_Init_FreeType(&_library) != 0) throw OException("Failed to load FreeType library.");
}