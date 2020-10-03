#include <string>

#include <glload/gl_load.hpp>

#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderObject.h"
#include "OsirisSDK/OShaderProgram.h"
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
	auto GetGlShaderType = [&](OShaderObject* aObject) -> GLuint {
		switch (aObject->type()) {
		case OShaderObject::Type::Vertex:	return GL_VERTEX_SHADER;
		case OShaderObject::Type::Fragment:	return GL_FRAGMENT_SHADER;
		case OShaderObject::Type::Compute:	return GL_COMPUTE_SHADER;
		}
		return 0;
	};

	auto programHandle = new GLuint(glCreateProgram());
	OExceptionPointerCheck(programHandle);
	aProgram->setGpuHandle(programHandle);

	aProgram->ForEachObject([&](OShaderObject* aObj) {
		auto objHandle = new GLuint(glCreateShader(GetGlShaderType(aObj)));
		OExceptionPointerCheck(objHandle);
		aObj->setGpuHandle(objHandle);

		std::string source;
		aObj->ForEachPreprocessorMacro([&](const char* aKey, const char* aValue) {
			source += "#define " + std::string(aKey) + " " + aValue + "\n";
		});
		source += std::string(aObj->source());

		auto src = source.c_str();
		glShaderSource(*objHandle, 1, &src, nullptr);
		glCompileShader(*objHandle);

		// check compile status
		GLint status;
		glGetShaderiv(*objHandle, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint infoLogLength;
			glGetShaderiv(*objHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *infoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(*objHandle, infoLogLength, NULL, infoLog);

			std::string strInfoLog(infoLog);
			delete[] infoLog;

			std::string errMsg = "Shader compilation error: " + strInfoLog;
			throw OException(errMsg.c_str());
		}

		glAttachShader(*programHandle, *objHandle);
	});
	
	glLinkProgram(*programHandle);

	// check link status
	GLint status;
	glGetProgramiv(*programHandle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(*programHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(*programHandle, infoLogLength, NULL, infoLog);

		std::string strInfoLog(infoLog);
		delete[] infoLog;

		std::string errMsg = "Shader link error: " + strInfoLog;
		throw OException(errMsg.c_str());
	}
}
