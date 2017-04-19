#include "OsirisSDK/OCamera.h"

OCamera::OCamera(float fieldOfViewDeg, float aspectRatio, float zNear, float zFar, const OVector3 & pos, const OVector3 & dir) :
	_perspectiveChanged(true),
	_cameraChanged(true),
	_fieldOfViewDeg(fieldOfViewDeg),
	_aspectRatio(aspectRatio),
	_zNear(zNear),
	_zFar(zFar),
	_state(OState::Object)
{
}

OCamera::~OCamera()
{
}

void OCamera::setFieldOfView(float valueDeg)
{
	_perspectiveChanged = true;
	_fieldOfViewDeg = valueDeg;
}

void OCamera::setAspectRatio(float value)
{
	_perspectiveChanged = true;
	_aspectRatio = value;
}

void OCamera::setCameraLimits(float zNear, float zFar)
{
	_perspectiveChanged = true;
	_zNear = zNear;
	_zFar = zFar;
}

void OCamera::setPosition(const OVector3 & position)
{
	_cameraChanged = true;
	_state.setMotionComponent(0, position, OState::Scene);
}

void OCamera::changePosition(const OVector3 & displacement)
{
	_cameraChanged = true;
	_state.addMotionComponent(0, displacement, OState::Scene);
}

void OCamera::setOrientation(const OVector3 & direction)
{
	_cameraChanged = true;
	_state.setOrientation(direction);
}

void OCamera::changeOrientation(const OVector3& rotation)
{
	_cameraChanged = true;
	_state.setOrientation(rotation);
}

float OCamera::fieldOfViewDegrees() const
{
	return _fieldOfViewDeg;
}

float OCamera::aspectRatio() const
{
	return _aspectRatio;
}

float OCamera::nearLimit() const
{
	return _zNear;
}

float OCamera::farLimit() const
{
	return _zFar;
}

const OVector3 OCamera::position() const
{
	return _state.motionComponent(0, OState::Scene);
}

const OVector3 OCamera::direction() const
{
	return _state.orientation();
}

OState * OCamera::state()
{
	return &_state;
}

const OMatrixStack* OCamera::transform()
{
	bool popCameraTransform = true;
	if (_perspectiveChanged) {
		if (!_transform.isEmpty()) {
			_transform.pop(); /* pop camera */
			_transform.pop(); /* pop perspective */
			_cameraChanged = true; /* force recalculate camera transform */
		}
		_transform.perspective(_fieldOfViewDeg, _aspectRatio, _zNear, _zFar);
		
		popCameraTransform = false; /* camera transform will still be created */
	}

	if (_cameraChanged) {
		if (popCameraTransform) _transform.pop();
		_transform.push();
		_transform.camera(_state.motionComponent(0, OState::Scene), 
				  _state.orientationQuaternion() * OVector3(0.0f, 0.0f, 1.0f) + _state.motionComponent(0, OState::Scene));
	}

	return &_transform;
}

