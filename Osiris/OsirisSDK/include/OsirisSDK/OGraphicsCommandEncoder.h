#pragma once

#include "OsirisSDK/defs.h"

/**
 @brief Graphics command encoder base class.
 */
class OAPI OGraphicsCommandEncoder
{
public:
	/**
	 @brief Encoder types.
	 */
	enum class Type {
		Undefined,		/**< Undefined type, uninitialized. */
		Resource,		/**< Resource command encoder. */
		Render,			/**< Render command encoder. */
		Compute			/**< Compute command encoder. */
	};

protected:
	/**
	 @brief Class constructor.
	 @param aType Command encoder type.
	 */
	OGraphicsCommandEncoder(Type aType);

public:
	/**
	 @brief Class destructor.
	 */
	virtual ~OGraphicsCommandEncoder() = default;

	/**
	 @brief Ends the encoding process.
	 */
	virtual void end() = 0;

	/**
	 @brief Returns the type of the command encoder.
	 */
	Type type() const;

private:
	Type			_type		= Type::Undefined;
};

inline OGraphicsCommandEncoder::OGraphicsCommandEncoder(Type aType) :
	_type(aType)
{
}

inline OGraphicsCommandEncoder::Type OGraphicsCommandEncoder::type() const
{
	return _type;
}
