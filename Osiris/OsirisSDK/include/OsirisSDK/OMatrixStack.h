#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <stack>

#include "defs.h"
#include "OMath.h"

/**
 \brief Matrix stack class.

 This class represents a stack of matrices, useful for handling coordinate system transformations on
 multiple levels. There is a top matrix on the stack, where transformations can be applied. When the need
 comes to preserve the top matrix, it can be pushed onto the stack, so it can be retrieved at a later time
 by using the pop() method.
*/
class OAPI OMatrixStack
{
public:
	OMatrixStack();
	virtual ~OMatrixStack();

	void push();
	void pop();
	OMatrix4x4 top() const;

	void translate(const OVector3 &dir);
	void translate(const float& dx, const float& dy, const float& dz);

	void rotate(const OVector3 & axis, const float& angle);
	void rotate(const float& axisX, const float& axisY, const float& axisZ, const float& angle);
	void rotateX(const float& angle);
	void rotateY(const float& angle);
	void rotateZ(const float& angle);

	void scale(const OVector3& factorVec);
	void scale(const float& uniformFactor);

	void perspective(float fieldOfViewDeg, float aspectRatio, float zNear, float zFar);
	void camera(const OVector3 &position, const OVector3 &direction, const OVector3 &up = OVector3(0, 1, 0));

private:
	OMatrix4x4 _currMtx;
	std::stack<OMatrix4x4> _stack;
};

