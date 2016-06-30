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
 \brief Set matrix for perspective, camera and world transforms.
 \param mtx The matrix that summarizes all perspective, camera and world transforms to be used on
 the vertex shader. 
*/
void OShaderProgram::setMatrixTransform(const OMatrix4x4& mtx)
{
	glUniformMatrix4fv(_transformMtx, 1, GL_FALSE, mtx.glArea());
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
	_transformMtx = glGetUniformLocation(_program, "transformMtx");
}
