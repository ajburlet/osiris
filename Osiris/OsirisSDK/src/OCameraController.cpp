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
	_movementMaxSpeed = maxSpeed / 1000.0f;

	_app->camera()->state()->minConstraint(1)->setAbsoluteValue(true);
	_app->camera()->state()->minConstraint(1)->setValue(OVector3::X, true, 0.0f);
	_app->camera()->state()->minConstraint(1)->setValue(OVector3::Y, true, 0.0f);
	_app->camera()->state()->minConstraint(1)->setValue(OVector3::Z, true, 0.0f);
	_app->camera()->state()->maxConstraint(1)->setAbsoluteValue(true);
	_app->camera()->state()->maxConstraint(1)->setValue(OVector3::X, true, _movementMaxSpeed);
	_app->camera()->state()->maxConstraint(1)->setValue(OVector3::Y, true, _movementMaxSpeed);
	_app->camera()->state()->maxConstraint(1)->setValue(OVector3::Z, true, _movementMaxSpeed);
}

float OCameraController::movementMaxSpeed() const
{
	return _movementMaxSpeed * 1000.0f;
}

void OCameraController::setMovementAcceleration(float acc)
{
	_movementAcceleration = acc / 1000.0f / 1000.0f;
}

float OCameraController::movementAcceleration() const
{
	return _movementAcceleration * 1000.0f * 1000.0f;
}


void OCameraController::setMoveEventKey(OKeyboardPressEvent::KeyCode key, CameraMoveDir camEvt)
{
	if (_keyBind.size() == 0) {
		_app->addEventRecipient(OEvent::KeyboardPressEvent, this);
		_app->addEventRecipient(OEvent::KeyboardReleaseEvent, this);
	}
	_keyBind[key] = camEvt;
}

void OCameraController::update(int timeIndex_ms)
{
	/* update camera orientation (orientation vector) */
	float deltaTheta = 0.0f, deltaPhi = 0.0f;
	if (_delta_mouse_x != 0) deltaTheta = 2 * atanf((float)_delta_mouse_x / 2 / _app->camera()->nearLimit());
	if (_delta_mouse_y != 0) deltaPhi = 2 * atanf((float)_delta_mouse_y / 2 / _app->camera()->nearLimit());
	if (deltaTheta != 0 || deltaPhi != 0) _app->camera()->changeOrientation(OVector3(OMath::deg2rad(deltaPhi), 0.0f, OMath::deg2rad(deltaTheta)));

	_delta_mouse_x = 0;
	_delta_mouse_y = 0;

	_app->camera()->state()->update(timeIndex_ms);
}

void OCameraController::onKeyboardPress(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;

	if (isMovementKeyPressed(it->second)) return;

	OState* camState = _app->camera()->state();
	float dir = 1.0f;
	switch (it->second) {
	case MoveDown:
		dir = -1.0f;
	case MoveUp:
		camState->addMotionComponent(2, OVector3(0.0f, dir*_movementAcceleration, 0.0f), OState::Object);
		break;
	case MoveLeft:
		dir = -1.0f;
	case MoveRight:
		camState->addMotionComponent(2, OVector3(dir*_movementAcceleration, 0.0f, 0.0f), OState::Object);
		break;
	case MoveBack:
		dir = -1.0f;
	case MoveForward:
		camState->addMotionComponent(2, OVector3(0.0f, 0.0f, dir*_movementAcceleration), OState::Object);
		break;
	}
	_pressedKeys[it->second] = true;
}

void OCameraController::onKeyboardRelease(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;
	
	if (!isMovementKeyPressed(it->second)) return;

	OState* camState = _app->camera()->state();
	float accSign;
	switch (it->second) {
	case MoveDown:
	case MoveUp:
		accSign = OMath::reverseSign(camState->motionComponent(1)->y());
		camState->motionComponent(2)->setY(accSign*_movementAcceleration);
		break;
	case MoveLeft:
	case MoveRight:
		accSign = OMath::reverseSign(camState->motionComponent(1)->x());
		camState->motionComponent(2)->setX(accSign*_movementAcceleration);
		break;
	case MoveBack:
	case MoveForward:
		accSign = OMath::reverseSign(camState->motionComponent(1)->z());
		camState->motionComponent(2)->setZ(accSign*_movementAcceleration);
		break;
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
