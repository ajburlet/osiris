#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OQuaternion.hpp"
#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/OState.h"
#include "OsirisSDK/OCameraController.h"

using namespace std;

OCameraController::OCameraController(OApplication * app, 
				     MouseOrientationMode orMode, OMouseClickEvent::MouseButton orMouseBtn,
				     float movementAcceleration, float movementMaxSpeed) :
	_app(app),
	_orMode(orMode),
	_orMouseBtn(orMouseBtn),
	_activeMoveMouseBtnPressed(false),
	_last_mouse_x(-1),
	_last_mouse_y(-1),
	_delta_mouse_x(0),
	_delta_mouse_y(0)
{
	_app->camera()->state()->setMotionComponent(2, OVector3F(0.0f, 0.0f, 0.0f), OState::Scene);
	for (int axis = (int)OVectorAxis::X; axis <= (int)OVectorAxis::Z; axis++) {
		_app->camera()->state()->minConstraint(1)->setValue((OVectorAxis)axis, true, 0.0f);
		_app->camera()->state()->minConstraint(1)->setForce(true);
		_app->camera()->state()->minConstraint(1)->setForce(false);
		_app->camera()->state()->minConstraint(1)->setAbsoluteValue(true);
		_app->camera()->state()->maxConstraint(1)->setAbsoluteValue(true);
	}

	setMovementMaxSpeed(movementMaxSpeed);
	setMovementAcceleration(movementAcceleration);
	setMouseSensitivity(1.0f);
	updateApplication();
}

OCameraController::~OCameraController()
{
}

void OCameraController::setMovementMaxSpeed(float maxSpeed)
{
	_movementMaxSpeed = maxSpeed / 1000000.0f;
	for (int axis = (int)OVectorAxis::X; axis <= (int)OVectorAxis::Z; axis++) {
		_app->camera()->state()->maxConstraint(1)->setValue((OVectorAxis)axis, true, _movementMaxSpeed);
	}
}

float OCameraController::movementMaxSpeed() const
{
	return _movementMaxSpeed * 1000000.0f;
}

void OCameraController::setMovementAcceleration(float acc)
{
	_movementAcceleration = acc / 1000000.0f / 1000000.0f;
}

float OCameraController::movementAcceleration() const
{
	return _movementAcceleration * 1000000.0f * 1000000.0f;
}

void OCameraController::setMouseSensitivity(float sens)
{
	_mouseSensitivity = sens;
}

float OCameraController::mouseSensitivity() const
{
	return _mouseSensitivity;
}


void OCameraController::setMoveEventKey(OKeyboardPressEvent::KeyCode key, CameraMoveDir camEvt)
{
	if (_keyBind.size() == 0) {
		_app->addEventRecipient(OEvent::KeyboardPressEvent, this);
		_app->addEventRecipient(OEvent::KeyboardReleaseEvent, this);
	}
	_keyBind[key] = camEvt;
}

void OCameraController::update(const OTimeIndex& timeIndex, int step_us)
{
	OQuaternion& orientation = _app->camera()->state()->orientation();
	float normFactor = _mouseSensitivity * 180.0f / _app->windowWidth();
	/* update camera orientation (orientation vector) */
	if (_delta_mouse_x != 0) {
		orientation = OQuaternion(OVector3F(0.0f, 1.0f, 0.0f), (float)_delta_mouse_x * normFactor) * orientation;
	}
	if (_delta_mouse_y != 0) {
		orientation *= OQuaternion(OVector3F(1.0f, 0.0f, 0.0f), _delta_mouse_y * normFactor);
	}
	orientation = orientation.normalize();

	_delta_mouse_x = 0;
	_delta_mouse_y = 0;

	_app->camera()->state()->update(timeIndex, step_us);
}

void OCameraController::onKeyboardPress(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;

	if (isMovementKeyPressed(it->second)) return;

	OState* camState = _app->camera()->state();

	float dir;
	OVectorAxis axis = directionToAxis(it->second, &dir);
	camState->motionComponent(2)[axis] = dir*_movementAcceleration;
	camState->minConstraint(1)->setValue(axis, false);
	_pressedKeys[it->second] = true;
	_pressedKeys[inverseDir(it->second)] = false;
}

void OCameraController::onKeyboardRelease(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;
	
	if (!isMovementKeyPressed(it->second)) return;

	OState* camState = _app->camera()->state();
	OVectorAxis axis = directionToAxis(it->second);
	float accSign = OMath::reverseSign(camState->motionComponent(1)[axis]);
	camState->motionComponent(2)[axis] = accSign*_movementAcceleration;
	camState->minConstraint(1)->setValue(axis, true, 0.0f);
	_pressedKeys[it->second] = false;
}

void OCameraController::onMouseMove(const OMouseMoveEvent * evt)
{
	/* making sure it's either passive mouse movement or enabled with the mouse button pressed */
	if (_orMode == PassiveOrientation || (_orMode == ActiveOrientation && _activeMoveMouseBtnPressed == true)) {
		/* setting deltas */
		_delta_mouse_x = _last_mouse_x - evt->x();
		_delta_mouse_y = _last_mouse_y - evt->y();

		/* setting last mouse position */
		_last_mouse_x = evt->x();
		_last_mouse_y = evt->y();
	}
}

void OCameraController::onMouseClick(const OMouseClickEvent * evt)
{
	if (evt->button() != _orMouseBtn)  return;
	
	switch (evt->status()) {
	case OMouseClickEvent::Press:
		_activeMoveMouseBtnPressed = true;
		_last_mouse_x = evt->x();
		_last_mouse_y = evt->y();
		break;
	case OMouseClickEvent::Release:
		_activeMoveMouseBtnPressed = false;
		_last_mouse_x = -1;
		_last_mouse_y = -1;
		break;
	}
}

void OCameraController::updateApplication() 
{
	switch (_orMode) {
	case ActiveOrientation:
		_app->addEventRecipient(OEvent::MouseClickEvent, this);
		_app->addEventRecipient(OEvent::MouseActiveMoveEvent, this);
		_app->removeEventRecipient(OEvent::MousePassiveMoveEvent, this);
		break;

	case PassiveOrientation:
		_app->removeEventRecipient(OEvent::MouseClickEvent, this);
		_app->removeEventRecipient(OEvent::MouseActiveMoveEvent, this);
		_app->addEventRecipient(OEvent::MousePassiveMoveEvent, this);
		break;
		
	case OrientationDisabled:
		_app->removeEventRecipient(OEvent::MouseClickEvent, this);
		_app->removeEventRecipient(OEvent::MouseActiveMoveEvent, this);
		_app->removeEventRecipient(OEvent::MousePassiveMoveEvent, this);
		break;
	}
}

bool OCameraController::isMovementKeyPressed(CameraMoveDir dir)
{
	map<CameraMoveDir, bool>::iterator it = _pressedKeys.find(dir);
	return (it != _pressedKeys.end()) ? it->second : false;
}

OCameraController::CameraMoveDir OCameraController::inverseDir(CameraMoveDir dir)
{
	switch (dir) {
	case MoveUp:		return MoveDown;
	case MoveDown:		return MoveUp;
	case MoveRight:		return MoveLeft;
	case MoveLeft:		return MoveRight;
	case MoveForward:	return MoveBack;
	case MoveBack:		return MoveForward;
	}
	throw OException("Invalid camera movement direction.");
}

OVectorAxis OCameraController::directionToAxis(CameraMoveDir dir, float * sign)
{
	float signVal = 1.0f;
	OVectorAxis axis;

	switch (dir) {
	case MoveLeft:		signVal = -1.0f;
	case MoveRight:		axis = OVectorAxis::X;	break;
	case MoveDown:		signVal = -1.0f;
	case MoveUp:		axis = OVectorAxis::Y;	break;
	case MoveForward:	signVal = -1.0f;
	case MoveBack:		axis = OVectorAxis::Z;	break;
	}
	if (sign) *sign = signVal;
	return axis;

}
