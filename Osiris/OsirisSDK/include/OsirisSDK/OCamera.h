#pragma once

#include <glm/vec3.hpp>

#include "OMatrixStack.h"

class OCamera
{
public:
	OCamera(float fieldOfViewDeg=45.0f, float aspectRatio=4.0f/3, float zNear=1.0f, float zFar=5.0f, 
		const glm::vec3 &pos=glm::vec3(0, 0, -1), const glm::vec3 &dir=glm::vec3(0));
	virtual ~OCamera();

	void setFieldOfView(float valueDeg);
	void setAspectRatio(float value);
	void setCameraLimits(float zNear, float zFar);
	void setPosition(const glm::vec3 &position);
	void setDirection(const glm::vec3 &direction);

	float fieldOfViewDegrees() const;
	float aspectRation() const;
	float nearLimit() const;
	float farLimit() const;
	glm::vec3 position() const;
	glm::vec3 direction() const;

	const OMatrixStack *perspectiveTransform();
	const OMatrixStack *cameraTransform();

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
	glm::vec3 _position;
	glm::vec3 _direction;

	/* calculated matrices */
	OMatrixStack _perspectiveTransform;
	OMatrixStack _cameraTransform;
};

