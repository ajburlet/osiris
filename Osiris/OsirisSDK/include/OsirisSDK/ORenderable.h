#pragma once

#include <cstdint>
#include "OsirisSDK/defs.h"

class ORenderComponents;

/**
 @brief Renderable object base class.
 */
class OAPI ORenderable {
public:
	/**
	 @brief Renderable type.
	 */
	enum class Type : uint8_t {
		Undefined,
		Mesh,
		Glyph
	};

protected:
	/**
	 @brief Default class constructor.
	 */
	ORenderable(Type aType);

	/**
	 @brief Class destructor.
	 */
	virtual ~ORenderable();
	
public:
	/**
	 @brief Renderable type.
	 */
	Type type() const;

	/**
	 @brief Sets the render components reference.
	 */
	void setRenderComponents(ORenderComponents* aRenderComponents);

	/**
	 @brief Render components.
	 */
	ORenderComponents* renderComponents();

private:
	Type			_type			= Type::Undefined;
	ORenderComponents*	_renderComponents	= nullptr;

};

inline ORenderable::Type ORenderable::type() const
{
	return _type;
}

inline void ORenderable::setRenderComponents(ORenderComponents * aRenderComponents)
{
	_renderComponents = aRenderComponents;
}

inline ORenderComponents * ORenderable::renderComponents()
{
	return _renderComponents;
}
