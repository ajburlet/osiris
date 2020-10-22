#pragma once

#include <vector>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OVectorDefs.h"
#include "OsirisSDK/OQuaternionDefs.h"

class OTimeIndex;

/**
 @brief State engine class, controls orientation and motion state.
 */
class OAPI OState
{
public:
	/**
	 @brief Stores motion component minimum or maximum constraints.

	 The OState engine will use the constraints as follows: if the force flag is turned on, and if the
	 component value exceeds the contraint (which may occur right after the constraint is set), then the 
	 constraint value will be assigned to the component. After that, the constraint will be observed as it
	 always is, so that subsequent OState::update() calls wont exceed it.

	 If the force flag is disabled and the current component value is exceeding the constraint, then
	 the OState engine will ignore the constraint. By default the force flag is enabled.

	 Also there is the absolute flag. It assures that only absolute values of the components will be 
	 used for comparison purposes.
	 */
	class OAPI Constraint {
	public:
		/**
		 @brief Class constructor.
		 */
		Constraint();

		/**
		 @brief Class destructor.
		 */
		virtual ~Constraint();
		
		/**
		 @brief Enables/disables constraint and sets its value.
		 @param axis Which axis must be changed (x, y and z).
		 @param enabled Constraint is enabled or disabled.
		 @param value Constraint value.
		 */
		void setValue(OVectorAxis axis, bool enabled, float value=0.0f);

		/**
		 @brief Returns whether the constraint for a given axis is enabled or disabled. 
		 */
		bool enabled(OVectorAxis axis) const;

		/**
		 @brief Returns constraint value.
		 */
		float value(OVectorAxis axis) const;

		/**
		 @brief Turns on/off absolute value comparison.
		 */
		void setAbsoluteValue(bool flag);

		/**
		 @brief Returns if absolute value comparison is turned on.
		 */
		bool absoluteValue() const;

		/**
		 @brief Turns on/off Force variable value to contraint.
		 */
		void setForce(bool flag);

		/**
		 @brief Returns if force variable value to constraint is enabled.
		 */
		bool force() const;

		/**
		 @brief Disable all axis constraints.
		 */
		void disableAll();

	private:
		struct ConstraintVal{
			bool enabled;
			float value;
		} _components[3];

		bool _absoluteValue;
		bool _force;
	};

	/**
	 @brief Specifies the rotation referencial to be taken into account while working 
		with the motion equation components.
	 */
	enum OrientationReferencial {
		Scene=0x01,		/**< Scene referencial */
		Object			/**< Object referencial */
	};

	/**
	 @brief Class constructor.
	 */
	OState(OrientationReferencial ref=Scene);

	/**
	 @brief Deleted copy constructor.
	 */
	OState(const OState& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OState(OState&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OState();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OState& operator=(const OState& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OState& operator=(OState&& aOther);

	/**
	 @brief Clones the state to another object. 
	 */
	void cloneTo(OState& aOther) const;

	/**
	 @brief Clones the state to a newly allocated object.
	 */
	OState* clone() const;

	/**
	 @brief Sets the orientation referencial used on the motion equation components.
	 */
	void setOrientationReferencial(OrientationReferencial orRef);

	/**
	 @brief Provides the orientation referencial used on the motion equation components.
	 */
	OrientationReferencial orientationReferencial() const;

	/**
	 @brief Defines motion equation vector components.
	 @param degree The degree of the component (i.e. 1 = velocity, 2 = acceleration, etc.): the n-th derivative
		       of the motion state equation.
	 @param component Component value. Time is given in microseconds.
	 @param orRef The orientation referencial that this new values is defined in.
	 */
	void setMotionComponent(int degree, const OVector3F& component, OrientationReferencial orRef);

	/**
	 @brief Adds to a motion equation vector component.
	 @param degree The degree of the component (i.e. 1 = velocity, 2 = acceleration, etc.): the n-th derivative
		       of the motion state equation.
	 @param component Component value. Time is given in microseconds.
	 @param orRef The orientation referencial that this new values is defined in.
	 */
	void addMotionComponent(int degree, const OVector3F& component, OrientationReferencial orRef);

	/**
	 @brief Obtain motion state components for a given degree in the specified orientation reference frame.
	 @param degree The degree of the component (i.e. 1 = velocity, 2 = acceleration, etc.): the n-th derivative
		       of the motion state equation.
	 @param orRef The orientation referencial that this new values is defined in.
	 @returns Vector component for a given degree. Time is given in microseconds.
	 */
	OVector3F motionComponent(int degree, OrientationReferencial orRef) const;

	/**
	 @brief Obtain motion state component for a given degree in the current orientation reference frame.
	 @param degree The degree of the component (i.e. 1 = velocity, 2 = acceleration, etc.): the n-th derivative
		       of the motion state equation.
	 @returns Pointer to the vector component for a given degree. Time is given in microseconds.
	 */
	OVector3F& motionComponent(int degree);

	/**
	 @brief Returns a pointer to the vector containing position coordinates.
	 */
	OVector3F& position();

	/**
	 @brief Set orientation in terms of Euler angles.
	 @param or Vector containing Euler angles representing rotation for each axis.
	 */
	void setOrientation(const OVector3F& or);

	/**
	 @brief Retrieves the reference to the quaternion used for orientation transform.
	 */
	OQuaternion& orientation();

	/**
	 @brief Retrieves the reference to three-dimensional vector that represents the object scale.
	 */
	OVector3F& scale();

	/**
	 @brief Return the minimum value constraint for a given motion state degree.
	 @param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 @returns Pointer to the constraint object, NULL if degree is non-existant.
	 */
	Constraint* minConstraint(int degree);
	
	/**
	 @brief Return the maximum value constraint for a given motion state degree.
	 @param degree The degree of the component (i.e. 0 = position, 1 = velocity, 2 = acceleration, etc.). 
	 @returns Pointer to the constraint object, NULL if degree is non-existant.
	 */
	Constraint* maxConstraint(int degree);

	/**
	 @brief Disable all constraints on the motion state.
	 */
	void disableAllConstraints();

	/**
	 @brief Update state for a given time index.
	 @param timeIndex Time index.
	 @param step_us Simulation step in microseconds.
	 */
	void update(const OTimeIndex& timeIndex, int step_us);

private:
	/**
	 @brief Validates if a component value for a given degree exceeds one of the constraints.
	 @param axis Axis index.
	 @param compIdx Component degree index.
	 @param value Component value.
	 @returns Returns 0 if value exceeds neither the minimum nor maximum constraints, -1 if it exceeds minimum,
	          and +1 if exceeds maximum.
	 */
	int validateConstraints(OVectorAxis axis, int compIdx, float value);

	/**
	 @brief Makes sure that all arrays are ready to handle a given degree level.
	 */
	void checkDegree(int degree);

	/**
	 @brief Checks if the vector is in the given referencial, and transforms it if not.
	 */
	OVector3F checkReferencial(const OVector3F& in, OrientationReferencial orRef) const;
	
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OState::OState(OState&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}
