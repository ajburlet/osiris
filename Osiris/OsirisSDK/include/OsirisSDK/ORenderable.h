#pragma once

#include <cstdint>
#include "OsirisSDK/defs.h"
#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"

class ORenderComponents;
class OShaderUniformArgument;
template <typename T, class Allocator> class OList;

/**
 @brief Renderable object base class.
 */
class OAPI ORenderable : public ONonCopiableT<ORenderable>
{
public:
	/**
	 @brief Shader uniform argument list.
	 */
	using UniformList = OList<OShaderUniformArgument, OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>>;

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
	 @brief Move constructor.
	 */
	ORenderable(ORenderable&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~ORenderable();
	
public:

	/**
	 @brief Move assignment operator.
	 @note If the uniforms are loaded on the target object, an exception will be thrown. These must be unloaded.
	 */
	ORenderable& operator=(ORenderable&& aOther);

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

	/**
	 @brief Provides access to the shader uniform argument list.
	 */
	UniformList& uniforms();

	/**
	 @brief Updates all uniforms with values from the renderable object.
	 */
	void updateUniforms();

	/**
	 @brief Sets uniform argument load status.
	 @param aLoaded True if uniform is loaded.
	 */
	void setUniformsLoaded(bool aLoaded);

	/**
	 @brief Returns true if uniform arguments are loaded.
	 */
	bool uniformsLoaded() const;

private:
	Type			_type			= Type::Undefined;
	ORenderComponents*	_renderComponents	= nullptr;
	UniformList*		_uniforms		= nullptr;
	bool			_uniformsLoaded		= false;
};

inline ORenderable::ORenderable(ORenderable&& aOther) :
	_type(aOther._type),
	_renderComponents(aOther._renderComponents),
	_uniforms(aOther._uniforms),
	_uniformsLoaded(aOther._uniformsLoaded)
{
	aOther._uniforms = nullptr;
	aOther._uniformsLoaded = false;
}

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

inline ORenderable::UniformList & ORenderable::uniforms()
{
	return *_uniforms;
}

