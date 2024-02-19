#pragma once

#include "OsirisSDK/OException.h"

/**
 @brief Singleton container class.
 @note The class must have a default class constructor.
 */
template <class T>
class OSingleton 
{
public:
	/**
	 @brief Concealed class constructor.
	 */
	OSingleton() = default;

	/**
	 @brief Class destructor.
	 */
	virtual ~OSingleton() = default;

	/**
	 @brief Access pointer operator.
	 */
	T* operator->();

	/**
	 @brief Const access pointer operator.
	 */
	const T* operator->() const;

private:
	/**
	 @brief Checks if instance exists and creates it if necessary.
	 */
	static void CheckInstance();

private:
	static T* _instance;
};
template<class T>
T* OSingleton<T>::_instance = nullptr;


template<class T>
inline T * OSingleton<T>::operator->()
{
	CheckInstance();
	return _instance;
}

template<class T>
inline const T * OSingleton<T>::operator->() const
{
	CheckInstance();
	return _instance;
}

template<class T>
inline void OSingleton<T>::CheckInstance()
{
	if (!_instance) {
		if ((_instance = new T()) == nullptr) {
			throw OEx("Unable to create singleton object.");
		}
	}
}
