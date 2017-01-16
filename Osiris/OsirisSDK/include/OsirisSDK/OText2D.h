#pragma once

#include <string>
#include <map>

#include "OMath.h"
#include "OShaderProgram.h"
#include "OFont.h"
#include "defs.h"

/**
 \brief Two-dimensional text handling class.

 This class enabled the rendering of text on the screen.
 */
class OAPI OText2D
{
public:
	OText2D(OFont *font, unsigned int fontSize, float x, float y, const OVector4& color = OVector4(1.0f),
		float scale_x=1, float scale_y=1, const char* content=NULL);
	virtual ~OText2D();

	void setFont(OFont* font, unsigned int fontSize);
	OFont* font() const;
	unsigned int fontSize() const;

	void setFontColor(const OVector4& color);
	OVector4 fontColor() const;

	void setPosition(float x, float y);
	float x() const;
	float y() const;
	
	void setScale(float sx, float sy);
	float scaleX() const;
	float scaleY() const;

	void setContent(const char* content);
	void append(const char* content);
	const char* content() const;
	
	void render();

private:
	GLuint _arrayObject;
	GLuint _arrayBuffer;
	OFont* _font;
	unsigned int _fontSize;
	OVector4 _fontColor;
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

	static void _Init();
};

