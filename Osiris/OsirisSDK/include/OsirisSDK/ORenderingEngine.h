#pragma once

class OGraphicsAPI;
class ORenderable;
class OMatrixStack;

/**
 @brief Intermediates interaction with the graphics engine.
 */
class ORenderingEngine
{
public:
	/**
	 @brief Class constructor.
	 @param aEngine The graphics engine.
	 */
	ORenderingEngine(OGraphicsAPI* aEngine);

	/**
	 @brief Class destructor.
	 */
	~ORenderingEngine();

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
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};
