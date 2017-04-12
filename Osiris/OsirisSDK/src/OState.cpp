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

void OStateConstraint::set(Axis axis, bool active, float value)
{
	_components[axis].active = active;
	_components[axis].value = value;
}

bool OStateConstraint::active(Axis axis) const
{
	return _components[axis].active;
}

float OStateConstraint::value(Axis axis) const
{
	return _components[axis].value;
}

// ****************************************************************************
// OState
// ****************************************************************************
OState::OState() 
{
}

OState::~OState()
{
}

void OState::setMotionComponent(int degree, const OVector3& component, bool objectOrientation)
{
	int currComponentsSize = _components.size();
	if ( currComponentsSize < degree + 1  ) {
		_components.resize(degree + 1);
		for (int i = currComponentsSize - 1; i <= degree; i++) _components[i] = OVector3(0.0f);
	}

	if (objectOrientation) _components[degree] = _orientation.inverse() * component;
	else _components[degree] = component;
}

const OVector3* OState::motionComponent(int degree) const
{
	return &_components[degree];
}

void OState::setOrientation(const OVector3& or)
{
	_orientation = OQuaternion(or);
}

OVector3&& OState::orientation() const
{
	return _orientation.toEulerAngles();
}

const OStateConstraint * OState::minConstraint(int degree)
{
	if (degree > _minConstraint.size() - 1) return NULL;
	return &_minConstraint[degree];
}

const OStateConstraint * OState::maxConstraint(int degree)
{
	if (degree > _maxConstraint.size() - 1) return NULL;
	return &_maxConstraint[degree];
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
				if (_minConstraint[i].active(axis) && newValue < _minConstraint[i].value(axis))
					newValue = _minConstraint[i].value(axis);
				if (_maxConstraint[i].active(axis) && newValue > _maxConstraint[i].value(axis))
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

