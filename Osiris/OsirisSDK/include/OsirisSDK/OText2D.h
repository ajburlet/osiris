#pragma once

#include <string>
#include <map>

#include "OsirisSDK/OObject.h"
#include "OsirisSDK/OVisualObject.h"
#include "OsirisSDK/OVectorDefs.h"
#include "OsirisSDK/defs.h"


class ORenderingEngine;
class OFont;

/**
 @brief Two-dimensional text handling class.

 This class enabled the rendering of text on the screen.
 */
class OAPI OText2D : public OObject, public OVisualObject
{
public:

	/**
	 @brief Class constructor.
	 @param aFont Object containing font definitions and cache.
	 @param aFontSize Font height in pixels.
	 @param aPosition Position of the beggining of the text box.
	 @param aContent Text content.
	 */
	OText2D(OFont *aFont, uint8_t aFontSize, const OVector2I32& aPosition, const OVector4FL& aColor, 
		const char* aContent=NULL);

	/**
	 @brief Deleted copy constructor.
	 */
	OText2D(const OText2D& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OText2D(OText2D&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OText2D();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OText2D& operator=(const OText2D& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OText2D& operator=(OText2D&& aOther);

	/**
	 @brief Sets font class.
	 */
	void setFont(OFont* aFont);

	/**
	 @brief Sets font size.
	 */
	void setFontSize(uint8_t aSize);
	
	/**
	 @brief Returns the font file being used.
	 */
	OFont* font() const;
	
	/**
	 @brief Returns the font size.
	 */
	uint8_t fontSize() const;

	/**
	 @brief Set the font color using a OVector4 class object as input.
	 @param color A four-dimensional vector containing the color components in 
		      the form (r, g, b, alpha).
	 */
	void setFontColor(const OVector4FL& color);
	
	/**
	 @brief Provides the font color in the form of a four-dimensional vector.
	 @return An OVector4 class object in the form (r, g, b, alpha).
	 */
	const OVector4FL& fontColor() const;

	/**
	 @brief Set line spacing.
	 @param Spacing in pixels.
	 */
	void setLineSpacing(int spacing);

	/**
	 @brief Returns line spacing.
	 @return Spacing in pixels.
	 */
	int lineSpacing() const;

	/**
	 @brief Sets the position of the text box.
	 @param aPosition Position of the text box in pixels.
	 */
	void setPosition(const OVector2I32& aPosition);
	
	/**
	 @brief Returns the position of the text box in pixels.
	 */
	const OVector2I32& position() const;
	
	/**
	 @brief Sets the scale of the text box.
	 @param sx Horizontal scale of the text box.
	 @param sx Vertical scale of the text box.
	 */
	void setScale(const OVector2F& aScale);
	
	/**
	 @brief Returns the scale components.
	 */
	const OVector2F& scale() const;

	/**
	 @brief Sets the text content of the text box.
	 @param content New content of the text box.
	 */
	void setContent(const char* content);
	
	/**
	 @brief Adds text to the end of the text box content.
	 @param content Content to be appended to the current string.
	 */
	void append(const char* content);
	
	/**
	 @brief Returns the content of the text box.
	 */
	const char* content() const;
	
	/**
	 @brief Renders the text string on the screen.
	 @param aRenderingEngine The rendering engine.
	 @param aMatrixStack Matrix stack (not relevant for this class).
	 */
	void render(ORenderingEngine* aRenderingEngine, OMatrixStack* aMatrixStack = nullptr);

	/**
	 @copydoc OObject::onScreenResize(const OResizeEvent*)
	 */
	void onScreenResize(const OResizeEvent* evt);

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OText2D::OText2D(OText2D&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}
