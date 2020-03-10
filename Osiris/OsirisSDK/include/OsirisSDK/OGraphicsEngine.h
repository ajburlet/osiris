#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"

class OMatrixStack;
class OGraphicsCommandQueue;
class OShaderable;
class OShaderProgram;
class ORenderable;

class OGraphicsEngine 
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
	virtual ~OGraphicsEngine();

public:
	/**
	 @brief Creates a new command queue.
	 */
	virtual OGraphicsCommandQueue* createCommandQueue() = 0;

	/**
	 @brief Loads vertices, textures and shaders to the GPU (if needed).
	 */
	virtual void load(ORenderable* aRenderable);

	/**
	 @brief Issues the appropriate render commands.
	 */
	virtual void render(ORenderable* aRenderable);

	/**
	 @brief Keeps a pointer to the pertinent MVP (model-view-projection) matrix.
	 */
	void setMatrixStack(OMatrixStack* aMatrixStack);

protected:
	/**
	 @brief Compiles the shader object and links the program.
	 */
	virtual void compile(OShaderProgram* aProgram) = 0;

private:
	struct Implementation;
	Implementation* _impl;
};