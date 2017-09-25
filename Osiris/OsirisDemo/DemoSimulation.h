#pragma once
#include <OsirisSDK/OSimulation.h>
#include <OsirisSDK/OObject.h>
#include <OsirisSDK/OEntity.hpp>
#include <OsirisSDK/OCameraController.h>
#include <OsirisSDK/OFont.h>
#include <OsirisSDK/OText2D.h>

class DemoSimulation : public OSimulation, public OObject
{
public:
	DemoSimulation(int argc, char **argv);
	~DemoSimulation();

	// Inherited via OSimulation
	virtual void init() override;

	void update(const OTimeIndex& idx) override;

	void onKeyboardPress(const OKeyboardPressEvent *evt);
private:
	OCameraController _camCtrl;
	OFont* _fontCourier;
	OText2D* _title;
	OText2D* _infoText;
	OText2D* _motionText;
	OText2D* _cameraText;
	OEntity<void>* _movingPiece;
};

