#ifdef WIN32
#include <string.h>
#include <stdlib.h>
#endif

#include "OsirisSDK/OText2D.h"
#include "OsirisSDK/OException.h"

#include "resource.h"

using namespace std;

FT_Library OText2D::_library = NULL;
OShaderProgram* OText2D::_shaderProgram = NULL;
GLuint OText2D::_shaderCoordAttr;
GLuint OText2D::_shaderTexUniform;
GLuint OText2D::_shaderColorUniform;

/**
 \brief Class constructor.
 \param fontName Font file name.
 \param fontSize Font height in pixels.
 \param x Position of the beggining of the text box in the X axis.
 \param y Position of the beggining of the text box in the Y axis.
 \param scale_x Horizontal size component.
 \param scale_y Vertical size component.
 \param content Text conte
 */
OText2D::OText2D(const char *fontName, unsigned int fontSize, float x, float y, const OVector4& color,
		 float scale_x, float scale_y, const char* content) :
	_x(x),
	_y(y),
	_fontColor(color),
	_scale_x(scale_x),
	_scale_y(scale_y),
	_face(NULL)
{
	_Init();

	if (content != NULL) _content = content;
	setFont(fontName, fontSize);
	glGenBuffers(1, &_arrayBuffer);
}

/**
 \brief Class destructor.
 */
OText2D::~OText2D()
{
	if (&_face != NULL) FT_Done_Face(_face);
	glDeleteBuffers(1, &_arrayBuffer);
}

/**
 \brief Set font file and size.
 \param fontName Name of the font file. In Windows, it will try to open from the 
		 default font directory first (%WINDIR%/fonts), unless a full or
		 relative path is specified.
 \param fontSize Size of the font.
 */
void OText2D::setFont(const char * fontName, unsigned int fontSize)
{
#ifdef WIN32
	char *windir;
	_dupenv_s(&windir, NULL, "WINDIR");
	if (windir != NULL && strchr(fontName, '/') != 0 && strchr(fontName, '\\') != 0) {
		_fontName = fontName;
	} else {
		_fontName = string(windir) + "/fonts/" + fontName;
	}
#else
	_fontName = fontName;
#endif

	_fontSize = fontSize;
	if (&_face != NULL) FT_Done_Face(_face);
	if (FT_New_Face(_library, _fontName.c_str(), 0, &_face) != 0) throw OException("Unable to open font.");
	if (FT_Set_Pixel_Sizes(_face, 0, _fontSize) != 0) throw OException("Unable to set font size.");
}

/**
 \brief Returns the font file being used.
 */
const char * OText2D::fontName() const
{
	return _fontName.c_str();
}

/**
 \brief Returns the font size.
 */
unsigned int OText2D::fontSize() const
{
	return _fontSize;
}

/**
 \brief Set the font color using a OVector4 class object as input.
 \param color A four-dimensional vector containing the color components in 
	      the form (r, g, b, alpha).
 */
void OText2D::setFontColor(const OVector4 & color)
{
}

/**
 \brief Provides the font color in the form of a four-dimensional vector.
 \return An OVector4 class object in the form (r, g, b, alpha).
 */
OVector4 OText2D::fontColor() const
{
	return _fontColor;
}

/**
 \brief Sets the position of the text box.
 \param x Position of the text box on the X axis in pixels.
 \para, y Position of the text box on the Y axis in pixels.
 */
void OText2D::setPosition(float x, float y)
{
	_x = x;
	_y = y;
}

/**
 \brief Returns the position of the text box on the X axis in pixels.
 */
float OText2D::x() const
{
	return _x;
}

/**
 \brief Returns the position of the text box on the Y axis in pixels.
 */
float OText2D::y() const
{
	return _y;
}

/**
 \brief Sets the size of the text box.
 \param sx Horizontal scale of the text box.
 \param sx Vertical scale of the text box.
 */
void OText2D::setScale(float sx, float sy)
{
	_scale_x = sx;
	_scale_y = sy;
}

/**
 \brief Returns the horizontal size of the text box in pixels.
 */
float OText2D::scaleX() const
{
	return _scale_x;
}

/**
 \brief Returns the vertical size of the text box in pixels.
 */
float OText2D::scaleY() const
{
	return _scale_y;
}

/**
 \brief Sets the text content of the text box.
 \param content New content of the text box.
 */
void OText2D::setContent(const char * content)
{
	_content = content;
}

/**
 \brief Adds text to the end of the text box content.
 \param content Content to be appended to the current string.
 */
void OText2D::append(const char * content)
{
	_content += content;
}

/**
 \brief Returns the content of the text box.
 */
const char * OText2D::content() const
{
	return _content.c_str();
}

/**
 \brief Renders the text string on the screen.
 */
void OText2D::render()
{
	FT_GlyphSlot g = _face->glyph;

	/* we use the specific shader program */
	_shaderProgram->use();

	/* let us define the texture that will hold the glyph */
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(_shaderTexUniform, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* now we enable attribute array and prepare the vertex array buffer */
	glEnableVertexAttribArray(_shaderCoordAttr);
	glBindBuffer(GL_ARRAY_BUFFER, _arrayBuffer);
	glVertexAttribPointer(_shaderCoordAttr, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* now we iterate through every character and render */
	float currX = _x;
	float currY = _y;
	for (const char *p = _content.c_str(); *p != '\0'; p++) {
		/* Load the char from FreeType library, ignore on failure */
		if (FT_Load_Char(_face, *p, FT_LOAD_RENDER) != 0) continue;

		/* Let us upload the texture to be used on the surface of the box -- glyph */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* now we estabilish the vertices that will delimeter the character box */
		float x2 = currX + g->bitmap_left*_scale_x;
		float y2 = currY + g->bitmap_top*_scale_y;
		float w = g->bitmap.width*_scale_x;
		float h = g->bitmap.rows*_scale_y;

		GLfloat boxVertices[4][4] = {
			{x2,	y2,	0,	0},
			{x2 + w,y2,	1,	0},
			{x2,	y2 + h,	0,	1},
			{x2 + w,y2 + h,	1,	1}
		};

		/* draw */
		glBufferData(GL_ARRAY_BUFFER, 4*4*sizeof(GLfloat), boxVertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		/* move the cursor */
		currX += (g->advance.x >> 6) * _scale_x;
		currY += (g->advance.y >> 6) * _scale_y;
	}

	glDisableVertexAttribArray(_shaderCoordAttr);
	glDeleteTextures(1, &tex);
}

/**
 \brief Initialize the freetype library for all class objects.
 */
void OText2D::_Init()
{
	if (!_library) {
		if (FT_Init_FreeType(&_library) != 0) throw OException("Failed to load FreeType library.");
		_shaderProgram = new OShaderProgram("OText2DRenderer");
#ifdef WIN32
		_shaderProgram->addShader(OShaderObject::ShaderType_Vertex, "Vertex-OText2D", IDR_SHADER_VERTEX_OTEXT2D);
		_shaderProgram->addShader(OShaderObject::ShaderType_Fragment, "Fragment-OText2D", IDR_SHADER_FRAGMENT_OTEXT2D);
#else
#error Embedded shader is not yet implemented for non-Windows platforms.
#endif
		_shaderProgram->compile();

		_shaderCoordAttr = _shaderProgram->attribLocation("coord");
		_shaderTexUniform = _shaderProgram->uniformLocation("tex");
		_shaderColorUniform = _shaderProgram->uniformLocation("color");
	}
}
