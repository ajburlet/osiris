#ifdef WIN32
#include <Windows.h>
#include <cstdio>
#include "resource.h"
#endif

#include <utility>
#include <string>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OShaderObject.h"

using namespace std;

struct OShaderObject::Implementation
{
	using PreprocessorPair = pair<string, string>;
	using PreprocessorList = list<PreprocessorPair>;

	Implementation(Type aType, const char* aSource) : type(aType), source(aSource) {}

	Type			type;
	std::string		source;
	PreprocessorList	preprocessorList;
};


OShaderObject::OShaderObject(Type aType, const char * aSource)
{
	_impl = new Implementation(aType, aSource);
	if (_impl == nullptr) throw OException("Error allocating implementation struct.");
}

OShaderObject::~OShaderObject()
{
	if (_shader != 0) glDeleteShader(_shader);
}

void OShaderObject::setSource(const char* source)
{
	_source = source;
}

void OShaderObject::addPreprocessorMacro(const char * aName, const char * aValue)
{
	_impl->preprocessorList.push_back(Implementation::PreprocessorPair(aName, aValue));
}

// ------------------------------------------------------------------------------------
// FROM HERE ON, EVERYTHING SHOULD BE REMOVED
// ------------------------------------------------------------------------------------
OShaderObject::OShaderObject(const char* shaderName, ShaderType shaderType, const char* source) :
	_shaderName(shaderName),
	_shaderType(shaderType),
	_source(source),
	_shader(0)
{
	
}

#ifdef WIN32
void OShaderObject::setSourceFromResource(int resourceId)
{
	const char *data = NULL;
	DWORD size = 0;

	HMODULE handle = ::LoadLibrary(OSIRIS_DLL_NAME);
	HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(resourceId), MAKEINTRESOURCE(OPENGL_SHADER));
	if (rc == NULL) {
		LPTSTR lastErrorText = NULL;
		DWORD lastError = ::GetLastError();
		::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
				0, lastError, 0, lastErrorText, MAX_PATH, 0);
		char errStr[256];
		snprintf(errStr, 256, "Failed to obtain shader as WIN32 resource [%d]: ERR#%d %ls", resourceId, 
			lastError, lastErrorText);
		::LocalFree(lastErrorText);

		throw OException(errStr);
	}
	HGLOBAL rcData = ::LoadResource(handle, rc);
	size = ::SizeofResource(handle, rc);
	data = static_cast<const char*>(::LockResource(rcData));

	_source.append(data, size);

	//delete[] data;
	::FreeLibrary(handle);
}
#endif

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

		string errMsg = "Shader compilation error [" + _shaderName + "]: " + strInfoLog;
		throw OException(errMsg.c_str());
	}

	return _shader;
}

GLuint OShaderObject::glReference() const
{
	return _shader;
}
