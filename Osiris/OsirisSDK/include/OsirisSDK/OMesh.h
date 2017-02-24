#pragma once

#include "GLdefs.h"
#include "defs.h"

#include "OMeshBuffer.hpp"
#include "OMatrixStack.h"
#include "OShaderProgram.h"
#include "OCamera.h"
#include "OMath.h"

/**
 \brief Base class that represents a group of vertices that together make a geometrical shape.

 Meshes are first defined by entering vertex data and indices. The mesh object can then be initialized
 and then redered.
*/
class OAPI OMesh
{
public:
	/**
	 \brief Class constructor.
	 \param program Pointer to the shader program that will be used to render the object.
	*/
	OMesh(OShaderProgram *program=NULL);

	/**
	 \brief Class destructor.
	*/
	virtual ~OMesh();

	/**
	 \brief Sets a new shader program to be used to render the object.
			glTexImage2D()
	 \param program Pointer to the new shader program.
	*/
	void setProgram(OShaderProgram *program);

	/**
	 \brief Returns the shader program used to render the object.
	 \return Pointer to the shader program object.
	*/
	OShaderProgram* getProgram();

	/**
	 \brief Returns the number of vertices.
	*/
	int vertexCount() const;

	/**
	 \brief Returns the number of faces.
	 */
	int faceCount() const;

	/**
	 \brief Add a vertex.
	 \param vx Vertex X axis component.
	 \param vy Vertex Y axis component.
	 \param vz Vertex Z axis component.
	*/
	void addVertexData(float vx, float vy, float vz);

	/**
	 \brief Define a single triangle by the vertex indices.
	 \param vi First triangle vertex index.
	 \param vj Second triangle vertex index.
	 \param vk Third triangle vertex index.
	*/
	void addIndexData(GLuint vi, GLuint vj, GLuint vk);

	/**
	 \brief Access vertex data.
	 \param idx Vertex index numbrt.
	 \return OVector3 class object containing vector coordinates.
	 */
	OVector3 vertexData(int idx) const;

	/**
	 \brief Access index information.
	 \param idx Face index number.
	 \return OVector3 class object containing face vertex index data. 
	 */
	OVector3 indexData(int idx) const;

	/**
	 \brief Initializes the mesh buffers and shader attributes.

	 Must be called after all the vertex data is entered and before rendering.
	*/
	void init();

	/**
	 \brief Starts the rendering process for the object.
	 \param mtx Pointer to the matrix stack that contains all the transformations.
	*/
	void render(OMatrixStack *mtx);

	/**
	 \brief Sets which face will be rendered when face culling is available.
	 \see setFaceCulling()
	*/
	enum CullFace {
		CullFace_Undefined=-1,		/**< Undefined face. */
		CullFace_Front=GL_FRONT,	/**< Front face. */
		CullFace_Back=GL_BACK		/**< Back face. */
	};

	/**
	 \brief In order to be able to ascertain which face is the front when dealing with
	 face culling, we use the order in which the vertices are laid on each triangle.
	 \see setFaceCulling()
	*/
	enum CullFront {
		CullFront_Undefined=-1,		/**< Undefined front face. */
		CullFront_CW=GL_CW,		/**< Front face is defined by clockwise vertex order. */
		CullFront_CCW=GL_CCW		/**< Front face is defined by counter-clockwise vertex order. */
	};

	/**
	 \brief Enables or disables face culling.

	 Face culling is a feature designed to save perfomance. For three-dimentional and closed meshes,
	 there is no need to render both sides of the triangles. By enabling face culling, only one face 
	 the triangles that makes the shape will be rendered by the rasterizer.

	 \param enabled Enable flag.
	 \param face Face that will be rendered.
	 \param front Defines which face is the front, by the order in which the vertices are set on the triangles.
	*/
	void setFaceCulling(bool enabled, CullFace face=CullFace_Undefined, CullFront front=CullFront_Undefined);

protected:
	/**
	 \brief Virtual method to be used by derived classes to set additional vertex data.
	*/
	virtual void setupAdditionalVertexArrays();

	/**
	 \brief Virtual method to be used by derived classes to set additional shader parameters.
	*/
	virtual void setupAdditionalShaderLocations();

	/**
	 \brief Returns the mesh vertex buffer.
	 \return Pointer to the mesh vertex buffer.
	*/
	OMeshBuffer<float>* vertexBuffer();

	/**
	 \brief Returns the mesh index buffer.
	 \return Pointer to the mesh index buffer.
	*/
	OMeshBuffer<GLuint>* indexBuffer();

	/**
	 \brief Read-only method to obtain the vertex buffer
	 \return Const pointer to the mesh vertex buffer.
	 */
	const OMeshBuffer<float>* vertexBufferConst() const;

	/**
	 \brief Read-only method to obtain the index buffer
	 \return Const pointer to the mesh index buffer.
	 */
	const OMeshBuffer<GLuint>* indexBufferConst() const;

private:
	GLuint _vaoObject;
	
	int _vertexCount;
	int _faceCount;

	OMeshBuffer<float> _vertexBuffer;
	OMeshBuffer<GLuint> _indexBuffer;

	OShaderProgram* _program;
	GLuint _transformMtxRef;

	bool _cullEnabled;
	CullFace _cullFace;
	CullFront _cullFront;
};

