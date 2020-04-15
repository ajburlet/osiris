#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderObject.h"
#include "OsirisSDK/OShaderProgram.h"

using namespace std;

struct OShaderProgram::Implementation {
	using ShaderObjectList = OList<OShaderObject*, OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>>;
	ShaderObjectList objects;
};

OShaderProgram::OShaderProgram(const char* name) :
	_programName(name)
{
	_program = glCreateProgram();
	if (_program == 0) throw OException("Failed to create shader program.");
}

OShaderProgram::OShaderProgram()
{
	OExceptionPointerCheck(_impl = new Implementation);
}

OShaderProgram::~OShaderProgram()
{
	for (auto object : _impl->objects) {
		delete object;
	}

	if (_impl) delete _impl;
	glDeleteProgram(_program); // remove this!
}

void OShaderProgram::addObject(OShaderObject * aObject)
{
	_impl->objects.pushBack(aObject);
}

void OShaderProgram::addPreprocessorMacro(const char * aName, const char * aValue)
{
	for (auto object : _impl->objects) {
		object->addPreprocessorMacro(aName, aValue);
	}
}

GLuint OShaderProgram::glReference() const
{
	return _program;
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

GLuint OShaderProgram::uniformLocation(const char * uniform_name)
{
	return glGetUniformLocation(_program, (const GLchar*)uniform_name);
}

GLuint OShaderProgram::attribLocation(const char * attrib_name)
{
	return glGetAttribLocation(_program, (const GLchar*)attrib_name);
}

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

void OShaderProgram::use()
{
	glUseProgram(_program);
}
