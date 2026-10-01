#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexedDrawInfo.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OResource.h"

/**
 @brief Mesh geometry holder, owns the render mode, vertex and index buffers.
 */
class OAPI OMeshGeometry : public OMemoryManagedObject<OGraphicsAllocators::Default>,
						   public OResource 
{
private:
	using Super = OResource;

public:
	/**
	 @brief Default class constructor.
	 */
	OMeshGeometry() = default;

	/**
	 @brief Class constructor.
	 @param name Resource name.
	 @param drawMode Mesh draw mode.
	 @param vertexBuffer Vertex buffer.
	 @param indexedDrawInfoArray Array of possible materials, with corresponding index buffers.
	 */
	OMeshGeometry(OString&& name, 
				  ORenderMode drawMode, 
				  OVertexBuffer&& vertexBuffer, 
				  OIndexedDrawInfo::Array&& indexedDrawInfoArray);

	/**
	 @brief Move constructor. 
	 */
	OMeshGeometry(OMeshGeometry&& aOther) 
		: Super(std::move(aOther))
		, _drawMode(aOther._drawMode)
		, _vertexBuffer(std::move(aOther._vertexBuffer))
		, _indexedDrawInfoArray(std::move(aOther._indexedDrawInfoArray))
	{

	}

	/**
	 @brief Class destructor.
	 */
	virtual ~OMeshGeometry() = default;

	/**
	 @brief Move assignment operator.
	 */
	OMeshGeometry& operator=(OMeshGeometry&& aOther);

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

inline OMeshGeometry::OMeshGeometry(OString&& aName, 
									ORenderMode aDrawMode, 
									OVertexBuffer&& aVertexBuffer, 
									OIndexedDrawInfo::Array&& aIndexedDrawInfoArray) 
	: Super(std::move(aName))
	, _drawMode(aDrawMode)
	, _vertexBuffer(std::move(aVertexBuffer))
	, _indexedDrawInfoArray(std::move(aIndexedDrawInfoArray))
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


