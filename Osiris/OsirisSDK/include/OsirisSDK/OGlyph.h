#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/ORenderable.h"

/**
 @brief The GPU entity that represents a font glyph.
 */
class OAPI OGlyph : public ORenderable
{
public:
	/**
	 @brief Class constructor.
	 @param aAdvanceX Font horizontal advance space to the next character.
	 @param aAdvanceY Font vertical advance space to the next character.
	 */
	OGlyph(uint16_t aAdvanceX, uint16_t aAdvanceY);

	/**
	 @brief Class destructor.
	 */
	~OGlyph() = default;
	
	/**
	 @brief Provides the font horizontal advance space to the next character.
	 */
	uint16_t advanceX() const;

	/**
	 @brief Provides the font vertical advance space to the next character.
	 */
	uint16_t advanceY() const;

private:
	uint16_t _advanceX;
	uint16_t _advanceY;

};

inline OGlyph::OGlyph(uint16_t aAdvanceX, uint16_t aAdvanceY) :
	ORenderable(Type::Glyph),
	_advanceX(aAdvanceX),
	_advanceY(aAdvanceY)
{
}

inline uint16_t OGlyph::advanceX() const
{
	return _advanceX;
}

inline uint16_t OGlyph::advanceY() const
{
	return _advanceY;
}
