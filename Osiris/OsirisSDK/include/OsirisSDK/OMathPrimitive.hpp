#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/vector_angle.hpp>     

#include "defs.h"
#include "GLdefs.h"

/**
 \brief Template class for the GLM-based classes in OMath.
 */
template <class DerivedType, class MType> 
class OMathPrimitive
{
public:
	/**
	 @brief The underlying GLM object type alias.
	 */
	using GLMType = typename MType;

	/**
	 @brief Value type.
	 */
	using ValueType = typename MType::value_type;

	/**
	 \brief Class constructor.
	 */
	OMathPrimitive();
	
	/**
	 \brief Copy constructor.
	 */
	OMathPrimitive(const OMathPrimitive<DerivedType, MType>& in);

	/**
	 \brief Class constructor from GLM object (for internal usage only).
	 \param in GLM object.
	 */
	OMathPrimitive(const MType& in);
	
	/**
	 \brief Class destructor.
	 */
	virtual ~OMathPrimitive();

	/**
	 \brief Creates a normalized object out of the current one.
	*/
	DerivedType normalize() const;

	/**
	 \brief Size of the object.
	 */
	float magnitude() const;

	DerivedType& operator=(const DerivedType& in);
	DerivedType& operator*=(const DerivedType& in);
	DerivedType operator*(const DerivedType& in) const;
	DerivedType& operator+=(const DerivedType& in);
	DerivedType operator+(const DerivedType& in) const;
	DerivedType& operator-=(const DerivedType& in);
	DerivedType operator-(const DerivedType& in) const;

	DerivedType& operator*=(ValueType x);
	virtual DerivedType operator*(ValueType x) const;
	DerivedType& operator/=(ValueType x);
	virtual DerivedType operator/(ValueType x) const;
	
	bool operator==(const DerivedType& in) const;
	bool operator!=(const DerivedType& in) const;

	DerivedType& operator=(const MType& in);
	DerivedType& operator*=(const MType& in);
	DerivedType operator*(const MType& in);
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

	/**
	 \brief Returns a pointer to the memory area to be used by OpenGL. For internal library usage only.
	*/
	const GLfloat* glArea() const;

protected:
	MType _glmInternal;
};


template<class DerivedType, class MType>
inline OMathPrimitive<DerivedType, MType>::OMathPrimitive()
{
}

template<class DerivedType, class MType>
inline OMathPrimitive<DerivedType, MType>::OMathPrimitive(const OMathPrimitive<DerivedType, MType>& in) :
	_glmInternal(in._glmInternal)
{
}

template<class DerivedType, class MType>
inline OMathPrimitive<DerivedType, MType>::OMathPrimitive(const MType & in) : 
	_glmInternal(in)
{
}

template<class DerivedType, class MType>
inline OMathPrimitive<DerivedType, MType>::~OMathPrimitive()
{
}

template<class DerivedType, class MType>
inline DerivedType 
OMathPrimitive<DerivedType, MType>::normalize() const
{
	return DerivedType(glm::normalize(_glmInternal));
}

template<class DerivedType, class MType>
inline float OMathPrimitive<DerivedType, MType>::magnitude() const
{
	return glm::length(_glmInternal);
}

template<class DerivedType, class MType>
inline DerivedType& 
OMathPrimitive<DerivedType, MType>::operator=(const DerivedType& in)
{
	_glmInternal = in._glmInternal;
	return *this;
}

template<class DerivedType, class MType>
inline DerivedType& 
OMathPrimitive<DerivedType, MType>::operator*=(const DerivedType& in)
{
	_glmInternal *= in._glmInternal;
	return *reinterpret_cast<DerivedType*>(this);
}

template<class DerivedType, class MType>
inline DerivedType 
OMathPrimitive<DerivedType, MType>::operator*(const DerivedType& in) const
{
	DerivedType res;
	res._glmInternal = this->_glmInternal * in._glmInternal;
	return res;
}

template<class DerivedType, class MType>
inline DerivedType& 
OMathPrimitive<DerivedType, MType>::operator+=(const DerivedType& in)
{
	_glmInternal += in._glmInternal;
	return *reinterpret_cast<DerivedType*>(this);
}

template<class DerivedType, class MType>
inline DerivedType 
OMathPrimitive<DerivedType, MType>::operator+(const DerivedType& in) const
{
	DerivedType res;
	res._glmInternal = this->_glmInternal + in._glmInternal;
	return res;
}

template<class DerivedType, class MType>
inline DerivedType& 
OMathPrimitive<DerivedType, MType>::operator-=(const DerivedType& in)
{
	_glmInternal -= in._glmInternal;
	return *reinterpret_cast<DerivedType*>(this);
}

template<class DerivedType, class MType>
inline DerivedType 
OMathPrimitive<DerivedType, MType>::operator-(const DerivedType& in) const
{
	DerivedType res;
	res._glmInternal = this->_glmInternal - in._glmInternal;
	return res;
}

template<class DerivedType, class MType>
inline DerivedType& 
OMathPrimitive<DerivedType, MType>::operator*=(ValueType x)
{
	_glmInternal *= x;
	return *reinterpret_cast<DerivedType*>(this);
}

template<class DerivedType, class MType>
inline DerivedType 
OMathPrimitive<DerivedType, MType>::operator*(ValueType x) const
{
	DerivedType res;
	res._glmInternal = _glmInternal * x;
	return res;
}

template<class DerivedType, class MType>
inline DerivedType& 
OMathPrimitive<DerivedType, MType>::operator/=(ValueType x)
{
	_glmInternal /= x;
	return *this;
}

template<class DerivedType, class MType>
inline DerivedType 
OMathPrimitive<DerivedType, MType>::operator/(ValueType x) const
{
	DerivedType res;
	res._glmInternal = _glmInternal / x;
	return res;
}

template<class DerivedType, class MType>
inline bool OMathPrimitive<DerivedType, MType>::operator==(const DerivedType& in) const
{
	return (_glmInternal == in._glmInternal);
}

template<class DerivedType, class MType>
inline bool OMathPrimitive<DerivedType, MType>::operator!=(const DerivedType& in) const
{
	return (_glmInternal != in._glmInternal);
}

template<class DerivedType, class MType>
inline DerivedType & OMathPrimitive<DerivedType, MType>::operator=(const MType & in)
{
	_glmInternal = in;
	return *this;
}

template<class DerivedType, class MType>
inline DerivedType & OMathPrimitive<DerivedType, MType>::operator*=(const MType & in)
{
	_glmInternal *= in;
	return *reinterpret_cast<DerivedType*>(this);
}

template<class DerivedType, class MType>
inline DerivedType OMathPrimitive<DerivedType, MType>::operator*(const MType & in)
{
	DerivedType res;
	res = _glmInternal * in;
	return res;
}

template<class DerivedType, class MType>
inline bool OMathPrimitive<DerivedType, MType>::operator==(const MType & in) const
{
	return (_glmInternal == in);
}

template<class DerivedType, class MType>
inline bool OMathPrimitive<DerivedType, MType>::operator!=(const MType & in) const
{
	return (_glmInternal != in);
}

template<class DerivedType, class MType>
inline void OMathPrimitive<DerivedType, MType>::setGlm(const MType & glm)
{
	_glmInternal = glm;
}

template<class DerivedType, class MType>
inline const MType & OMathPrimitive<DerivedType, MType>::glm() const
{
	return _glmInternal;
}

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template<class DerivedType, class MType>
inline const GLfloat * OMathPrimitive<DerivedType, MType>::glArea() const
{
	return glm::value_ptr(_glmInternal);
}

