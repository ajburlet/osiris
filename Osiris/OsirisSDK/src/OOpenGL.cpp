#include <glload/gl_load.hpp>

#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OOpenGLCommandQueue.h"
#include "OsirisSDK/OOpenGL.h"

OOpenGL::OOpenGL() : OGraphicsAPI(Type::OpenGL)
{
	glload::LoadFunctions();
	if (!glload::IsVersionGEQ(OSIRIS_GL_VERSION)) {
		throw OException("Incorrect OpenGL version.");
	}
	
	/* z-buffer -- TODO: this must be set on the rendering encoders somehow!! */
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);
}

OGraphicsCommandQueue * OOpenGL::createCommandQueue()
{
	return new OOpenGLCommandQueue;
}

void OOpenGL::compile(OShaderProgram * aProgram)
{
	// TODO
}
