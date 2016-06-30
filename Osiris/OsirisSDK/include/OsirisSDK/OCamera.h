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
	OCamera(float fieldOfViewDeg=45.0f, float aspectRatio=4.0f/3, float zNear=1.0f, float zFar=5.0f, 
		const OVector3 &pos=OVector3(0, 0, -1), const OVector3 &dir=OVector3(0, 0, 1));
	virtual ~OCamera();

	void setFieldOfView(float valueDeg);
	void setAspectRatio(float value);
	void setCameraLimits(float zNear, float zFar);
	void setPosition(const OVector3 &position);
	void setDirection(const OVector3 &direction);

	float fieldOfViewDegrees() const;
	float aspectRatio() const;
	float nearLimit() const;
	float farLimit() const;
	OVector3 position() const;
	OVector3 direction() const;

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

