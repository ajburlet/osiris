#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"

class OMatrixStack;
class OGraphicsCommandQueue;
class OShaderProgram;

/**
 @brief Graphics engine interface.
 */
class OAPI OGraphicsAPI 
{
public:
	/**
	 @brief Graphics engine type.
	 */
	enum class Type {
		OpenGL
	};

protected:
	/**
	 @brief Default class constructor.
	 */
	OGraphicsAPI(Type aType);

	/**
	 @brief Class destructor.
	 */
	virtual ~OGraphicsAPI() = default;

public:
	/**
	 @brief Returns the graphics engine type.
	 */
	Type type() const;

	/**
	 @brief Creates a new command queue.
	 */
	virtual OGraphicsCommandQueue* createCommandQueue() = 0;

	/**
	 @brief Compiles the shader object and links the program.
	 */
	virtual void compile(OShaderProgram* aProgram) = 0;

private:
	Type _type;
};

inline OGraphicsAPI::OGraphicsAPI(Type aType) : _type(aType)
{
}

inline OGraphicsAPI::Type OGraphicsAPI::type() const
{
	return _type;
}
