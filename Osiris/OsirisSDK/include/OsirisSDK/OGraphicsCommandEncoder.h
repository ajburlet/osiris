#pragma once

class OGraphicsCommandQueue;

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

	/**
	 @brief Class constructor.
	 @param aCommandQueue The queue on which the encoder writes the commands to.
	 */
	OGraphicsCommandEncoder(Type aType, OGraphicsCommandQueue* aCommandQueue = nullptr);

	/**
	 @brief Class destructor.
	 */
	virtual ~OGraphicsCommandEncoder() = default;

	/**
	 @brief Returns the command queue on which the encoder writes the commands to.
	 */
	OGraphicsCommandQueue* commandQueue();

	/**
	 @brief Ends the encoding process.
	 */
	virtual void end();

	/**
	 @brief Returns the type of the command encoder.
	 */
	Type type() const;

private:
	Type			_type		= Type::Undefined;
	OGraphicsCommandQueue*	_commandQueue	= nullptr;
};

inline OGraphicsCommandEncoder::OGraphicsCommandEncoder(Type aType, OGraphicsCommandQueue* aCommandQueue) :
	_type(aType),
	_commandQueue(aCommandQueue)
{
}

inline OGraphicsCommandQueue * OGraphicsCommandEncoder::commandQueue()
{
	return _commandQueue;
}

inline void OGraphicsCommandEncoder::end()
{
	// does nothing by default
}

inline OGraphicsCommandEncoder::Type OGraphicsCommandEncoder::type() const
{
	return _type;
}
