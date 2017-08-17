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

OText2D::OText2D(OFont* font, unsigned int fontSize, float x, float y, const OVector4& color,
		 const char* content) :
	_x(x),
	_y(y),
	_font(font),
	_fontSize(fontSize),
	_fontColor(color),
	_lineSpacing(0)
{
	_Init();

	if (content != NULL) _content = content;

	_scale_x = 2.0f / OApplication::activeInstance()->windowWidth();
	_scale_y = 2.0f / OApplication::activeInstance()->windowHeight();

	OApplication::activeInstance()->addEventRecipient(OEvent::ResizeEvent, this);

	glGenBuffers(1, &_arrayBuffer);
	glGenVertexArrays(1, &_arrayObject);
}

OText2D::~OText2D()
{
	glDeleteBuffers(1, &_arrayBuffer);
	glDeleteVertexArrays(1, &_arrayObject);
}

void OText2D::setFont(OFont * font, unsigned int fontSize)
{
	_font = font;
	_fontSize = fontSize;
}

OFont * OText2D::font() const
{
	return _font;
}

unsigned int OText2D::fontSize() const
{
	return _fontSize;
}

void OText2D::setFontColor(const OVector4 & color)
{
	_fontColor = color;
}

OVector4 OText2D::fontColor() const
{
	return _fontColor;
}

void OText2D::setLineSpacing(int spacing)
{
	_lineSpacing = spacing;
}

int OText2D::lineSpacing() const
{
	return _lineSpacing;
}

void OText2D::setPosition(float x, float y)
{
	_x = x;
	_y = y;
}

float OText2D::x() const
{
	return _x;
}

float OText2D::y() const
{
	return _y;
}

void OText2D::setScale(float sx, float sy)
{
	_scale_x = sx;
	_scale_y = sy;
}

float OText2D::scaleX() const
{
	return _scale_x;
}

float OText2D::scaleY() const
{
	return _scale_y;
}

void OText2D::setContent(const char * content)
{
	_content = content;
}

void OText2D::append(const char * content)
{
	_content += content;
}

const char * OText2D::content() const
{
	return _content.c_str();
}

void OText2D::render(OMatrixStack* mtx)
{
	if (isHidden()) return;

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
		/* if char is new line */
		if (*p == '\n') {
			currX = _x;
			currY = _y + _font->lineSpacing() * _scale_y + _lineSpacing;
			continue;
		}

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
