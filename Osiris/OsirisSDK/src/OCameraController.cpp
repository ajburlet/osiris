#include "OsirisSDK/OMatrixStack.h"

#include "OsirisSDK/OCameraController.h"

using namespace std;

OCameraController::OCameraController(OApplication * app, 
				     MouseOrientationMode orMode, OMouseClickEvent::MouseButton orMouseBtn,
				     float movementAcceleration, float movementMaxSpeed) :
	_app(app),
	_orMode(orMode),
	_orMouseBtn(orMouseBtn),
	_movementAcceleration(movementAcceleration),
	_movementMaxSpeed(movementMaxSpeed),
	_activeMoveMouseBtnPressed(false),
	_last_mouse_x(-1),
	_last_mouse_y(-1),
	_delta_mouse_x(0),
	_delta_mouse_y(0)
{

	updateApplication();
}

OCameraController::~OCameraController()
{
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
	/* update camera orientation (direction vector) */
	float deltaTheta = 0.0f, deltaPhi = 0.0f;
	if (_delta_mouse_x != 0) deltaTheta = 2 * atanf((float)_delta_mouse_x / 2 / _app->camera()->nearLimit());
	if (_delta_mouse_y != 0) deltaPhi = 2 * atanf((float)_delta_mouse_y / 2 / _app->camera()->nearLimit());
	if (deltaTheta != 0 || deltaPhi != 0) _app->camera()->changeOrientation(OVector3(deltaTheta, deltaPhi, 0.0f));

	_delta_mouse_x = 0;
	_delta_mouse_y = 0;

	_app->camera()->state()->update(timeIndex_ms);
}

void OCameraController::onKeyboardPress(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;

	OState* camState = _app->camera()->state();
	switch (it->second) {
	case MoveUp:
		if (!isMovementKeyPressed(MoveDown)) {
			camState->addMotionComponent(2, OVector3(0.0f, _movementAcceleration, 0.0f), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::y, false);
			camState->maxConstraint(1)->setValue(OStateConstraint::y, true, _movementMaxSpeed);
		}
		break;
	case MoveDown:
		if (!isMovementKeyPressed(MoveUp)) {
			camState->addMotionComponent(2, OVector3(0.0f, -_movementAcceleration, 0.0f), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::y, true, -_movementMaxSpeed);
			camState->maxConstraint(1)->setValue(OStateConstraint::y, false);
		}
		break;
	case MoveRight:
		if (!isMovementKeyPressed(MoveLeft)) {
			camState->addMotionComponent(2, OVector3(_movementAcceleration, 0.0f, 0.0f), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::x, false);
			camState->maxConstraint(1)->setValue(OStateConstraint::x, true, _movementMaxSpeed);
		}
		break;
	case MoveLeft:
		if (!isMovementKeyPressed(MoveRight)) {
			camState->addMotionComponent(2, OVector3(-_movementAcceleration, 0.0f, 0.0f), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::x, true, -_movementMaxSpeed);
			camState->maxConstraint(1)->setValue(OStateConstraint::x, false);
		}
		break;
	case MoveForward:
		if (!isMovementKeyPressed(MoveBack)) {
			camState->addMotionComponent(2, OVector3(0.0f, 0.0f, _movementAcceleration), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::z, false);
			camState->maxConstraint(1)->setValue(OStateConstraint::z, true, _movementMaxSpeed);
		}
		break;
	case MoveBack:
		if (!isMovementKeyPressed(MoveForward)) {
			camState->addMotionComponent(2, OVector3(0.0f, 0.0f, -_movementAcceleration), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::z, true, -_movementMaxSpeed);
			camState->maxConstraint(1)->setValue(OStateConstraint::z, false);
		}
		break;
	}
	_pressedKeys[it->second] = true;
}

void OCameraController::onKeyboardRelease(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;
	
	OState* camState = _app->camera()->state();
	switch (it->second) {
	case MoveUp:
		if (!isMovementKeyPressed(MoveDown)) {
			camState->addMotionComponent(2, OVector3(0.0f, _movementAcceleration, 0.0f), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::y, false);
			camState->maxConstraint(1)->setValue(OStateConstraint::y, true, _movementMaxSpeed);
		}
		break;
	case MoveDown:
		if (!isMovementKeyPressed(MoveUp)) {
			camState->addMotionComponent(2, OVector3(0.0f, -_movementAcceleration, 0.0f), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::y, true, -_movementMaxSpeed);
			camState->maxConstraint(1)->setValue(OStateConstraint::y, false);
		}
		break;
	case MoveRight:
		if (!isMovementKeyPressed(MoveLeft)) {
			camState->addMotionComponent(2, OVector3(_movementAcceleration, 0.0f, 0.0f), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::x, false);
			camState->maxConstraint(1)->setValue(OStateConstraint::x, true, _movementMaxSpeed);
		}
		break;
	case MoveLeft:
		if (!isMovementKeyPressed(MoveRight)) {
			camState->addMotionComponent(2, OVector3(-_movementAcceleration, 0.0f, 0.0f), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::x, true, -_movementMaxSpeed);
			camState->maxConstraint(1)->setValue(OStateConstraint::x, false);
		}
		break;
	case MoveForward:
		if (!isMovementKeyPressed(MoveBack)) {
			camState->addMotionComponent(2, OVector3(0.0f, 0.0f, _movementAcceleration), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::z, false);
			camState->maxConstraint(1)->setValue(OStateConstraint::z, true, _movementMaxSpeed);
		}
		break;
	case MoveBack:
		if (!isMovementKeyPressed(MoveForward)) {
			camState->addMotionComponent(2, OVector3(0.0f, 0.0f, -_movementAcceleration), OState::Object);
			camState->minConstraint(1)->setValue(OStateConstraint::z, true, -_movementMaxSpeed);
			camState->maxConstraint(1)->setValue(OStateConstraint::z, false);
		}
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
