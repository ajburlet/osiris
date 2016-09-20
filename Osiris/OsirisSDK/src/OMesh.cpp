#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMesh.h"

#include <stdio.h>

/**
 \brief Class constructor.
 \param program Pointer to the shader program that will be used to render the object.
*/
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

/**
 \brief Class destructor.
*/
OMesh::~OMesh()
{
}

/**
 \brief Sets a new shader program to be used to render the object.
 \param program Pointer to the new shader program.
*/
void OMesh::setProgram(OShaderProgram * program)
{
	_program = program;
}

/**
 \brief Returns the shader program used to render the object.
 \return Pointer to the shader program object.
*/
OShaderProgram * OMesh::getProgram()
{
	return _program;
}

/**
 \brief Returns the number of vertices.
*/
int OMesh::vertexCount() const
{
	return _vertexCount;
}

/**
 \brief Returns the number of faces.
 */
int OMesh::faceCount() const
{
	return _faceCount;
}

/**
 \brief Add a vertex.
 \param vx Vertex X axis component.
 \param vy Vertex Y axis component.
 \param vz Vertex Z axis component.
*/
void OMesh::addVertexData(float vx, float vy, float vz)
{
	_vertexBuffer.addData(vx, vy, vz);
	_vertexCount++;
}

/**
 \brief Define a single triangle by the vertex indices.
 \param vi First triangle vertex index.
 \param vj Second triangle vertex index.
 \param vk Third triangle vertex index.
*/
void OMesh::addIndexData(GLuint vi, GLuint vj, GLuint vk)
{
	_indexBuffer.addData(vi, vj, vk);
	_faceCount++;
}

/**
 \brief Access vertex data.
 \param idx Vertex index numbrt.
 \return OVector3 class object containing vector coordinates.
 */
OVector3 OMesh::vertexData(int idx) const
{
	const float *vb;
	if (idx >= vertexCount()) throw OException("Attempt to access invalid vertex item.");

	vb = vertexBufferConst()->buffer();

	return OVector3(vb[idx*3], vb[idx*3 + 1], vb[idx*3 + 2]);
}

/**
 \brief Access index information.
 \param idx Face index number.
 \return OVector3 class object containing face vertex index data. 
 */
OVector3 OMesh::indexData(int idx) const
{
	const GLuint* ib;
	if (idx >= faceCount()) throw OException("Attempt to access invalid face item.");

	ib = indexBufferConst()->buffer();

	return OVector3(ib[idx*3], ib[idx*3 + 1], ib[idx*3 + 2]);
}

/**
 \brief Initializes the mesh buffers and shader attributes.

 Must be called after all the vertex data is entered and before rendering.
*/
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

/**
 \brief Starts the rendering process for the object.
 \param mtx Pointer to the matrix stack that contains all the transformations.
*/
void OMesh::render(OMatrixStack *mtx)
{
	/* check if there is a shader program defined */
	if (_program == NULL) throw OException("Mesh defined without a shader program.");

	/* bind VAO */
	glBindVertexArray(_vaoObject);

	/* bind vertex buffer */
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.glReference()); 

	/* prepare program -- assign necessary transformations */
	glUseProgram(_program->glReference());
	_program->setMatrixTransform(mtx->top());
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

/**
 \brief Enables or disables face culling.

 Face culling is a feature designed to save perfomance. For three-dimentional and closed meshes,
 there is no need to render both sides of the triangles. By enabling face culling, only one face 
 the triangles that makes the shape will be rendered by the rasterizer.

 \param enabled Enable flag.
 \param face Face that will be rendered.
 \param front Defines which face is the front, by the order in which the vertices are set on the triangles.
*/
void OMesh::setFaceCulling(bool enabled, CullFace face, CullFront front)
{
	_cullEnabled = enabled;
	_cullFace = face;
	_cullFront = front;
}

/**
 \brief Virtual method to be used by derived classes to set additional vertex data.
*/
void OMesh::setupAdditionalVertexArrays()
{
}

/**
 \brief Virtual method to be used by derived classes to set additional shader parameters.
*/
void OMesh::setupAdditionalShaderLocations()
{

}

/**
 \brief Returns the mesh vertex buffer.
 \return Pointer to the mesh vertex buffer.
*/
OMeshBuffer<float>* OMesh::vertexBuffer()
{
	return &_vertexBuffer;
}

/**
 \brief Returns the mesh index buffer.
 \return Pointer to the mesh index buffer.
*/
OMeshBuffer<GLuint>* OMesh::indexBuffer()
{
	return &_indexBuffer;
}

/**
 \brief Read-only method to obtain the vertex buffer
 \return Const pointer to the mesh vertex buffer.
 */
const OMeshBuffer<float>* OMesh::vertexBufferConst() const
{
	return &_vertexBuffer;
}

/**
 \brief Read-only method to obtain the index buffer
 \return Const pointer to the mesh index buffer.
 */
const OMeshBuffer<GLuint>* OMesh::indexBufferConst() const
{
	return &_indexBuffer;
}

