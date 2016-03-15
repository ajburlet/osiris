#include <glload/gl_3_3.h>
#include <glm/gtc/type_ptr.hpp>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderProgram.h"

using namespace std;

OShaderProgram::OShaderProgram(const string &name) :
	_programName(name)
{
	_program = glCreateProgram();
	_modelMtxGlRef = glGetUniformLocation(_program, "modelMtx");
	_perspectiveMtxGlRef = glGetUniformLocation(_program, "perspectiveMtx");
	_cameraMtxGlRef = glGetUniformLocation(_program, "cameraMtx");
}

OShaderProgram::~OShaderProgram()
{
	glDeleteProgram(_program);
}

GLuint OShaderProgram::glReference() const
{
	return _program;
}

void OShaderProgram::setCamera(OCamera * camera)
{
	const OMatrixStack *perspectiveTransf, *cameraTransf;

	perspectiveTransf = camera->perspectiveTransform();
	cameraTransf = camera->cameraTransform();

	if (!perspectiveTransf || !cameraTransf) throw OException("Incomplete camera transformations.");

	glUniformMatrix4fv(_perspectiveMtxGlRef, 1, GL_FALSE, glm::value_ptr(perspectiveTransf->top()));
	glUniformMatrix4fv(_cameraMtxGlRef, 1, GL_FALSE, glm::value_ptr(cameraTransf->top()));
}

void OShaderProgram::setModelTransformation(const OMatrixStack * modelMtx)
{
	if (modelMtx != NULL) {
		glUniformMatrix4fv(_modelMtxGlRef, 1, GL_FALSE, glm::value_ptr(modelMtx->top()));
	}
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
