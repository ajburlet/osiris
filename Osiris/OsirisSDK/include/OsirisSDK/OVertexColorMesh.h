#pragma once

#include "OMesh.h"

class OVertexColorMesh : public OMesh
{
public:
	OVertexColorMesh();
	virtual ~OVertexColorMesh();

	void addVertexColorData(float r, float g, float b, float alpha);

protected:
	void setupAdditionalVertexArrays();

};

