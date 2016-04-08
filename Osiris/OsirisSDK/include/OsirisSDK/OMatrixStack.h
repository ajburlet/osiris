#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <stack>

#include "defs.h"
#include "OMath.h"

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
	void camera(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up = glm::vec3(0, 1, 0));

private:
	OMatrix4x4 _currMtx;
	std::stack<OMatrix4x4> _stack;
};

