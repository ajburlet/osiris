#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OGlyph.h"

struct OGlyph::Impl {
	OVector2F	positionOffset;
	OVector2F	scale;
	OVector4FL	color;
};

OGlyph::OGlyph(uint16_t aAdvanceX, uint16_t aAdvanceY) :
	ORenderable(Type::Glyph),
	_advanceX(aAdvanceX),
	_advanceY(aAdvanceY)
{
	OExceptionPointerCheck(_impl = new Impl);
}

OGlyph::~OGlyph()
{
	if (_impl != nullptr) delete _impl;
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
