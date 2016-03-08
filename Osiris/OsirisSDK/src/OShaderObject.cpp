#include <glload/gl_3_3.h>

#include "OsirisSDK/OShaderObject.h"
#include "OsirisSDK/OException.h"

using namespace std;

OShaderObject::OShaderObject(const string &shaderName, GLenum shaderType, const string & source) :
	_shaderName(shaderName),
	_shaderType(shaderType),
	_source(source),
	_shader(0)
{
	
}

OShaderObject::~OShaderObject()
{
	if (_shader != 0) glDeleteShader(_shader);
}

void OShaderObject::setSource(const string & source)
{
	_source = source;
}

GLuint OShaderObject::compile()
{
	const char *shaderSrc = _source.c_str();

	_shader = glCreateShader(_shaderType);
	glShaderSource(_shader, 1, &shaderSrc, NULL);
	glCompileShader(_shader);

	GLint status;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(_shader, infoLogLength, NULL, infoLog);

		std::string strInfoLog(infoLog);
		delete[] infoLog;

		throw OException("Shader compilation error [" + _shaderName + "]: " + strInfoLog);
	}

	return _shader;
}

GLuint OShaderObject::glReference() const
{
	return _shader;
}
