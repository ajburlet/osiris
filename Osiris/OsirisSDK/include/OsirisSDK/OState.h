#pragma once

#include <vector>

#include "defs.h"
#include "OMath.h"

/**
 \brief Stores motion component minimum or maximum constraints.
 */
class OAPI OStateConstraint {
public:
	/**
	 \brief Class constructor.
	 */
	OStateConstraint();

	/**
	 \brief Class destructor.
	 */
	virtual ~OStateConstraint();
	
	/**
	 \brief Axis enumeration for internal reference.
	 */
	enum Axis {
		x=0,	/**< X axis reference */
		y,	/**< Y axis reference */
		z	/**< Z axis reference */
	};

	/**
	 \brief Enables/disables constraint and sets its value.
	 \param axis Which axis must be changed (x, y and z).
	 \param enabled Constraint is enabled or disabled.
	 \param value Constraint value.
	 */
	void setValue(Axis axis, bool enabled, float value=0.0f);

	/**
	 \brief Returns whether the constraint for a given axis is enabled or disabled. 
	 */
	bool enabled(Axis axis) const;

	/**
	 \brief Returns constraint value.
	 */
	float value(Axis axis) const;

	/**
	 \brief Disable all axis constraints.
	 */
	void disableAll();

private:
	struct OStateConstraintVal{
		bool enabled;
		float value;
	} _components[3];
};

/**
 \brief Controls object orientation and motion state.
 */
class OAPI OState
{
public:
	/**
	 \brief Specifies the rotation referencial to be taken into account while working 
		with the motion equation components.
	 */
	enum OrientationReferencial {
		Scene=0x01,		/**< Scene referencial */
		Object			/**< Object referencial */
	};

	/**
	 \brief Class constructor.
	 */
	OState(OrientationReferencial ref=Scene);

	/**
	 \brief Class destructor.
	 */
	virtual ~OState();

	/**
	 \brief Sets the orientation referencial used on the motion equation components.
	 */
	void setOrientationReferencial(OrientationReferencial orRef);

	/**
	 \brief Provides the orientation referencial used on the motion equation components.
	 */
	OrientationReferencial orientationReferencial() const;

	/**
	 \brief Defines motion equation vector components.
	 \param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 \param component Component value. Time is given in milliseconds.
	 \param orRef The orientation referencial that this new values is defined in.
	 */
	void setMotionComponent(int degree, const OVector3& component, OrientationReferencial orRef);

	/**
	 \brief Adds to a motion equation vector component.
	 \param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 \param component Component value. Time is given in milliseconds.
	 \param orRef The orientation referencial that this new values is defined in.
	 */
	void addMotionComponent(int degree, const OVector3& component, OrientationReferencial orRef);

	/**
	 \brief Obtain motion state components for a given degree in the specified orientation reference frame.
	 \param degree Momentum degree (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.).
	 \param orRef The orientation referencial that this new values is defined in.
	 \returns Vector component for a given degree. Time is given in milliseconds.
	 */
	const OVector3 motionComponent(int degree, OrientationReferencial orRef) const;

	/**
	 \brief Obtain motion state component for a given degree in the current orientation reference frame.
	 \param degree Momentum degree (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.).
	 \returns Pointer to the vector component for a given degree. Time is given in milliseconds. NULL if 
		  degree is non-existant.
	 */
	const OVector3* motionComponent(int degree) const;

	/**
	 \brief Set orientation in terms of Euler angles.
	 \param or Vector containing Euler angles representing rotation for each axis.
	 */
	void setOrientation(const OVector3& or);

	/**
	 \brief Add to orientation in terms of Euler angles.
	 \param or Vector containing Euler angles representing the additional rotation for each axis.
	 */
	void addOrientation(const OVector3& or);

	/**
	 \brief Returns a three-dimentional vector containing Euler angles.
	 \returns Vector containing Euler angles representing rotation for each axis.
	 */
	OVector3 orientation() const;

	/**
	 \brief Retrieves a pointer to the quaternion used for orientation transform.
	 */
	OQuaternion& orientationQuaternion();

	/**
	 \brief Provides a rotation transformation matrix given the orientation.
	 */
	OMatrix4x4 orientationTransform() const;

	/**
	 \brief Return the minimum value constraint for a given motion state degree.
	 \param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 \returns Pointer to the constraint object, NULL if degree is non-existant.
	 */
	OStateConstraint* minConstraint(int degree);
	
	/**
	 ´\brief Return the maximum value constraint for a given motion state degree.
	 \param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 \returns Pointer to the constraint object, NULL if degree is non-existant.
	 */
	OStateConstraint* maxConstraint(int degree);

	/**
	 \brief Disable all constraints on the motion state.
	 */
	void disableAllConstraints();

	/**
	 \brief Update state for a given time index.
	 \param timeIndex_ms Time index in milliseconds.
	 */
	void update(int timeIndex_ms);

private:
	std::vector<OVector3> _components;
	std::vector<OStateConstraint> _minConstraint;
	std::vector<OStateConstraint> _maxConstraint;
	OQuaternion _orientation;
	OrientationReferencial _orientationRef;

	int _lastTimeIndex_ms;

	void checkDegree(int degree);
	const OVector3 checkReferencial(const OVector3& in, OrientationReferencial orRef) const;
};

