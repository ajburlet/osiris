#include "OsirisSDK\OMatrixStack.h"

using namespace std;

OMatrixStack::OMatrixStack() :
	currMtx(1.0f)
{
	push();
}

OMatrixStack::~OMatrixStack()
{
}

void OMatrixStack::push()
{
	_stack.push(currMtx);
}

void OMatrixStack::pop()
{
	_stack.pop();
	currMtx = _stack.top();
}
