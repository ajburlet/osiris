#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <stack>

class OMatrixStack
{
public:
	OMatrixStack();
	virtual ~OMatrixStack();

	void push();
	void pop();

	void translate(const glm::vec3 &dir);
	void translate(const float& dx, const float& dy, const float& dz);

	void rotate(const glm::vec3& axis, const float& angle);
	void rotate(const float& axisX, const float& axisY, const float& axisZ, const float& angle);
	void rotateX(const float& angle);
	void rotateY(const float& angle);
	void rotateZ(const float& angle);

	void scale(const glm::vec3& factorVec);
	void scale(const float& uniformFactor);

private:
	glm::mat4x4 _currMtx;
	std::stack<glm::mat4x4> _stack;
};

