#include "OsirisSDK\OMesh.h"


OMesh::OMesh() :
	vaoObject(0)
{
}

OMesh::~OMesh()
{
}

void OMesh::render(const OMatrixStack &mtx)
{

}

void OMesh::addVertexData(const glm::vec3 & v)
{
	addVertexData(v.x, v.y, v.z);
}

void OMesh::addVertexData(float vx, float vy, float vz)
{

}

void OMesh::addIndexData(const glm::vec3 & v)
{
	addIndexData(v.x, v.y, v.z);
}

void OMesh::addIndexData(float vx, float vy, float vz)
{
}
