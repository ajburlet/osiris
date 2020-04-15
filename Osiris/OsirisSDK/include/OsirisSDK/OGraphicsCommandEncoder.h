#pragma once

class OGraphicsCommandQueue;

/**
 @brief Graphics command encoder base class.
 */
class OAPI OGraphicsCommandEncoder
{
public:
	/**
	 @brief Class constructor.
	 @param aCommandQueue The queue on which the encoder writes the commands to.
	 */
	OGraphicsCommandEncoder(OGraphicsCommandQueue* aCommandQueue);

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

private:
	OGraphicsCommandQueue* _commandQueue = nullptr;
};

inline OGraphicsCommandEncoder::OGraphicsCommandEncoder(OGraphicsCommandQueue* aCommandQueue) :
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
