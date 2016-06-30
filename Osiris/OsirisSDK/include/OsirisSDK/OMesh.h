#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "GLdefs.h"
#include "defs.h"

#include "OMeshBuffer.hpp"
#include "OMatrixStack.h"
#include "OShaderProgram.h"
#include "OCamera.h"

/**
 \brief Base class that represents a group of vertices that together make a geometrical shape.

 Meshes are first defined by entering vertex data and indices. The mesh object can then be initialized
 and then redered.
*/
class OAPI OMesh
{
public:
	OMesh(OShaderProgram *program=NULL);
	virtual ~OMesh();

	void setProgram(OShaderProgram *program);
	OShaderProgram* getProgram();

	int vertexCount() const;

	void addVertexData(float vx, float vy, float vz);
	void addIndexData(GLuint vi, GLuint vj, GLuint vk);

	void init();
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

