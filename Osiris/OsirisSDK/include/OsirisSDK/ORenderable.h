#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"

class OVertexBuffer;
class OIndexBuffer;
class OTexture;

/**
 @brief Renderable object base class.
 */
class OAPI ORenderable 
{
public:
	/**
	 @brief Renderable type.
	 */
	enum class Type : uint8_t {
		Mesh,
		Text2D
	};

protected:
	/**
	 @brief Default class constructor.
	 */
	ORenderable(Type aType);

	/**
	 @brief Class destructor.
	 */
	virtual ~ORenderable() = default;

public:
	/**
	 @brief Returns the vertex buffer.
	 */
	const OVertexBuffer* vertexBuffer() const;

	/**
	 @brief Returns the index buffer.
	 */
	const OIndexBuffer* indexBuffer() const;

	/**
	 @brief Returns the texture.
	 */
	const OTexture* texture() const;

	/**
	 @brief Sets the vertex buffer.
	 @param aVertexBuffer The vertex buffer to be set.
	 */
	void setVertexBuffer(OVertexBuffer* aVertexBuffer);

	/**
	 @brief Sets the index buffer.
	 @param aIndexBuffer The index buffer to be set.
	 */
	void setIndexBuffer(OIndexBuffer* aIndexBuffer);

	/**
	 @brief Sets the texture.
	 @param aTexture The texture to be set.
	 */
	void setTexture(OTexture* aTexture);

private:
	Type		_type;
	OVertexBuffer*	_vertexBuffer	= nullptr;
	OIndexBuffer*	_indexBuffer	= nullptr;
	OTexture*	_texture	= nullptr;
};


ORenderable::ORenderable(Type aType) : _type(aType) 
{
}

inline const OVertexBuffer * ORenderable::vertexBuffer() const
{
	return _vertexBuffer;
}

inline const OIndexBuffer * ORenderable::indexBuffer() const
{
	return _indexBuffer;
}

inline const OTexture * ORenderable::texture() const
{
	return _texture;
}

inline void ORenderable::setVertexBuffer(OVertexBuffer * aVertexBuffer)
{
	_vertexBuffer = aVertexBuffer;
}

inline void ORenderable::setIndexBuffer(OIndexBuffer * aIndexBuffer)
{
	_indexBuffer = aIndexBuffer;
}

inline void ORenderable::setTexture(OTexture * aTexture)
{
	_texture = aTexture;
}





