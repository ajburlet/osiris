#pragma once

#include <OsirisSDK/OApplication.h>
#include <OsirisSDK/OObject.h>
#include <OsirisSDK/OMesh.h>
#include <OsirisSDK/OText2D.h>
#include <OsirisSDK/OCameraController.h>

class DemoApplication :	public OApplication, public OObject
{
public:
	DemoApplication(int argc=0, char **argv=NULL);
	virtual ~DemoApplication();

	// Inherited via OApplication
	virtual void init() override;
	virtual void update(const OTimeIndex& timeIndex) override;
	virtual void render() override;
	
	void onKeyboardPress(const OKeyboardPressEvent *evt);

private:
	OMesh* _cube;
	OMesh* _torus;
	OFont *_fontCourier;
	OText2D* _title;
	OText2D* _fpsText;
	OText2D* _perfText;
	OText2D* _idleText;
	OText2D* _cameraText;
	OText2D* _renderText;
	OCameraController _camCtrl;

	float _movRadiusA;
	float _movRadiusB;
	float _thetaA;
	float _thetaB;
	float _periodA;
	float _periodB;

	OTimeIndex _last_timeIndex;

	bool _pauseFlag;
};

