#pragma once

#include "defs.h"
#include "GLdefs.h"

/**
 \brief Template class for the GLM-based classes in OMath.
 */
template <class MType> class OAPI OMathPrimitive
{
public:
	OMathPrimitive();
	OMathPrimitive(const OMathPrimitive<MType>& in);
	virtual ~OMathPrimitive();

	OMathPrimitive<MType>& operator=(const OMathPrimitive<MType>& in);
	OMathPrimitive<MType>& operator*=(const OMathPrimitive<MType>& in);
	OMathPrimitive<MType> operator*(const OMathPrimitive<MType>& in);
	bool operator==(const OMathPrimitive<MType>& in) const;
	bool operator!=(const OMathPrimitive<MType>& in) const;

#ifdef OSIRISSDK_EXPORTS
	OMathPrimitive<MType>& operator=(const MType& in);
	OMathPrimitive<MType>& operator*=(const MType& in);
	OMathPrimitive<MType> operator*(const MType& in);
	bool operator==(const MType& in) const;
	bool operator!=(const MType& in) const;
	const MType & glm() const;
#endif

	const GLfloat* glArea() const;

protected:
	MType _glmInternal;
};


#ifdef OSIRISSDK_EXPORTS
/**
 \brief Class constructor.
 */
template<class MType>
inline OMathPrimitive<MType>::OMathPrimitive()
{
}

/**
 \brief Copy constructor.
 */
template<class MType>
inline OMathPrimitive<MType>::OMathPrimitive(const OMathPrimitive<MType>& in) :
	_glmInternal(in._glmInternal)
{
}

/**
 \brief Class destructor.
 */
template<class MType>
inline OMathPrimitive<MType>::~OMathPrimitive()
{
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
inline OMathPrimitive<MType> OMathPrimitive<MType>::operator*(const OMathPrimitive<MType>& in)
{
	OMathPrimitive<MType> res;
	res._glmInternal = this->_glmInternal * in._glmInternal;
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

/**
 \brief Access to the internal GLM object.
 \return Internal GLM object.
 */
template<class MType>
inline const MType & OMathPrimitive<MType>::glm() const
{
	return _glmInternal;
}

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 \brief Returns a pointer to the memory area to be used by OpenGL.
*/
template<class MType>
inline const GLfloat * OMathPrimitive<MType>::glArea() const
{
	return glm::value_ptr(_glmInternal);
}
#endif

