#include <OsirisSDK/OVertexColorMesh.h>
#include <OsirisSDK/OWavefrontObjectFile.h>

#include "DemoSimulation.h"
#include "Environment.h"

DemoSimulation::DemoSimulation(int argc, char **argv) :
	OSimulation("DemoSimulation", argc, argv),
	_camCtrl(this)
{
}

DemoSimulation::~DemoSimulation()
{
}

void DemoSimulation::init()
{
	/* setting initial position & orientation */
	camera()->setPosition(OVector3(3.0f, 1.5f, 7.0f));
	camera()->setOrientation(OVector3(-30.0f, 30.0f, 0.0f));
	camera()->setCameraLimits(1.0f, 100.0f);

	/* set camera movement keys */
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_a, OCameraController::MoveLeft);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_s, OCameraController::MoveBack);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_d, OCameraController::MoveRight);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_w, OCameraController::MoveForward);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_q, OCameraController::MoveUp);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_e, OCameraController::MoveDown);
	
	/* subscribe to keyboard event */
	addEventRecipient(OEvent::KeyboardPressEvent, this);

	/* setting up the cube */
	OVertexColorMesh *cube = new OVertexColorMesh();
	cube->addVertexData(-0.5f, -0.5f, -0.5f);
	cube->addVertexData(-0.5f, -0.5f, 0.5f);
	cube->addVertexData(-0.5f, 0.5f, -0.5f);
	cube->addVertexData(-0.5f, 0.5f, 0.5f);
	cube->addVertexData(0.5f, -0.5f, -0.5f);
	cube->addVertexData(0.5f, -0.5f, 0.5f);
	cube->addVertexData(0.5f, 0.5f, -0.5f);
	cube->addVertexData(0.5f, 0.5f, 0.5f);

	OVector4 color1(1.0f, 0.8f, 1.0f, 1.0f);
	OVector4 color2(0.0f, 0.2f, 0.0f, 1.0f);
	cube->addVertexColorData(color1);
	cube->addVertexColorData(color1);
	cube->addVertexColorData(color2);
	cube->addVertexColorData(color2);
	cube->addVertexColorData(color1);
	cube->addVertexColorData(color1);
	cube->addVertexColorData(color2);
	cube->addVertexColorData(color2);

	cube->addIndexData(0, 4, 1);
	cube->addIndexData(1, 2, 0);
	cube->addIndexData(1, 4, 5);
	cube->addIndexData(1, 7, 3);
	cube->addIndexData(2, 4, 0);
	cube->addIndexData(2, 7, 6);
	cube->addIndexData(3, 2, 1);
	cube->addIndexData(3, 7, 2);
	cube->addIndexData(4, 7, 5);
	cube->addIndexData(5, 7, 1);
	cube->addIndexData(6, 4, 2);
	cube->addIndexData(6, 7, 4);

	cube->setFaceCulling(true, OMesh::CullFace_Front, OMesh::CullFront_CW);
	cube->init();

	/* loading torus mesh from file */
	OVertexColorMesh* torus = new OVertexColorMesh();
	OWavefrontObjectFile torusFile("Meshes/TestTorus.mesh.obj");
	int objectCount=0;
	const char **objectList = torusFile.objectList(&objectCount);
	if (objectCount > 0) torusFile.loadMesh(objectList[0], torus);

	for (int i = 0; i < torus->vertexCount(); i++) {
		OVector3 v = torus->vertexData(i);
		torus->addVertexColorData(fabs(v.x()), fabs(v.y()), fabs(v.z()), 1);
	}
	torus->init();

	/* creating the table entity */
	EnvironmentAttributes *tableAtt = new EnvironmentAttributes(OVector3(-1.5f, 0, -3.5f), OVector3(1.5f, 0, 3.5f));
	EnvironmentBehavior *tableBehavior = new EnvironmentBehavior;
	OEntity<EnvironmentAttributes> *table = new OEntity<EnvironmentAttributes>(tableAtt, tableBehavior, cube);
	table->state()->curr()->position() = OVector3(0.0f, -0.25f/2, 0.0f);
	table->state()->curr()->scale() = OVector3(3.0f, 0.15f, 7.0f);
	entities()->add(table);

	/* creating the moving piece */
	_movingPiece = new OEntity<void>(NULL, NULL, torus);
	tableAtt->addPiece(_movingPiece);
	entities()->add(_movingPiece);
	_movingPiece->state()->curr()->position() = OVector3(0.0f, 0.0f, 0.0f);
	_movingPiece->state()->curr()->setMotionComponent(1, OVector3(0.3f, 0.0f, 0.3f) / 1e6, OState::Object);
	_movingPiece->state()->curr()->scale() = OVector3(0.25);

	/* creating text */
	_fontCourier = new OFont("cour.ttf");
	_title = new OText2D(_fontCourier, 12, -1.0f, 0.95f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	_title->setContent("Osiris Framework\nDemo Application");
	_infoText = new OText2D(_fontCourier, 12, 0.45f, 0.95f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	_motionText = new OText2D(_fontCourier, 12, -1.0f, 0.50f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	_cameraText = new OText2D(_fontCourier, 12, -1.0f, -0.90f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	renderObjects()->add(_title);
	renderObjects()->add(_infoText);
	renderObjects()->add(_motionText);
	renderObjects()->add(_cameraText);
}

void DemoSimulation::update(const OTimeIndex & idx)
{
	char buff[256];
	
	/* update simulation */
	OSimulation::update(idx);

	/* update camera */
	_camCtrl.update(idx);

	/* calculate FPS average and update the text object (to show on the screen) */
	if (targetFPS() == 0) snprintf(buff, 32, "%.02f fps", fpsStats().average());
	else snprintf(buff, 256, "%.02f/%d fps", fpsStats().average(), targetFPS());
	
	/* simulation stats and idle time */
	snprintf(buff, 256,
		 "%s\n"
		 "Perf coef: %.04f\n"
		 "Idle time: %.02f us\n"
		 "Render time: %.02f us", 
		 buff, performanceStats().average(), idleTimeStats().average(), renderTimeStats().average());
	_infoText->setContent(buff);

	/* motion info */
	OVector3 movPos = _movingPiece->state()->curr()->position();
	OVector3 movSpd = _movingPiece->state()->curr()->motionComponent(1) * 1e6;
	snprintf(buff, 256, "Moving piece\nPosition: (%.02f, %.02f, %.02f)\nVelocity:(%.02f, %.02f, %.02f)",
		movPos.x(), movPos.y(), movPos.z(), movSpd.x(), movSpd.y(), movSpd.z());
	_motionText->setContent(buff);

	/* camera speed and position */
	OVector3 camSpeed = camera()->state()->motionComponent(1, OState::Scene) * 1e6;
	OVector3 orientation = camera()->state()->orientation().toEulerAngles();
	snprintf(buff, 128, "Camera @ (%.02f, %.02f, %.02f), spd: (%.02f, %.02f, %.02f)/sec, or: Euler(%.02f, %.02f, %.02f)",
		camera()->position().x(), camera()->position().y(), camera()->position().z(),
		camSpeed.x(), camSpeed.y(), camSpeed.z(),
		orientation.x(), orientation.y(), orientation.z()
	);
	_cameraText->setContent(buff);
}

void DemoSimulation::onKeyboardPress(const OKeyboardPressEvent * evt)
{
	switch (evt->code()) {
	case OKeyboardPressEvent::OKey_Space:
		if (targetFPS() == 0) setTargetFPS(40);
		else setTargetFPS(0);
		break;
	}
}

