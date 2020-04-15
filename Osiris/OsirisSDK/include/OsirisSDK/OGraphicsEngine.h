#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"

class OMatrixStack;
class OGraphicsCommandQueue;
class OShaderable;
class OShaderProgram;
class ORenderable;

/**
 @brief Graphics engine interface.
 */
class OAPI OGraphicsEngine 
{
public:
	/**
	 @brief Graphics engine type.
	 */
	enum class Type : uint8_t {
		OpenGL
	};

protected:
	/**
	 @brief Default class constructor.
	 */
	OGraphicsEngine(Type aType);

	/**
	 @brief Class destructor.
	 */
	virtual ~OGraphicsEngine() = default;

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

inline OGraphicsEngine::OGraphicsEngine(Type aType) : _type(aType)
{
}

inline OGraphicsEngine::Type OGraphicsEngine::type() const
{
	return _type;
}
