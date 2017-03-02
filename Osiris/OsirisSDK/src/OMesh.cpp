#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMesh.h"

#include <stdio.h>

OMesh::OMesh(OShaderProgram *program) :
	_vaoObject(0),
	_vertexCount(0),
	_faceCount(0),
	_program(program),
	_cullEnabled(false),
	_cullFace(CullFace_Undefined),
	_cullFront(CullFront_Undefined)
{
}

OMesh::~OMesh()
{
}

void OMesh::setProgram(OShaderProgram * program)
{
	_program = program;
}

OShaderProgram * OMesh::getProgram()
{
	return _program;
}

int OMesh::vertexCount() const
{
	return _vertexCount;
}

int OMesh::faceCount() const
{
	return _faceCount;
}

void OMesh::addVertexData(float vx, float vy, float vz)
{
	_vertexBuffer.addData(vx, vy, vz);
	_vertexCount++;
}

void OMesh::addIndexData(GLuint vi, GLuint vj, GLuint vk)
{
	_indexBuffer.addData(vi, vj, vk);
	_faceCount++;
}

OVector3 OMesh::vertexData(int idx) const
{
	const float *vb;
	if (idx >= vertexCount()) throw OException("Attempt to access invalid vertex item.");

	vb = vertexBufferConst()->buffer();

	return OVector3(vb[idx*3], vb[idx*3 + 1], vb[idx*3 + 2]);
}

OVector3 OMesh::indexData(int idx) const
{
	const GLuint* ib;
	if (idx >= faceCount()) throw OException("Attempt to access invalid face item.");

	ib = indexBufferConst()->buffer();

	return OVector3((float)ib[idx*3], (float)ib[idx*3 + 1], (float)ib[idx*3 + 2]);
}

void OMesh::init()
{
	GLuint vertexArray;
	GLuint indexArray;

	/* init & bind VAO */
	glGenVertexArrays(1, &_vaoObject);
	glBindVertexArray(_vaoObject);
	
	/* setup and bind vertex/index array */
	vertexArray = _vertexBuffer.generateGLBufferObject(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray);
	indexArray = _indexBuffer.generateGLBufferObject(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);

	/* setup attribute array -- for use inside sharers */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	setupAdditionalVertexArrays();

	/* unbind VAO & buffer object */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OMesh::render(OMatrixStack *mtx)
{
	/* check if there is a shader program defined */
	if (_program == NULL) throw OException("Mesh defined without a shader program.");

	/* bind VAO */
	glBindVertexArray(_vaoObject);

	/* bind vertex buffer */
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.glReference()); 

	/* prepare program -- assign necessary transformations */
	_program->use();
	glUniformMatrix4fv(_program->uniformLocation("transformMtx"), 1, GL_FALSE, mtx->top().glArea());
	setupAdditionalShaderLocations();

	/* face culling */
	if (_cullEnabled) {
		if (_cullFace == CullFace_Undefined || _cullFront == CullFront_Undefined) {
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(0);
			throw OException("Invalid face culling configuration");
		}
		glEnable(GL_CULL_FACE);
		glCullFace(_cullFace);
		glFrontFace(_cullFront);
	} else {
		glDisable(GL_CULL_FACE);
	}

	/* draw */
	glDrawElements(GL_TRIANGLES, _indexBuffer.count(), GL_UNSIGNED_INT, 0);

	/* unbind everything */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void OMesh::setFaceCulling(bool enabled, CullFace face, CullFront front)
{
	_cullEnabled = enabled;
	_cullFace = face;
	_cullFront = front;
}

void OMesh::setupAdditionalVertexArrays()
{
}

void OMesh::setupAdditionalShaderLocations()
{

}

OMeshBuffer<float>* OMesh::vertexBuffer()
{
	return &_vertexBuffer;
}

OMeshBuffer<GLuint>* OMesh::indexBuffer()
{
	return &_indexBuffer;
}

const OMeshBuffer<float>* OMesh::vertexBufferConst() const
{
	return &_vertexBuffer;
}

const OMeshBuffer<GLuint>* OMesh::indexBufferConst() const
{
	return &_indexBuffer;
}

