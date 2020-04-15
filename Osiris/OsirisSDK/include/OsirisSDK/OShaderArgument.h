#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGPUObject.h"
#include "OsirisSDK/OGraphicsDefinitions.h"

/**
 @brief The shader attribute description class.
 */
class OAPI OShaderArgument
{
public:
	/**
	 @brief Class constructor.
	 @param aType Attribute type.
	 @param aPrecision Precision type.
	 @param aDim Number of dimensions.
	 */
	OShaderArgument(OVarType aType, OVarPrecision aPrecision, uint8_t aDim);

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

inline OVarType OShaderArgument::type() const
{
	return _type;
}

inline OVarPrecision OShaderArgument::precision() const
{
	return _precision;
}

inline uint8_t OShaderArgument::dimensions() const
{
	return _dim;
}

inline uint32_t OShaderArgument::size() const
{
	return _size;
}

/**
 @brief A shader argument instance on the GPU.
 */
class OAPI OShaderArgumentInstance : public OShaderArgument, public OGPUObject
{
public:
	/**
	 @brief Class constructor.
	 @param aType Attribute type.
	 @param aPrecision Precision type.
	 @param aDim Number of dimensions.
	 */
	OShaderArgumentInstance(OVarType aType, OVarPrecision aPrecision, uint8_t aDim);

	/**
	 @brief Class destructor.
	 */
	~OShaderArgumentInstance();

	/**
	 @brief Returns the pointer to the buffer.
	 */
	void* buffer();

	/**
	 @brief Returns the reference to the value cast to a given type name.
	 */
	template <typename T>
	T& castTo();

	/**
	 @brief Creates and initializes an argument instance.
	 @param aType Attribute type.
	 @param aPrecision Precision type.
	 @param aDim Number of dimensions.
	 @param aValue The initial value of the argument instance.
	 @return The newly created argument instance.
	 */
	template <typename T>
	static OShaderArgumentInstance* create(OVarType aType, OVarPrecision aPrecision, uint8_t aDim, const T& aValue);

private:
	void* _buffer	= nullptr;
};

inline void* OShaderArgumentInstance::buffer()
{
	return _buffer;
}

template<typename T>
inline T& OShaderArgumentInstance::castTo()
{
	return static_cast<T>(*_buffer);
}

template<typename T>
inline OShaderArgumentInstance * OShaderArgumentInstance::create(OVarType aType, OVarPrecision aPrecision, uint8_t aDim, const T & aValue)
{
	auto var = new OShaderArgumentInstance(aType, aPrecision, aDim);
	OExceptionPointerCheck(var);
	var->castTo<T>() = aValue;
	return var;
}
