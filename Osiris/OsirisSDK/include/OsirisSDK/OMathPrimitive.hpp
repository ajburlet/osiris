#pragma once

#include "defs.h"
#include "GLdefs.h"

/**
 \brief Template class for the GLM-based classes in OMath.
 */
template <class MType> class OAPI OMathPrimitive
{
public:
	/**
	 \brief Class constructor.
	 */
	OMathPrimitive();
	
	/**
	 \brief Copy constructor.
	 */
	OMathPrimitive(const OMathPrimitive<MType>& in);

#ifdef OSIRISSDK_EXPORTS
	/**
	 \brief Class constructor from GLM object (for internal usage only).
	 \param in GLM object.
	 */
	OMathPrimitive(const MType& in);
#endif
	
	/**
	 \brief Class destructor.
	 */
	virtual ~OMathPrimitive();

	/**
	 \brief Creates a normalized object out of the current one.
	*/
	OMathPrimitive<MType> normalize();

	/**
	 \brief Size of the object.
	 */
	float magnitude() const;

	OMathPrimitive<MType>& operator=(const OMathPrimitive<MType>& in);
	OMathPrimitive<MType>& operator*=(const OMathPrimitive<MType>& in);
	OMathPrimitive<MType> operator*(const OMathPrimitive<MType>& in) const;
	OMathPrimitive<MType>& operator+=(const OMathPrimitive<MType>& in);
	OMathPrimitive<MType> operator+(const OMathPrimitive<MType>& in) const;
	OMathPrimitive<MType>& operator-=(const OMathPrimitive<MType>& in);
	OMathPrimitive<MType> operator-(const OMathPrimitive<MType>& in) const;

	OMathPrimitive<MType>& operator*=(float x);
	virtual OMathPrimitive<MType> operator*(float x) const;
	OMathPrimitive<MType>& operator/=(float x);
	virtual OMathPrimitive<MType> operator/(float x) const;
	
	bool operator==(const OMathPrimitive<MType>& in) const;
	bool operator!=(const OMathPrimitive<MType>& in) const;

#ifdef OSIRISSDK_EXPORTS
	OMathPrimitive<MType>& operator=(const MType& in);
	OMathPrimitive<MType>& operator*=(const MType& in);
	OMathPrimitive<MType> operator*(const MType& in);
	bool operator==(const MType& in) const;
	bool operator!=(const MType& in) const;

	/**
	 \brief Set internal GLM object. For internal library usage only.
	 \param glm New internal GLM object.
	 */
	void setGlm(const MType& glm);

	/**
	 \brief Access to the internal GLM object.
	 \return Internal GLM object.
	 */
	const MType & glm() const;
#endif

	/**
	 \brief Returns a pointer to the memory area to be used by OpenGL. For internal library usage only.
	*/
	const GLfloat* glArea() const;

protected:
	MType _glmInternal;
};


#ifdef OSIRISSDK_EXPORTS
template<class MType>
inline OMathPrimitive<MType>::OMathPrimitive()
{
}

template<class MType>
inline OMathPrimitive<MType>::OMathPrimitive(const OMathPrimitive<MType>& in) :
	_glmInternal(in._glmInternal)
{
}

#ifdef OSIRISSDK_EXPORTS
template<class MType>
inline OMathPrimitive<MType>::OMathPrimitive(const MType & in) : 
	_glmInternal(in)
{
}
#endif

template<class MType>
inline OMathPrimitive<MType>::~OMathPrimitive()
{
}

template<class MType>
inline OMathPrimitive<MType> OMathPrimitive<MType>::normalize()
{
	return OMathPrimitive<MType>(glm::normalize(_glmInternal));
}

template<class MType>
inline float OMathPrimitive<MType>::magnitude() const
{
	return glm::length(_glmInternal);
}

template<class MType>
inline OMathPrimitive<MType> & OMathPrimitive<MType>::operator=(const OMathPrimitive<MType>& in)
{
	_glmInternal = in._glmInternal;
	return *this;
}

template<class MType>
inline OMathPrimitive<MType> & OMathPrimitive<MType>::operator*=(const OMathPrimitive<MType>& in)
{
	_glmInternal *= in._glmInternal;
	return *this;
}

template<class MType>
inline OMathPrimitive<MType> OMathPrimitive<MType>::operator*(const OMathPrimitive<MType>& in) const
{
	OMathPrimitive<MType> res;
	res._glmInternal = this->_glmInternal * in._glmInternal;
	return res;
}

template<class MType>
inline OMathPrimitive<MType> & OMathPrimitive<MType>::operator+=(const OMathPrimitive<MType>& in)
{
	_glmInternal += in._glmInternal;
	return *this;
}

template<class MType>
inline OMathPrimitive<MType> OMathPrimitive<MType>::operator+(const OMathPrimitive<MType>& in) const
{
	OMathPrimitive<MType> res;
	res._glmInternal = this->_glmInternal + in._glmInternal;
	return res;
}

template<class MType>
inline OMathPrimitive<MType> & OMathPrimitive<MType>::operator-=(const OMathPrimitive<MType>& in)
{
	_glmInternal -= in._glmInternal;
	return *this;
}

template<class MType>
inline OMathPrimitive<MType> OMathPrimitive<MType>::operator-(const OMathPrimitive<MType>& in) const
{
	OMathPrimitive<MType> res;
	res._glmInternal = this->_glmInternal - in._glmInternal;
	return res;
}

template<class MType>
inline OMathPrimitive<MType>& OMathPrimitive<MType>::operator*=(float x)
{
	_glmInternal *= x;
	return *this;
}

template<class MType>
inline OMathPrimitive<MType> OMathPrimitive<MType>::operator*(float x) const
{
	OMathPrimitive<MType> res;
	res._glmInternal = _glmInternal * x;
	return res;
}

template<class MType>
inline OMathPrimitive<MType>& OMathPrimitive<MType>::operator/=(float x)
{
	_glmInternal /= x;
	return *this;
}

template<class MType>
inline OMathPrimitive<MType> OMathPrimitive<MType>::operator/(float x) const
{
	OMathPrimitive<MType> res;
	res._glmInternal = _glmInternal / x;
	return res;
}

template<class MType>
inline bool OMathPrimitive<MType>::operator==(const OMathPrimitive<MType>& in) const
{
	return (_glmInternal == in._glmInternal);
}

template<class MType>
inline bool OMathPrimitive<MType>::operator!=(const OMathPrimitive<MType>& in) const
{
	return (_glmInternal * in._glmInternal);
}

template<class MType>
inline OMathPrimitive<MType> & OMathPrimitive<MType>::operator=(const MType & in)
{
	_glmInternal = in;
	return *this;
}

template<class MType>
inline OMathPrimitive<MType> & OMathPrimitive<MType>::operator*=(const MType & in)
{
	_glmInternal *= in;
	return *this;
}

template<class MType>
inline OMathPrimitive<MType> OMathPrimitive<MType>::operator*(const MType & in)
{
	OMathPrimitive<MType> res;
	res = _glmInternal * in;
	return *this;
}

template<class MType>
inline bool OMathPrimitive<MType>::operator==(const MType & in) const
{
	return (_glmInternal == in);
}

template<class MType>
inline bool OMathPrimitive<MType>::operator!=(const MType & in) const
{
	return (_glmInternal != in);
}

template<class MType>
inline void OMathPrimitive<MType>::setGlm(const MType & glm)
{
	_glmInternal = glm;
}

template<class MType>
inline const MType & OMathPrimitive<MType>::glm() const
{
	return _glmInternal;
}

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template<class MType>
inline const GLfloat * OMathPrimitive<MType>::glArea() const
{
	return glm::value_ptr(_glmInternal);
}
#endif

