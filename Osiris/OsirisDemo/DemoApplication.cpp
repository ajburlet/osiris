#include <OsirisSDK/OVertexColorMesh.h>
#include <OsirisSDK/OWavefrontObjectFile.h>

#include "DemoApplication.h"

#define PI	3.1415f

const float toRad = 2 * PI / 360.0f;

DemoApplication::DemoApplication(int argc, char **argv) :
	OApplication("DemoApplication", argc, argv),
	_cube(NULL),
	_torus(NULL),
	_camCtrl(this)
{
}

DemoApplication::~DemoApplication()
{
	delete _cube;
	delete _torus;
	delete _title;
	delete _fpsText;
	delete _cameraText;
}

void DemoApplication::init()
{
	/* set camera movement keys */
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_a, OCameraController::MoveLeft);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_s, OCameraController::MoveBack);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_d, OCameraController::MoveRight);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_w, OCameraController::MoveForward);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_q, OCameraController::MoveUp);
	_camCtrl.setMoveEventKey(OKeyboardPressEvent::OKey_e, OCameraController::MoveDown);
	
	/* subscribe to keyboard event */
	addEventRecipient(OEvent::KeyboardPressEvent, this);

	/* text */
	_fontCourier = new OFont("cour.ttf");
	_title = new OText2D(_fontCourier, 12, -1.0f, -0.95f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	_title->setContent("Osiris Framework");
	_fpsText = new OText2D(_fontCourier, 12, 0.55f, -0.95f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	_perfText = new OText2D(_fontCourier, 12, 0.55f, -0.90f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	_idleText = new OText2D(_fontCourier, 12, 0.55f, -0.85f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	_renderText = new OText2D(_fontCourier, 12, 0.55f, -0.80f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));
	_cameraText = new OText2D(_fontCourier, 12, -1.0f, 0.90f, OVector4(0.0f, 1.0f, 0.0f, 1.0f));

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

	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(0.0f, 1.0f, 0.0f, 1.0f);
	cube->addVertexColorData(0.0f, 0.0f, 1.0f, 1.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(0.0f, 1.0f, 0.0f, 1.0f);
	cube->addVertexColorData(0.0f, 0.0f, 1.0f, 1.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(0.0f, 1.0f, 0.0f, 1.0f);

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
	_cube = cube;


	/* setting up Torus */
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
	_torus = torus;

	/* camera */
	camera()->setCameraLimits(1.0f, 10.0f);
	camera()->setPosition(OVector3(0.0f, 0.0f, 3.0f));
	camera()->setOrientation(OVector3(0.0f));

	_movRadiusA = 1.0f;
	_movRadiusB = 1.0f;
	_thetaA = 0;
	_thetaB = PI;
	_periodA = 6.0f;
	_periodB = 6.0f;

	_pauseFlag = false;
	_last_timeIndex = OTimeIndex::current();
}

void DemoApplication::update(const OTimeIndex& timeIndex)
{
	int deltaTime_us = (timeIndex - _last_timeIndex).toInt();
	char buff[128];

	/* update camera */
	_camCtrl.update(timeIndex);

	/* calculate FPS average and update the text object (to show on the screen) */
	if (targetFPS() == 0) snprintf(buff, 32, "%.02f fps", fpsStats().average());
	else snprintf(buff, 32, "%.02f/%d fps", fpsStats().average(), targetFPS());
	_fpsText->setContent(buff);
	
	/* simulation stats and idle time */
	snprintf(buff, 32, "Perf coef: %.04f", performanceStats().average());
	_perfText->setContent(buff);
	snprintf(buff, 32, "Idle time: %.02f", idleTimeStats().average());
	_idleText->setContent(buff);
	snprintf(buff, 32, "Render time: %.02f", renderTimeStats().average());
	_renderText->setContent(buff);

	OVector3 camSpeed = camera()->state()->motionComponent(1, OState::Scene) * 1e6;
	OVector3 orientation = camera()->state()->orientation().toEulerAngles();
	snprintf(buff, 128, "Camera @ (%.02f, %.02f, %.02f), spd: (%.02f, %.02f, %.02f)/sec, or: Euler(%.02f, %.02f, %.02f)",
		camera()->position().x(), camera()->position().y(), camera()->position().z(),
		camSpeed.x(), camSpeed.y(), camSpeed.z(),
		orientation.x(), orientation.y(), orientation.z()
	);
	_cameraText->setContent(buff);

	/* calculating new positions */
	if (!_pauseFlag) {
		_thetaA = _thetaA + 2 * PI * deltaTime_us / (_periodA * 1000000);
		_thetaB = _thetaB + 2 * PI * deltaTime_us / (_periodB * 1000000);

		if (_thetaA > 2 * PI) _thetaA -= 2 * PI;
		if (_thetaB > 2 * PI) _thetaB -= 2 * PI;
	}
	
	/* update last time index */
	_last_timeIndex = timeIndex;
}

void DemoApplication::render()
{
	OVector3 posA(0.0f);
	OVector3 posB(0.0f);
	OMatrixStack mtx;

	/* calculating displacement vectors */
	posA.setX(_movRadiusA*cosf(_thetaA));
	posA.setZ(_movRadiusA*sinf(_thetaA));
	posB.setX(_movRadiusB*cosf(_thetaB));
	posB.setZ(_movRadiusB*sinf(_thetaB));

	/* Get initial matrix from camera related transforms */
	mtx = *(camera()->transform());

	/* render cube */
	mtx.push();
	mtx.translate(posA);
	mtx.scale(0.5f);
	_cube->render(&mtx);
	mtx.pop();

	/* render torus */
	mtx.push();
	mtx.translate(posB);
	mtx.scale(0.25f);
	mtx.rotateX(45.0f);
	_torus->render(&mtx);
	mtx.pop();

	/* render text */
	_title->render();
	_fpsText->render();
	_perfText->render();
	_idleText->render();
	_cameraText->render();
	_renderText->render();
}

void DemoApplication::onKeyboardPress(const OKeyboardPressEvent *evt)
{
	switch (evt->code()) {
	case OKeyboardPressEvent::OKey_Space: 
		_pauseFlag = !_pauseFlag;
		break;
	case OKeyboardPressEvent::OKey_L:
	case OKeyboardPressEvent::OKey_l:
		setTargetFPS((targetFPS() == 0) ? 40 : 0);
		break;
	}
}
