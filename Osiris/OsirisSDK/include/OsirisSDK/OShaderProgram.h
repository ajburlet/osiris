#pragma once

#include <string>

#include "GLdefs.h"

#include "OMatrixStack.h"
#include "OCamera.h"

#include "OShaderObject.h"


class OShaderProgram
{
public:
	OShaderProgram(const std::string &name);
	virtual ~OShaderProgram();

	GLuint glReference() const;

	void setCamera(OCamera *camera);
	void setModelTransformation(const OMatrixStack* modelMtx);
	
	void addShader(const OShaderObject* shader);
	void compile();

private:
	std::string _programName;
	GLuint _program;

	GLuint _modelMtxGlRef;
	GLuint _perspectiveMtxGlRef;
	GLuint _cameraMtxGlRef;
};

