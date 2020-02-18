#pragma once

#include <stdint.h>
#include <assert.h>
#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGPUObject.h"

/**
 @brief The vertex attribute description class.
 */
class OAPI OVertexAttributeDescriptor
{
public:
	/**
	 @brief Class constructor.
	 @param aType Attribute type.
	 @param aPrecision Precision type.
	 @param aDim Number of dimensions.
	 */
	OVertexAttributeDescriptor(OVarType aType, OVarPrecision aPrecision, uint8_t aDim);

	/**
	 @brief Returns the attribute type.
	 */
	OVarType type() const;

	/**
	 @brief Returns the attribute precision.
	 */
	OVarPrecision precision() const;

	/**
	 @brief Returns the attribute dimensions.
	 */
	uint8_t dimensions() const;

	/**
	 @brief Attribute size in bytes.
	 */
	uint32_t size() const;

protected:
	OVarType	_type		= OVarType::Undefined;
	OVarPrecision	_precision	= OVarPrecision::Undefined;
	uint8_t		_dim		= 0;
	uint32_t	_size		= 0;
};

inline OVarType OVertexAttributeDescriptor::type() const
{
	return _type;
}

inline OVarPrecision OVertexAttributeDescriptor::precision() const
{
	return _precision;
}

inline uint8_t OVertexAttributeDescriptor::dimensions() const
{
	return _dim;
}

inline uint32_t OVertexAttributeDescriptor::size() const
{
	return _size;
}

/**
 @brief The vertex buffer descriptor.
 */
class OAPI OVertexBufferDescriptor
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
	uint8_t addAttribute(const OVertexAttributeDescriptor& aAttribute);

	/**
	 @brief Returns the attribute at a given index.
	 @param aAttributeIndex Attribute index.
	 @return The attribute info object.
	 */
	const OVertexAttributeDescriptor& attributeAtIndex(uint8_t aAttributeIndex) const;

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
	struct Impl;
	Impl* _impl = nullptr;
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
	OVertexBuffer(const OVertexBufferDescriptor& aDescriptor, uint32_t aVertexCount=0);

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
	const OVertexBufferDescriptor& descriptor() const;

	/**
	 @brief Returns the pointer to the buffer memory.
	 */
	const void* buffer() const;

	/**
	 @brief Sets an attribute value for a given vertex index.
	 @param aAttributeIndex Attribute index.
	 @param aVertexIndex Vertex index.
	 @param aDimensions The number of dimensions.
	 @param aValues An array with the values of the attribute components.
	 */
	template<typename T>
	void setAttributeValue(uint32_t aAttributeIndex, uint32_t aVertexIndex,
			       uint32_t aDimensions, const T* aValuesArr);
	
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
	OVertexBufferDescriptor _descriptor;
	uint32_t		_vertexCount	= 0;
	uint8_t*		_buffer		= nullptr;
};

inline uint32_t OVertexBuffer::vertexCount() const
{
	return _vertexCount;
}

inline const OVertexBufferDescriptor& OVertexBuffer::descriptor() const
{
	return _descriptor;
}

inline const void* OVertexBuffer::buffer() const
{
	return _buffer;
}


template<typename T>
inline void OVertexBuffer::setAttributeValue(uint32_t aAttributeIndex, uint32_t aVertexIndex, 
					     uint32_t aDimensions, const T* aValuesArr)
{
	if (aDimensions != _descriptor.attributeAtIndex(aAttributeIndex).dimensions()) {
		throw OException("Incompatible vertex attribute: dimensions.");
	}
	putValue(aVertexIndex*_descriptor.stride() + _descriptor.offset(aAttributeIndex),
		 aValuesArr, _descriptor.attributeAtIndex(aAttributeIndex).size());

}

template<typename T>
inline void OVertexBuffer::putValue(uint32_t aOffset, const T & aValue)
{
	putValue(aOffset, &avalue, sizeof(T));
}
