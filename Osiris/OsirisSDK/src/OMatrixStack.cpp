#include <glm/gtc/matrix_transform.hpp>

#include <stack>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMatrix.hpp"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OQuaternion.hpp"
#include "OsirisSDK/OMatrixStack.h"

#define PI	3.1415f

using namespace std;

float toRad = 2 * PI / 360;


struct OMatrixStack::Impl {
	Impl() : currMtx(1.0f) {}
	OMatrix4x4F currMtx;
	std::stack<OMatrix4x4F> stack;
};

OMatrixStack::OMatrixStack()
{
	OExceptionPointerCheck(_impl = new Impl);
	push();
}

OMatrixStack::~OMatrixStack()
{
	if (_impl != nullptr) delete _impl;
}

void OMatrixStack::push()
{
	_impl->stack.push(_impl->currMtx);
}

void OMatrixStack::pop()
{
	if (_impl->stack.empty() == false) {
		_impl->currMtx = _impl->stack.top();
		_impl->stack.pop();
	} else {
		_impl->currMtx = OMatrix4x4F(1);
	}
}

OMatrix4x4F& OMatrixStack::top() const
{
	return _impl->currMtx;
}

bool OMatrixStack::isEmpty() const
{
	return (_impl->currMtx == OMatrix4x4F(1) && _impl->stack.empty());
}

void OMatrixStack::clear()
{
	while (!_impl->stack.empty()) _impl->stack.pop();
	_impl->currMtx = OMatrix4x4F(1);
}

OMatrixStack & OMatrixStack::operator=(const OMatrixStack & in)
 {
	_impl->currMtx = in._impl->currMtx;
	_impl->stack = in._impl->stack;
	return *this;
 }

OMatrixStack & OMatrixStack::operator*=(const OMatrixStack & in)
{
	product(in);
	return *this;
}

OMatrixStack & OMatrixStack::operator*=(const OMatrix4x4F & in)
{
	product(in);
	return *this;
}

OMatrixStack & OMatrixStack::operator*=(const OQuaternion & in)
{
	product(in);
	return *this;
}

void OMatrixStack::product(const OMatrixStack & in)
{
	_impl->currMtx *= in._impl->currMtx;
}

void OMatrixStack::product(const OMatrix4x4F & in)
{
	_impl->currMtx *= in;
}

void OMatrixStack::product(const OQuaternion & in)
{
	_impl->currMtx *= in.toMatrix4();
}

void OMatrixStack::translate(const OVector3F & dir)
{
	translate(dir.x(), dir.y(), dir.z());
}

void OMatrixStack::translate(const float & dx, const float & dy, const float & dz)
{
	OMatrix4x4F ret(1.0f);

	ret.setValue(0, 3, dx);
	ret.setValue(1, 3, dy);
	ret.setValue(2, 3, dz);

	_impl->currMtx *= ret;
}

void OMatrixStack::rotate(const OVector3F & axis, const float & angle)
{
	rotate(axis.x(), axis.y(), axis.z(), angle);
}

void OMatrixStack::rotate(const float & axisX, const float & axisY, const float & axisZ, const float & angle)
{
	float cosA = cosf(toRad*angle);
	float cosComp = 1 - cosA;
	float sinA = sinf(toRad*angle);
	float sinComp = 1 - sinA;

	OMatrix4x4F ret(1.0f);

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

	_impl->currMtx *= ret;
}

void OMatrixStack::rotateX(const float & angle)
{
	OMatrix4x4F ret(1.0f);

	float sinA = sinf(toRad*angle);
	float cosA = cosf(toRad*angle);

	ret.setValue(1, 1, sinA);
	ret.setValue(2, 1, cosA);

	ret.setValue(1, 2, -sinA);
	ret.setValue(2, 2, cosA);

	_impl->currMtx *= ret;
}

void OMatrixStack::rotateY(const float & angle)
{
	OMatrix4x4F ret(1.0f);

	float sinA = sinf(toRad*angle);
	float cosA = cosf(toRad*angle);

	ret.setValue(0, 0, cosA);
	ret.setValue(2, 0, -sinA);

	ret.setValue(0, 2, sinA);
	ret.setValue(2, 2, cosA);

	_impl->currMtx *= ret;
}

void OMatrixStack::rotateZ(const float & angle)
{
	OMatrix4x4F ret(1.0f);

	float sinA = sinf(toRad*angle);
	float cosA = cosf(toRad*angle);

	ret.setValue(0, 0, cosA);
	ret.setValue(1, 0, sinA);

	ret.setValue(0, 1, -sinA);
	ret.setValue(1, 1, cosA);

	_impl->currMtx *= ret;
}

void OMatrixStack::scale(const OVector3F & factorVec)
{
	OMatrix4x4F ret(1.0f);

	ret.setValue(0, 0, factorVec.x());
	ret.setValue(1, 1, factorVec.y());
	ret.setValue(2, 2, factorVec.z());

	_impl->currMtx *= ret;
}

void OMatrixStack::scale(const float & uniformFactor)
{
	OVector3F vec(uniformFactor);
	scale(vec);
}

void OMatrixStack::perspective(float fieldOfViewDeg, float aspectRatio, float zNear, float zFar)
{
	_impl->currMtx *= glm::perspective(fieldOfViewDeg*toRad, aspectRatio, zNear, zFar);
}

void OMatrixStack::camera(const OVector3F &position, const OVector3F &direction, const OVector3F &up)
{
	_impl->currMtx *= glm::lookAt(position.glm(), direction.glm(), up.glm());
}

void OMatrixStack::camera(const OVector3F & position, const OVector3F & direction)
{
	camera(position, direction, OVector3F(0, 1, 0));
}

