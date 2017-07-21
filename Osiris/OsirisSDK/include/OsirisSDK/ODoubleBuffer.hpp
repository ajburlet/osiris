#pragma once

#include "defs.h"

/**
 @brief Double-buffer implementation. 
 */
template <class BType> class OAPI ODoubleBuffer
{
public:
	/**
	 @brief Class constructor.
	 */
	ODoubleBuffer();

	/**
	 @brief Class destructor.
	 */
	virtual ~ODoubleBuffer();

	/**
	 @brief Returns pointer to the current state.
	 */
	BType* curr();

	/**
	 @brief Returns pointer to the next state.
	 */
	BType* next();
	
	/**
	 @brief Swap buffers.
	 */
	void swap();

private:
	BType _buffers[2];
	int _currIdx;
};

#ifdef OSIRISSDK_EXPORTS
template<class BType>
inline ODoubleBuffer<BType>::ODoubleBuffer() :
	_currIdx(0)
{
}

template<class BType>
inline ODoubleBuffer<BType>::~ODoubleBuffer()
{
}

template<class BType>
inline BType * ODoubleBuffer<BType>::curr()
{
	return &buffers[_currIdx];
}

template<class BType>
inline BType * ODoubleBuffer<BType>::next()
{
	return &_buffers[!_currIdx];
}

template<class BType>
inline void ODoubleBuffer<BType>::swap()
{
	_currIdx != _currIdx;
	_buffers[!_currIdx] = _buffers[_currIdx];
}
#endif