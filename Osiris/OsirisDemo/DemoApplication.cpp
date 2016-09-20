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
}

void DemoApplication::init()
{
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


	/* setting up Torus */
	OVertexColorMesh* torus = new OVertexColorMesh();
	OWavefrontObjectFile torusFile("Meshes/TestTorus.obj");
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

	cube->init();

	_cube = cube;

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

	/* update last time index */
	_last_timeIndex_ms = timeIndex_ms;
}

void DemoApplication::onKeyboardPress(unsigned char key, int mouse_x, int mouse_y)
{
	if (key == ' ') _pauseFlag = !_pauseFlag;
}
