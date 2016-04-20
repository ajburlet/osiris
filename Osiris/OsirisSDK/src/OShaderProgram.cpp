#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderProgram.h"

using namespace std;

OShaderProgram::OShaderProgram(const char* name) :
	_programName(name)
{
	_program = glCreateProgram();
	if (_program == 0) throw OException("Failed to create shader program.");
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

	glUniformMatrix4fv(_perspectiveMtxGlRef, 1, GL_FALSE, perspectiveTransf->top().glArea());
	glUniformMatrix4fv(_cameraMtxGlRef, 1, GL_FALSE, cameraTransf->top().glArea());
}

void OShaderProgram::setModelTransformation(const OMatrixStack * modelMtx)
{
	if (modelMtx != NULL) {
		glUniformMatrix4fv(_modelMtxGlRef, 1, GL_FALSE, modelMtx->top().glArea());
	}
}

void OShaderProgram::addShader(OShaderObject * shader)
{
	_shaderList.push_back(shader);
}

#ifdef WIN32
void OShaderProgram::addShader(OShaderObject::ShaderType type, const char* name, int resourceId)
{
	OShaderObject *obj = new OShaderObject(name, type);
	obj->setSourceFromResource(resourceId);
	addShader(obj);
}
#endif

void OShaderProgram::compile()
{
	for (list<OShaderObject*>::iterator sit = _shaderList.begin(); sit != _shaderList.end(); sit++) {
		(*sit)->compile();
		glAttachShader(_program, (*sit)->glReference());
	}

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

		string errMsg = "Shader link error [" + _programName + "]: " + strInfoLog;
		throw OException(errMsg.c_str());
	}

	/* get proper locations */
	_modelMtxGlRef = glGetUniformLocation(_program, "modelMtx");
	_perspectiveMtxGlRef = glGetUniformLocation(_program, "perspectiveMtx");
	_cameraMtxGlRef = glGetUniformLocation(_program, "cameraMtx");
}
