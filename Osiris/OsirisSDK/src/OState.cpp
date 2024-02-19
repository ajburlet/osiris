#include <utility>
#include <cstring>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OQuaternion.hpp"
#include "OsirisSDK/OTimeIndex.h"
#include "OsirisSDK/OState.h"

using namespace std;

// ****************************************************************************
// OState::Constraint
// ****************************************************************************
OState::Constraint::Constraint() :
	_absoluteValue(false),
	_force(true)
{
	memset(&_components, 0, 3 * sizeof(OState::Constraint::ConstraintVal));
}

OState::Constraint::~Constraint()
{
}

void OState::Constraint::setValue(OVectorAxis axis, bool active, float value)
{
	_components[static_cast<int>(axis)].enabled = active;
	_components[static_cast<int>(axis)].value = value;
}

bool OState::Constraint::enabled(OVectorAxis axis) const
{
	return _components[static_cast<int>(axis)].enabled;
}

float OState::Constraint::value(OVectorAxis axis) const
{
	return _components[static_cast<int>(axis)].value;
}

void OState::Constraint::setAbsoluteValue(bool flag)
{
	_absoluteValue = flag;
}

bool OState::Constraint::absoluteValue() const
{
	return _absoluteValue;
}

void OState::Constraint::setForce(bool flag)
{
	_force = flag;
}

bool OState::Constraint::force() const
{
	return _force;
}

void OState::Constraint::disableAll()
{
	setValue(OVectorAxis::X, false);
	setValue(OVectorAxis::Y, false);
	setValue(OVectorAxis::Z, false);
}


// ****************************************************************************
// OState
// ****************************************************************************
struct OState::Impl {
	Impl(OrientationReferencial aRef) :
		orientationRef(aRef),
		position(0.0f),
		orientation(OVector3F(0.0f)),
		scale(1.0f)
	{}

	OArray<OVector3F>	components;
	OArray<Constraint>	minConstraint;
	OArray<Constraint>	maxConstraint;
	OVector3F		position;
	OQuaternion		orientation;
	OVector3F		scale;
	OrientationReferencial	orientationRef;
};

OState::OState(OrientationReferencial ref) 
{
	OExPointerCheck(_impl = new Impl(ref));
}

OState::~OState()
{
	if (_impl != nullptr) delete _impl;
}

OState & OState::operator=(OState && aOther)
{
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

void OState::cloneTo(OState & aOther) const
{
	aOther._impl->components.cloneTo(_impl->components);
	aOther._impl->minConstraint.cloneTo(_impl->minConstraint);
	aOther._impl->maxConstraint.cloneTo(_impl->maxConstraint);
	_impl->position = aOther._impl->position;
	_impl->orientation = aOther._impl->orientation;
	_impl->scale = aOther._impl->scale;
	_impl->orientationRef = aOther._impl->orientationRef;
}

OState * OState::clone() const
{
	auto newClone = new OState;
	OExPointerCheck(newClone);
	cloneTo(*newClone);
	return newClone;
}

void OState::setOrientationReferencial(OrientationReferencial orRef)
{
	_impl->orientationRef = orRef;
}

OState::OrientationReferencial OState::orientationReferencial() const
{
	return _impl->orientationRef;
}

void OState::setMotionComponent(int degree, const OVector3F& component, OrientationReferencial orRef)
{
	if (degree == 0) {
		if (orRef == Object) throw OEx("Cannot set object position in it's frame of reference.");
		_impl->position = component;
	} else {
		checkDegree(degree-1);
		_impl->components[degree-1] = checkReferencial(component, orRef);
	}
}

void OState::addMotionComponent(int degree, const OVector3F & component, OrientationReferencial orRef)
{
	if (degree == 0) {
		if (orRef == Object) throw OEx("Cannot set object position in it's frame of reference.");
		_impl->position += component;
	} else {
		checkDegree(degree-1);
		_impl->components[degree-1] += checkReferencial(component, orRef);
	}
}

OVector3F OState::motionComponent(int degree, OrientationReferencial orRef) const
{
	if (degree == 0) {
		if (orRef == Object) throw OEx("Cannot retrieve object position in it's frame of reference.");
		return _impl->position;
	} else {
		if ((size_t)degree > _impl->minConstraint.size()) throw OEx("Invalid degree on motion component access.");
		return checkReferencial(_impl->components[degree-1], orRef);
	}
}

OVector3F& OState::motionComponent(int degree)
{
	if (degree == 0) {
		return _impl->position;
	} else {
		if ((size_t)degree > _impl->minConstraint.size()) throw OEx("Invalid degree on motion component access.");
		return _impl->components[degree-1];
	}
}

OVector3F& OState::position()
{
	return _impl->position;
}

void OState::setOrientation(const OVector3F& or)
{
	_impl->orientation = OQuaternion(or);
}

OQuaternion& OState::orientation()
{
	return _impl->orientation;
}

OVector3F& OState::scale()
{
	return _impl->scale;
}

OState::Constraint * OState::minConstraint(int degree)
{
	if ((size_t)degree > _impl->minConstraint.size() - 1) return NULL;
	return &_impl->minConstraint[degree-1];
}

OState::Constraint * OState::maxConstraint(int degree)
{
	if ((size_t)degree > _impl->maxConstraint.size() - 1) return NULL;
	return &_impl->maxConstraint[degree-1];
}

void OState::disableAllConstraints()
{
	for (size_t i = 0; i < _impl->minConstraint.size(); i++) _impl->minConstraint[i].disableAll();
	for (size_t i = 0; i < _impl->maxConstraint.size(); i++) _impl->maxConstraint[i].disableAll();
}

void OState::update(const OTimeIndex& timeIndex, int step_us)
{
#define SIGN(in) in/abs(in)
	/* iterate over all the components of the motion equation */
	for (int i = _impl->components.size() - 2; i >= 0; i--) {
		/* iterate over the degrees of freedom */
		for (int df = 0; df < 3; df++) {
			OVectorAxis axis = static_cast<OVectorAxis>(df);
			float oldValue = _impl->components[i][axis];
			float newValue = oldValue + _impl->components[i + 1][axis] * step_us;

			int prevConstraintStatus = validateConstraints(axis, i, oldValue);
			int nextConstraintStatus = validateConstraints(axis, i, newValue);
		
			/* !!! we are not checking if the constraints are enabled !!! */
			if (_impl->minConstraint[i].enabled(axis)) {
				if (_impl->minConstraint[i].absoluteValue() && _impl->minConstraint[i].value(axis) == 0.0f &&  
				    (oldValue == 0.0f || SIGN(oldValue) != SIGN(newValue)) ) {
					newValue = 0.0f;
				} else if (nextConstraintStatus < 0 && (_impl->minConstraint[i].force() || prevConstraintStatus >= 0)) {
					newValue = _impl->minConstraint[i].value(axis);
				}
			}
			if (_impl->maxConstraint[i].enabled(axis)) {
				if (nextConstraintStatus > 0 && (_impl->maxConstraint[i].force() || prevConstraintStatus <= 0)) {
					newValue = (_impl->minConstraint[i].absoluteValue()) ? SIGN(oldValue)*_impl->maxConstraint[i].value(axis) : 
						_impl->maxConstraint[i].value(axis);
				}
			}

			_impl->components[i][axis] = newValue;
		}
	}

	/* update position */
	if (_impl->components.size() > 0) {
		if (_impl->orientationRef == Object) {
			_impl->position += _impl->orientation * _impl->components[0] * (float)step_us;
		} else {
			_impl->position += _impl->components[0] * (float)step_us;
		}
	}
}


int OState::validateConstraints(OVectorAxis axis, int compIdx, float value)
{
	if (_impl->minConstraint[compIdx].enabled(axis)) {
		if ((!_impl->minConstraint[compIdx].absoluteValue() && value < _impl->minConstraint[compIdx].value(axis)) ||
			(_impl->minConstraint[compIdx].absoluteValue() && abs(value) < _impl->minConstraint[compIdx].value(axis)))
			return -1;
	}
	if (_impl->maxConstraint[compIdx].enabled(axis)) {
		if ((!_impl->maxConstraint[compIdx].absoluteValue() && value > _impl->maxConstraint[compIdx].value(axis)) ||
			(_impl->maxConstraint[compIdx].absoluteValue() && abs(value) > _impl->maxConstraint[compIdx].value(axis)))
			return 1;
	}
	return 0;
}

void OState::checkDegree(int degree)
{
	int currComponentsSize = _impl->components.size();
	if ( currComponentsSize < degree + 1  ) {
		_impl->components.resize(degree + 1);
		_impl->minConstraint.resize(degree + 1);
		_impl->maxConstraint.resize(degree + 1);
		for (int i = currComponentsSize; i <= degree; i++) _impl->components[i] = OVector3F(0.0f);
	}

}

OVector3F OState::checkReferencial(const OVector3F &in, OrientationReferencial orRef) const 
{
	if (orRef != _impl->orientationRef) {
		switch (orRef) {
		case Scene:	return _impl->orientation.inverse() * in;
		case Object:	return _impl->orientation * in;
		}
	}

	return in;
}

