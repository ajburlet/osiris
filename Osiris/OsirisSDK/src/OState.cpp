#include <utility>

#include "OsirisSDK/OState.h"

using namespace std;

// ****************************************************************************
// OStateConstraint
// ****************************************************************************
OStateConstraint::OStateConstraint()
{
}

OStateConstraint::~OStateConstraint()
{
}

void OStateConstraint::setValue(Axis axis, bool active, float value)
{
	_components[axis].enabled = active;
	_components[axis].value = value;
}

bool OStateConstraint::enabled(Axis axis) const
{
	return _components[axis].enabled;
}

float OStateConstraint::value(Axis axis) const
{
	return _components[axis].value;
}

void OStateConstraint::disableAll()
{
	setValue(x, false);
	setValue(y, false);
	setValue(z, false);
}


// ****************************************************************************
// OState
// ****************************************************************************
OState::OState(OrientationReferencial ref) :
	_orientationRef(ref)
{
}

OState::~OState()
{
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
	checkDegree(degree);
	_components[degree] = checkReferencial(component, orRef);
}

void OState::addMotionComponent(int degree, const OVector3 & component, OrientationReferencial orRef)
{
	checkDegree(degree);
	_components[degree] += checkReferencial(component, orRef);
}

const OVector3 OState::motionComponent(int degree, OrientationReferencial orRef) const
{
	return checkReferencial(_components[degree], orRef);
}

const OVector3 * OState::motionComponent(int degree) const
{
	if ((size_t)degree > _minConstraint.size() - 1) return NULL;
	return &_components[degree];
}

void OState::setOrientation(const OVector3& or)
{
	_orientation = OQuaternion(or);
}

void OState::addOrientation(const OVector3 & or)
{
	_orientation *= OQuaternion(or);
}

OVector3 OState::orientation() const
{
	return _orientation.toEulerAngles();
}

OQuaternion& OState::orientationQuaternion()
{
	return _orientation;
}

OMatrix4x4 OState::orientationTransform() const
{
	return _orientation.toMatrix4();
}

OStateConstraint * OState::minConstraint(int degree)
{
	if ((size_t)degree > _minConstraint.size() - 1) return NULL;
	return &_minConstraint[degree];
}

OStateConstraint * OState::maxConstraint(int degree)
{
	if ((size_t)degree > _maxConstraint.size() - 1) return NULL;
	return &_maxConstraint[degree];
}

void OState::disableAllConstraints()
{
	for (size_t i = 0; i < _minConstraint.size(); i++) _minConstraint[i].disableAll();
	for (size_t i = 0; i < _maxConstraint.size(); i++) _maxConstraint[i].disableAll();
}

void OState::update(int timeIndex_ms)
{
	if (_lastTimeIndex_ms != 0) {
		/* iterate over all the components of the motion equation */
		float deltaT_ms = (float)(timeIndex_ms - _lastTimeIndex_ms);
		for (int i = _components.size() - 1; i >= 0; i--) {
			/* iterate over the degrees of freedom */
			for (int df = 0; df < 3; df++) {
				float newValue;
				switch (df) {
				case 0:	newValue = _components[i].x() + _components[i + 1].x() * deltaT_ms;	break;
				case 1:	newValue = _components[i].y() + _components[i + 1].y() * deltaT_ms;	break;
				case 2:	newValue = _components[i].z() + _components[i + 1].z() * deltaT_ms;	break;
				}

				OStateConstraint::Axis axis = (OStateConstraint::Axis)df;
				if (_minConstraint[i].enabled(axis) && newValue < _minConstraint[i].value(axis))
					newValue = _minConstraint[i].value(axis);
				if (_maxConstraint[i].enabled(axis) && newValue > _maxConstraint[i].value(axis))
					newValue = _maxConstraint[i].value(axis);

				switch (df) {
				case 0:	_components[i].setX(newValue);	break;
				case 1:	_components[i].setY(newValue);	break;
				case 2:	_components[i].setZ(newValue);	break;
				}
			}
		}
	}
	_lastTimeIndex_ms = timeIndex_ms;
}

void OState::checkDegree(int degree)
{
	int currComponentsSize = _components.size();
	if ( currComponentsSize < degree + 1  ) {
		_components.resize(degree + 1);
		for (int i = currComponentsSize - 1; i <= degree; i++) _components[i] = OVector3(0.0f);
	}

}

const OVector3 OState::checkReferencial(const OVector3 &in, OrientationReferencial orRef) const 
{
	if (orRef != _orientationRef) {
		switch (orRef) {
		case Scene:	return _orientation.inverse() * in;
		case Object:	return _orientation * in;
		}
	}

	return in;
}

