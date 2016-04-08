#pragma once

#include "defs.h"

#include "OMesh.h"

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

