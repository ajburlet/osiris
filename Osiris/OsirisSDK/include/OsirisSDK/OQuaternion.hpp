#pragma once

#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

#include "OsirisSDK/OMath.h"
#include "OsirisSDK/OMathPrimitive.hpp"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMatrix.hpp"
#include "OsirisSDK/OQuaternionDefs.h"

template <typename T = float, glm::qualifier Q = glm::defaultp>
class OBaseQuaternion : public OMathPrimitive<glm::qua<T, Q>>
{
public:
	/**
	 @brief Class constructor.
	 */
	OBaseQuaternion() = default;

	/**
	 @brief Class copy constructor.
	 */
	OBaseQuaternion(const OBaseQuaternion& aOther);

	/**
	 @brief Copy constructor for the base template class.
	 */
	OBaseQuaternion(const Super& aOther);
	
	/**
	 @brief Class constructor, based on the quaternion components.
	 */
	OBaseQuaternion(float aX, float aY, float aZ, float aW);

	/**
	 @brief Class constructor for a 3D rotation quaternion.
	 @param aRotationAxis Rotation axis.
	 @param aAngle The angle of rotation in degrees.
	 */
	OBaseQuaternion(const OVector3<T, Q>& aRotationAxis, float aAngle);

	/**
	 @brief Class constructor based on the Euler angles for object orientation.
	 @brief aEulerAngles Euler orientation angles (pitch, roll and yaw).
	 */
	OBaseQuaternion(const OVector3<T, Q>& aEulerAngles);

	/**
	 @brief Class destructor.
	 */
	virtual ~OBaseQuaternion() = default;

	/**
	 @brief Quaternion product operator overload for three-dimentional vectors.
	 */
	OVector3<T, Q> operator*(const OVector3<T, Q>& aOther) const;

	/**
	 @brief Quaternion product operator.
	 */
	OQuaternion operator*(const OQuaternion& aOther) const;

	/**
	 @brief Conversion to 4x4 rotation matrix.
	 */
	OMatrix4x4<T, Q> toMatrix4() const;

	/**
	 @brief Conversion to a three-dimensional vector containing Euler angles.
	 */
	OVector3<T, Q> toEulerAngles() const;

	/**
	 @brief Calculate the inverse quaternion.
	 */
	OBaseQuaternion inverse() const;

	/**
	 @brief Rotate the quaternion around an axis.
	 @param aRotationAxis Rotation axis.
	 @param aRotationAngle The angle of rotation in degrees.
	 */
	void rotate(const OVector3<T, Q>& aRotationAxis, float aRotationAngle);
};


template<typename T, glm::qualifier Q>
inline OBaseQuaternion<T, Q>::OBaseQuaternion(const OBaseQuaternion & aOther) :
	Super(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OBaseQuaternion<T, Q>::OBaseQuaternion(const Super & aOther) :
	Super(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OBaseQuaternion<T, Q>::OBaseQuaternion(float aX, float aY, float aZ, float aW) :
	Super(GLMType(aX, aY, aZ, aW))
{
}

template<typename T, glm::qualifier Q>
inline OBaseQuaternion<T, Q>::OBaseQuaternion(const OVector3<T, Q>& aRotationAxis, float aAngle) :
	Super(GLMType(glm::angleAxis(OMath::deg2rad(aAngle), aRotationAxis.glm())))
{
}

template<typename T, glm::qualifier Q>
inline OBaseQuaternion<T, Q>::OBaseQuaternion(const OVector3<T, Q>& aEulerAngles) :
	Super(GLMType(aEulerAngles.glm() * (2.0f * OMath::PI / 360.0f)))
{
}

template<typename T, glm::qualifier Q>
inline OVector3<T, Q> OBaseQuaternion<T, Q>::operator*(const OVector3<T, Q>& aOther) const
{
	return OVector3<T, Q>(_glmInternal * aOther.glm());
}

template<typename T, glm::qualifier Q>
inline OQuaternion OBaseQuaternion<T, Q>::operator*(const OQuaternion & aOther) const
{
	return _glmInternal * aOther._glmInternal;
}

template<typename T, glm::qualifier Q>
inline OMatrix4x4<T, Q> OBaseQuaternion<T, Q>::toMatrix4() const
{
	return OMatrix4x4<T, Q>(glm::toMat4(_glmInternal));
}

template<typename T, glm::qualifier Q>
inline OVector3<T, Q> OBaseQuaternion<T, Q>::toEulerAngles() const
{
	return OVector3<T, Q>(glm::eulerAngles(_glmInternal) * (360.0f / (2 * OMath::PI)));
}

template<typename T, glm::qualifier Q>
inline OBaseQuaternion<T, Q> OBaseQuaternion<T, Q>::inverse() const
{
	return OBaseQuaternion(glm::inverse(_glmInternal));
}

template<typename T, glm::qualifier Q>
inline void OBaseQuaternion<T, Q>::rotate(const OVector3<T, Q>& aRotationAxis, float aRotationAngle)
{
	OBaseQuaternion rot(aRotationAxis, aRotationAngle);
	*this *= rot;
}
