
#include "OsirisSDK/OOpenGLCommandQueue.h"
#include "OsirisSDK/OOpenGL.h"

OGraphicsCommandQueue * OOpenGL::createCommandQueue()
{
	return new OOpenGLCommandQueue;
}

void OOpenGL::compile(OShaderProgram * aProgram)
{
	// TODO
}
