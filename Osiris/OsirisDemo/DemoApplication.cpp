#include <OsirisSDK/OVertexColorMesh.h>
#include <OsirisSDK/OWavefrontObjectFile.h>

#include "DemoApplication.h"

#define PI	3.1415f

const float toRad = 2 * PI / 360.0f;

DemoApplication::DemoApplication(int argc, char **argv) :
	OApplication("DemoApplication", argc, argv),
	_cube(NULL),
	_torus(NULL)
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
	/* subscribe to keyboard event */
	addEventRecipient(OEvent::KeyboardPressEvent, this);

	/* text */
	_fontCourier = new OFont("cour.ttf");
	_title = new OText2D(_fontCourier, 12, -1.0f, -0.95f, OVector4(0.0f, 1.0f, 0.0f, 1.0f), 2.0f/windowWidth(), 2.0f/windowHeight());
	_title->setContent("Osiris Framework");
	_fpsText = new OText2D(_fontCourier, 12, 0.8f, -0.95f, OVector4(0.0f, 1.0f, 0.0f, 1.0f), 2.0f / windowWidth(), 2.0f / windowHeight());
	_cameraText = new OText2D(_fontCourier, 12, -1.0f, 0.90f, OVector4(0.0f, 1.0f, 0.0f, 1.0f), 2.0f / windowWidth(), 2.0f / windowHeight());

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
	camera()->setPosition(OVector3(0.0f, 0.0f, -3.0f));
	camera()->setDirection(OVector3(0.0f, 0.0f, 1.0f));

	_movRadiusA = 1.0f;
	_movRadiusB = 1.0f;
	_thetaA = 0;
	_thetaB = PI;
	_periodA = 6.0f;
	_periodB = 6.0f;

	_pauseFlag = false;
	_last_timeIndex_ms = 0;
}

void DemoApplication::update(int timeIndex_ms)
{
	OVector3 posA(0.0f);
	OVector3 posB(0.0f);
	int deltaTime_ms = timeIndex_ms - _last_timeIndex_ms;
	char fpsBuff[32];
	char cameraBuff[128];

	/* calculate FPS and update the text object (to show on the screen) */
	snprintf(fpsBuff, 32, "%.02f fps", 1 / ((float)deltaTime_ms / 1000));
	_fpsText->setContent(fpsBuff);
	snprintf(cameraBuff, 128, "Camera @ (%.02f, %.02f, %.02f), direction: (%.02f, %.02f, %.02f)",
		camera()->position().x(), camera()->position().y(), camera()->position().z(),
		camera()->direction().x(), camera()->direction().y(), camera()->direction().z());
	_cameraText->setContent(cameraBuff);

	/* calculating new positions */
	if (!_pauseFlag) {
		_thetaA = _thetaA + 2 * PI * deltaTime_ms / (_periodA * 1000);
		_thetaB = _thetaB + 2 * PI * deltaTime_ms / (_periodB * 1000);

		if (_thetaA > 2 * PI) _thetaA -= 2 * PI;
		if (_thetaB > 2 * PI) _thetaB -= 2 * PI;
	}

	/* calculating displacement vectors */
	posA.setX(_movRadiusA*cosf(_thetaA));
	posA.setZ(_movRadiusA*sinf(_thetaA));
	posB.setX(_movRadiusB*cosf(_thetaB));
	posB.setZ(_movRadiusB*sinf(_thetaB));

	/* Get initial matrix from camera related transforms */
	_mtx = *(camera()->transform());

	/* render cube */
	_mtx.push();
	_mtx.translate(posA);
	_mtx.scale(0.5f);
	_cube->render(&_mtx);
	_mtx.pop();

	/* render torus */
	_mtx.push();
	_mtx.translate(posB);
	_mtx.scale(0.25f);
	_mtx.rotateX(45.0f);
	_torus->render(&_mtx);
	_mtx.pop();

	/* render text */
	_title->render();
	_fpsText->render();
	_cameraText->render();

	/* update last time index */
	_last_timeIndex_ms = timeIndex_ms;
}

void DemoApplication::onKeyboardPress(const OKeyboardPressEvent *evt)
{
	if (evt->code() == OKeyboardPressEvent::OKey_Space) _pauseFlag = !_pauseFlag;
}
