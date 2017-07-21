#pragma once

#ifdef WIN32
#include "windows.h"
#endif

#include "defs.h"

/**
 @brief Deals with the elapsed simulation time.
 */
class OAPI OTimeIndex
{
public:
	/**
	 @brief Class constructor.
	 */
	OTimeIndex();

	/**
	 @brief Class constructor.
	 @param sec Time in seconds.
	 @param usec Additional time in microseconds. 
	 */
	OTimeIndex(int sec, int usec);

	/**
	 @brief Class constructor.
	 @param timeIndex_ms Time index in microseconds.
	 */
	OTimeIndex(long long timeIndex_us);

	/**
	 @brief Class destructor.
	 */
	virtual ~OTimeIndex();

	/**
	 @brief Subtraction operator.
	 */
	inline OTimeIndex operator-(const OTimeIndex& in) const;
	
	/**
	 @brief Subtraction operator.
	 @param in Time index in microseconds.
	 */
	inline OTimeIndex operator-(int in) const;
	
	/**
	 @brief Subtraction operator.
	 */
	inline OTimeIndex& operator-=(const OTimeIndex& in);
	
	/**
	 @brief Subtraction operator.
	 @param in Time index in microseconds.
	 */
	inline OTimeIndex& operator-=(int in);

	/**
	 @brief Addition operator.
	 */
	inline OTimeIndex operator+(const OTimeIndex& in) const;
	
	/**
	 @brief Addition operator.
	 @param in Time index in microseconds.
	 */
	inline OTimeIndex operator+(int in) const;

	/**
	 @brief Addition operator.
	 */
	inline OTimeIndex& operator+=(const OTimeIndex& in);
	
	/**
	 @brief Addition operator.
	 @param in Time index in microseconds.
	 */
	inline OTimeIndex& operator+=(int in);

	/**
	 @brief Assignment operator.
	 @param in Time index in microseconds.
	 */
	inline OTimeIndex& operator=(int in);

	/**
	 @brief Less than comparison operator.
	 */
	inline bool operator<(const OTimeIndex& in) const;
	
	/**
	 @brief Less than comparison operator.
	 @param in Time index in microseconds.
	 */
	inline bool operator<(int in) const;

	/**
	 @brief Greater than comparison operator.
	 */
	inline bool operator>(const OTimeIndex& in) const;

	/**
	 @brief Greater than comparison operator.
	 @param in Time index in microseconds.
	 */
	inline bool operator>(int in) const;

	/**
	 @brief Less than or equal comparison operator.
	 */
	inline bool operator<=(const OTimeIndex& in) const;

	/**
	 @brief Less than or equal comparison operator.
	 @param in Time index in microseconds.
	 */
	inline bool operator<=(int in) const;

	/**
	 @brief Greater than or equal comparison operator.
	 */
	inline bool operator>=(const OTimeIndex& in) const;

	/**
	 @brief Greater than or equal comparison operator.
	 @param in Time index in microseconds.
	 */
	inline bool operator>=(int in) const;

	/**
	 @brief Equals comparison operator.
	 */
	inline bool operator==(const OTimeIndex& in) const;

	/**
	 @brief Equal comparison operator.
	 @brief in Time index in microseconds.
	 */
	inline bool operator==(int in) const;

	/**
	 @brief Not equal comparison operator.
	 */
	inline bool operator!=(const OTimeIndex& in) const;

	/**
	 @brief Not equal comparison operator.
	 @brief in Time index in microseconds.
	 */
	inline bool operator!=(int in) const;

	/**
	 @brief Set time index component values.
	 @param sec Time in seconds.
	 @param usec Additional time in microseconds. 
	 */
	void setValue(int sec, int uSec);

	/**
	 @brief Returns the time index as an integer in seconds.
	 */
	int sec() const;

	/**
	 @brief Returns the additional microseconds part of the index.
	 */
	int uSec() const;

	/**
	 @brief Converts the time index to an integer in microseconds.
	 */
	int toInt() const;

	/**
	 @brief Class initialization method.
	 */
	static void init();

	/**
	 @brief Returns current time index.
	 */
	static OTimeIndex current();

private:
	unsigned int _sec;
	unsigned int _usec;

#ifdef WIN32
	static LARGE_INTEGER _wFrequency;
	static LARGE_INTEGER _wStartingStamp;
#else
	OTimeIndex _uStartingStamp;
#endif
	void setTimeFromInteger(long long timeIndex);
};
