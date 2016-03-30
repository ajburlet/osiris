#pragma once

#include <string>
#include "GLdefs.h"

class OShaderObject
{
public:
	enum ShaderType {
		ShaderType_Vertex=GL_VERTEX_SHADER,
		ShaderType_Fragment=GL_FRAGMENT_SHADER
	};
	OShaderObject(const std::string& shaderName, ShaderType shaderType, const std::string& source="");
	virtual ~OShaderObject();

	void setSource(const std::string& source);
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

