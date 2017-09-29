#pragma once

#include <vector>

#include "defs.h"
#include "OMath.h"
#include "OTimeIndex.h"

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
	 \brief Enables/disables constraint and sets its value.
	 \param axis Which axis must be changed (x, y and z).
	 \param enabled Constraint is enabled or disabled.
	 \param value Constraint value.
	 */
	void setValue(OVector3::Axis axis, bool enabled, float value=0.0f);

	/**
	 \brief Returns whether the constraint for a given axis is enabled or disabled. 
	 */
	bool enabled(OVector3::Axis axis) const;

	/**
	 \brief Returns constraint value.
	 */
	float value(OVector3::Axis axis) const;

	/**
	 \brief Turns on/off absolute value comparison.
	 */
	void setAbsoluteValue(bool flag);

	/**
	 \brief Returns if absolute value comparison is turned on.
	 */
	bool absoluteValue() const;

	/**
	 \brief Disable all axis constraints.
	 */
	void disableAll();

private:
	struct OStateConstraintVal{
		bool enabled;
		float value;
	} _components[3];

	bool _absoluteValue;
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
	 \brief Assignment operator.
	 */
	OState& operator=(const OState& in);

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
	 \param degree The degree of the component (i.e. 1 = velocity, 2 = acceleration, etc.): the n-th derivative
		       of the motion state equation.
	 \param component Component value. Time is given in microseconds.
	 \param orRef The orientation referencial that this new values is defined in.
	 */
	void setMotionComponent(int degree, const OVector3& component, OrientationReferencial orRef);

	/**
	 \brief Adds to a motion equation vector component.
	 \param degree The degree of the component (i.e. 1 = velocity, 2 = acceleration, etc.): the n-th derivative
		       of the motion state equation.
	 \param component Component value. Time is given in microseconds.
	 \param orRef The orientation referencial that this new values is defined in.
	 */
	void addMotionComponent(int degree, const OVector3& component, OrientationReferencial orRef);

	/**
	 \brief Obtain motion state components for a given degree in the specified orientation reference frame.
	 \param degree The degree of the component (i.e. 1 = velocity, 2 = acceleration, etc.): the n-th derivative
		       of the motion state equation.
	 \param orRef The orientation referencial that this new values is defined in.
	 \returns Vector component for a given degree. Time is given in microseconds.
	 */
	OVector3 motionComponent(int degree, OrientationReferencial orRef) const;

	/**
	 \brief Obtain motion state component for a given degree in the current orientation reference frame.
	 \param degree The degree of the component (i.e. 1 = velocity, 2 = acceleration, etc.): the n-th derivative
		       of the motion state equation.
	 \returns Pointer to the vector component for a given degree. Time is given in microseconds.
	 */
	OVector3& motionComponent(int degree);

	/**
	 \brief Returns a pointer to the vector containing position coordinates.
	 */
	OVector3& position();

	/**
	 \brief Set orientation in terms of Euler angles.
	 \param or Vector containing Euler angles representing rotation for each axis.
	 */
	void setOrientation(const OVector3& or);

	/**
	 \brief Retrieves the reference to the quaternion used for orientation transform.
	 */
	OQuaternion& orientation();

	/**
	 @brief Retrieves the reference to three-dimensional vector that represents the object scale.
	 */
	OVector3& scale();

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
	 \param timeIndex Time index.
	 \param step_us Simulation step in microseconds.
	 */
	void update(const OTimeIndex& timeIndex, int step_us);

private:
	std::vector<OVector3> _components;
	std::vector<OStateConstraint> _minConstraint;
	std::vector<OStateConstraint> _maxConstraint;
	OVector3 _position;
	OQuaternion _orientation;
	OVector3 _scale;
	OrientationReferencial _orientationRef;

	void checkDegree(int degree);
	OVector3 checkReferencial(const OVector3& in, OrientationReferencial orRef) const;
};

