#pragma once

#include <glm/mat4x4.hpp>
#include <stack>

class OMatrixStack
{
private:
	glm::mat4x4 currMtx;
	std::stack<glm::mat4x4> _stack;

public:
	OMatrixStack();
	virtual ~OMatrixStack();

	void push();
	void pop();
};

