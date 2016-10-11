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
 \brief Get the shader uniform parameter by name.
 \param uniform_name Name of the uniform parameter.
 \return The OpenGL reference to the uniform.
 */
GLuint OShaderProgram::uniformLocation(const char * uniform_name)
{
	return glGetUniformLocation(_program, (const GLchar*)uniform_name);
}

/**
 \brief Get the shader attribute by name.
 \param attrib_name Name of the shader attribute.
 \return The OpenGL reference to the attribute.
 */
GLuint OShaderProgram::attribLocation(const char * attrib_name)
{
	return glGetAttribLocation(_program, (const GLchar*)attrib_name);
}

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
}
/**
 \brief Tells OpenGL to use the program for the next rendering iteration.
 */
void OShaderProgram::use()
{
	glUseProgram(_program);
}
