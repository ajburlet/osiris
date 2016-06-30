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
 \brief Calculates the perspective and camera transformations.
 \return Matrix stack containing the transformation matrix.
 */
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
		_transform.camera(_position, _direction);
	}

	return &_transform;
}

