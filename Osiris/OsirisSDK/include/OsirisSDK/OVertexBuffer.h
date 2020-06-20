#pragma once

#include <stdint.h>
#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGPUObject.h"

class OShaderArgument;

/**
 @brief The vertex buffer descriptor.
 */
class OAPI OVertexBufferDescriptor : public OGPUObject
{
public:
	/**
	 @brief Default class constructor.
	 */
	OVertexBufferDescriptor();

	/**
	 @brief Class destructor.
	 */
	virtual ~OVertexBufferDescriptor();

	/**
	 @brief Adds a vertex buffer component.
	 @param aAttribute The vertex attribute to be added.
	 @return The attribute index.
	 */
	uint8_t addAttribute(const OShaderArgument& aAttribute);

	/**
	 @brief Returns the attribute at a given index.
	 @param aAttributeIndex Attribute index.
	 @return The attribute info object.
	 */
	const OShaderArgument& attributeAtIndex(uint8_t aAttributeIndex) const;

	/**
	 @brief Returns the attribute count.
	 */
	uint32_t attributeCount() const;

	/**
	 @brief Attribute memory offset.
	 @param aAttributeIndex Attribute index.
	 @return The memory offset from the start of the buffer.
	 */
	uint32_t offset(uint32_t aAttributeIndex) const;

	/**
	 @brief Returns the stride value (number of bytes until the next item on the array).
	 */
	uint32_t stride() const;

protected:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

/**
 @brief Vertex buffer class.
 */
class OAPI OVertexBuffer : public OGPUObject
{
public:
	/**
	 @brief Class constructor.
	 @param aDescriptor Vertex buffer descriptor.
	 @param aVertexCount Vertex count.
	 */
	OVertexBuffer(OVertexBufferDescriptor& aDescriptor, uint32_t aVertexCount=0);

	/**
	 @brief Class destructor.
	 */
	virtual ~OVertexBuffer();

	/**
	 @brief Returns the vertex count.
	 */
	uint32_t vertexCount() const;

	/**
	 @brief Resizes the buffer with a new vertex count.
	 @param aNewVertexCount The new vertex count.
	 */
	void resize(uint32_t aNewVertexCount);

	/**
	 @brief Returns the vertex descriptor.
	 */
	OVertexBufferDescriptor& descriptor();

	/**
	 @brief Returns the pointer to the buffer memory.
	 */
	const void* buffer() const;

	/**
	 @brief Sets an attribute value for a given vertex index.
	 @param aAttributeIndex Attribute index.
	 @param aVertexIndex Vertex index.
	 @param aValues An array with the values of the attribute components.
	 */
	template<typename T>
	void setAttributeValue(uint32_t aAttributeIndex, uint32_t aVertexIndex, const T* aValuesArr);
	
	/**
	 @brief Total buffer size.
	 */
	uint32_t size() const;

	
protected:
	/**
	 @brief Sets value for a single attribute component.
	 @param aOffset The global buffer offset to the attribute.
	 @param aValue The value to be written.
	 */
	template <typename T>
	void putValue(uint32_t aOffset, const T& aValue);

	/**
	 @brief Sets value for a single attribute component.
	 @param aOffset The global buffer offset to the attribute.
	 @param aPtr The pointer to the value that must be set.
	 @param aValue The size of the attribute.
	 */
	void putValue(uint32_t aOffset, const void* aPtr, size_t aSize);

private:
	OVertexBufferDescriptor*	_descriptor	= nullptr;
	uint32_t			_vertexCount	= 0;
	uint8_t*			_buffer		= nullptr;
};

inline uint32_t OVertexBuffer::vertexCount() const
{
	return _vertexCount;
}

inline OVertexBufferDescriptor& OVertexBuffer::descriptor()
{
	return *_descriptor;
}

inline const void* OVertexBuffer::buffer() const
{
	return _buffer;
}

template<typename T>
inline void OVertexBuffer::setAttributeValue(uint32_t aAttributeIndex, uint32_t aVertexIndex, const T* aValuesArr)
{
	if (aDimensions != _descriptor.attributeAtIndex(aAttributeIndex).dimensions()) {
		throw OException("Incompatible vertex attribute: dimensions.");
	}
	putValue(aVertexIndex*_descriptor->stride() + _descriptor->offset(aAttributeIndex),
		 aValuesArr, _descriptor->attributeAtIndex(aAttributeIndex).size());

}

inline uint32_t OVertexBuffer::size() const
{
	return vertexCount() * _descriptor->stride();
}

template<typename T>
inline void OVertexBuffer::putValue(uint32_t aOffset, const T & aValue)
{
	putValue(aOffset, &avalue, sizeof(T));
}
