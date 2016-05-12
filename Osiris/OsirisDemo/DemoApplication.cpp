#include <OsirisSDK/OVertexColorMesh.h>

#include "DemoApplication.h"

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
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);

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

	/*
	cube->addVertexData(-0.5f, -0.5f, 0.0f);
	cube->addVertexData(0.5f, -0.5f, 0.0f);
	cube->addVertexData(0.0f, 0.5f, 0.0f);
	cube->addVertexColorData(1.0f, 0.0f, 0.0f, 1.0f);
	cube->addVertexColorData(0.0f, 1.0f, 0.0f, 1.0f);
	cube->addVertexColorData(0.0f, 0.0f, 1.0f, 1.0f);
	cube->addIndexData(0, 1, 2);
	*/

	camera()->setPosition(OVector3(0.0f, 0.0f, 0.0f));
	camera()->setDirection(OVector3(0.0f, 0.0f, 1.0f));

	cube->init();

	_cube = cube;

	_mtx.translate(0.0f, 0.5f, 2.0f);
	_mtx.rotateZ(45.0f);
	_mtx.scale(0.5f);
}

void DemoApplication::update(int deltaTime_ms)
{
	_cube->render(camera(), &_mtx);
}
