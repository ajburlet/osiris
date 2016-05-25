#include "OsirisSDK/OCamera.h"

/**
 \brief Class constructor.

 The constructor takes in all the attributes of the scene camera.

 \param fieldOfViewDeg Angle of the camera's field of view in degrees.
 \param aspectRatio The aspect ration of the screen (width/height).
 \param zNear Nearest camera depth.
 \param zFar Farthest camera depth.
 \param pos Camera position.
 \param dir Camera direction vector.
*/
OCamera::OCamera(float fieldOfViewDeg, float aspectRatio, float zNear, float zFar, const OVector3 & pos, const OVector3 & dir) :
	_perspectiveChanged(true),
	_cameraChanged(true),
	_fieldOfViewDeg(fieldOfViewDeg),
	_aspectRatio(aspectRatio),
	_zNear(zNear),
	_zFar(zFar),
	_position(pos),
	_direction(dir)
{
	_perspectiveTransform.push();
	_cameraTransform.push();
}

/**
 \brief Class destructor.
*/
OCamera::~OCamera()
{
}

/**
 \brief Sets the camera field of view.
 \param valueDeg Field of view in degrees.
*/
void OCamera::setFieldOfView(float valueDeg)
{
	_perspectiveChanged = true;
	_fieldOfViewDeg = valueDeg;
}

/**
 \brief Sets camera aspect ratio (width/height).
*/
void OCamera::setAspectRatio(float value)
{
	_perspectiveChanged = true;
	_aspectRatio = value;
}

/**
 \brief Sets camera depth limits.
 \param zNear Nearest camera depth.
 \param zFar Farthest camera depth.
*/
void OCamera::setCameraLimits(float zNear, float zFar)
{
	_perspectiveChanged = true;
	_zNear = zNear;
	_zFar = zFar;
}

/**
 \brief Set camera position.
 \param position Camera position coordinates.
*/
void OCamera::setPosition(const OVector3 & position)
{
	_cameraChanged = true;
	_position = position;
}

/**
 \brief Set camera direction.
 \param direction Camera direction vector.
*/
void OCamera::setDirection(const OVector3 & direction)
{
	_cameraChanged = true;
	_direction = direction;
}

/**
 \brief Returns the camera field of view.
 \return Field of view in degrees.
*/
float OCamera::fieldOfViewDegrees() const
{
	return _fieldOfViewDeg;
}

/**
 \brief Returns screen aspect ratio.
 \return Screen aspect ratio (width/height)
*/
float OCamera::aspectRatio() const
{
	return _aspectRatio;
}

/**
 \brief Returns camera nearest limit.
*/
float OCamera::nearLimit() const
{
	return _zNear;
}

/**
 \brief Returns camera farthest limit.
*/
float OCamera::farLimit() const
{
	return _zFar;
}

/**
 \brief Returns the camera current position.
 \return Camera position coordinates.
*/
OVector3 OCamera::position() const
{
	return _position;
}

/**
 \brief Returns the camera looking direction.
 \return Camera direction vector.
*/
OVector3 OCamera::direction() const
{
	return _direction;
}

/**
 \brief Calculates the current perspective transformation matrix.
 \return Perspective transformation matrix.
*/
const OMatrixStack * OCamera::perspectiveTransform()
{
	if (_perspectiveChanged) {
		_perspectiveTransform.pop();
		_perspectiveTransform.push();
		_perspectiveTransform.perspective(_fieldOfViewDeg, _aspectRatio, _zNear, _zFar);
	}
	return &_perspectiveTransform;
}

/**
 \brief Calculates the camera transformation matrix.
 \return Camera transformation matrix.
*/
const OMatrixStack * OCamera::cameraTransform()
{
	if (_cameraChanged) {
		_cameraTransform.pop();
		_cameraTransform.push();
		_cameraTransform.camera(_position, _direction);
	}
	return &_cameraTransform;
}
