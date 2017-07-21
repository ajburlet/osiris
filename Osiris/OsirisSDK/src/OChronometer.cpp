#include "OsirisSDK/OChronometer.h"


OChronometer::OChronometer()
{
	reset();
}

OChronometer::~OChronometer()
{
}

void OChronometer::reset()
{
	_start = _last = OTimeIndex::current();
}

int OChronometer::partial()
{
	int ret;
	OTimeIndex curr = OTimeIndex::current();
	ret = (curr - _last).toInt();
	_last = curr;
	return ret;
}

int OChronometer::totalElapsed() const
{
	OTimeIndex curr = OTimeIndex::current();
	return (curr - _start).toInt();
}

OTimeIndex OChronometer::startTime() const
{
	return _start;
}

OTimeIndex OChronometer::lastPartialTime() const
{
	return _last;
}
