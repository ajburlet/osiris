#pragma once

#include <cstdint>
#include <functional>
#include <type_traits>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGPUObject.h"
#include "OsirisSDK/OGraphicsDefinitions.h"

class ORenderable;

/**
 @brief The shader attribute description class.
 */
class OAPI OShaderArgument
{
public:
	/**
	 @brief Class default constructor.
	 */
	OShaderArgument() = default;

	/**
	 @brief Class constructor.
	 @param aType Attribute type.
	 @param aArrayLength Array length (if not an array, value must be 1).
	 */
	OShaderArgument(OVarType aType,  uint8_t aArrayLength=1);

	/**
	 @brief Class destructor.
	 */
	~OShaderArgument() = default;

	/**
	 @brief Returns the attribute type.
	 */
	OVarType type() const;

	/**
	 @brief Returns the attribute dimensions.
	 */
	uint8_t arrayLength() const;

	/**
	 @brief Attribute size in bytes.
	 */
	uint32_t size() const;

protected:
	OVarType	_type		= OVarType::Undefined;
	uint8_t		_arrayLength	= 0;
	uint32_t	_size		= 0;
};

inline OVarType OShaderArgument::type() const
{
	return _type;
}

inline uint8_t OShaderArgument::arrayLength() const
{
	return _arrayLength;
}

inline uint32_t OShaderArgument::size() const
{
	return _size;
}

/**
 @brief Shader vertex argument.
 */
class OAPI OShaderVertexArgument : public OShaderArgument
{
public:
	static constexpr uint32_t DefaultIndex = 0;

	/**
	 @brief Class constructor.
	 @param aType Attribute type.
	 @param aIndex The attribute index.
	 @param aArrayLength Array length (if not an array, value must be 1).
	 */
	OShaderVertexArgument(OVarType aType, uint32_t aIndex=DefaultIndex, uint8_t aArrayLength=1);
	
	/**
	 @brief Class destructor.
	 */
	virtual ~OShaderVertexArgument() = default;

	/**
	 @brief Argument index.
	 */
	uint32_t index() const;


protected:
	uint32_t _index = DefaultIndex;
};

inline OShaderVertexArgument::OShaderVertexArgument(OVarType aType, uint32_t aIndex, uint8_t aArrayLength) :
	OShaderArgument(aType, aArrayLength)
{}

inline uint32_t OShaderVertexArgument::index() const
{
	return _index;
}


/**
 @brief A shader argument instance on the GPU.
 */
class OAPI OShaderArgumentInstance : public OShaderArgument, public OGPUObject
{
public:
	/**
	 @brief Update callback function type.

	 The function is in the form of <code>void function(OShaderArgumentInstance&)</code>.
	 */
	using UpdateCallbackFn = std::function<void(OShaderArgumentInstance&, const ORenderable*)>;

	/**
	 @brief Class constructor.
	 @param aType Attribute type.
	 @param aArrayLength Array length (if not an array, value must be 1).
	 */
	OShaderArgumentInstance(OVarType aType, uint8_t aArrayLength=1);

	/**
	 @brief Class destructor.
	 */
	~OShaderArgumentInstance();

	/**
	 @brief Returns the pointer to the buffer.
	 */
	void* buffer();

	/**
	 @brief Copies data into the buffer.
	 @param aSrc The pointer to the source memory.
	 @param aOffset The amount of bytes to be skipped from the beginning of the buffer.
	 @param aBytes Number of bytes to be copied (if zero, will copy until the end of the buffer).
	 */
	void copyFrom(const void* aSrc, uint32_t aOffset = 0, uint32_t aBytes = 0);

	/**
	 @brief Returns the reference to the value cast to a given type name.
	 */
	template <typename T>
	T& castTo();

	/**
	 @brief Sets the update callback function.
	 */
	void setUpdateCallbackFunction(UpdateCallbackFn aCallback);

	/**
	 @brief Executes the update callback function for a given renderable.
	 */
	void update(const ORenderable* aRenderable);

	/**
	 @brief Creates and initializes an argument instance.
	 @param aType Attribute type.
	 @param aArrayLength Number of dimensions.
	 @param aValue The initial value of the argument instance.
	 @return The newly created argument instance.
	 */
	template <typename T>
	static OShaderArgumentInstance* create(OVarType aType, uint8_t aArrayLength, const T& aValue);

private:
	uint8_t*		_buffer	= nullptr;
	UpdateCallbackFn	_updateCallback;

};

inline void* OShaderArgumentInstance::buffer()
{
	return _buffer;
}

inline void OShaderArgumentInstance::setUpdateCallbackFunction(UpdateCallbackFn aCallback)
{
	_updateCallback = aCallback;
}

inline void OShaderArgumentInstance::update(const ORenderable* aRenderable)
{
	if (_updateCallback) _updateCallback(*this, aRenderable);
}

template<typename T>
inline T& OShaderArgumentInstance::castTo()
{
	if (std::is_pointer<T>::value) {
		return reinterpret_cast<T&>(_buffer);
	}
	return *reinterpret_cast<T*>(_buffer);
}

template<typename T>
inline OShaderArgumentInstance * OShaderArgumentInstance::create(OVarType aType, uint8_t aArrayLength, const T & aValue)
{
	auto var = new OShaderArgumentInstance(aType, aArrayLength);
	OExceptionPointerCheck(var);
	var->castTo<T>() = aValue;
	return var;
}