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
	 \param active Constraint is active or inactive.
	 \param value Constraint value.
	 */
	void set(Axis axis, bool active, float value=0.0f);

	/**
	 \brief Returns whether the constraint for a given axis is active or inactive. 
	 */
	bool active(Axis axis) const;

	/**
	 \brief Returns constraint value.
	 */
	float value(Axis axis) const;

private:
	struct OStateConstraintVal{
		bool active;
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
	 \brief Class constructor.
	 */
	OState();

	/**
	 \brief Class destructor.
	 */
	virtual ~OState();

	/**
	 \brief Defines motion equation vector components.
	 \param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 \param component Component value. Time is given in milliseconds.
	 \param objectOrientation Se to true if the vector is relative to the orientation.
	 */
	void setMotionComponent(int degree, const OVector3& component, bool objectOrientation=false);

	/**
	 \brief Obtain motion state momentum for a given degree.
	 \param degree Momentum degree (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.).
	 \returns Pointer to vector component for a given degree. Time is given in milliseconds. Returns NULL if degree is invalid.
	 */
	const OVector3* motionComponent(int degree) const;

	/**
	 \brief Set orientation in terms of Euler angles.
	 \param or Vector containing Euler angles representing rotation for each axis.
	 */
	void setOrientation(const OVector3& or);

	/**
	 \brief Returns a three-dimentional vector containing Euler angles.
	 \returns Vector containing Euler angles representing rotation for each axis.
	 */
	OVector3&& orientation() const;

	/**
	 \brief Return the minimum value constraint for a given motion state degree.
	 \param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 \returns Pointer to the constraint object, NULL if degree is non-existant.
	 */
	const OStateConstraint* minConstraint(int degree);
	
	/**
	 ´\brief Return the maximum value constraint for a given motion state degree.
	 \param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 \returns Pointer to the constraint object, NULL if degree is non-existant.
	 */
	const OStateConstraint* maxConstraint(int degree);

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
	int _lastTimeIndex_ms;
};

