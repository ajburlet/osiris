#include <utility>
#include <cstring>

#include "OsirisSDK/OState.h"
#include "OsirisSDK/OException.h"

using namespace std;

// ****************************************************************************
// OStateConstraint
// ****************************************************************************
OStateConstraint::OStateConstraint() :
	_absoluteValue(false)
{
	memset(&_components, 0, 3 * sizeof(OStateConstraintVal));
}

OStateConstraint::~OStateConstraint()
{
}

void OStateConstraint::setValue(OVector3::Axis axis, bool active, float value)
{
	_components[axis].enabled = active;
	_components[axis].value = value;
}

bool OStateConstraint::enabled(OVector3::Axis axis) const
{
	return _components[axis].enabled;
}

float OStateConstraint::value(OVector3::Axis axis) const
{
	return _components[axis].value;
}

void OStateConstraint::setAbsoluteValue(bool flag)
{
	_absoluteValue = flag;
}

bool OStateConstraint::absoluteValue() const
{
	return _absoluteValue;
}

void OStateConstraint::disableAll()
{
	setValue(OVector3::X, false);
	setValue(OVector3::Y, false);
	setValue(OVector3::Z, false);
}


// ****************************************************************************
// OState
// ****************************************************************************
OState::OState(OrientationReferencial ref) :
	_orientationRef(ref),
	_position(0.0f),
	_orientation(OVector3(0.0f)),
	_scale(1.0f)
{
}

OState::~OState()
{
}

OState & OState::operator=(const OState & in)
{
	if (_components.size() != in._components.size()) _components.resize(in._components.size());
	for (int i = 0; i < (int)_components.size(); i++) _components[i] = in._components[i];
	if (_minConstraint.size() != in._minConstraint.size()) _minConstraint.resize(in._minConstraint.size());
	for (int i = 0; i < (int)_minConstraint.size(); i++) _minConstraint[i] = in._minConstraint[i];
	if (_maxConstraint.size() != in._maxConstraint.size()) _maxConstraint.resize(in._maxConstraint.size());
	for (int i = 0; i < (int)_maxConstraint.size(); i++) _maxConstraint[i] = in._maxConstraint[i];
	_position = in._position;
	_orientation = in._orientation;
	_scale = in._scale;
	_orientationRef = in._orientationRef;
	return *this;
}

void OState::setOrientationReferencial(OrientationReferencial orRef)
{
	_orientationRef = orRef;
}

OState::OrientationReferencial OState::orientationReferencial() const
{
	return _orientationRef;
}

void OState::setMotionComponent(int degree, const OVector3& component, OrientationReferencial orRef)
{
	if (degree == 0) {
		if (orRef == Object) throw OException("Cannot set object position in it's frame of reference.");
		_position = component;
	} else {
		checkDegree(degree-1);
		_components[degree-1] = checkReferencial(component, orRef);
	}
}

void OState::addMotionComponent(int degree, const OVector3 & component, OrientationReferencial orRef)
{
	if (degree == 0) {
		if (orRef == Object) throw OException("Cannot set object position in it's frame of reference.");
		_position += component;
	} else {
		checkDegree(degree-1);
		_components[degree-1] += checkReferencial(component, orRef);
	}
}

OVector3 OState::motionComponent(int degree, OrientationReferencial orRef) const
{
	if (degree == 0) {
		if (orRef == Object) throw OException("Cannot retrieve object position in it's frame of reference.");
		return _position;
	} else {
		if ((size_t)degree > _minConstraint.size()) throw OException("Invalid degree on motion component access.");
		return checkReferencial(_components[degree-1], orRef);
	}
}

OVector3& OState::motionComponent(int degree)
{
	if (degree == 0) {
		return _position;
	} else {
		if ((size_t)degree > _minConstraint.size()) throw OException("Invalid degree on motion component access.");
		return _components[degree-1];
	}
}

OVector3& OState::position()
{
	return _position;
}

void OState::setOrientation(const OVector3& or)
{
	_orientation = OQuaternion(or);
}

OQuaternion& OState::orientation()
{
	return _orientation;
}

OVector3& OState::scale()
{
	return _scale;
}

OStateConstraint * OState::minConstraint(int degree)
{
	if ((size_t)degree > _minConstraint.size() - 1) return NULL;
	return &_minConstraint[degree-1];
}

OStateConstraint * OState::maxConstraint(int degree)
{
	if ((size_t)degree > _maxConstraint.size() - 1) return NULL;
	return &_maxConstraint[degree-1];
}

void OState::disableAllConstraints()
{
	for (size_t i = 0; i < _minConstraint.size(); i++) _minConstraint[i].disableAll();
	for (size_t i = 0; i < _maxConstraint.size(); i++) _maxConstraint[i].disableAll();
}

void OState::update(const OTimeIndex& timeIndex, int step_us)
{
	/* iterate over all the components of the motion equation */
	for (int i = _components.size() - 2; i >= 0; i--) {
		/* iterate over the degrees of freedom */
		for (int df = 0; df < 3; df++) {
			OVector3::Axis axis = (OVector3::Axis)df;
			float newValue = _components[i][axis] + _components[i + 1][axis] * step_us;

			if (_minConstraint[i].enabled(axis)) {
				if ((!_minConstraint[i].absoluteValue() && newValue < _minConstraint[i].value(axis)) ||
					(_minConstraint[i].absoluteValue() && abs(newValue) < _minConstraint[i].value(axis)))
					newValue = _minConstraint[i].value(axis);
			}
			if (_maxConstraint[i].enabled(axis)) {
				if ((!_maxConstraint[i].absoluteValue() && newValue > _maxConstraint[i].value(axis)) ||
					(_maxConstraint[i].absoluteValue() && abs(newValue) > _maxConstraint[i].value(axis)))
					newValue = _maxConstraint[i].value(axis);
			}

			_components[i][axis] = newValue;
		}
	}

	/* update position */
	if (_components.size() > 0) {
		if (_orientationRef == Object) {
			_position += _orientation * _components[0] * step_us;
		} else {
			_position += _components[0] * step_us;
		}
	}
}

void OState::checkDegree(int degree)
{
	int currComponentsSize = _components.size();
	if ( currComponentsSize < degree + 1  ) {
		_components.resize(degree + 1);
		_minConstraint.resize(degree + 1);
		_maxConstraint.resize(degree + 1);
		for (int i = currComponentsSize; i <= degree; i++) _components[i] = OVector3(0.0f);
	}

}

OVector3 OState::checkReferencial(const OVector3 &in, OrientationReferencial orRef) const 
{
	if (orRef != _orientationRef) {
		switch (orRef) {
		case Scene:	return _orientation.inverse() * in;
		case Object:	return _orientation * in;
		}
	}

	return in;
}

