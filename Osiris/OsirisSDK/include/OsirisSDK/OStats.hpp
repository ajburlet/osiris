#pragma once

#include <list>
#include <cmath>
#include "defs.h"

#ifndef OSTATS_DEFAULT_SAMPLE_SIZE
#define OSTATS_DEFAULT_SAMPLE_SIZE	5
#endif

/**
 @brief Template class to handle statistical data.
 */
template <class VType> class OStats
{
public:
	/**
	 @brief Class constructor.
	 @param sampleSize Number of samples considered to provide statistical data.
	 */
	OStats(int sampleSize=OSTATS_DEFAULT_SAMPLE_SIZE);

	/**
	 @brief Class destructor.
	 */
	virtual ~OStats();

	/**
	 @brief Set sample size.
	 */
	void setSampleSize(int size);

	/**
	 @brief Returns sample size.
	 */
	int sampleSize() const;

	/**
	 @brief Add new entry to the ensamble.
	 */
	void add(VType val);

	/**
	 @brief Average value.
	 */
	float average() const;

	/**
	 @brief Standard deviation.
	 */
	float stdev() const;

	/**
	 @brief Minimum value.
	 */
	VType minimum() const;

	/**
	 @brief Maximum value. 
	 */
	VType maximum() const;

private:
	int _sampleSize;
	std::list<VType> _samples;
	VType _sum;
	VType _sumsq;
};

template<class VType>
inline OStats<VType>::OStats(int sampleSize) :
	_sampleSize(sampleSize),
	_sum(0),
	_sumsq(0)
{
}

template<class VType>
inline OStats<VType>::~OStats()
{
}

template<class VType>
inline void OStats<VType>::setSampleSize(int size)
{
	while (_samples.size() > size) _samples.pop_front();
	_sampleSize = size;
}

template<class VType>
inline int OStats<VType>::sampleSize() const
{
	return _sampleSize;
}

template<class VType>
inline void OStats<VType>::add(VType val)
{
	_sum += val;
	_sumsq += val*val;
	if (_samples.size() == _sampleSize) {
		_sum -= _samples.front();
		_sumsq -= _samples.front()*_samples.front();
		_samples.pop_front();
	}
	_samples.push_back(val);
}

template<class VType>
inline float OStats<VType>::average() const
{
	return (float)_sum/_samples.size();
}

template<class VType>
inline float OStats<VType>::stdev() const
{
	return sqrt(abs(_sum*_sum - _sumsq))/_samples.size();
}

template<class VType>
inline VType OStats<VType>::minimum() const
{
	bool first=true;
	VType min;
	for (std::list<VType>::iterator it = _samples.begin(); it != _samples.end(); it++) {
		if (first || *it < min) min = *it;
		first = false;
	}
	return min;
}

template<class VType>
inline VType OStats<VType>::maximum() const
{
	bool first=true;
	VType max;
	for (std::list<VType>::iterator it = _samples.begin(); it != _samples.end(); it++) {
		if (first || *it > max) max = *it;
		first = false;
	}
	return max;
}
