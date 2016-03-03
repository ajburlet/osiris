#include "OsirisSDK\OMatrixStack.h"

using namespace std;

OMatrixStack::OMatrixStack() :
	_currMtx(1.0f)
{
	push();
}

OMatrixStack::~OMatrixStack()
{
}

void OMatrixStack::push()
{
	_stack.push(_currMtx);
}

void OMatrixStack::pop()
{
	_stack.pop();
	_currMtx = _stack.top();
}

void OMatrixStack::translate(const glm::vec3 & dir)
{
	translate(dir.x, dir.y, dir.z);
}

void OMatrixStack::translate(const float & dx, const float & dy, const float & dz)
{
	glm::mat4x4 ret(1.0f);

	ret[3].x = dx;
	ret[3].y = dy;
	ret[3].z = dz;

	_currMtx *= ret;
}

void OMatrixStack::rotate(const glm::vec3 & axis, const float & angle)
{
	rotate(axis.x, axis.y, axis.z, angle);
}

void OMatrixStack::rotate(const float & axisX, const float & axisY, const float & axisZ, const float & angle)
{
	float cosA = cosf(angle);
	float cosComp = 1 - cosA;
	float sinA = sinf(angle);
	float sinComp = 1 - sinA;

	glm::mat4x4 ret(1.0f);

	// 1st col
	ret[0].x = axisX*axisX + (1 - axisX*axisX)*cosA;
	ret[0].y = cosComp*axisX*axisY + axisZ*sinA;
	ret[0].z = cosComp*axisX*axisZ - axisY*sinA;

	// 2nd col
	ret[1].x = cosComp*axisX*axisY - axisZ*sinA;
	ret[1].y = axisY*axisY + (1 - axisY*axisY)*cosA;
	ret[1].z = cosComp*axisY*axisZ + axisX*sinA;

	// 3rd col
	ret[2].x = cosComp*axisX*axisZ + axisY*sinA;
	ret[2].y = cosComp*axisY*axisZ - axisX*sinA;
	ret[2].z = axisZ*axisZ + (1 - axisZ*axisZ)*cosA;

	_currMtx *= ret;
}

void OMatrixStack::rotateX(const float & angle)
{
	glm::mat4x4 ret(1.0f);

	float sinA = sinf(angle);
	float cosA = cosf(angle);

	ret[1].y = sinA;
	ret[1].z = cosA;

	ret[1].y = -sinA;
	ret[1].z = -cosA;

	_currMtx *= ret;
}

void OMatrixStack::rotateY(const float & angle)
{
	glm::mat4x4 ret(1.0f);

	float sinA = sinf(angle);
	float cosA = cosf(angle);
	
	ret[0].x = cosA;
	ret[0].z = -sinA;

	ret[2].x = sinA;
	ret[2].z = cosA;

	_currMtx *= ret;
}

void OMatrixStack::rotateZ(const float & angle)
{
	glm::mat4x4 ret(1.0f);

	float sinA = sinf(angle);
	float cosA = cosf(angle);

	ret[0].x = cosA;
	ret[0].y = sinA;

	ret[1].x = -sinA;
	ret[1].y = cosA;

	_currMtx *= ret;
}

void OMatrixStack::scale(const glm::vec3 & factorVec)
{
	glm::mat4x4 ret(1.0f);

	ret[0].x = factorVec.x;
	ret[1].y = factorVec.y;
	ret[2].z = factorVec.z;

	_currMtx *= ret;
}

void OMatrixStack::scale(const float & uniformFactor)
{
	glm::vec3 vec(1.0f);
	scale(vec);
}

