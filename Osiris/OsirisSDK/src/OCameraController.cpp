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
	_lastTimeIndex_ms(0),
	_activeMoveMouseBtnPressed(false),
	_last_mouse_x(-1),
	_last_mouse_y(-1),
	_delta_mouse_x(0),
	_delta_mouse_y(0),
	_cameraSpeed(0)
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
	float delta_ts = (float)(timeIndex_ms - _lastTimeIndex_ms) / 1000.0f;

	/* update camera orientation (direction vector) */
	float deltaTheta = 0.0f, deltaPhi = 0.0f;
	if (_delta_mouse_x != 0) deltaTheta = 2 * atanf((float)_delta_mouse_x / 2 / _app->camera()->nearLimit());
	if (_delta_mouse_y != 0) deltaPhi = 2 * atanf((float)_delta_mouse_y / 2 / _app->camera()->nearLimit());
	if (deltaTheta != 0 || deltaPhi != 0) _app->camera()->changeDirection(deltaTheta, deltaPhi);

	_delta_mouse_x = 0;
	_delta_mouse_y = 0;

	/* update camera position for each direction */
	updateCameraSpeed(delta_ts);
	updateCameraPosition(delta_ts);
	_lastTimeIndex_ms = timeIndex_ms;
}

void OCameraController::onKeyboardPress(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;
	_pressedKeys[it->second] = true;
}

void OCameraController::onKeyboardRelease(const OKeyboardPressEvent * evt)
{
	map<OKeyboardPressEvent::KeyCode, CameraMoveDir>::iterator it;
	if ((it = _keyBind.find(evt->code())) == _keyBind.end()) return;
	_pressedKeys[it->second] = false;
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

bool OCameraController::isMovementKeyPressed(CameraMoveDir dir)
{
	map<CameraMoveDir, bool>::iterator it = _pressedKeys.find(dir);
	return (it != _pressedKeys.end()) ? it->second : false;
}

void OCameraController::updateCameraSpeed(float deltaTs_s)
{
	float deltaSpd = deltaTs_s*_movementAcceleration;
	float newSpdComponent;

	/* movement on Z-axis: back/forward */
	newSpdComponent = 0.0f;
	if (!isMovementKeyPressed(MoveBack) && !isMovementKeyPressed(MoveForward)) {
		if (deltaSpd > abs(_cameraSpeed.z())) {
			newSpdComponent = -_cameraSpeed.z();
		} else if (_cameraSpeed.z() != 0.0f) {
			/* signal is reverse to the current momentum -- tend to zero */
			newSpdComponent = -abs(_cameraSpeed.z()) / _cameraSpeed.z()*deltaSpd; 
		}
	} else {
		if (isMovementKeyPressed(MoveBack)) newSpdComponent += -deltaSpd;
		if (isMovementKeyPressed(MoveForward)) newSpdComponent += deltaSpd;
	}
	_cameraSpeed.setZ(_cameraSpeed.z() + newSpdComponent);
	
	/* movement on X-axis: left/right */
	newSpdComponent = 0.0f;
	if (!isMovementKeyPressed(MoveLeft) && !isMovementKeyPressed(MoveRight)) {
		if (deltaSpd > abs(_cameraSpeed.x())) {
			newSpdComponent = -_cameraSpeed.x();
		} else if (_cameraSpeed.x() != 0.0f) {
			/* signal is reverse to the current momentum -- tend to zero */
			newSpdComponent = -abs(_cameraSpeed.x()) / _cameraSpeed.x()*deltaSpd; 
		}
	} else {
		if (isMovementKeyPressed(MoveLeft)) newSpdComponent += -deltaSpd;
		if (isMovementKeyPressed(MoveRight)) newSpdComponent += deltaSpd;
	}
	_cameraSpeed.setX(_cameraSpeed.x() + newSpdComponent);
	
	/* movement on Y-axis: up/down */
	newSpdComponent = 0.0f;
	if (!isMovementKeyPressed(MoveDown) && !isMovementKeyPressed(MoveUp)) {
		if (deltaSpd > abs(_cameraSpeed.y())) {
			newSpdComponent = -_cameraSpeed.y();
		} else if (_cameraSpeed.y() != 0.0f) {
			/* signal is reverse to the current momentum -- tend to zero */
			newSpdComponent = -abs(_cameraSpeed.y()) / _cameraSpeed.y()*deltaSpd; 
		}
	} else {
		if (isMovementKeyPressed(MoveDown)) newSpdComponent += -deltaSpd;
		if (isMovementKeyPressed(MoveUp)) newSpdComponent += deltaSpd;
	}
	_cameraSpeed.setY(_cameraSpeed.y() + newSpdComponent);
}

void OCameraController::updateCameraPosition(float deltaTs_s)
{
	OVector4 displacement = OVector4(_cameraSpeed.x(), _cameraSpeed.y(), _cameraSpeed.z(), 0.0f) * deltaTs_s;

	/* the displacement vector is in the camera's relative perspective. We have to transform the vector */
	OMatrixStack mtx;
	mtx.rotateX(90.0f - _app->camera()->direction().phi());
	mtx.rotateY(90.0f - _app->camera()->direction().theta());
	displacement = mtx*displacement;

	_app->camera()->changePosition(OVector3(displacement.x(), displacement.y(), displacement.z()));
}
