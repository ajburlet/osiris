#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OGlyph.h"

struct OGlyph::Impl {
	OVector2F	positionOffset;
	OVector2F	scale;
	OVector4FL	color;
};

OGlyph::OGlyph(char aCharCode, uint16_t aAdvanceX, uint16_t aAdvanceY, const OVector2F& aPositionOffset,
		const OVector4FL& aColor, const OVector2F& aScale) :
	ORenderable(Type::Glyph),
	_charCode(aCharCode),
	_advanceX(aAdvanceX),
	_advanceY(aAdvanceY)
{
	OExceptionPointerCheck(_impl = new Impl);
	setPositionOffset(aPositionOffset);
	setColor(aColor);
	setScale(aScale);
}

OGlyph::~OGlyph()
{
	if (_impl != nullptr) delete _impl;
}

OGlyph & OGlyph::operator=(OGlyph && aOther)
{
	_charCode = aOther._charCode;
	_advanceX = aOther._advanceX;
	_advanceY = aOther._advanceY;

	if (_impl != nullptr) delete _impl;

	_impl = aOther._impl;
	aOther._impl = nullptr;

	return *this;
}

void OGlyph::setPositionOffset(const OVector2F & aOffset)
{
	_impl->positionOffset = aOffset;
}

const OVector2F & OGlyph::positionOffset() const
{
	return _impl->positionOffset;
}

void OGlyph::setScale(const OVector2F & aScale)
{
	_impl->scale = aScale;
}

const OVector2F & OGlyph::scale() const
{
	return _impl->scale;
}

void OGlyph::setColor(const OVector4FL & aColor)
{
	_impl->color = aColor;
}

const OVector4FL & OGlyph::color() const
{
	return _impl->color;
}
