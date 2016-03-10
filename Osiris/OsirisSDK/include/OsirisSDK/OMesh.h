#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GL/freeglut.h>

#include "OMeshBuffer.h"
#include "OMatrixStack.h"
#include "OShaderProgram.h"


class OMesh
{
public:
	OMesh();
	virtual ~OMesh();

	void addVertexData(float vx, float vy, float vz);
	void addIndexData(GLuint vx, GLuint vy, GLuint vz);

	void init();
	void render(const OMatrixStack& mtx);

protected:
	virtual void setupAdditionalVertexArrays();

private:
	GLuint _vaoObject;
	
	OMeshBuffer<float> _vertexBuffer;
	OMeshBuffer<GLuint> _indexBuffer;

	OShaderProgram* _program;
};

