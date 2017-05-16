#include "OsirisSDK/OTimeIndex.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#ifdef WIN32
LARGE_INTEGER OTimeIndex::_wFrequency;
LARGE_INTEGER OTimeIndex::_wStartingStamp;
#else
OTimeIndex OTimeIndex::_uStartingStamp;
#endif

OTimeIndex::OTimeIndex()
{
}

OTimeIndex::OTimeIndex(int sec, int usec) :
	_sec(sec),
	_usec(usec)
{
}


OTimeIndex::OTimeIndex(long long timeIndex_us)
{
	setTimeFromInteger(timeIndex_us);
}

OTimeIndex::~OTimeIndex()
{
}

inline OTimeIndex OTimeIndex::operator-(const OTimeIndex & in) const
{
	OTimeIndex res(*this);
	res.operator-=(in);
	return res;
}

inline OTimeIndex OTimeIndex::operator-(int in) const
{
	return this->operator-(OTimeIndex(in));
}

inline OTimeIndex & OTimeIndex::operator-=(const OTimeIndex & in)
{
	_sec -= in._sec;
	_usec -= in._usec;
	if (_usec < 0) {
		_sec--;
		_usec = 1 - _usec;
	}
	return *this;
}

inline OTimeIndex & OTimeIndex::operator-=(int in)
{
	return operator-=(OTimeIndex(in));
}

inline OTimeIndex OTimeIndex::operator+(const OTimeIndex & in) const
{
	OTimeIndex res(*this);
	res.operator+=(in);
	return res;
}

inline OTimeIndex OTimeIndex::operator+(int in) const
{
	return operator+(OTimeIndex(in));
}

inline OTimeIndex & OTimeIndex::operator+=(const OTimeIndex & in)
{
	_sec += in._sec;
	_usec += in._usec;
	if (_usec > 1000000) {
		_sec %= 1000000;
		_usec %= 1000000;
	}
	return *this;
}

inline OTimeIndex & OTimeIndex::operator+=(int in)
{
	return operator+=(OTimeIndex(in));
}

inline OTimeIndex & OTimeIndex::operator=(int in)
{
	setTimeFromInteger(in);
	return *this;
}

inline bool OTimeIndex::operator<(const OTimeIndex & in) const
{
	if (_sec < in._sec) return true;
	else if (_sec == in._sec && _usec < in._usec) return true;
	return false;
}

inline bool OTimeIndex::operator<(int in) const
{
	return operator<(OTimeIndex(in));
}

inline bool OTimeIndex::operator>(const OTimeIndex & in) const
{
	if (_sec > in._usec) return true;
	else if (_sec == _usec && _usec > in._usec) return true;
	return false;
}

inline bool OTimeIndex::operator>(int in) const
{
	return operator>(OTimeIndex(in));
}

inline bool OTimeIndex::operator<=(const OTimeIndex & in) const
{
	if (_sec < in._sec) return true;
	else if (_sec == in._sec && _usec <= in._usec) return true;
	return false;
}

inline bool OTimeIndex::operator<=(int in) const
{
	return operator<=(OTimeIndex(in));
}

inline bool OTimeIndex::operator>=(const OTimeIndex & in) const
{
	if (_sec > in._usec) return true;
	else if (_sec == _usec && _usec >= in._usec) return true;
	return false;
}

inline bool OTimeIndex::operator>=(int in) const
{
	return operator>=(OTimeIndex(in));
}

void OTimeIndex::setValue(int sec, int uSec)
{
	_sec = sec;
	_usec = uSec;
}

int OTimeIndex::sec() const
{
	return _sec;
}

int OTimeIndex::uSec() const
{
	return _usec;
}

int OTimeIndex::toInt() const
{
	return _sec*1000000 + _usec;
}

void OTimeIndex::init()
{
#ifdef WIN32
	QueryPerformanceFrequency(&_wFrequency);
	QueryPerformanceCounter(&_wStartingStamp);
#else
	struct timeval tv;
	gettimeofday(&tv);
	_uStartingStamp.setValue(tv.tv_sec, tv.tv_usec);
#endif
}

OTimeIndex OTimeIndex::current()
{
#ifdef WIN32
	LARGE_INTEGER currStamp;
	QueryPerformanceCounter(&currStamp);
	long long diff = (long long)(currStamp.QuadPart - _wStartingStamp.QuadPart) * 1000000 / _wFrequency.QuadPart;
	return OTimeIndex(diff);
#else
	OTimeStamp currStamp;
	struct timeval tv;
	gettimeofday(&tv);
	currStamp.setValue(tv.tv_sec, tv.tv_usec);
	return currStamp - _uStartingStamp;
#endif
}

void OTimeIndex::setTimeFromInteger(long long timeIndex)
{
	_sec = (int)(timeIndex / 1000000);
	_usec = (int)(timeIndex % 1000000);
}
