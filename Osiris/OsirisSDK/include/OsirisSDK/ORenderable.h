#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"

class OVertexBuffer;
class OIndexBuffer;
class OTexture;
class OShaderProgram;
class OShaderArgumentInstance;

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
	virtual ~ORenderable();

public:
	/**
	 @brief Renderable type.
	 */
	Type type() const;

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
	 * @brief Returns the shader program.
	 */
	const OShaderProgram* shaderProgram() const;

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

	/**
	 @brief Sets the shader program.
	 @param aShaderProgram The shader program to be set.
	 */
	void setShaderProgram(OShaderProgram* aShaderProgram);

	/**
	 @brief Creates a new shader uniform argument instance.
	 */
	void addUniformArgument(OShaderArgumentInstance* aNewArgument);

private:
	Type		_type;
	OVertexBuffer*	_vertexBuffer	= nullptr;
	OIndexBuffer*	_indexBuffer	= nullptr;
	OTexture*	_texture	= nullptr;
	OShaderProgram*	_shaderProgram  = nullptr;

	struct Impl;
	Impl* _impl = nullptr;
};


inline ORenderable::Type ORenderable::type() const
{
	return _type;
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

inline const OShaderProgram * ORenderable::shaderProgram() const
{
	return _shaderProgram;
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

inline void ORenderable::setShaderProgram(OShaderProgram * aShaderProgram)
{
	_shaderProgram = aShaderProgram;
}

