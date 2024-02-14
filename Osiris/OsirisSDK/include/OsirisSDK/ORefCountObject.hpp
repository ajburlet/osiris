#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OException.h"

/**
 @brief Base reference countable object.
 */
class OBaseRefCountObject
{
public:
	/**
	 @brief Retains an object reference.
	 */
	virtual void retain() = 0;

	/**
	 @brief Releases an object reference.
	 */
	virtual void release() = 0;

	/**
	 @brief Returns the reference count.
	 */
	virtual uint32_t referenceCount() const = 0;
};

/**
 @brief Reference countable object.
 @tparam CountType The type of the variable that tracks the reference count. 
 */
template <typename CountType=uint32_t>
class ORefCountObject : public OBaseRefCountObject
{
public:
	/**
	 @brief Default class constructor.
	 */
	ORefCountObject() = default;

	/**
	 @brief Class destructor.
	 */
	virtual ~ORefCountObject();

	virtual void retain() override;

	virtual void release() override;

	virtual uint32_t referenceCount() const override;

private:
	CountType _refCount = 0;
};

template<typename CountType>
inline ORefCountObject<CountType>::~ORefCountObject()
{
	//assert(_refCount > 0);
}

template<typename CountType>
inline void ORefCountObject<CountType>::retain()
{
	++_refCount;
}

template<typename CountType>
inline void ORefCountObject<CountType>::release()
{
	--_refCount;
}

template<typename CountType>
inline uint32_t ORefCountObject<CountType>::referenceCount() const
{
	return static_cast<uint32_t>(_refCount);
}


/**
 @brief Smart pointer-like class to automatically track reference count.
 @tparam The class of the reference countable object.
 */
template <class RefCountT>
class ORefCountPtr
{
public:
	/**
	 @brief Default class constructor.
	 */
	ORefCountPtr() = default;

	/**
	 @brief Class constructor.
	 @param aPtr The pointer to be tracked.
	 */
	ORefCountPtr(RefCountT* aPtr);

	/**
	 @brief Class copy constructor.
	 */
	ORefCountPtr(const ORefCountPtr& aOther);

	/**
	 @brief Move constructor.
	 */
	ORefCountPtr(ORefCountPtr&& aOther);

	/**
	 @brief Class destructor.
	 */
	~ORefCountPtr();

	/**
	 @brief Copy assignment operator.
	 */
	ORefCountPtr& operator=(const ORefCountPtr& aOther);

	/**
	 @brief Move assignment operator.
	 */
	ORefCountPtr& operator=(ORefCountPtr&& aOther);

	/**
	 @brief Assignment operator.
	 */
	ORefCountPtr& operator=(RefCountT* aPtr);

	/**
	 @brief Pointer operator.
	 */
	RefCountT* operator->();

	/**
	 @brief Deref operator.
	 */
	RefCountT& operator*();

	/**
	 @copydoc operator->()
	 */
	const RefCountT* operator->() const;

	/**
	 @copydoc operator*()
	 */
	const RefCountT& operator*() const;

	/**
	 @brief Equality operator.
	 */
	bool operator==(const ORefCountPtr& aOther) const;
	
	/**
	 @brief Equality operator.
	 */
	bool operator==(const RefCountT* aOther) const;

	/**
	 @brief Inequality operator.
	 */
	bool operator!=(const ORefCountPtr& aOther) const;

	/**
	 @brief Inequality operator.
	 */
	bool operator!=(const RefCountT& aOther) const;

	/**
	 @brief Returns true if internal pointer is null.
	 */
	bool isNull() const;

	/**
	 @brief Returns the internal pointer.
	 */
	RefCountT* getPtr();

	/**
	 @copydoc getPtr()
	 */
	const RefCountT* getPtr() const;

	/**
	 brief Sets the internal pointer value.
	 */
	void setPtr(RefCountT* aPtr);

	/**
	 @brief Allocates new object and wraps.
	 */
	template <typename... Args>
	static ORefCountPtr create(Args&&... aArgs);

private:
	RefCountT*	_ptr = nullptr;
};

template<class RefCountT>
inline ORefCountPtr<RefCountT>::ORefCountPtr(RefCountT * aPtr) : _ptr(aPtr)
{
	if (aPtr != nullptr) aPtr->retain();
}

template<class RefCountT>
inline ORefCountPtr<RefCountT>::ORefCountPtr(const ORefCountPtr & aOther) : _ptr(aOther._ptr)
{
	if (aOther._ptr != nullptr) aOther._ptr->retain();
}

template<class RefCountT>
inline ORefCountPtr<RefCountT>::ORefCountPtr(ORefCountPtr && aOther) : _ptr(aOther._ptr)
{
	aOther._ptr = nullptr;
}

template<class RefCountT>
inline ORefCountPtr<RefCountT>::~ORefCountPtr()
{
	if (_ptr) _ptr->release();
}

template<class RefCountT>
inline ORefCountPtr<RefCountT> & ORefCountPtr<RefCountT>::operator=(const ORefCountPtr & aOther)
{
	setPtr(aOther._ptr);
	return *this;
}

template<class RefCountT>
inline ORefCountPtr<RefCountT> & ORefCountPtr<RefCountT>::operator=(ORefCountPtr && aOther)
{
	if (_ptr != nullptr) _ptr->release();
	_ptr = aOther._ptr;
	aOther._ptr = nullptr;
	return *this;
}

template<class RefCountT>
inline ORefCountPtr<RefCountT> & ORefCountPtr<RefCountT>::operator=(RefCountT * aPtr)
{
	set(aPtr);
	return *this;
}

template<class RefCountT>
inline RefCountT * ORefCountPtr<RefCountT>::operator->()
{
	return _ptr;
}

template<class RefCountT>
inline RefCountT & ORefCountPtr<RefCountT>::operator*()
{
	return *_ptr;
}

template<class RefCountT>
inline const RefCountT * ORefCountPtr<RefCountT>::operator->() const
{
	return _ptr;
}

template<class RefCountT>
inline const RefCountT & ORefCountPtr<RefCountT>::operator*() const
{
	return *_ptr;
}

template<class RefCountT>
inline bool ORefCountPtr<RefCountT>::operator==(const ORefCountPtr & aOther) const
{
	return (_ptr == aOther._ptr);
}

template<class RefCountT>
inline bool ORefCountPtr<RefCountT>::operator==(const RefCountT * aOther) const
{
	return (_ptr == aOther);
}

template<class RefCountT>
inline bool ORefCountPtr<RefCountT>::operator!=(const ORefCountPtr & aOther) const
{
	return !operator==(aOther);
}

template<class RefCountT>
inline bool ORefCountPtr<RefCountT>::operator!=(const RefCountT & aOther) const
{
	return !operator==(aOther);
}

template<class RefCountT>
inline bool ORefCountPtr<RefCountT>::isNull() const
{
	return (_ptr == nullptr);
}

template<class RefCountT>
inline RefCountT * ORefCountPtr<RefCountT>::getPtr()
{
	return _ptr;
}

template<class RefCountT>
inline const RefCountT * ORefCountPtr<RefCountT>::getPtr() const
{
	return _ptr;
}

template<class RefCountT>
inline void ORefCountPtr<RefCountT>::setPtr(RefCountT * aPtr)
{
	if (_ptr != nullptr) _ptr->release();
	_ptr = aPtr;
	if (_ptr != nullptr) _ptr->retain();
}

template<class RefCountT>
template<typename ...Args>
inline ORefCountPtr<RefCountT> ORefCountPtr<RefCountT>::create(Args && ...aArgs)
{
	return ORefCountPtr(new RefCountT(std::move(aArgs)...));
}
