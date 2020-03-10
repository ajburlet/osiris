#pragma once

#include <string>
#include <list>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OGPUObject.h"
#include "OsirisSDK/OShaderObject.h" /* replace by a forward declaration */

/* REPLACE BY A CONCEALED IMPLEMENTATION */
#ifdef WIN32
#	pragma warning (disable : 4251) /* STL is restricted for class internal usage only */
#endif

/**
 \brief Class that represents a shader program.
*/
class OAPI OShaderProgram : public OGPUObject
{
public:
	/**
	 \brief Class constructor.
	 \param name Shader name.
	*/
	OShaderProgram(const char* name);
	
	/**
	 \brief Class destructor.
	*/
	virtual ~OShaderProgram();

	// TODO: THIS MUST HARBOR the UNIFORM ATTRIBUTES REFERENCES

	/**
	 \brief Returns the OpenGL reference ID.
	*/
	GLuint glReference() const;

	/**
	 \brief Add a shader object to the program.
	 \param shader Pointer to the shader object that will be added.
	*/
	void addShader(OShaderObject* shader);

#ifdef WIN32
	/**
	 NO NEED FOR THIS
	
	 \brief Add a shader object to the program using the Visual Studio resource file ID (WIN32 only).
	 \param type Shader type.
	 \param name Shader name.
	 \param resourceId Visual Studio resource file ID that references the shader source code.
	*/
	void addShader(OShaderObject::ShaderType type, const char* name, int resourceId); 
#endif

	/**
	 \brief Get the shader uniform parameter by name.
	 \param uniform_name Name of the uniform parameter.
	 \return The OpenGL reference to the uniform.
	 */
	GLuint uniformLocation(const char* uniform_name);
	
	/**
	 \brief Get the shader attribute by name.
	 \param attrib_name Name of the shader attribute.
	 \return The OpenGL reference to the attribute.
	 */
	GLuint attribLocation(const char* attrib_name);

	/**
	 \brief Compiles shader objects and links the shader program.
	*/
	void compile();

	/**
	 \brief Tells OpenGL to use the program for the next rendering iteration.
	 */
	void use();

private:
	std::string _programName;
	GLuint _program;

	std::list<OShaderObject*> _shaderList;
};

