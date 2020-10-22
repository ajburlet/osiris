#pragma once

#include "defs.h"

/**
 @brief Double-buffer implementation. 
 */
template <class BType> class ODoubleBuffer
{
public:
	/**
	 @brief Class constructor.
	 */
	ODoubleBuffer() : _currIdx(0) { }

	/**
	 @brief Class destructor.
	 */
	virtual ~ODoubleBuffer() { }

	/**
	 @brief Returns pointer to the current state.
	 */
	BType* curr() { return &_buffers[_currIdx]; }

	/**
	 @brief Returns pointer to the next state.
	 */
	BType* next() { return &_buffers[!_currIdx]; }
	
	/**
	 @brief Swap buffers.
	 */
	void swap()
	{
		_currIdx = !_currIdx;
	}

	/**
	 @brief Equalize buffers, making next equal to current.
	 */
	void equalize()
	{
		_buffers[_currIdx].cloneTo(_buffers[!_currIdx]);
	}

private:
	BType _buffers[2];
	int _currIdx;
};
