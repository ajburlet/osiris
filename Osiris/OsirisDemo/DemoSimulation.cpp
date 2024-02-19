#include <OsirisSDK/OString.hpp>
#include <OsirisSDK/OVector.hpp>
#include <OsirisSDK/OMesh.h>
#include <OsirisSDK/OQuaternion.hpp>
#include <OsirisSDK/OWavefrontObjectFile.h>
#include <OsirisSDK/OParameterList.h>
#include <OsirisSDK/OStats.hpp>
#include <OsirisSDK/OMeshGeometry.h>
#include <OsirisSDK/ORenderingEngine.h>
#include <OsirisSDK/OGeometryManager.h>

#include "DemoSimulation.h"
#include "PieceBehavior.h"

DemoSimulation::DemoSimulation(int argc, char **argv) :
	OSimulation("DemoSimulation", argc, argv, OApplication::GraphicsAPI::OpenGL),
	_camCtrl(this),
	_table(NULL),
	_movingPiece(NULL)
{
}

DemoSimulation::~DemoSimulation()
{
	if (_table) {
		delete _table->mesh();
		delete _table;
	}

	if (_movingPiece) {
		delete _movingPiece->attributes();
		delete _movingPiece->behavior();
		delete _movingPiece->mesh();
		delete _movingPiece;
	}
}

void DemoSimulation::init()
{
	/* setting initial position & orientation */
	camera()->setPosition(OVector3F(3.0f, 1.5f, 7.0f));
	camera()->setOrientation(OVector3F(-30.0f, 30.0f, 0.0f));
	camera()->setCameraLimits(1.0f, 100.0f);

	/* set camera movement keys */
	_camCtrl.setMoveEventKey(OKeyCode::a, OCameraController::MoveLeft);
	_camCtrl.setMoveEventKey(OKeyCode::s, OCameraController::MoveBack);
	_camCtrl.setMoveEventKey(OKeyCode::d, OCameraController::MoveRight);
	_camCtrl.setMoveEventKey(OKeyCode::w, OCameraController::MoveForward);
	_camCtrl.setMoveEventKey(OKeyCode::q, OCameraController::MoveUp);
	_camCtrl.setMoveEventKey(OKeyCode::e, OCameraController::MoveDown);
	
	/* subscribe to keyboard event */
	addEventRecipient(OEventType::KeyboardPressEvent, this);

	geometryManager().registerFile(OGeometryManager::FileType::WavefrontObjectFile,
					OSIRIS_DEMO_BASEDIR "/Meshes/Cube.mesh.obj",
					"TestTorus");
	auto torus_geometry = geometryManager().loadFromFile("TestTorus", "Cube", "TestTorus");

	auto torus = new OMesh;
	OExPointerCheck(torus);
	torus->setGeometry(torus_geometry);
	renderingEngine().load(torus);

	/* setting up the cube */
	//OVertexColorMesh *cube = new OVertexColorMesh();
	//cube->addVertexData(-0.5f, -0.5f, -0.5f);
	//cube->addVertexData(-0.5f, -0.5f, 0.5f);
	//cube->addVertexData(-0.5f, 0.5f, -0.5f);
	//cube->addVertexData(-0.5f, 0.5f, 0.5f);
	//cube->addVertexData(0.5f, -0.5f, -0.5f);
	//cube->addVertexData(0.5f, -0.5f, 0.5f);
	//cube->addVertexData(0.5f, 0.5f, -0.5f);
	//cube->addVertexData(0.5f, 0.5f, 0.5f);

	//OVector4F color1(1.0f, 0.8f, 1.0f, 1.0f);
	//OVector4F color2(0.0f, 0.2f, 0.0f, 1.0f);
	//cube->addVertexColorData(color1);
	//cube->addVertexColorData(color1);
	//cube->addVertexColorData(color2);
	//cube->addVertexColorData(color2);
	//cube->addVertexColorData(color1);
	//cube->addVertexColorData(color1);
	//cube->addVertexColorData(color2);
	//cube->addVertexColorData(color2);

	//cube->addIndexData(0, 4, 1);
	//cube->addIndexData(1, 2, 0);
	//cube->addIndexData(1, 4, 5);
	//cube->addIndexData(1, 7, 3);
	//cube->addIndexData(2, 4, 0);
	//cube->addIndexData(2, 7, 6);
	//cube->addIndexData(3, 2, 1);
	//cube->addIndexData(3, 7, 2);
	//cube->addIndexData(4, 7, 5);
	//cube->addIndexData(5, 7, 1);
	//cube->addIndexData(6, 4, 2);
	//cube->addIndexData(6, 7, 4);

	//cube->setFaceCulling(true, OMesh::CullFace_Front, OMesh::CullFront_CW);
	//cube->init();

	/* loading torus mesh from file */
	//OVertexColorMesh* torus = new OVertexColorMesh();
	//OWavefrontObjectFile torusFile("Meshes/TestTorus.mesh.obj");
	//int objectCount=0;
	//const char **objectList = torusFile.objectList(&objectCount);
	//if (objectCount > 0) torusFile.loadMesh(objectList[0], torus);

	//for (int i = 0; i < torus->vertexCount(); i++) {
	//	OVector3F v = torus->vertexData(i);
	//	torus->addVertexColorData(fabs(v.x()), fabs(v.y()), fabs(v.z()), 1);
	//}
	//torus->init();

	/* creating the table entity */
	//_table = new OEntity(NULL, NULL, cube);
	//_table->state()->curr()->position() = OVector3F(0.0f, -0.25f/2, 0.0f);
	//_table->state()->curr()->scale() = OVector3F(3.0f, 0.15f, 7.0f);
	//entities()->add(_table);

	/* creating moving piece */
	PieceBehavior *behavior = new PieceBehavior();
	OParameterList *attributeList = new OParameterList(4);
	(*attributeList)[PieceBehavior::attrMinX] = -1.5f;
	(*attributeList)[PieceBehavior::attrMinZ] = -3.5f;
	(*attributeList)[PieceBehavior::attrMaxX] = 1.5f;
	(*attributeList)[PieceBehavior::attrMaxZ] = 3.5f;
	_movingPiece = new OEntity(attributeList, behavior, torus);
	_movingPiece->state()->curr()->position() = OVector3F(0.0f, 0.0f, 0.0f);
	_movingPiece->state()->curr()->setMotionComponent(1, OVector3F(0.3f, 0.0f, 0.3f) / 1e6, OState::Object);
	_movingPiece->state()->curr()->scale() = OVector3F(0.25);
	addEntity(_movingPiece);

	/* creating text */
	_fontCourier = new OFont(&renderingEngine(), "cour.ttf");
	_title = new OText2D(_fontCourier, 12, OVector2I32(10, 12), OVector4FL(0.0f, 1.0f, 0.0f, 1.0f));
	_title->setContent("Osiris Framework\nDemo Application");
	_infoText = new OText2D(_fontCourier, 12, OVector2I32(400, 12), OVector4FL(0.0f, 1.0f, 0.0f, 1.0f));
	_motionText = new OText2D(_fontCourier, 12, OVector2I32(10, 600), OVector4FL(0.0f, 1.0f, 0.0f, 1.0f));
	_cameraText = new OText2D(_fontCourier, 12, OVector2I32(10, 620), OVector4FL(0.0f, 1.0f, 0.0f, 1.0f));
	addVisualObject(_title);
	addVisualObject(_infoText);
	addVisualObject(_motionText);
	addVisualObject(_cameraText);
}

void DemoSimulation::update(const OTimeIndex & idx, int step_us)
{
	char buff[256];
	
	/* update simulation */
	OSimulation::update(idx, step_us);

	/* update camera */
	_camCtrl.update(idx, step_us);

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
	/*
	OVector3F movPos = _movingPiece->state()->curr()->position();
	OVector3F movSpd = _movingPiece->state()->curr()->motionComponent(1) * 1e6;
	snprintf(buff, 256, "Moving piece\nPos: (%.02f, %.02f, %.02f)\nSpd: (%.02f, %.02f, %.02f)",
		movPos.x(), movPos.y(), movPos.z(), movSpd.x(), movSpd.y(), movSpd.z());
	_motionText->setContent(buff);
	*/

	/* camera speed and position */
	OVector3F camSpeed = camera()->state()->motionComponent(1, OState::Scene) * 1e6;
	OVector3F orientation = camera()->state()->orientation().toEulerAngles();
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
	case OKeyCode::Space:
		if (targetFPS() == 0) setTargetFPS(40);
		else setTargetFPS(0);
		break;
		
	case OKeyCode::o:
		camera()->setPosition(OVector3F(3.0f, 1.5f, 7.0f));
		camera()->state()->setMotionComponent(1, OVector3F(0.0f, 0.0f, 0.0f), OState::Scene);
		camera()->state()->setMotionComponent(2, OVector3F(0.0f, 0.0f, 0.0f), OState::Scene);
		camera()->setOrientation(OVector3F(-30.0f, 30.0f, 0.0f));
		camera()->setCameraLimits(1.0f, 100.0f);
		break;
	}
}

