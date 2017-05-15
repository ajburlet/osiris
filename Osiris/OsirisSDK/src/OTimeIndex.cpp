#include "OsirisSDK/OTimeIndex.h"

OTimeIndex::OTimeIndex()
{
}

OTimeIndex::OTimeIndex(int sec, int usec) :
	_sec(sec),
	_usec(usec)
{
}


OTimeIndex::OTimeIndex(int timeIndex_ms)
{
	setTimeFromInteger(timeIndex_ms);
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

void OTimeIndex::setTimeFromInteger(int timeIndex)
{
	_sec = timeIndex / 1000000;
	_usec = timeIndex % 1000000;
}
