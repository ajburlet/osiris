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
 @brief Contains all data pertaining to GPU rendering of a renderable object.
 */
class OAPI ORenderComponents 
{
public:
	/**
	 @brief Default class constructor.
	 */
	ORenderComponents();

	/**
	 @brief Class destructor.
	 */
	virtual ~ORenderComponents();

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
	 @brief Returns true if the render data have already been loaded to the GPU. 
	 */
	bool loaded() const;

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
	 @brief Returns the uniform argument list.
	 */
	OShaderArgumentInstanceList* uniformArgumentList();

private:
	ORenderMode			_renderMode		= ORenderMode::Undefined;
	OVertexBuffer*			_vertexBuffer		= nullptr;
	OIndexBuffer*			_indexBuffer		= nullptr;
	OTexture*			_texture		= nullptr;
	OShaderProgram*			_shaderProgram		= nullptr;
	bool				_loaded			= false;
	OShaderArgumentInstanceList*	_argumentInstanceList	= nullptr;
};


inline OVertexBuffer * ORenderComponents::vertexBuffer()
{
	return _vertexBuffer;
}

inline OIndexBuffer * ORenderComponents::indexBuffer()
{
	return _indexBuffer;
}

inline OTexture * ORenderComponents::texture()
{
	return _texture;
}

inline OShaderProgram * ORenderComponents::shaderProgram()
{
	return _shaderProgram;
}

inline void ORenderComponents::setRenderMode(ORenderMode aRenderMode)
{
	_renderMode = aRenderMode;
}

inline void ORenderComponents::setVertexBuffer(OVertexBuffer * aVertexBuffer)
{
	_vertexBuffer = aVertexBuffer;
}

inline void ORenderComponents::setIndexBuffer(OIndexBuffer * aIndexBuffer)
{
	_indexBuffer = aIndexBuffer;
}

inline void ORenderComponents::setTexture(OTexture * aTexture)
{
	_texture = aTexture;
}

inline void ORenderComponents::setShaderProgram(OShaderProgram * aShaderProgram)
{
	_shaderProgram = aShaderProgram;
}

inline OShaderArgumentInstanceList * ORenderComponents::uniformArgumentList()
{
	return _argumentInstanceList;
}

