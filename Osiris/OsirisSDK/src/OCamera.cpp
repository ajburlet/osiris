#include "..\include\OsirisSDK\OCamera.h"

OCamera::OCamera(float fieldOfViewDeg, float aspectRatio, float zNear, float zFar, const glm::vec3 & pos, const glm::vec3 & dir) :
	_perspectiveChanged(true),
	_cameraChanged(true),
	_fieldOfViewDeg(fieldOfViewDeg),
	_aspectRatio(aspectRatio),
	_zNear(zNear),
	_zFar(zFar),
	_position(pos),
	_direction(dir)
{
	/* we save the initial state of */
	_perspectiveTransform.push();
	_cameraTransform.push();
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

void OCamera::setPosition(const glm::vec3 & position)
{
	_cameraChanged = true;
	_position = position;
}

void OCamera::setDirection(const glm::vec3 & direction)
{
	_cameraChanged = true;
	_direction = direction;
}

float OCamera::fieldOfViewDegrees() const
{
	return _fieldOfViewDeg;
}

float OCamera::aspectRation() const
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

glm::vec3 OCamera::position() const
{
	return _position;
}

glm::vec3 OCamera::direction() const
{
	return _direction;
}

const OMatrixStack * OCamera::perspectiveTransform()
{
	if (_perspectiveChanged) {
		_perspectiveTransform.pop();
		_perspectiveTransform.push();
		_perspectiveTransform.perspective(_fieldOfViewDeg, _aspectRatio, _zNear, _zFar);
	}
	return &_perspectiveTransform;
}

const OMatrixStack * OCamera::cameraTransform()
{
	if (_cameraChanged) {
		_cameraTransform.pop();
		_cameraTransform.push();
		_cameraTransform.camera(_position, _direction);
	}
	return &_cameraTransform;
}
