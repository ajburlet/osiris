#pragma once

class OGraphicsAPI;
class ORenderable;
class OMatrixStack;
class ORenderComponents;
class OTrashBin;

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
	 @brief Unloads vertices and textures from the GPU.
	 @param aRenderable The renderable object containing the GPU resources to be unloaded.
	 */
	virtual void unload(ORenderable* aRenderable);

	/**
	 @brief Unloads vertices and textures from the GPU.
	 @param aRenderComponents The rendering components containing the GPU resources to be unloaded.
	 */
	virtual void unload(ORenderComponents* aRenderComponents);

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

	/**
	 @brief The trash bean for rendering-related objects.
	 */
	OTrashBin& trashBin();

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
