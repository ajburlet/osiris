#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OQuaternion.hpp"
#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/OState.h"
#include "OsirisSDK/OCamera.h"

const OVector3F OCamera::DefaultPosition	= OVector3F(0.0f);
const OVector3F OCamera::DefaultOrientation	= OVector3F(0.0f, 0.0f, -1.0f);
constexpr float OCamera::DefaultFieldOfView;
constexpr float OCamera::DefaultAspectRatio;
constexpr float OCamera::DefaultZNear;
constexpr float OCamera::DefaultZFar;


struct OCamera::Impl {
	Impl(float aFieldOfViewDeg, float aAspectRatio, float aZNear, float aZFar) : 
		fieldOfViewDeg(aFieldOfViewDeg),
		aspectRatio(aAspectRatio),
		zNear(aZNear),
		zFar(aZFar),
		state(OState::Object)
	{}

	/* control change to avoid unnecessary matrix recalculation */
	bool perspectiveChanged = true;

	/* perspective */
	float fieldOfViewDeg;
	float aspectRatio;
	float zNear;
	float zFar;

	/* camera state */
	OState state;

	/* transform matrix: camera + perspective */
	OMatrixStack transform;
};

OCamera::OCamera(float aFieldOfViewDeg, float aAspectRatio, float aZNear, float aZFar)
{
	OExceptionPointerCheck(_impl = new Impl(aFieldOfViewDeg, aAspectRatio, aZNear, aZFar));
	setPosition(DefaultPosition);
	setOrientation(DefaultOrientation);
}

OCamera::OCamera(const OVector3F & aPosition, const OVector3F & aOrientation, float aFieldOfViewDeg,
		 float aAspectRatio, float aZNear, float aZFar) 
{
	OExceptionPointerCheck(_impl = new Impl(aFieldOfViewDeg, aAspectRatio, aZNear, aZFar));
	setPosition(aPosition);
	setOrientation(aOrientation);
}

OCamera::~OCamera()
{
	if (_impl != nullptr) delete _impl;
}

void OCamera::setFieldOfView(float aValueDeg)
{
	_impl->perspectiveChanged = true;
	_impl->fieldOfViewDeg = aValueDeg;
}

void OCamera::setAspectRatio(float aValue)
{
	_impl->perspectiveChanged = true;
	_impl->aspectRatio = aValue;
}

void OCamera::setCameraLimits(float aZNear, float aZFar)
{
	_impl->perspectiveChanged = true;
	_impl->zNear = aZNear;
	_impl->zFar = aZFar;
}

void OCamera::setPosition(const OVector3F & aPosition)
{
	_impl->state.setMotionComponent(0, aPosition, OState::Scene);
}

void OCamera::changePosition(const OVector3F & aDisplacement)
{
	_impl->state.addMotionComponent(0, aDisplacement, OState::Scene);
}

void OCamera::setOrientation(const OVector3F & aOrientation)
{
	_impl->state.setOrientation(aOrientation);
}

float OCamera::fieldOfViewDegrees() const
{
	return _impl->fieldOfViewDeg;
}

float OCamera::aspectRatio() const
{
	return _impl->aspectRatio;
}

float OCamera::nearLimit() const
{
	return _impl->zNear;
}

float OCamera::farLimit() const
{
	return _impl->zFar;
}

OVector3F& OCamera::position()
{
	return _impl->state.position();
}

OState * OCamera::state()
{
	return &_impl->state;
}

const OMatrixStack* OCamera::transform()
{
	/* perspective transformation */
	bool popCameraTransform = true;
	if (_impl->perspectiveChanged) {
		if (!_impl->transform.isEmpty()) {
			_impl->transform.pop(); /* pop camera */
			_impl->transform.pop(); /* pop perspective */
		}
		_impl->transform.perspective(_impl->fieldOfViewDeg, _impl->aspectRatio, _impl->zNear, _impl->zFar);
		
		popCameraTransform = false; /* camera transform will still be created */
	}

	if (popCameraTransform) _impl->transform.pop();
	_impl->transform.push();
	_impl->transform.camera(state()->position(), state()->position() + state()->orientation()*OVector3F(0.0f, 0.0f, -1.0f));
	return &_impl->transform;
}

