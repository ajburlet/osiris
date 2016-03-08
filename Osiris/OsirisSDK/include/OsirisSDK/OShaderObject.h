#pragma once

#include <string>
#include <GL/freeglut.h>

class OShaderObject
{
public:
	OShaderObject(const std::string& shaderName, GLenum shaderType, const std::string& source="");
	virtual ~OShaderObject();

	void setSource(const std::string& source);

	GLuint compile();
	GLuint glReference() const;

private:
	std::string _shaderName;
	GLenum _shaderType;
	std::string _source;
	GLuint _shader;
};

