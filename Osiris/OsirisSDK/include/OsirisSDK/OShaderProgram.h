#pragma once

#include <string>
#include <list>

#include "GLdefs.h"
#include "defs.h"

#include "OMatrixStack.h"
#include "OCamera.h"

#include "OShaderObject.h"

#ifdef WIN32
#	pragma warning (disable : 4251) /* STL is restricted for class internal usage only */
#endif

class OAPI OShaderProgram
{
public:
	OShaderProgram(const char* name);
	virtual ~OShaderProgram();

	GLuint glReference() const;

	void setCamera(OCamera *camera);
	void setModelTransformation(const OMatrixStack* modelMtx);
	
	void addShader(OShaderObject* shader);
#ifdef WIN32
	void addShader(OShaderObject::ShaderType type, const char* name, int resourceId); 
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

