#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GL/freeglut.h>

#include "OMeshBuffer.h"
#include "OMatrixStack.h"
#include "OShaderProgram.h"
#include "OCamera.h"


class OMesh
{
public:
	OMesh(OShaderProgram *program);
	virtual ~OMesh();

	void addVertexData(float vx, float vy, float vz);
	void addIndexData(GLuint vx, GLuint vy, GLuint vz);

	void init();
	void render(OCamera *cam, OMatrixStack *mtx);

protected:
	virtual void setupAdditionalVertexArrays();
	virtual void setupAdditionalShaderLocations();

private:
	GLuint _vaoObject;
	
	OMeshBuffer<float> _vertexBuffer;
	OMeshBuffer<GLuint> _indexBuffer;

	OShaderProgram* _program;
};

