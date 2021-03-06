#include <glm/gtc/matrix_transform.hpp>

#include "OsirisSDK/OMatrixStack.h"

#define PI	3.1415f

using namespace std;

float toRad = 2 * PI / 360;

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
	if (_stack.empty() == false) {
		_currMtx = _stack.top();
		_stack.pop();
	} else {
		_currMtx = OMatrix4x4(1);
	}
}

OMatrix4x4 OMatrixStack::top() const
{
	return _currMtx;
}

bool OMatrixStack::isEmpty() const
{
	return (_currMtx == OMatrix4x4(1) && _stack.empty());
}

void OMatrixStack::clear()
{
	while (!_stack.empty()) _stack.pop();
	_currMtx = OMatrix4x4(1);
}

OMatrixStack & OMatrixStack::operator=(const OMatrixStack & in)
 {
	_currMtx = in._currMtx;
	_stack = in._stack;
	return *this;
 }

OMatrixStack & OMatrixStack::operator*=(const OMatrixStack & in)
{
	product(in);
	return *this;
}

OMatrixStack & OMatrixStack::operator*=(const OMatrix4x4 & in)
{
	product(in);
	return *this;
}

OMatrixStack & OMatrixStack::operator*=(const OQuaternion & in)
{
	product(in);
	return *this;
}

OVector4 OMatrixStack::operator*(const OVector4 & in)
{
	return top() * in;
}

void OMatrixStack::product(const OMatrixStack & in)
{
	_currMtx *= in._currMtx;
}

void OMatrixStack::product(const OMatrix4x4 & in)
{
	_currMtx *= in;
}

void OMatrixStack::product(const OQuaternion & in)
{
	_currMtx *= in.toMatrix4();
}

void OMatrixStack::translate(const OVector3 & dir)
{
	translate(dir.x(), dir.y(), dir.z());
}

void OMatrixStack::translate(const float & dx, const float & dy, const float & dz)
{
	OMatrix4x4 ret(1.0f);

	ret.setValue(0, 3, dx);
	ret.setValue(1, 3, dy);
	ret.setValue(2, 3, dz);

	_currMtx *= ret;
}

void OMatrixStack::rotate(const OVector3 & axis, const float & angle)
{
	rotate(axis.x(), axis.y(), axis.z(), angle);
}

void OMatrixStack::rotate(const float & axisX, const float & axisY, const float & axisZ, const float & angle)
{
	float cosA = cosf(toRad*angle);
	float cosComp = 1 - cosA;
	float sinA = sinf(toRad*angle);
	float sinComp = 1 - sinA;

	OMatrix4x4 ret(1.0f);

	// 1st col
	ret.setValue(0, 0, axisX*axisX + (1 - axisX*axisX)*cosA);
	ret.setValue(1, 0, cosComp*axisX*axisY + axisZ*sinA);
	ret.setValue(2, 0, cosComp*axisX*axisZ - axisY*sinA);

	// 2nd col
	ret.setValue(0, 1, cosComp*axisX*axisY - axisZ*sinA);
	ret.setValue(1, 1, axisY*axisY + (1 - axisY*axisY)*cosA);
	ret.setValue(2, 1, cosComp*axisY*axisZ + axisX*sinA);

	// 3rd col
	ret.setValue(0, 2, cosComp*axisX*axisZ + axisY*sinA);
	ret.setValue(1, 2, cosComp*axisY*axisZ - axisX*sinA);
	ret.setValue(2, 2, axisZ*axisZ + (1 - axisZ*axisZ)*cosA);

	_currMtx *= ret;
}

void OMatrixStack::rotateX(const float & angle)
{
	OMatrix4x4 ret(1.0f);

	float sinA = sinf(toRad*angle);
	float cosA = cosf(toRad*angle);

	ret.setValue(1, 1, sinA);
	ret.setValue(2, 1, cosA);

	ret.setValue(1, 2, -sinA);
	ret.setValue(2, 2, cosA);

	_currMtx *= ret;
}

void OMatrixStack::rotateY(const float & angle)
{
	OMatrix4x4 ret(1.0f);

	float sinA = sinf(toRad*angle);
	float cosA = cosf(toRad*angle);

	ret.setValue(0, 0, cosA);
	ret.setValue(2, 0, -sinA);

	ret.setValue(0, 2, sinA);
	ret.setValue(2, 2, cosA);

	_currMtx *= ret;
}

void OMatrixStack::rotateZ(const float & angle)
{
	OMatrix4x4 ret(1.0f);

	float sinA = sinf(toRad*angle);
	float cosA = cosf(toRad*angle);

	ret.setValue(0, 0, cosA);
	ret.setValue(1, 0, sinA);

	ret.setValue(0, 1, -sinA);
	ret.setValue(1, 1, cosA);

	_currMtx *= ret;
}

void OMatrixStack::scale(const OVector3 & factorVec)
{
	OMatrix4x4 ret(1.0f);

	ret.setValue(0, 0, factorVec.x());
	ret.setValue(1, 1, factorVec.y());
	ret.setValue(2, 2, factorVec.z());

	_currMtx *= ret;
}

void OMatrixStack::scale(const float & uniformFactor)
{
	OVector3 vec(uniformFactor);
	scale(vec);
}

void OMatrixStack::perspective(float fieldOfViewDeg, float aspectRatio, float zNear, float zFar)
{
	_currMtx *= glm::perspective(fieldOfViewDeg*toRad, aspectRatio, zNear, zFar);
}

void OMatrixStack::camera(const OVector3 &position, const OVector3 &direction, const OVector3 &up)
{
	_currMtx *= glm::lookAt(position.glm(), direction.glm(), up.glm());
}

