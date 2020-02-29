#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"

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
	 @brief Loads mesh resources.
	 */
	virtual void load(const ORenderable* aMesh);

	/**
	 @brief Renders the mesh.
	 */
	virtual void render(const ORenderable* aMesh);


protected:


private:
	struct Implementation;
	Implementation* _impl;
};