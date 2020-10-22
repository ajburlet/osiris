#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGPUObject.h"

/**
 @brief Index buffer class.
 */
class OAPI OIndexBuffer : public OGPUObject
{
public:
	/**
	 @brief Class constructor.
	 @param aFaceCount Vertex count.
	 */
	OIndexBuffer(uint32_t aFaceCount = 0);

	/**
	 @brief Deleted copy constructor.
	 */
	OIndexBuffer(const OIndexBuffer& aOther) = delete;

	/**
	 @brief Move constructor
	 */
	OIndexBuffer(OIndexBuffer&& aOther);

	/**
	 @brief Class destructor.
	 */
	~OIndexBuffer();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OIndexBuffer& operator=(const OIndexBuffer& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OIndexBuffer& operator=(OIndexBuffer&& aOther);

	/**
	 @brief Sets the new face count, redimensioning the buffer memory area.
	 @param aNewFaceCount The new face count.
	 */
	void setFaceCount(uint32_t aNewFaceCount);

	/**
	 @brief Sets the triangular face indexes.
	 @param aFaceIndex The face index.
	 @param aVertexIndex1 The first vertex index.
	 @param aVertexIndex2 The second vertex index.
	 @param aVertexIndex3 The third vertex index.
	 */
	void setFace(uint32_t aFaceIndex, uint32_t aVertexIndex1, uint32_t aVertexIndex2,
		     uint32_t aVertexIndex3);

	/**
	 @brief Adds a triangular face to the end of the index buffer.
	 @param aVertexIndex1 The first vertex index.
	 @param aVertexIndex2 The second vertex index.
	 @param aVertexIndex3 The third vertex index.
	 */
	void addFace(uint32_t aVertexIndex1, uint32_t aVertexIndex2, uint32_t aVertexIndex3);

	/**
	 @brief Returns the pointer to the buffer memory.
	 */
	const void* buffer() const;

	/**
	 @brief Returns the number of faces.
	 */
	uint32_t faceCount() const;

	/**
	 @brief Returns the total buffer size.
	 */
	uint32_t size() const;

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OIndexBuffer::OIndexBuffer(OIndexBuffer&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}