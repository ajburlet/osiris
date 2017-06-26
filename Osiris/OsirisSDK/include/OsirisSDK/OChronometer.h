#pragma once

#include "defs.h"
#include "OTimeIndex.h"

/**
 @brief Class designed to measure time intervals.
 */
class OAPI OChronometer
{
public:
	/**
	 @brief Class constructor.

	 Time starts being measured after the constructor is called.
	 */
	OChronometer();

	/**
	 @brief Class destructor.
	 */
	virtual ~OChronometer();

	/**
	 @brief Reset chronometer.
	 */
	void reset();

	/**
	 @brief Interval since last partial call, in microseconds. 
	 */
	int partial();

	/**
	 @brief Total elapsed time in microseconds.
	 */
	int totalElapsed() const;

	/**
	 @brief Start time.
	 */
	OTimeIndex startTime() const;

	/**
	 @brief Last partial time.
	 */
	OTimeIndex lastPartialTime() const;

private:
	OTimeIndex _start;
	OTimeIndex _last;
};

