#include <glload/gl_3_3.h>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMesh.h"


OMesh::OMesh() :
	_vaoObject(0),
	_program(NULL)
{
}

OMesh::~OMesh()
{
}

void OMesh::addVertexData(float vx, float vy, float vz)
{
	_vertexBuffer.addData(vx, vy, vz);
}

void OMesh::addIndexData(GLuint vx, GLuint vy, GLuint vz)
{
	_indexBuffer.addData(vx, vy, vz);
}

void OMesh::init()
{
	GLuint vertexArray;
	GLuint indexArray;

	/* init & bind VAO */
	glGenVertexArrays(1, &_vaoObject);
	glBindVertexArray(_vaoObject);
	
	/* setup and bind vertex/index array */
	vertexArray = _vertexBuffer.generateGLBufferObject(GL_VERTEX_ARRAY);
	glBindBuffer(GL_VERTEX_ARRAY, vertexArray);
	indexArray = _indexBuffer.generateGLBufferObject(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	
	/* setup attribute array -- for use inside sharers */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	setupAdditionalVertexArrays();

	/* unbind VAO */
	glBindVertexArray(0);
}

void OMesh::render(const OMatrixStack &mtx)
{
	/* check if there is a shader program defined */
	if (_program == NULL) throw OException("Mesh defined without a shader program.");

	/* bind VAO */
	glBindVertexArray(_vaoObject);

	/* bind vertex buffer and program */
	glBindBuffer(GL_VERTEX_ARRAY, _vertexBuffer.glReference()); 
	glUseProgram(_program->glReference());

	/* draw */
	glDrawElements(GL_TRIANGLES, _indexBuffer.count(), GL_UNSIGNED_SHORT, 0);

	/* unbind everything */
	glBindBuffer(GL_VERTEX_ARRAY, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	
}

void OMesh::setupAdditionalVertexArrays()
{
}
