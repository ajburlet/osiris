#pragma once

#include <string>

#include "GLdefs.h"
#include "defs.h"

#ifdef WIN32
#	pragma warning (disable : 4251) /* std::string is encapsulated inside the class */
#endif


class OAPI OShaderObject
{
public:
	enum ShaderType {
		ShaderType_Vertex=GL_VERTEX_SHADER,
		ShaderType_Fragment=GL_FRAGMENT_SHADER
	};
	OShaderObject(const char* shaderName, ShaderType shaderType, const char* source="");
	virtual ~OShaderObject();

	void setSource(const char* source);
#ifdef WIN32
	void setSourceFromResource(int resourceId);
#endif

	GLuint compile();
	GLuint glReference() const;

private:
	std::string _shaderName;
	ShaderType _shaderType;
	std::string _source;
	GLuint _shader;
};

