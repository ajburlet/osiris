#pragma once

#include "defs.h"
#include "OMatrixStack.h"
#include "OMath.h"

/**
 \brief Class that represents a camera on the scene.
*/
class OAPI OCamera
{
public:
	/**
	 \brief Class constructor.

	 The constructor takes in all the attributes of the scene camera.

	 \param fieldOfViewDeg Angle of the camera's field of view in degrees.
	 \param aspectRatio The aspect ration of the screen (width/height).
	 \param zNear Nearest camera depth.
	 \param zFar Farthest camera depth.
	 \param pos Camera position.
	 \param dir Camera direction vector as a unit vector in spherical coordinates (r, theta, phi).
	*/
	OCamera(float fieldOfViewDeg = 45.0f, float aspectRatio = 4.0f / 3, float zNear = 1.0f, float zFar = 5.0f,
		const OVector3 &pos = OVector3(0.0f , 0.0f, -1.0f), const OVector3 &dir = OVector3(1.0f, 90.0f, 90.0f));

	/**
	 \brief Class destructor.
	*/
	virtual ~OCamera();

	/**
	 \brief Sets the camera field of view.
	 \param valueDeg Field of view in degrees.
	*/
	void setFieldOfView(float valueDeg);
	
	/**
	 \brief Sets camera aspect ratio (width/height).
	*/
	void setAspectRatio(float value);
	
	/**
	 \brief Sets camera depth limits.
	 \param zNear Nearest camera depth.
	 \param zFar Farthest camera depth.
	*/
	void setCameraLimits(float zNear, float zFar);
	
	/**
	 \brief Set camera position.
	 \param position Camera position coordinates.
	*/
	void setPosition(const OVector3 &position);

	/**
	 \brief Change camera position using a displacement vector.
	 \param displacement Displacement vector.
	 */
	void changePosition(const OVector3 &displacement);

	/**
	 \brief Set camera direction.
	 \param direction Camera direction vector in spherical coordinates.
	*/
	void setDirection(const OVector3 &direction);

	/**
	 \brief Change camera direction by incrementing azimuthal and polar angles.
	 \param deltaTheta Change in azimulthal angle in degrees.
	 \param deltaPhi Change in polar angle in degrees.
	 */
	void changeDirection(float deltaTheta, float deltaPhi);

	/**
	 \brief Returns the camera field of view.
	 \return Field of view in degrees.
	*/
	float fieldOfViewDegrees() const;

	/**
	 \brief Returns screen aspect ratio.
	 \return Screen aspect ratio (width/height)
	*/
	float aspectRatio() const;

	/**
	 \brief Returns camera nearest limit.
	*/
	float nearLimit() const;
	
	/**
	 \brief Returns camera farthest limit.
	*/
	float farLimit() const;
	
	/**
	 \brief Returns the camera current position.
	 \return Camera position coordinates.
	*/
	OVector3 position() const;
	
	/**
	 \brief Returns the camera looking direction.
	 \return Camera direction vector in spherical coordinates.
	*/
	OVector3 direction() const;

	/**
	 \brief Calculates the perspective and camera transformations.
	 \return Matrix stack containing the transformation matrix.
	 */
	const OMatrixStack* transform();
	
private:
	/* control change to avoid unnecessary matrix recalculation */
	bool _perspectiveChanged; 
	bool _cameraChanged;

	/* perspective */
	float _fieldOfViewDeg;
	float _aspectRatio;
	float _zNear;
	float _zFar;

	/* camera */
	OVector3 _position;
	OVector3 _direction;

	/* transform matrix: camera + perspective */
	OMatrixStack _transform;
};

