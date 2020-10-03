#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OVectorDefs.h"
#include "OsirisSDK/ORenderable.h"

/**
 @brief The GPU entity that represents a font glyph.
 */
class OAPI OGlyph : public ORenderable
{
public:
	/**
	 @brief Class constructor.
	 @param aCharCode Charecter code byte.
	 @param aAdvanceX Font horizontal advance space to the next character.
	 @param aAdvanceY Font vertical advance space to the next character.
	 @param aPositionOffset Position offset (NDC).
	 @param aColor Font color.
	 @param aScale Scale vector.
	 */
	OGlyph(char aCharCode, uint16_t aAdvanceX, uint16_t aAdvanceY, const OVector2F& aPositionOffset,
		const OVector4FL& aColor, const OVector2F& aScale);

	/**
	 @brief Class destructor.
	 */
	~OGlyph();

	/**
	 @brief Charecter code byte.
	 */
	char charCode() const;

	/**
	 @brief Provides the font horizontal advance space to the next character.
	 */
	uint16_t advanceX() const;

	/**
	 @brief Provides the font vertical advance space to the next character.
	 */
	uint16_t advanceY() const;

	/**
	 @brief Sets the position offset from the starting point of the text box. 
	 */
	void setPositionOffset(const OVector2F& aOffset);

	/**
	 @brief Returns the position offset from the starting point of the text box (const). 
	 */
	const OVector2F& positionOffset() const;

	/**
	 @brief The scale vector to be applied. 
	 */
	void setScale(const OVector2F& aScale);

	/**
	 @brief The scale vector to be applied (const).
	 */
	const OVector2F& scale() const;

	/**
	 @brief Sets the glyph color.
	 */
	void setColor(const OVector4FL& aColor);

	/**
	 @brief Returns the glyph color.
	 */
	const OVector4FL& color() const;

private:
	char		_charCode;
	uint16_t	_advanceX;
	uint16_t	_advanceY;

	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline char OGlyph::charCode() const
{
	return _charCode;
}

inline uint16_t OGlyph::advanceX() const
{
	return _advanceX;
}

inline uint16_t OGlyph::advanceY() const
{
	return _advanceY;
}

