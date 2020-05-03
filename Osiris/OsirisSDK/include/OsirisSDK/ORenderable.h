#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"

class OVertexBuffer;
class OIndexBuffer;
class OTexture;
class OShaderProgram;
class OShaderArgumentInstance;
class OShaderArgumentInstanceList;

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
	 @brief Returns the render mode.
	 */
	ORenderMode renderMode() const;

	/**
	 @brief Returns the vertex buffer.
	 */
	OVertexBuffer* vertexBuffer();

	/**
	 @brief Returns the index buffer.
	 */
	OIndexBuffer* indexBuffer();

	/**
	 @brief Returns the texture.
	 */
	OTexture* texture();

	/**
	 * @brief Returns the shader program.
	 */
	OShaderProgram* shaderProgram();

	/**
	 @brief Sets the render mode.
	 */
	void setRenderMode(ORenderMode aRenderMode);

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
	 @brief Fetches a given uniform argument.
	 @param aIndex The argument index.
	 @return A pointer to the argument instance.
	 */
	OShaderArgumentInstanceList* uniformArgumentList();

private:
	Type				_type;
	ORenderMode			_renderMode		= ORenderMode::Undefined;
	OVertexBuffer*			_vertexBuffer		= nullptr;
	OIndexBuffer*			_indexBuffer		= nullptr;
	OTexture*			_texture		= nullptr;
	OShaderProgram*			_shaderProgram		= nullptr;
	OShaderArgumentInstanceList*	_argumentInstanceList	= nullptr;
};


inline ORenderable::Type ORenderable::type() const
{
	return _type;
}

inline OVertexBuffer * ORenderable::vertexBuffer()
{
	return _vertexBuffer;
}

inline OIndexBuffer * ORenderable::indexBuffer()
{
	return _indexBuffer;
}

inline OTexture * ORenderable::texture()
{
	return _texture;
}

inline OShaderProgram * ORenderable::shaderProgram()
{
	return _shaderProgram;
}

inline void ORenderable::setRenderMode(ORenderMode aRenderMode)
{
	_renderMode = aRenderMode;
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

inline OShaderArgumentInstanceList * ORenderable::uniformArgumentList()
{
	return _argumentInstanceList;
}

