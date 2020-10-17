#pragma once

class OGraphicsAPI;
class ORenderable;
class ORenderComponents;
class OVertexBuffer;
class OIndexBuffer;
class OTexture;
class OTrashBin;

/**
 @brief Intermediates interaction with the graphics engine.
 */
class ORenderingEngine
{
public:
	/**
	 @brief Class constructor.
	 @param aGraphicsAPI The graphics API.
	 */
	ORenderingEngine(OGraphicsAPI* aGraphicsAPI);

	/**
	 @brief Class destructor.
	 */
	~ORenderingEngine();

	/**
	 @brief Loads vertices, textures and shaders to the GPU (if needed).
	 */
	virtual void load(ORenderable* aRenderable);

	/**
	 @brief Loads vertices, textures and shaders to the GPU (if needed).
	 */
	virtual void load(ORenderComponents* aRenderComponents);

	/**
	 @brief Loads vertex buffer.
	 */
	virtual void load(OVertexBuffer* aVertexBuffer);

	/**
	 @brief Loads index buffer.
	 */
	virtual void load(OIndexBuffer* aIndexBuffer);

	/**
	 @brief Loads texture.
	 */
	virtual void load(OTexture* aTexture);

	/**
	 @brief Unloads vertices and textures from the GPU.
	 @param aRenderable The renderable object to be unloaded.
	 @param aUnloadAll Unloads all rendering resources (buffers, textures, etc.).
	 */
	virtual void unload(ORenderable* aRenderable, bool aUnloadAll=false);

	/**
	 @brief Unloads vertices and textures from the GPU.
	 @param aRenderComponents The rendering components containing the GPU resources to be unloaded.
	 @param aUnloadAll Unloads all rendering resources (buffers, textures, etc.).
	 */
	virtual void unload(ORenderComponents* aRenderComponents, bool aUnloadAll=false);

	/**
	 @brief Unloads vertex buffer.
	 */
	virtual void unload(OVertexBuffer* aVertexBuffer);

	/**
	 @brief Unloads index buffer.
	 */
	virtual void unload(OIndexBuffer* aIndexBuffer);

	/**
	 @brief Unloads texture.
	 */
	virtual void unload(OTexture* aTexture);

	/**
	 @brief Issues the appropriate render commands.
	 */
	virtual void render(ORenderable* aRenderable);

	/**
	 @brief Flushes all pending commands to the GPU.
	 */
	virtual void flush();

	/**
	 @brief The trash bean for rendering-related objects.
	 */
	OTrashBin& trashBin();

	/**
	 @brief Clear render target.
	 @param aColor The color used to set every screen pixel.
	 */
	void clearRenderTarget(const OVector4F& aColor);

	/**
	 @brief Clear depth buffer.
	 @param aValue The value used to set every depth buffer bin.
	 */
	void clearDepthBuffer(float aValue);

	/**
	 @brief Wait until all pending commands are executed.
	 */
	void waitUntilCompleted();

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
