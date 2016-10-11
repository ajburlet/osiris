#include "resource.h"

#include "OsirisSDK/OVertexColorMesh.h"

OShaderProgram* OVertexColorMesh::_colorVertexProgram = NULL;

/**
 \brief Class constructor.
*/
OVertexColorMesh::OVertexColorMesh() : OMesh()
{
	if (_colorVertexProgram == NULL) {
		_colorVertexProgram = new OShaderProgram("ColorVertex");
#ifdef WIN32
		_colorVertexProgram->addShader(OShaderObject::ShaderType_Vertex, "VertexSmoothColor", IDR_SHADER_VERTEX_SMOOTHCOLOR);
		_colorVertexProgram->addShader(OShaderObject::ShaderType_Fragment, "FragmentStandardColor", IDR_SHADER_FRAGMENT_STANDARDCOLOR);
#else
#error Embedded shader still needs to be addressed for non-Windows platforms.
#endif
		_colorVertexProgram->compile();
	}
	setProgram(_colorVertexProgram);
}

/**
 \brief Class destructor.
*/
OVertexColorMesh::~OVertexColorMesh()
{
}

/**
 \brief Add color definition values for each vertex. 
 
 The colors should be set after all vertex where defined.

 \param r Red component value.
 \param g Green component value.
 \param b Blue component value.
 \param alpha Alpha channel component value.
*/
void OVertexColorMesh::addVertexColorData(float r, float g, float b, float alpha)
{
	vertexBuffer()->addData(r, g, b, alpha);
}

void OVertexColorMesh::setupAdditionalVertexArrays()
{
	GLuint vertexColorOffset = (GLuint)vertexCount()*sizeof(float) * 3;

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)vertexColorOffset);
}
