#include <glload/gl_3_3.h>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderProgram.h"

using namespace std;

OShaderProgram::OShaderProgram(const string &name) :
	_programName(name)
{
	_program = glCreateProgram();
}

OShaderProgram::~OShaderProgram()
{
	glDeleteProgram(_program);
}

GLuint OShaderProgram::glReference() const
{
	return _program;
}

void OShaderProgram::addShader(const OShaderObject * shader)
{
	glAttachShader(_program, shader->glReference());
}

void OShaderProgram::compile()
{
	glLinkProgram(_program);

	GLint status;
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(_program, infoLogLength, NULL, infoLog);

		string strInfoLog(infoLog);
		delete[] infoLog;

		throw OException("Shader link error [" + _programName + "]: " + strInfoLog);
	}
}
