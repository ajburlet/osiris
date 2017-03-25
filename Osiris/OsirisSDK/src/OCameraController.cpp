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
	_lastTimeIndex_ms(0),
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
	if (_keyBind.size() == 0) _app->addEventRecipient(OEvent::KeyboardPressEvent, this);
	_keyBind[key] = camEvt;
}

void OCameraController::update(int timeIndex_ms)
{
	float delta_ts = (float)(timeIndex_ms - _lastTimeIndex_ms) / 1000.0f;
	OVector3 camDispl = OVector3(0);

	/* update camera orientation (direction vector) */
	float deltaTheta = 0.0f, deltaPhi = 0.0f;
	if (_delta_mouse_x != 0) deltaTheta = 2 * atanf((float)_delta_mouse_x / 2 / _app->camera()->nearLimit());
	if (_delta_mouse_y != 0) deltaPhi = 2 * atanf((float)_delta_mouse_y / 2 / _app->camera()->nearLimit());
	if (deltaTheta != 0 || deltaPhi != 0) _app->camera()->changeDirection(deltaTheta, deltaPhi);

	_delta_mouse_x = 0;
	_delta_mouse_y = 0;

	/* update camera position for each direction */
	for (map<CameraMoveDir, bool>::iterator it = _pressedKeys.begin(); it != _pressedKeys.end(); it++) {
		map<CameraMoveDir, float>::iterator itSpd = _directionSpeed.find(it->first);
		if (itSpd == _directionSpeed.end()) {
			pair<map<CameraMoveDir, float>::iterator, bool> insRet;
			insRet = _directionSpeed.insert(make_pair(it->first, 0.0f));
			itSpd = insRet.first;
		}

		/* update current speed */
		if (it->second) {
			itSpd->second += (float)delta_ts * _movementAcceleration;
			if (itSpd->second > _movementMaxSpeed) itSpd->second = _movementMaxSpeed;
		} else {
			itSpd->second -= (float)delta_ts * _movementAcceleration;
			if (itSpd->second < 0) itSpd->second = 0;
		}

		/* update camera position */
		float compDispl = _directionSpeed[it->first] * delta_ts; /* component displacement */
		switch (it->first) {
		case MoveBack:
			compDispl = -compDispl;
		case MoveForward:
			camDispl += OVector3(0, 0, compDispl);
			break;

		case MoveRight:
			compDispl = -compDispl;
		case MoveLeft:
			camDispl += OVector3(compDispl, 0, 0);
			break;

		case MoveDown:
			compDispl = -compDispl;
		case MoveUp:
			camDispl += OVector3(0, compDispl, 0);
			break;
		}

		it->second = false;
	}
	_app->camera()->changePosition(camDispl);
	_lastTimeIndex_ms = timeIndex_ms;
}

void OCameraController::onKeyboardPress(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;
	_pressedKeys[it->second] = true;
}

void OCameraController::onMouseMove(const OMouseMoveEvent * evt)
{
	/* making sure it's either passive mouse movement or active with the mouse button pressed */
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
