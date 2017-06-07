#include "OsirisSDK/OMatrixStack.h"
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
	_app->camera()->state()->setMotionComponent(2, OVector3(0.0f, 0.0f, 0.0f), OState::Scene);
	setMovementMaxSpeed(movementMaxSpeed);
	setMovementAcceleration(movementAcceleration);
	updateApplication();
}

OCameraController::~OCameraController()
{
}

void OCameraController::setMovementMaxSpeed(float maxSpeed)
{
	_movementMaxSpeed = maxSpeed / 1000000.0f;
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


void OCameraController::setMoveEventKey(OKeyboardPressEvent::KeyCode key, CameraMoveDir camEvt)
{
	if (_keyBind.size() == 0) {
		_app->addEventRecipient(OEvent::KeyboardPressEvent, this);
		_app->addEventRecipient(OEvent::KeyboardReleaseEvent, this);
	}
	_keyBind[key] = camEvt;
}

void OCameraController::update(const OTimeIndex& timeIndex)
{
	OQuaternion& orientation = _app->camera()->state()->orientation();
	/* update camera orientation (orientation vector) */
	if (_delta_mouse_x != 0) {
		float deltaX =  _app->windowWidth() * (float)_delta_mouse_x / 2;
		float deltaTheta = 2 * atanf(deltaX / 2 / _app->camera()->nearLimit());
		orientation = OQuaternion(OVector3(0.0f, 1.0f, 0.0f), deltaTheta) * orientation;
	}
	if (_delta_mouse_y != 0) {
		float deltaY = _app->windowHeight() * (float)_delta_mouse_y / 2;
		float deltaPhi = 2 * atanf(deltaY / 2 / _app->camera()->nearLimit());
		orientation *= OQuaternion(OVector3(1.0f, 0.0f, 0.0f), deltaPhi);
	}

	_delta_mouse_x = 0;
	_delta_mouse_y = 0;

	_app->camera()->state()->update(timeIndex);
}

void OCameraController::onKeyboardPress(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;

	if (isMovementKeyPressed(it->second)) return;

	OState* camState = _app->camera()->state();

	float dir;
	OVector3::Axis axis = directionToAxis(it->second, &dir);
	camState->motionComponent(2)[axis] = dir*_movementAcceleration;
	if (dir > 0) {
		camState->minConstraint(1)->setValue(axis, false);
		camState->maxConstraint(1)->setValue(axis, true, _movementMaxSpeed);
	} else {
		camState->minConstraint(1)->setValue(axis, true, -_movementMaxSpeed);
		camState->maxConstraint(1)->setValue(axis, false);
	}

	_pressedKeys[it->second] = true;
}

void OCameraController::onKeyboardRelease(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;
	
	if (!isMovementKeyPressed(it->second)) return;

	OState* camState = _app->camera()->state();
	OVector3::Axis axis = directionToAxis(it->second);
	float accSign = OMath::reverseSign(camState->motionComponent(1)[axis]);
	camState->motionComponent(2)[axis] = accSign*_movementAcceleration;
	if (accSign > 0) {
		camState->minConstraint(1)->setValue(axis, false);
		camState->maxConstraint(1)->setValue(axis, true, 0.0f);
	} else {
		camState->minConstraint(1)->setValue(axis, true, 0.0f);
		camState->maxConstraint(1)->setValue(axis, false);

	}
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

OVector3::Axis OCameraController::directionToAxis(CameraMoveDir dir, float * sign)
{
	float signVal = 1.0f;
	OVector3::Axis axis;

	switch (dir) {
	case MoveLeft:		signVal = -1.0f;
	case MoveRight:		axis = OVector3::X;	break;
	case MoveDown:		signVal = -1.0f;
	case MoveUp:		axis = OVector3::Y;	break;
	case MoveBack:		signVal = -1.0f;
	case MoveForward:	axis = OVector3::Z;	break;
	}
	if (sign) *sign = signVal;
	return axis;

}
