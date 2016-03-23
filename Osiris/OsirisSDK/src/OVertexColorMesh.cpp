#include "OsirisSDK/OVertexColorMesh.h"


OVertexColorMesh::OVertexColorMesh() : OMesh()
{
}

OVertexColorMesh::~OVertexColorMesh()
{
}

void OVertexColorMesh::addVertexColorData(float r, float g, float b, float alpha)
{
	vertexBuffer()->addData(r, g, b, alpha);
}

void OVertexColorMesh::setupAdditionalVertexArrays()
{
	GLuint vertexColorOffset = (GLuint)vertexCount()*sizeof(float) * 3;

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, &vertexColorOffset);
}
