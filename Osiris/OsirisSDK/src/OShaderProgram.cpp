#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderProgram.h"

using namespace std;

/**
 \brief Class constructor.
 \param name Shader name.
*/
OShaderProgram::OShaderProgram(const char* name) :
	_programName(name)
{
	_program = glCreateProgram();
	if (_program == 0) throw OException("Failed to create shader program.");
}

/**
 \brief Class destructor.
*/
OShaderProgram::~OShaderProgram()
{
	glDeleteProgram(_program);
}

/**
 \brief Returns the OpenGL reference ID.
*/
GLuint OShaderProgram::glReference() const
{
	return _program;
}

/**
 \brief Set camera that will be used by this shader program.
 \param camera Pointer to the camera object.
 \todo This will be eliminated on the next version. The camera transformations must be included 
 in the transformation matrix stack passed to each mesh during the rendering run.
*/
void OShaderProgram::setCamera(OCamera * camera)
{
	const OMatrixStack *perspectiveTransf, *cameraTransf;

	perspectiveTransf = camera->perspectiveTransform();
	cameraTransf = camera->cameraTransform();

	if (!perspectiveTransf || !cameraTransf) throw OException("Incomplete camera transformations.");

	glUniformMatrix4fv(_perspectiveMtxGlRef, 1, GL_FALSE, perspectiveTransf->top().glArea());
	glUniformMatrix4fv(_cameraMtxGlRef, 1, GL_FALSE, cameraTransf->top().glArea());
}

/**
 \brief Set model specific transformations (translations, rotations, scaling).
 \modelMtx Pointer to the matrix stack containing the transformations.
 \todo This will be eliminated on the next version. A single matrix will be passed 
 to the shader containing both camera/perspective and model transformations. There
 will be a single transformation set method.
*/
void OShaderProgram::setModelTransformation(const OMatrixStack * modelMtx)
{
	if (modelMtx != NULL) {
		glUniformMatrix4fv(_modelMtxGlRef, 1, GL_FALSE, modelMtx->top().glArea());
	}
}

/**
 \brief Add a shader object to the program.
 \param shader Pointer to the shader object that will be added.
*/
void OShaderProgram::addShader(OShaderObject * shader)
{
	_shaderList.push_back(shader);
}

#ifdef WIN32
/**
 \brief Add a shader object to the program using the Visual Studio resource file ID (WIN32 only).
 \param type Shader type.
 \param name Shader name.
 \param resourceId Visual Studio resource file ID that references the shader source code.
*/
void OShaderProgram::addShader(OShaderObject::ShaderType type, const char* name, int resourceId)
{
	OShaderObject *obj = new OShaderObject(name, type);
	obj->setSourceFromResource(resourceId);
	addShader(obj);
}
#endif

/**
 \brief Compiles shader objects and links the shader program.
*/
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
