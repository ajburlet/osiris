#ifdef WIN32
#include <string.h>
#include <stdlib.h>
#endif

#include "OsirisSDK/OText2D.h"
#include "OsirisSDK/OApplication.h"
#include "OsirisSDK/OException.h"

#include "resource.h"

using namespace std;

bool OText2D::_initialized = false;
OShaderProgram* OText2D::_shaderProgram = NULL;
GLuint OText2D::_shaderCoordAttr;
GLuint OText2D::_shaderTexUniform;
GLuint OText2D::_shaderColorUniform;

/**
 \brief Class constructor.
 \param font Object containing font definitions and cache.
 \param fontSize Font height in pixels.
 \param x Position of the beggining of the text box in the X axis.
 \param y Position of the beggining of the text box in the Y axis.
 \param content Text conte
 */
OText2D::OText2D(OFont* font, unsigned int fontSize, float x, float y, const OVector4& color,
		 const char* content) :
	_x(x),
	_y(y),
	_font(font),
	_fontSize(fontSize),
	_fontColor(color)
{
	_Init();

	if (content != NULL) _content = content;

	_scale_x = 2.0f / OApplication::activeInstance()->windowWidth();
	_scale_y = 2.0f / OApplication::activeInstance()->windowHeight();

	OApplication::activeInstance()->addEventRecipient(OEvent::ResizeEvent, this);

	glGenBuffers(1, &_arrayBuffer);
	glGenVertexArrays(1, &_arrayObject);
}

/**
 \brief Class destructor.
 */
OText2D::~OText2D()
{
	glDeleteBuffers(1, &_arrayBuffer);
	glDeleteVertexArrays(1, &_arrayObject);
}

/**
 \brief Sets font class and size.
 */
void OText2D::setFont(OFont * font, unsigned int fontSize)
{
	_font = font;
	_fontSize = fontSize;
}

/**
 \brief Returns the font file being used.
 */
OFont * OText2D::font() const
{
	return _font;
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
	_fontColor = color;
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
 \param y Position of the text box on the Y axis in pixels.
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
	/* enabling array object */
	glBindVertexArray(_arrayObject);

	/* we use the specific shader program */
	_shaderProgram->use();

	/* enable blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* let us define the texture that will hold the glyph */
	glUniform1i(_shaderTexUniform, 0);

	/* now we enable attribute array and prepare the vertex array buffer */
	glBindBuffer(GL_ARRAY_BUFFER, _arrayBuffer);
	glEnableVertexAttribArray(_shaderCoordAttr);
	glVertexAttribPointer(_shaderCoordAttr, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* set font color */
	glUniform4fv(_shaderColorUniform, 1, _fontColor.glArea());

	/* now we iterate through every character and render */
	float currX = _x;
	float currY = _y;
	for (const char *p = _content.c_str(); *p != '\0'; p++) {
		/* Get font data */
		const OFont::CacheEntry *fEntry = _font->entry(*p, _fontSize);
		if (fEntry == NULL) throw OException("Failed to load full font charset.");

		/* Activate the texture unit 0 and bind the font texture */ 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fEntry->texId);

		/* now we estabilish the vertices that will delimeter the character box */
		float x2 = currX + fEntry->left * _scale_x;
		float y2 = currY - fEntry->top * _scale_y;
		float w = fEntry->width * _scale_x;
		float h = fEntry->rows * _scale_y;

		GLfloat boxVertices[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		/* draw */
		glBufferData(GL_ARRAY_BUFFER, 4*4*sizeof(GLfloat), boxVertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		/* move the cursor */
		currX += (fEntry->advance_x >> 6) * _scale_x;
		currY += (fEntry->advance_y >> 6) * _scale_y;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(_shaderCoordAttr);
	glBindVertexArray(0);
}

void OText2D::onScreenResize(const OResizeEvent * evt)
{
	_scale_x = 2.0f / evt->width();
	_scale_y = 2.0f / evt->height();
}

/**
 \brief Initialize the freetype library for all class objects.
 */
void OText2D::_Init()
{
	if (!_initialized) {
		_shaderProgram = new OShaderProgram("OText2DRenderer");
#ifdef WIN32
		_shaderProgram->addShader(OShaderObject::ShaderType_Vertex, "Vertex-OText2D", IDR_SHADER_VERTEX_OTEXT2D);
		_shaderProgram->addShader(OShaderObject::ShaderType_Fragment, "Fragment-OText2D", IDR_SHADER_FRAGMENT_OTEXT2D);
#else
#error Embedded shader is not yet implemented for non-Windows platforms.
#endif
		_shaderProgram->compile();

		_shaderCoordAttr = _shaderProgram->attribLocation("position");
		_shaderTexUniform = _shaderProgram->uniformLocation("tex");
		_shaderColorUniform = _shaderProgram->uniformLocation("color");
		
		//if (_shaderCoordAttr == -1 || _shaderTexUniform == -1 || _shaderColorUniform == -1)
		//	throw OException("Error accessing text shader parameters.");
	}
}
