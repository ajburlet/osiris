#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "GLdefs.h"
#include "defs.h"

#include "OMeshBuffer.hpp"
#include "OMatrixStack.h"
#include "OShaderProgram.h"
#include "OCamera.h"

class OAPI OMesh
{
public:
	OMesh(OShaderProgram *program=NULL);
	virtual ~OMesh();

	void setProgram(OShaderProgram *program);
	OShaderProgram* getProgram();

	int vertexCount() const;

	void addVertexData(float vx, float vy, float vz);
	void addIndexData(GLuint vx, GLuint vy, GLuint vz);

	void init();
	void render(OCamera *cam, OMatrixStack *mtx);

	enum CullFace {
		CullFace_Undefined=-1,
		CullFace_Front=GL_FRONT,
		CullFace_Back=GL_BACK
	};

	enum CullFront {
		CullFront_Undefined=-1,
		CullFront_CW=GL_CW,
		CullFront_CCW=GL_CCW
	};

	void setFaceCulling(bool enabled, CullFace face=CullFace_Undefined, CullFront front=CullFront_Undefined);

protected:
	virtual void setupAdditionalVertexArrays();
	virtual void setupAdditionalShaderLocations();

	OMeshBuffer<float>* vertexBuffer();
	OMeshBuffer<GLuint>* indexBuffer();

private:
	GLuint _vaoObject;
	
	int _vertexCount;

	OMeshBuffer<float> _vertexBuffer;
	OMeshBuffer<GLuint> _indexBuffer;

	OShaderProgram* _program;

	bool _cullEnabled;
	CullFace _cullFace;
	CullFront _cullFront;
};

