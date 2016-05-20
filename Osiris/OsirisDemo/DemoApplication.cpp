#include <OsirisSDK/OVertexColorMesh.h>

#include "DemoApplication.h"

#define PI	3.1415f

const float toRad = 2 * PI / 360.0f;

DemoApplication::DemoApplication(int argc, char **argv) :
	OApplication("DemoApplication", argc, argv),
	_cube(NULL)
{
}

DemoApplication::~DemoApplication()
{
	delete _cube;
}

void DemoApplication::init()
{
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

	camera()->setPosition(OVector3(0.0f, 0.0f, -2.0f));
	camera()->setDirection(OVector3(0.0f, 0.0f, 1.0f));

	cube->init();

	_cube = cube;

	_movRadiusA = 1.0f;
	_movRadiusB = 1.0f;
	_thetaA = 0;
	_thetaB = PI;
	_periodA = 3.0f;
	_periodB = 3.0f;

	_pauseFlag = false;
	_last_timeIndex_ms = 0;
}

void DemoApplication::update(int timeIndex_ms)
{
	OVector3 posA(0.0f);
	OVector3 posB(0.0f);
	int deltaTime_ms = timeIndex_ms - _last_timeIndex_ms;

	if (!_pauseFlag) {
		_thetaA = _thetaA + 2 * PI * deltaTime_ms / (_periodA * 1000);
		_thetaB = _thetaB + 2 * PI * deltaTime_ms / (_periodB * 1000);

		if (_thetaA > 2 * PI) _thetaA -= 2 * PI;
		if (_thetaB > 2 * PI) _thetaB -= 2 * PI;
	}

	posA.setX(_movRadiusA*cosf(_thetaA));
	posA.setZ(_movRadiusA*sinf(_thetaA));
	posB.setX(_movRadiusB*cosf(_thetaB));
	posB.setZ(_movRadiusB*sinf(_thetaB));

	_mtx.push();
	_mtx.translate(posA);
	_mtx.scale(0.5f);
	_cube->render(camera(), &_mtx);
	_mtx.pop();

	_mtx.push();
	_mtx.translate(posB);
	_mtx.scale(0.5f);
	_cube->render(camera(), &_mtx);
	_mtx.pop();

	_last_timeIndex_ms = timeIndex_ms;
}
