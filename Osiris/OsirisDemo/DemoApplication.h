#pragma once

#include <OsirisSDK/OApplication.h>
#include <OsirisSDK/OMesh.h>
#include <OsirisSDK/OText2D.h>

class DemoApplication :	public OApplication
{
public:
	DemoApplication(int argc=0, char **argv=NULL);
	virtual ~DemoApplication();

	// Inherited via OApplication
	virtual void init() override;
	virtual void update(int timeIndex_ms) override;
	virtual void onKeyboardPress(unsigned char key, int mouse_x, int mouse_y) override;

private:
	OMesh* _cube;
	OMesh* _torus;
	OText2D* _title;
	OText2D* _fpsText;
	OText2D* _cameraText;
	OMatrixStack _mtx;

	float _movRadiusA;
	float _movRadiusB;
	float _thetaA;
	float _thetaB;
	float _periodA;
	float _periodB;

	int _last_timeIndex_ms;

	bool _pauseFlag;
};

