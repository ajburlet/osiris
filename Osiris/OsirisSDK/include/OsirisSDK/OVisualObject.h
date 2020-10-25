#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"

class ORenderingEngine;
class OMatrixStack;

/**
 @brief Common interface for objects capable of rendering.
 */
class OAPI OVisualObject : public OMemoryManagedObject<OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>>
{
public:
	/**
	 @brief Defines the rendering order of the visual object, object with lower priority values 
		will be rendered first.
	 */
	using RenderPriority = uint32_t;

	/**
	 @brief Default render priority.
	 */
	static constexpr RenderPriority DefaultRenderPriority = 1000000;

	/**
	 @brief Class constructor.
	 */
	OVisualObject(RenderPriority aRenderPriority = DefaultRenderPriority) : _renderPriority(aRenderPriority) {}

	/**
	 @brief Class destructor.
	 */
	virtual ~OVisualObject() = default;

	/**
	 @brief Sets the rendering priority.
	 */
	void setRenderingPriority(RenderPriority aRenderPriority);

	/**
	 @brief Returns the render priority.
	 */
	RenderPriority renderPriority() const;

	/**
	 @brief Enables rendering capability.
	 */
	void show();

	/**
	 @brief Disables rendering capability.
	 */
	void hide();

	/**
	 @brief Returns true if rendering capability is disabled.
	 */
	bool isHidden() const;

	/**
	 @brief Renderization call.
	 @param aRenderingEngine The rendering engine.
	 @param aMatrixStack Stacked transformation matrices.
	 */
	virtual void render(ORenderingEngine* aRenderingEngine, OMatrixStack* aMatrixStack = nullptr) = 0;

private:
	RenderPriority _renderPriority;
	bool _hidden = false;
};

inline void OVisualObject::setRenderingPriority(RenderPriority aRenderPriority)
{
	_renderPriority = aRenderPriority;
}

inline OVisualObject::RenderPriority OVisualObject::renderPriority() const
{
	return _renderPriority;
}

inline void OVisualObject::show()
{
	_hidden = false;
}

inline void OVisualObject::hide()
{
	_hidden = true;
}

inline bool OVisualObject::isHidden() const
{
	return _hidden;
}
