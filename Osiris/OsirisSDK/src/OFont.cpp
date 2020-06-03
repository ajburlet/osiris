#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/ORenderComponents.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OGlyph.h"
#include "OsirisSDK/ORenderingEngine.h"
#include "OsirisSDK/OTrashBin.h"
#include "OsirisSDK/OFont.h"

using namespace std;


struct OFont::Impl {
	using GlyphArray = OArray<ORenderComponents*>;
	using GlyphCache = OArray<GlyphArray*>;

	static constexpr uint8_t minFontSize = 7;
	static constexpr uint8_t maxFontSize = 100;

	ORenderingEngine*		renderingEngine	= nullptr;
	string				fontName;
	FT_Face				face		= nullptr;
	GlyphCache			cache;
	
	static FT_Library		library;
	static OVertexBufferDescriptor*	vertexBufferDescriptor;
};

FT_Library OFont::Impl::library = nullptr;
OVertexBufferDescriptor* OFont::Impl::vertexBufferDescriptor = nullptr;

OFont::OFont(ORenderingEngine* aRenderingEngine, const char* aFontName)
{
	OExceptionPointerCheck(_impl = new Impl);
	for (auto& entry : _impl->cache) entry = nullptr;
	_impl->renderingEngine = aRenderingEngine;
	init();

#ifdef WIN32
	char *windir;
	struct stat st;

	_dupenv_s(&windir, NULL, "WINDIR");

	/* if the local file exists, or the fontName refers to a full path or no WINDIR variable is set */
	if (stat(aFontName, &st) == 0 || (strchr(aFontName, '/') != 0 && strchr(aFontName, '\\') != 0) || !windir) {
		_impl->fontName = aFontName;
	} else {
		_impl->fontName = string(windir) + "/fonts/" + aFontName;
	}
#else
	_impl->fontName = aFontName;
#endif
	
	if (FT_New_Face(Impl::library, _impl->fontName.c_str(), 0, &_impl->face) != 0) {
		throw OException("Unable to open font.");
	}

	_lastSize = 0;
	_currCacheArray = NULL;
}

OFont::~OFont()
{
	cleanCache();
	if (_impl->face != NULL) FT_Done_Face(_impl->face);
	if (_impl != nullptr) delete _impl;
}

void OFont::cleanCache()
{
	for (auto& entry : _impl->cache) {
		if (entry != nullptr) {
			for (auto rc : *entry) {
				if (rc != nullptr) {
					_impl->renderingEngine->unload(rc);
					_impl->renderingEngine->trashBin().trash(rc);
				}
			}
			delete entry;
		}
	}
	_impl->cache.clear();
}

const OFont::CacheEntry * OFont::entry(char character, int size)
{
	if (size != _lastSize) {
		map<int, CacheEntry*>::iterator it = _cache.find(size);
		if (it == _cache.end()) {
			_currCacheArray = loadGlyphs(size);
		} else {
			_currCacheArray = it->second;
		}
		_lastSize = size;
	}
	if (!_currCacheArray || _currCacheArray[character].texId == 0) return NULL;
	return &_currCacheArray[character];
}

OGlyph * OFont::createGlyph(char aCharCode, uint8_t aSize, uint16_t aAdvanceX, uint16_t aAdvanceY)
{
	if (aSize < Impl::minFontSize || aSize > Impl::maxFontSize) throw OException("Invalid font size.");

	uint8_t fontIdx = aSize - Impl::minFontSize;

	if (_impl->cache[fontIdx] == nullptr) loadToCache(aSize);

	auto renderComponents = _impl->cache[fontIdx]->get(aCharCode);
	if (renderComponents == nullptr) throw OException("No corresponding font glyph for char code.");

	auto glyph = new OGlyph(aAdvanceX, aAdvanceY);
	OExceptionPointerCheck(glyph);
	try {
		glyph->setRenderComponents(renderComponents);
		if (renderComponents->loaded() == false) _impl->renderingEngine->load(glyph);
	} catch (OException& e) {
		delete glyph;
		throw e;
	}

	return glyph;
}

int OFont::lineSpacing() const
{
	return _impl->face->size->metrics.height / 64;
}

void OFont::loadToCache(uint8_t aSize)
{
	if (FT_Set_Pixel_Sizes(_impl->face, 0, aSize) != 0) throw OException("Unable to set font size.");

	auto& sizeCache = _impl->cache[aSize-Impl::minFontSize];
	if (sizeCache != nullptr) throw OException("Font size already loaded.");

	OExceptionPointerCheck(sizeCache = new Impl::GlyphArray(UINT8_MAX, true));
	ORenderComponents* renderComponents = nullptr;
	OVertexBuffer* vertexBuffer = nullptr;
	OTexture* texture = nullptr;

	try {
		for (uint8_t charCode = 0; charCode <= UINT8_MAX; charCode++) {
			if (FT_Load_Char(_impl->face, charCode, FT_LOAD_RENDER) != 0) continue;

			auto& cacheEntry = (*sizeCache)[charCode];
			OExceptionPointerCheck(cacheEntry = new ORenderComponents);
			cacheEntry->setRenderMode(ORenderMode::TriangleStrip);

			OExceptionPointerCheck(vertexBuffer = new OVertexBuffer(*Impl::vertexBufferDescriptor, 4));
			float x2 = (float)_impl->face->glyph->bitmap_left;
			float y2 = (float)-_impl->face->glyph->bitmap_top;
			float w = (float)_impl->face->glyph->bitmap.width;
			float h = (float)_impl->face->glyph->bitmap.rows;
			float data[4][4] = {
				{ x2, -y2, 0, 0 },
				{ x2 + w, -y2, 1, 0 },
				{ x2, -y2 - h, 0, 1 },
				{ x2 + w, -y2 - h, 1, 1 },
			};
			vertexBuffer->setAttributeValue(0, 0, data[0]);
			vertexBuffer->setAttributeValue(0, 1, data[1]);
			vertexBuffer->setAttributeValue(0, 2, data[2]);
			vertexBuffer->setAttributeValue(0, 3, data[3]);

			OExceptionPointerCheck(texture = new OTexture);
			texture->setMipmapLevelCount(1);
			texture->setMinFilter(OTexture::FilterType::Linear);
			texture->setMagFilter(OTexture::FilterType::Linear);
			texture->setWrapType(OTexture::Coordinate::S, OTexture::WrapMode::ClampToEdge);
			texture->setWrapType(OTexture::Coordinate::T, OTexture::WrapMode::ClampToEdge);
			texture->setPixelFormat(OTexture::PixelFormat::R, OTexture::PixelDataType::UnsignedByte,
				OTexture::PixelFormat::R);
			texture->setContent(0, _impl->face->glyph->bitmap.width, _impl->face->glyph->bitmap.rows,
					    _impl->face->glyph->bitmap.buffer,
					    _impl->face->glyph->bitmap.width*_impl->face->glyph->bitmap.rows);

			cacheEntry->setVertexBuffer(vertexBuffer);
			cacheEntry->setTexture(texture);
			vertexBuffer = nullptr;
			texture = nullptr;
		}
	} catch (OException& e) {
		if (vertexBuffer != nullptr) delete vertexBuffer;
		if (texture != nullptr) delete texture;
		if (renderComponents != nullptr) delete renderComponents;
		throw e;
	}

}

OFont::CacheEntry* OFont::loadGlyphs(int size)
{
	if (FT_Set_Pixel_Sizes(_impl->face, 0, size) != 0) throw OException("Unable to set font size.");

	CacheEntry *entArray = (CacheEntry*) malloc(255 * sizeof(CacheEntry));
	memset(entArray, 0, sizeof(CacheEntry) * 255);
	for (unsigned char character = 0; character < 255; character++) {
		if (FT_Load_Char(_impl->face, character, FT_LOAD_RENDER) != 0) continue;

		entArray[character].advance_x = _impl->face->glyph->advance.x;
		entArray[character].advance_y = _impl->face->glyph->advance.y;
	
		/* Activating texture unit and binding texture ID */
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &entArray[character].texId);
		glBindTexture(GL_TEXTURE_2D, entArray[character].texId);
		
		/* Clamping to edges is important to preventArray[character].artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* We require 1 byte alignmentArray[character].when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _impl->face->glyph->bitmap.width, _impl->face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, _impl->face->glyph->bitmap.buffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		/* Now we estabilish the vertices that will delimeter the character box */
		glGenBuffers(1, &entArray[character].arrBufId);
		glBindBuffer(GL_ARRAY_BUFFER, entArray[character].arrBufId);
		float x2 = (float)_impl->face->glyph->bitmap_left;
		float y2 = (float)-_impl->face->glyph->bitmap_top;
		float w = (float)_impl->face->glyph->bitmap.width;
		float h = (float)_impl->face->glyph->bitmap.rows;

		GLfloat boxVertices[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};
		
		glBufferData(GL_ARRAY_BUFFER, 4*4*sizeof(GLfloat), boxVertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	_cache[size] = entArray;
	return entArray;
}

void OFont::init()
{
	if (Impl::library == nullptr && FT_Init_FreeType(&Impl::library) != 0) {
		throw OException("Failed to load FreeType library.");
	}
	if (Impl::vertexBufferDescriptor == nullptr) {
		OExceptionPointerCheck(Impl::vertexBufferDescriptor = new OVertexBufferDescriptor);
		Impl::vertexBufferDescriptor->addAttribute(OShaderArgument(OVarType::Float, OVarPrecision::High, 4));
	}
}