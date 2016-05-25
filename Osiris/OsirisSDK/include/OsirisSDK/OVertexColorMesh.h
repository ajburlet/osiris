#pragma once

#include "defs.h"

#include "OMesh.h"

/**
 \brief Mesh class in which colors can be defined for each vertex.

 The surface color of each triangle will be an interpolation between the values set
 for each vertex.
*/
class OAPI OVertexColorMesh : public OMesh
{
public:
	OVertexColorMesh();
	virtual ~OVertexColorMesh();

	void addVertexColorData(float r, float g, float b, float alpha);

protected:
	void setupAdditionalVertexArrays();

private:
	static OShaderProgram *_colorVertexProgram;
};

