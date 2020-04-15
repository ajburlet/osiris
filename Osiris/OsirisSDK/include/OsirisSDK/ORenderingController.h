#pragma once

class OGraphicsEngine;
class ORenderable;
class OMatrixStack;

/**
 @brief Intermediates interaction with the graphics engine.
 */
class ORenderingController
{
public:
	/**
	 @brief Class constructor.
	 @param aEngine The graphics engine.
	 */
	ORenderingController(OGraphicsEngine* aEngine);

	/**
	 @brief Class destructor.
	 */
	~ORenderingController();

	/**
	 @brief Loads vertices, textures and shaders to the GPU (if needed).
	 */
	virtual void load(ORenderable* aRenderable);

	/**
	 @brief Issues the appropriate render commands.
	 */
	virtual void render(ORenderable* aRenderable);

	/**
	 @brief Flushes all pending commands to the GPU.
	 */
	virtual void flush();

	/**
	 @brief Keeps a pointer to the pertinent MVP (model-view-projection) matrix.
	 */
	void setMatrixStack(OMatrixStack* aMatrixStack);

protected:
	struct Impl;
	Impl* _impl = nullptr;
};
