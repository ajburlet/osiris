#pragma once

#include <string>
#include <map>

#include "OsirisSDK/ORenderable.h"
#include "OsirisSDK/OText2D.h"
#include "OObject.h"
#include "ORenderObject.h"
#include "OMath.h"
#include "OShaderProgram.h"
#include "OFont.h"
#include "defs.h"

/**
 \brief Two-dimensional text handling class.

 This class enabled the rendering of text on the screen.
 */
class OAPI OText2D : public OObject, public ORenderObject
{
public:

	/**
	 \brief Class constructor.
	 \param font Object containing font definitions and cache.
	 \param fontSize Font height in pixels.
	 \param x Position of the beggining of the text box in the X axis.
	 \param y Position of the beggining of the text box in the Y axis.
	 \param content Text content.
	 */
	OText2D(OFont *font, unsigned int fontSize, float x, float y, const OVector4& color = OVector4(1.0f), 
		const char* content=NULL);

	
	/**
	 \brief Class destructor.
	 */
	virtual ~OText2D();

	/**
	 \brief Sets font class and size.
	 */
	void setFont(OFont* font, unsigned int fontSize);
	
	/**
	 \brief Returns the font file being used.
	 */
	OFont* font() const;
	
	/**
	 \brief Returns the font size.
	 */
	unsigned int fontSize() const;

	/**
	 \brief Set the font color using a OVector4 class object as input.
	 \param color A four-dimensional vector containing the color components in 
		      the form (r, g, b, alpha).
	 */
	void setFontColor(const OVector4& color);
	
	/**
	 \brief Provides the font color in the form of a four-dimensional vector.
	 \return An OVector4 class object in the form (r, g, b, alpha).
	 */
	OVector4 fontColor() const;

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
	 \brief Sets the position of the text box.
	 \param x Position of the text box on the X axis in pixels.
	 \param y Position of the text box on the Y axis in pixels.
	 */
	void setPosition(float x, float y);
	
	/**
	 \brief Returns the position of the text box on the X axis in pixels.
	 */
	float x() const;
	
	/**
	 \brief Returns the position of the text box on the Y axis in pixels.
	 */
	float y() const;
	
	/**
	 \brief Sets the size of the text box.
	 \param sx Horizontal scale of the text box.
	 \param sx Vertical scale of the text box.
	 */
	void setScale(float sx, float sy);
	
	/**
	 \brief Returns the horizontal size of the text box in pixels.
	 */
	float scaleX() const;
	
	/**
	 \brief Returns the vertical size of the text box in pixels.
	 */
	float scaleY() const;

	/**
	 \brief Sets the text content of the text box.
	 \param content New content of the text box.
	 */
	void setContent(const char* content);
	
	/**
	 \brief Adds text to the end of the text box content.
	 \param content Content to be appended to the current string.
	 */
	void append(const char* content);
	
	/**
	 \brief Returns the content of the text box.
	 */
	const char* content() const;
	
	/**
	 \brief Renders the text string on the screen.
	 */
	void render(OMatrixStack* mtx=NULL);

	/* inherited from OOBject */
	void onScreenResize(const OResizeEvent* evt);

private:
	GLuint _arrayObject;
	OFont* _font;
	unsigned int _fontSize;
	OVector4 _fontColor;
	int _lineSpacing;
	float _x;
	float _y;
	float _scale_x;
	float _scale_y;
	std::string _content;

	/* static properties and methods */
	static bool _initialized;
	static OShaderProgram *_shaderProgram;
	static GLuint _shaderCoordAttr;
	static GLuint _shaderTexUniform;
	static GLuint _shaderColorUniform;
	static GLuint _shaderPosition;
	static GLuint _shaderScale;

	/**
	 \brief Initialize the freetype library for all class objects.
	 */
	static void _Init();
};

