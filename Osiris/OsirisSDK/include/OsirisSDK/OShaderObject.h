#pragma once

#include <string>

#include "GLdefs.h"
#include "defs.h"

#ifdef WIN32
#	pragma warning (disable : 4251) /* std::string is encapsulated inside the class */
#endif

/**
 \brief Class that represents a shader object that make up shader programs.
*/
class OAPI OShaderObject
{
public:
	/**
	 \brief Shader object type.
	*/
	enum ShaderType {
		ShaderType_Vertex=GL_VERTEX_SHADER,		/**< Vertex shader. */
		ShaderType_Fragment=GL_FRAGMENT_SHADER		/**< Fragment shader. */
	};

	/**
	 \brief Class constructor.
	 \param shaderName Shader name.
	 \param shaderType Shader type.
	 \param source Shader source code.
	*/
	OShaderObject(const char* shaderName, ShaderType shaderType, const char* source="");
	
	/**
	 \brief Class destructor.
	*/
	virtual ~OShaderObject();

	/**
	 \brief Set shader source code.
	*/
	void setSource(const char* source);

#ifdef WIN32
	/**
	 \brief Set shader source code from DLL resource (WIN32 only).
	 \param resourceId Visual Studio resource file ID.
	*/
	void setSourceFromResource(int resourceId);
#endif

	/**
	 \brief Compiles the shader object code.
	*/
	GLuint compile();
	
	/**
	 \brief Returns the OpenGL shader object reference ID.
	*/
	GLuint glReference() const;

private:
	std::string _shaderName;
	ShaderType _shaderType;
	std::string _source;
	GLuint _shader;
};

