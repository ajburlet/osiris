#pragma once

#include <string>
#include <list>

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
	
	void addShader(OShaderObject* shader);
#ifdef WIN32
	void addShader(OShaderObject::ShaderType type, const std::string& name, int resourceId); 
#endif
	void compile();

private:
	std::string _programName;
	GLuint _program;

	std::list<OShaderObject*> _shaderList;

	GLuint _modelMtxGlRef;
	GLuint _perspectiveMtxGlRef;
	GLuint _cameraMtxGlRef;
};

