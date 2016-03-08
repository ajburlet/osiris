#pragma once

#include <gl/freeglut.h>
#include <string>

#include "OShaderObject.h"

class OShaderProgram
{
public:
	OShaderProgram(const std::string &name);
	virtual ~OShaderProgram();

	GLuint glReference() const;

	void addShader(const OShaderObject* shader);
	void compile();
private:
	std::string _programName;
	GLuint _program;
};

