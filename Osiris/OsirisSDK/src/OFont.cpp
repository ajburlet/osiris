#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

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
	struct CacheEntry {
		ORenderComponents*	renderComponents	= nullptr;
		uint16_t		advanceX		= 0;
		uint16_t		advanceY		= 0;
	};

	using GlyphArray = OArray<CacheEntry>;
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
	_impl->cache.resize(Impl::maxFontSize - Impl::minFontSize);
	for (uint32_t i = 0; i < _impl->cache.capacity(); i++) _impl->cache[i] = nullptr;
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
			for (auto ce: *entry) {
				if (ce.renderComponents != nullptr) {
					_impl->renderingEngine->unload(ce.renderComponents);
					_impl->renderingEngine->trashBin().trash(ce.renderComponents);
				}
			}
			delete entry;
		}
	}
	_impl->cache.clear();
}

OGlyph * OFont::createGlyph(char aCharCode, uint8_t aSize, const OVector2F& aPosition, const OVector4FL& aColor,
			    const OVector2F& aScale)
{
	if (aSize < Impl::minFontSize || aSize > Impl::maxFontSize) throw OException("Invalid font size.");

	uint8_t fontIdx = aSize - Impl::minFontSize;

	if (_impl->cache[fontIdx] == nullptr) loadToCache(aSize);

	auto cacheEntry = _impl->cache[fontIdx]->get(aCharCode);
	if (cacheEntry.renderComponents == nullptr) throw OException("No corresponding font glyph for char code.");

	auto glyph = new OGlyph(aCharCode, cacheEntry.advanceX, cacheEntry.advanceY, aPosition, aColor, aScale);
	OExceptionPointerCheck(glyph);
	try {
		glyph->setRenderComponents(cacheEntry.renderComponents);
		if (cacheEntry.renderComponents->componentsLoaded() == false) _impl->renderingEngine->load(glyph);
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

	OExceptionPointerCheck(sizeCache = new Impl::GlyphArray(UINT8_MAX+1));
	ORenderComponents* renderComponents = nullptr;
	OVertexBuffer* vertexBuffer = nullptr;
	OTexture* texture = nullptr;

	try {
		for (uint16_t charCode = 0; charCode <= UINT8_MAX; charCode++) {
			if (FT_Load_Char(_impl->face, charCode, FT_LOAD_RENDER) != 0) continue;

			auto& cacheEntry = (*sizeCache)[charCode];
			OExceptionPointerCheck(cacheEntry.renderComponents = new ORenderComponents);
			cacheEntry.renderComponents->setRenderMode(ORenderMode::TriangleStrip);
			cacheEntry.renderComponents->setColorBlending(true, OBlendFactor::SourceAlpha,
								      OBlendFactor::OneMinusSourceAlpha);
			cacheEntry.renderComponents->setFaceCulling(false);

			cacheEntry.advanceX = static_cast<uint16_t>(_impl->face->glyph->advance.x);
			cacheEntry.advanceY = static_cast<uint16_t>(_impl->face->glyph->advance.y);

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
			texture->setUnpackAlignment(OTexture::RowAlignment::Byte);
			texture->setContent(0, _impl->face->glyph->bitmap.width, _impl->face->glyph->bitmap.rows,
					    _impl->face->glyph->bitmap.buffer,
					    _impl->face->glyph->bitmap.width*_impl->face->glyph->bitmap.rows);

			cacheEntry.renderComponents->setVertexBuffer(vertexBuffer);
			cacheEntry.renderComponents->setTexture(texture);
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

void OFont::init()
{
	if (Impl::library == nullptr && FT_Init_FreeType(&Impl::library) != 0) {
		throw OException("Failed to load FreeType library.");
	}
	if (Impl::vertexBufferDescriptor == nullptr) {
		OExceptionPointerCheck(Impl::vertexBufferDescriptor = new OVertexBufferDescriptor);
		Impl::vertexBufferDescriptor->addAttribute(OShaderVertexArgument(OVarType::Float4, 0));
	}
}