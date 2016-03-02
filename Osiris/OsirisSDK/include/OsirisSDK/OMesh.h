#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GL/freeglut.h>

#include "OMeshBuffer.h"
#include "OMatrixStack.h"


class OMesh
{
public:
	OMesh();
	virtual ~OMesh();

	void render(const OMatrixStack& mtx);

protected:
	void addVertexData(const glm::vec3 &v);
	void addVertexData(float vx, float vy, float vz);

	void addIndexData(const glm::vec3 &v);
	void addIndexData(float vx, float vy, float vz);

private:
	GLuint vaoObject;
	
	OMeshBuffer<float> vertexBuffer;
	OMeshBuffer<GLuint> indexBuffer;
};

