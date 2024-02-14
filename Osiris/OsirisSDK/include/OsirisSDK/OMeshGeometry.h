#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/ONonCopiable.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexedDrawInfo.h"
#include "OsirisSDK/ORefCountObject.hpp"

/**
 @brief Mesh geometry holder, owns the render mode, vertex and index buffers.
 */
class OAPI OMeshGeometry : public ORefCountObject<>, public ONonCopiableT<OMeshGeometry, false>
{
public:
	/**
	 @brief Default class constructor.
	 */
	OMeshGeometry() = default;

	/**
	 @brief Class constructor.
	 @param aDrawMode Mesh draw mode.
	 @param aVertexBuffer Vertex buffer.
	 @param aIndexedDrawInfoList Array of possible materials, with corresponding index buffers.
	 */
	OMeshGeometry(ORenderMode aDrawMode, OVertexBuffer&& aVertexBuffer, OIndexedDrawInfo::Array&& aIndexedDrawInfoList);

	/**
	 @brief Move constructor. 
	 */
	OMeshGeometry(OMeshGeometry&& aOther) :
		_drawMode(aOther._drawMode),
		_vertexBuffer(std::move(aOther._vertexBuffer)),
		_indexedDrawInfoArray(std::move(aOther._indexedDrawInfoArray))
	{

	}

	/**
	 @brief Class destructor.
	 */
	virtual ~OMeshGeometry() = default;

	/**
	 @brief Move assignment operator.
	 */
	OMeshGeometry& operator= (OMeshGeometry&& aOther);

	/**
	 @brief Returns the draw mode.
	 */
	ORenderMode drawMode() const;

	/**
	 @brief Returns the vertex buffer.
	 */
 	OVertexBuffer& vertexBuffer();

	/**
	 @brief Returns the array of indexed draw info (index buffer + material). 
	 */
	OIndexedDrawInfo::Array& indexedDrawInfoArray ();

private:
	ORenderMode				_drawMode		= ORenderMode::Undefined;
	OVertexBuffer			_vertexBuffer;
	OIndexedDrawInfo::Array	_indexedDrawInfoArray;
};

inline OMeshGeometry::OMeshGeometry (ORenderMode aDrawMode, OVertexBuffer&& aVertexBuffer, OIndexedDrawInfo::Array&& aIndexedDrawInfoList) :
	_drawMode(aDrawMode),
	_vertexBuffer(std::move(aVertexBuffer)),
	_indexedDrawInfoArray(std::move(_indexedDrawInfoArray))
{
}

inline OMeshGeometry& OMeshGeometry::operator= (OMeshGeometry&& aOther)
{
	_drawMode = aOther._drawMode;
	_vertexBuffer = std::move(aOther._vertexBuffer);
	_indexedDrawInfoArray = std::move(aOther._indexedDrawInfoArray);
	return *this;
}

inline ORenderMode OMeshGeometry::drawMode() const
{
	return _drawMode;
}

inline OVertexBuffer& OMeshGeometry::vertexBuffer()
{
	return _vertexBuffer;
}

inline OIndexedDrawInfo::Array& OMeshGeometry::indexedDrawInfoArray()
{
	return _indexedDrawInfoArray;
}


