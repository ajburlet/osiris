#pragma once

#include <cmath>

#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec4.hpp>

#include "OsirisSDK/OMath.h"
#include "OsirisSDK/OMathPrimitive.hpp"
#include "OsirisSDK/OVectorDefs.h"
#include "OsirisSDK/OException.h"

/**
 @brief Base template vector class.
 */
template <uint8_t Dim, typename T, glm::qualifier Q = glm::defaultp>
class OVector : public OMathPrimitive<glm::vec<Dim, T, Q>>
{
public:
	/**
	 @brief Default class constructor.
	 */
	OVector() = default;
	
	/**
	 @brief Copy constructor for the OMathPrimitive superclass.
	 */
	OVector(const Super& aOther);
	
	/**
	 @brief Constructor for the base GLM type.
	 */
	OVector(const GLMType& aGLM);

	/**
	 @brief Class destructor.
	 */
	virtual ~OVector() = default;

	/**
	 @brief Returns a vector component.
	 @param aComponent The component identifier.
	 */
	virtual T& getComponent(OVectorAxis aComponent) = 0;

	/**
	 @brief Returns a vector component.
	 @param aComponent The component identifier.
	 */
	virtual const T& getComponent(OVectorAxis aComponent) const;

	/**
	 @brief Access operator for the vector components.
	 */
	T& operator[](OVectorAxis aComponent);

	/**
	 @brief Const access operator for the vector components.
	 */
	const T& operator[](OVectorAxis aComponent) const;
};

template<uint8_t Dim, typename T, glm::qualifier Q>
inline OVector<Dim, T, Q>::OVector(const Super & aOther) :
	Super(aOther)
{
}

template<uint8_t Dim, typename T, glm::qualifier Q>
inline OVector<Dim, T, Q>::OVector(const GLMType & aGLM) :
	Super(aGLM)
{
}

template<uint8_t Dim, typename T, glm::qualifier Q>
inline const T & OVector<Dim, T, Q>::getComponent(OVectorAxis aComponent) const
{
	return static_cast<const T&>(const_cast<OVector*>(this)->getComponent(aComponent));
}

template <uint8_t Dim, typename T, glm::qualifier Q>
inline T& OVector<Dim, T, Q>::operator[](OVectorAxis aComponent)
{
	return getComponent(aComponent);
}

template<uint8_t Dim, typename T, glm::qualifier Q>
inline const T & OVector<Dim, T, Q>::operator[](OVectorAxis aComponent) const
{
	return getComponent(aComponent);
}

/**
 @brief Represents two-dimentional vectors.
 */
template <typename T, glm::qualifier Q = glm::defaultp>
class OVector2 : public OVector<2, T, Q>
{
public:
	/**
	 @brief Default class constructor.
	 */
	OVector2() = default;

	/**
	 @brief Class copy constructor.
	 */
	OVector2(const OVector2& aOther);
	
	/**
	 @brief Copy constructor for the OMathPrimitive superclass.
	 */
	OVector2(const Super& aOther);

	/**
	 @brief Class constructor, creates a vector with all components with the same value.
	 @param aValue The value to be used by all of the three components.
	*/
	OVector2(float aValue);
	
	/**
	 @brief Class constructor, creates a vector taking in the values for each component (cartesian or polar). 
	*/
	OVector2(float aX, float aY);

	/**
	 @brief Class destructor.
	 */
	~OVector2() = default;

	/**
	 @copydoc OVector::getComponent(OVectorAxis)
	 */
	T& getComponent(OVectorAxis aComponent) override;

	/**
	 @brief Set the value for the cartesian X axis component.
	*/
	void setX(T aValue);
	
	/**
	 @brief Set the value for the cartesian Y axis component.
	*/
	void setY(T aValue);

	/**
	 @brief Returns x component.
	 */
	T x() const;

	/**
	 @brief Returns y component.
	 */
	T y() const;
};

template<typename T, glm::qualifier Q>
inline OVector2<T, Q>::OVector2(const OVector2 & aOther) :
	OVector(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OVector2<T, Q>::OVector2(const Super & aOther) :
	OVector(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OVector2<T, Q>::OVector2(float aValue) :
	OVector(GLMType(aValue))
{
}

template<typename T, glm::qualifier Q>
inline OVector2<T, Q>::OVector2(float aX, float aY) :
	OVector(GLMType(aX, aY))
{
}

template<typename T, glm::qualifier Q>
inline T & OVector2<T, Q>::getComponent(OVectorAxis aComponent)
{
	switch (aComponent) {
	case OVectorAxis::X:	return _glmInternal.x;
	case OVectorAxis::Y:	return _glmInternal.y;
	default:		throw OException("Invalid vector component.");
	}
}

template<typename T, glm::qualifier Q>
inline void OVector2<T, Q>::setX(T aValue)
{
	_glmInternal.x = aValue;
}

template<typename T, glm::qualifier Q>
inline void OVector2<T, Q>::setY(T aValue)
{
	_glmInternal.y = aValue;
}

template<typename T, glm::qualifier Q>
inline T OVector2<T, Q>::x() const
{
	return _glmInternal.x;
}

template<typename T, glm::qualifier Q>
inline T OVector2<T, Q>::y() const
{
	return _glmInternal.y;
}

/**
 @brief Represents three-dimentional vectors.
*/
template <typename T, glm::qualifier Q = glm::defaultp>
class OVector3 : public OVector<3, T, Q>
{
public:
	/**
	 @brief Class constructor.
	*/
	OVector3() = default;
	
	/**
	 @brief Class copy constructor.
	*/
	OVector3(const OVector3& aOther);
	
	/**
	 @brief Copy constructor for the base template class.
	 */
	OVector3(const Super& aOther);

	/**
	 @brief Class constructor.
	 
	 Creates a OVector3 with all components with the same value.
	 
	 @param aValue The value to be used by all of the three components.
	*/
	OVector3(float aValue);
	
	/**
	 @brief Class constructor.

	 Creates a OVector3 taking in the values for each component (cartesian or spherical). 
	*/
	OVector3(float aX, float aY, float aZ);

	/**
	 @copydoc OVector::getComponent(OVectorAxis)
	 */
	T& getComponent(OVectorAxis aComponent) override;

	/**
	 @brief Class destructor.
	*/
	virtual ~OVector3() = default;

	/**
	 @brief Vector cross product.
	 @param aOther Right value in the cross operation.
	 @return Resulting vector of the cross product.
	*/
	OVector3 cross(const OVector3& aOther) const;
	
	/**
	 @brief Vector cross product.
	 @param aLeft Left value of the operation.
	 @param aRight Right value of the operation.
	 @return Resulting vector of the cross product.
	*/
	static OVector3 cross(const OVector3& aLeft, const OVector3& aRight);

	/**
	 @brief Vector dot product.
	 @param aOther Right value in the dot operation.
	 @return Resulting vector of the dot product.
	*/
	float dot(const OVector3& aOther) const;

	/**
	 @brief Vector dot product.
	 @param aLeft Left value in the dot operation.
	 @param aRight Right value in the dot operation.
	 @return Resulting value of the dot product.
	*/
	static float dot(const OVector3& aLeft, const OVector3& aRight);

	/**
	 @brief Set the value for the cartesian X axis component.
	*/
	void setX(T aValue);
	
	/**
	 @brief Set the value for the cartesian Y axis component.
	*/
	void setY(T aValue);
	
	/**
	 @brief Set the value for the cartesian Z axis component.
	*/
	void setZ(T aValue);

	/**
	 @brief Returns the value of the cartesian X axis component.
	*/
	T x() const;
	
	/**
	 @brief Returns the value of the cartesian Y axis component.
	*/
	T y() const;
	
	/**
	 @brief Returns the value of the cartesian Z axis component.
	*/
	T z() const;

	/**
	 @brief Set the value for the spherical radius component.
	 */
	void setR(T aValue);

	/**
	 @brief Set the value for the spherical azimuthal component (in degrees).
	 */
	void setTheta(T aValue);

	/**
	 @brief Set the value for the spherical polar component (in degrees).
	 */
	void setPhi(T aValue);

	/**
	 @brief Returns the value for the spherical radius component.
	 */
	T r() const;

	/**
	 @brief Returns the value for the spherical azimuthal component (in degrees).
	 */
	T theta() const;

	/**
	 @brief Returns the value for the spherical polar component (in degrees).
	 */
	T phi() const;

	/**
	 @brief Returns a new vector converted to spherical coordinates.
	 */
	OVector3 toSpherical() const;
	
	/**
	 @brief Returns a new vector converted to cartesian coordinates.
	 */
	OVector3 toCartesian() const;
};

template<typename T, glm::qualifier Q>
inline OVector3<T, Q>::OVector3(const OVector3& aOther) :
	OVector(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OVector3<T, Q>::OVector3(const Super & aOther) :
	OVector(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OVector3<T, Q>::OVector3(float aValue) :
	OVector(GLMType(aValue))
{
}

template<typename T, glm::qualifier Q>
inline OVector3<T, Q>::OVector3(float aX, float aY, float aZ) :
	OVector(GLMType(aX, aY, aZ))
{
}

template<typename T, glm::qualifier Q>
inline T & OVector3<T, Q>::getComponent(OVectorAxis aComponent)
{
	switch (aComponent) {
	case OVectorAxis::X:	return _glmInternal.x;
	case OVectorAxis::Y:	return _glmInternal.y;
	case OVectorAxis::Z:	return _glmInternal.z;
	default:		throw OException("Invalid vector component.");
	}
}

template<typename T, glm::qualifier Q>
inline OVector3<T,Q> OVector3<T, Q>::cross(const OVector3 & aOther) const
{
	OVector3 res;
	res._glmInternal = glm::cross(_glmInternal, aOther._glmInternal);
	return res;
}

template<typename T, glm::qualifier Q>
inline OVector3<T, Q> OVector3<T, Q>::cross(const OVector3 & aLeft, const OVector3 & aRight)
{
	return aLeft.cross(aRight);
}

template<typename T, glm::qualifier Q>
inline float OVector3<T, Q>::dot(const OVector3 & aOther) const
{
	return glm::dot(_glmInternal, aOther._glmInternal);
}

template<typename T, glm::qualifier Q>
inline float OVector3<T, Q>::dot(const OVector3 & aLeft, const OVector3 & aRight)
{
	return aLeft.dot(aRight);
}

template<typename T, glm::qualifier Q>
inline void OVector3<T, Q>::setX(T aValue)
{
	_glmInternal.x = aValue;
}

template<typename T, glm::qualifier Q>
inline void OVector3<T, Q>::setY(T aValue)
{
	_glmInternal.y = aValue;
}

template<typename T, glm::qualifier Q>
inline void OVector3<T, Q>::setZ(T aValue)
{
	_glmInternal.z = aValue;
}

template<typename T, glm::qualifier Q>
inline T OVector3<T, Q>::x() const
{
	return _glmInternal.x;
}

template<typename T, glm::qualifier Q>
inline T OVector3<T, Q>::y() const
{
	return _glmInternal.y;
}

template<typename T, glm::qualifier Q>
inline T OVector3<T, Q>::z() const
{
	return _glmInternal.z;
}

template<typename T, glm::qualifier Q>
inline void OVector3<T, Q>::setR(T aValue)
{
	setx(aValue);
}

template<typename T, glm::qualifier Q>
inline void OVector3<T, Q>::setTheta(T aValue)
{
	setY(aValue);
}

template<typename T, glm::qualifier Q>
inline void OVector3<T, Q>::setPhi(T aValue)
{
	setZ(aValue);
}

template<typename T, glm::qualifier Q>
inline T OVector3<T, Q>::r() const
{
	return x();
}

template<typename T, glm::qualifier Q>
inline T OVector3<T, Q>::theta() const
{
	return y();
}

template<typename T, glm::qualifier Q>
inline T OVector3<T, Q>::phi() const
{
	return z();
}

template<typename T, glm::qualifier Q>
inline OVector3<T, Q> OVector3<T, Q>::toSpherical() const
{
	return OVector3(static_cast<T>(sqrtf(x()*x()+y()*y()+z()*z())), 
			static_cast<T>(atanf(OMath::deg2rad(z()/x()))), 
			static_cast<T>(atanf(OMath::deg2rad(sqrtf(x()*x()+z()*z())/y())))
			);
}

template<typename T, glm::qualifier Q>
inline OVector3<T, Q> OVector3<T, Q>::toCartesian() const
{
	return OVector3(static_cast<T>(r()*cosf(OMath::deg2rad(theta()))*sinf(OMath::deg2rad(phi()))), 
			static_cast<T>(r()*cosf(OMath::deg2rad(phi()))),
			static_cast<T>(r()*sinf(OMath::deg2rad(theta()))*sinf(OMath::deg2rad(phi())))
			);
}


/**
 @brief Represents four-dimentional vectors.
*/
template <typename T, glm::qualifier Q>
class OVector4 : public OVector<4, T, Q> 
{
public:

	/**
	 @brief Class constructor
	*/
	OVector4() = default;

	/**
	 @brief Class copy constructor.
	*/
	OVector4(const OVector4& aOther);

	/**
	 @brief Class copy constructor for the base template class.
	 */
	OVector4(const Super& aOther);

	/**
	 @brief Class constructor.

	 Creates a OVector4 with all components with the same value.

	 @param aValue The value to be used by all of the three components.
	*/
	OVector4(float aValue);

	/**
	 @brief Class constructor.

	 Creates a OVector4 taking in the values for each component.
	*/
	OVector4(float aX, float aY, float aZ, float aW);

	/**
	 @brief Class constructor.

	 Creates an OVector4 from an OVector3 object, in which the X, Y and Z components
	 will receive the values from the three-dimensional vectors.
	 */
	OVector4(const OVector3<T,Q>& aVec3, float aW = 0.0f);

	/**
	 @brief Class destructor.
	*/
	virtual ~OVector4() = default;

	/**
	 @copydoc OVector::getComponent(OVectorAxis)
	 */
	T& getComponent(OVectorAxis aComponent) override;

	/**
	 @brief Set the value for the X axis component.
	*/
	void setX(T aValue);
	
	/**
	 @brief Set the value for the Y axis component.
	*/
	void setY(T aValue);
	
	/**
	 @brief Set the value for the Z axis component.
	*/
	void setZ(T aValue);
	
	/**
	 @brief Set the value for the W axis component.
	*/
	void setW(T aValue);

	/**
	 @brief Returns the value of the X axis component.
	*/
	T x() const;
	
	/**
	 @brief Returns the value of the Y axis component.
	*/
	T y() const;
	
	/**
	 @brief Returns the value of the Z axis component.
	*/
	T z() const;
	
	/**
	 @brief Returns the value of the W axis component.
	*/
	T w() const;
};

template<typename T, glm::qualifier Q>
inline OVector4<T, Q>::OVector4(const OVector4 & aOther) :
	OVector(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OVector4<T, Q>::OVector4(const Super & aOther) :
	OVector(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OVector4<T, Q>::OVector4(float aValue) :
	OVector(GLMType(aValue))
{
}

template<typename T, glm::qualifier Q>
inline OVector4<T, Q>::OVector4(float aX, float aY, float aZ, float aW) :
	OVector(GLMType(aX, aY, aZ, aW))
{
}

template<typename T, glm::qualifier Q>
inline OVector4<T, Q>::OVector4(const OVector3<T, Q>& aVec3, float aW) :
	OVector(GLMType(aVec3.x(), aVec3.y(), aVec3.z(), aW))
{
}

template<typename T, glm::qualifier Q>
inline T & OVector4<T, Q>::getComponent(OVectorAxis aComponent)
{
	switch (aComponent) {
	case OVectorAxis::X:	return _glmInternal.x;
	case OVectorAxis::Y:	return _glmInternal.y;
	case OVectorAxis::Z:	return _glmInternal.z;
	case OVectorAxis::W:	return _glmInternal.w;
	default:		throw OException("Invalid vector component.");
	}
}

template<typename T, glm::qualifier Q>
inline void OVector4<T, Q>::setX(T aValue)
{
	_glmInternal.x = aValue;
}

template<typename T, glm::qualifier Q>
inline void OVector4<T, Q>::setY(T aValue)
{
	_glmInternal.y = aValue;
}

template<typename T, glm::qualifier Q>
inline void OVector4<T, Q>::setZ(T aValue)
{
	_glmInternal.z = aValue;
}

template<typename T, glm::qualifier Q>
inline void OVector4<T, Q>::setW(T aValue)
{
	_glmInternal.w = aValue;
}

template<typename T, glm::qualifier Q>
inline T OVector4<T, Q>::x() const
{
	return _glmInternal.x;
}

template<typename T, glm::qualifier Q>
inline T OVector4<T, Q>::y() const
{
	return _glmInternal.y;
}

template<typename T, glm::qualifier Q>
inline T OVector4<T, Q>::z() const
{
	return _glmInternal.z;
}

template<typename T, glm::qualifier Q>
inline T OVector4<T, Q>::w() const
{
	return _glmInternal.w;
}

