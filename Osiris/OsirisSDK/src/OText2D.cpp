#ifdef WIN32
#include <string.h>
#include <stdlib.h>
#endif

#include "OsirisSDK/OText2D.h"
#include "OsirisSDK/OApplication.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OGlyph.h"
#include "OsirisSDK/ORenderingEngine.h"
#include "OsirisSDK/OFont.h"
#include "OsirisSDK/OException.h"

using namespace std;

// ****************************************************************************
// OText2D concealed members & methods
// ****************************************************************************
struct OText2D::Impl {
	OFont*		font		= nullptr;
	uint8_t		fontSize	= 0;
	int		lineSpacing	= 0;

	OVector2I32	position;
	OVector4FL	fontColor;
	OVector2F	scale;
	std::string	content;
	OList<OGlyph*>	glyph_list;
	bool		redraw		= false;

	void UpdateViewportConversion(uint32_t aViewportWidth, uint32_t aViewportHeight);
	OVector2F ConvertToNDC(const OVector2I32& a_viewport_coordinate);
};

void OText2D::Impl::UpdateViewportConversion(uint32_t aViewportWidth, uint32_t aViewportHeight)
{
	// NDC space goes from -1.0 to 1.0 on X and Y axes
	scale = OVector2F(2.0f / aViewportWidth, 2.0f / aViewportHeight);
}

OVector2F OText2D::Impl::ConvertToNDC(const OVector2I32 & aViewportCoordinate)
{
	return OVector2F(aViewportCoordinate.x() * scale.x() - 1.0f, - aViewportCoordinate.y() * scale.y() + 1.0f);
}

// ****************************************************************************
// OText2D class methods
// ****************************************************************************
OText2D::OText2D(OFont* aFont, uint8_t aFontSize, const OVector2I32& aPosition, const OVector4FL& aColor,
		 const char* aContent) 
{
	OExceptionPointerCheck(_impl = new Impl);

	_impl->font = aFont;
	_impl->fontSize = aFontSize;
	if (aContent != NULL) _impl->content = aContent;
	_impl->position = aPosition;
	_impl->fontColor = aColor;
	_impl->UpdateViewportConversion(OApplication::activeInstance()->windowWidth(),
					OApplication::activeInstance()->windowHeight());
	
	OApplication::activeInstance()->addEventRecipient(OEventType::ResizeEvent, this);
}

OText2D::~OText2D()
{
	if (_impl != nullptr) {
		for (auto glyph : _impl->glyph_list) delete glyph;
		delete _impl;
	}
}

OText2D & OText2D::operator=(OText2D && aOther)
{
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

void OText2D::setFont(OFont * font)
{
	_impl->font = font;
}

void OText2D::setFontSize(uint8_t aSize)
{
	_impl->fontSize = aSize;
}

OFont * OText2D::font() const
{
	return _impl->font;
}

uint8_t OText2D::fontSize() const
{
	return _impl->fontSize;
}

void OText2D::setFontColor(const OVector4FL & aColor)
{
	_impl->fontColor = aColor;
}

const OVector4FL& OText2D::fontColor() const
{
	return _impl->fontColor;
}

void OText2D::setLineSpacing(int spacing)
{
	_impl->lineSpacing = spacing;
}

int OText2D::lineSpacing() const
{
	return _impl->lineSpacing;
}

void OText2D::setPosition(const OVector2I32& aPosition)
{
	_impl->position = aPosition;
}

const OVector2I32& OText2D::position() const
{
	return _impl->position;
}

void OText2D::setScale(const OVector2F& aScale)
{
	_impl->scale = aScale;
}

const OVector2F & OText2D::scale() const
{
	return _impl->scale;
}

void OText2D::setContent(const char * content)
{
	_impl->redraw = true;
	_impl->content = content;
}

void OText2D::append(const char * content)
{
	_impl->redraw = true;
	_impl->content += content;
}

const char * OText2D::content() const
{
	return _impl->content.c_str();
}

void OText2D::render(ORenderingEngine* aRenderingEngine, OMatrixStack*)
{
	/* now we iterate through every character and render */
	if (_impl->redraw) {
		for (auto glyph : _impl->glyph_list) delete glyph;
		_impl->glyph_list.clear();

		auto initialPos = _impl->ConvertToNDC(_impl->position);
		auto currPos = initialPos;
		for (const char *p = _impl->content.c_str(); *p != '\0'; p++) {
			/* if char is new line */
			if (*p == '\n') {
				currPos.setX(initialPos.x());
				currPos.setY(currPos.y() - (_impl->font->lineSpacing() + _impl->lineSpacing) * _impl->scale.y());
				continue;
			}

			/* Get font data */
			auto glyph = _impl->font->createGlyph(*p, _impl->fontSize, currPos, _impl->fontColor, _impl->scale);
			_impl->glyph_list.pushBack(glyph);

			/* move the cursor */
			currPos += OVector2F((glyph->advanceX() >> 6) * _impl->scale.x(), 
					     (glyph->advanceY() >> 6) * _impl->scale.y());
		}

		_impl->redraw = false;
	}

	for (auto glyph : _impl->glyph_list) {
		aRenderingEngine->render(glyph);
	}
}

void OText2D::onScreenResize(const OResizeEvent * evt)
{
	_impl->UpdateViewportConversion(evt->width(), evt->height());
	_impl->redraw = true;
}

