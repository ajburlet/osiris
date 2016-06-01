#include <glm/gtc/matrix_transform.hpp>

#include "OsirisSDK/OMatrixStack.h"

#define PI	3.1415f

using namespace std;

float toRad = 2 * PI / 360;

/**
 \brief Class constructor.
*/
OMatrixStack::OMatrixStack() :
	_currMtx(1.0f)
{
	push();
}

/**
 \brief Class destructor.
*/
OMatrixStack::~OMatrixStack()
{
}

/**
 \brief Pushes the current matrix on the stack.
*/
void OMatrixStack::push()
{
	_stack.push(_currMtx);
}

/**
 \brief Discards the matrix on top of the stack, and brings up the next one.
*/
void OMatrixStack::pop()
{
	_currMtx = _stack.top();
	_stack.pop();
}

/**
 \brief Retrieves the current (top) matrix.
*/
OMatrix4x4 OMatrixStack::top() const
{
	return _currMtx;
}
/**
 \see multiply(const OMatrixStack&)
*/
OMatrixStack & OMatrixStack::operator*=(const OMatrixStack & in)
{
	multiply(in);
	return *this;
}
/**
 \see multiply(const OMatrix4x4&)
*/
OMatrixStack & OMatrixStack::operator*=(const OMatrix4x4 & in)
{
	multiply(in);
	return *this;
}

/**
 \brief Multiply the top matrix by another stack top matrix.
 \param in Stack matrix that contains the top matrix that is going to be used in this operation.
*/
void OMatrixStack::multiply(const OMatrixStack & in)
{
	_currMtx *= in._currMtx;
}

/**
 \brief Multiply the top matrix by another matrix.
 \param in Right value matrix in this product.
*/
void OMatrixStack::multiply(const OMatrix4x4 & in)
{
	_currMtx *= in;
}

/**
 \brief Applies the translation transformation to a given direction.
 \param dir Displacement direction vector.
*/
void OMatrixStack::translate(const OVector3 & dir)
{
	translate(dir.x(), dir.y(), dir.z());
}

/**
 \brief Applies the translation transform to a given direction.
 \param dx Displacement direction on the X axis.
 \param dy Displacement direction on the Y axis.
 \param dz Displacement direction on the Z axis.
*/
void OMatrixStack::translate(const float & dx, const float & dy, const float & dz)
{
	OMatrix4x4 ret(1.0f);

	ret.setValue(0, 3, dx);
	ret.setValue(1, 3, dy);
	ret.setValue(2, 3, dz);

	_currMtx *= ret;
}

/**
 \brief Applies the rotation transformation around a given axis.
 \param axis Rotation axis vector.
 \param angle Rotation angle in degrees.
*/
void OMatrixStack::rotate(const OVector3 & axis, const float & angle)
{
	rotate(axis.x(), axis.y(), axis.z(), angle);
}

/**
 \brief Applies the rotation transformation around a given axis.
 \param axisX Rotation axis vector component on the X axis.
 \param axisY Rotation axis vector component on the Y axis.
 \param axisZ Rotation axis vector component on the Z axis.
 \param angle Rotation angle in degrees.
*/
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

/**
 \brief Applies the rotation transformation around the X axis.
 \param angle Rotation angle in degrees.
*/
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

/**
 \brief Applies the rotation transformation around the Y axis.
 \param angle Rotation angle in degrees.
*/
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

/**
 \brief Applies the rotation transformation around the Z axis.
 \param angle Rotation angle in degrees.
*/
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

/**
 \brief Applies the scale transformation, which re-scales the coordinate system by given factors
 in each component axis.
 \param factorVec Scale factors for each axis component.
*/
void OMatrixStack::scale(const OVector3 & factorVec)
{
	OMatrix4x4 ret(1.0f);

	ret.setValue(0, 0, factorVec.x());
	ret.setValue(1, 1, factorVec.y());
	ret.setValue(2, 2, factorVec.z());

	_currMtx *= ret;
}

/**
 \brief Applies the scale transformation by a uniform parameter for all space axes.
 \param uniformFactor Scale factor for all space axes.
*/
void OMatrixStack::scale(const float & uniformFactor)
{
	OVector3 vec(uniformFactor);
	scale(vec);
}

/**
 \brief Applies the perspective projection transformation.
 \param fieldOfViewDeg Angle of the camera's field of view in degrees.
 \param aspectRatio The aspect ration of the screen (width/height).
 \param zNear Nearest camera depth.
 \param zFar Farthest camera depth.
*/
void OMatrixStack::perspective(float fieldOfViewDeg, float aspectRatio, float zNear, float zFar)
{
	_currMtx *= glm::perspective(fieldOfViewDeg*toRad, aspectRatio, zNear, zFar);
}

/**
 \brief Applies the camera transformation, moving the coordinate system to account for the 
 current camera position and looking direction.
 \param position Current camera position coordinates.
 \param direction Camera looking direction vector.
 \param up Vertical reference vector (direction when the camera is facing "up").
*/
void OMatrixStack::camera(const OVector3 &position, const OVector3 &direction, const OVector3 &up)
{
	_currMtx *= glm::lookAt(position.glm(), direction.glm(), up.glm());
}

