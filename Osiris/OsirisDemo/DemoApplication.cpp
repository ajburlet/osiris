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
	cube->addVertexData(-1.0f, -1.0f, -1.0f);
	cube->addVertexData(-1.0f, -1.0f, 1.0f);
	cube->addVertexData(-1.0f, 1.0f, -1.0f);
	cube->addVertexData(-1.0f, 1.0f, 1.0f);
	cube->addVertexData(1.0f, -1.0f, -1.0f);
	cube->addVertexData(1.0f, -1.0f, 1.0f);
	cube->addVertexData(1.0f, 1.0f, -1.0f);
	cube->addVertexData(1.0f, 1.0f, 1.0f);

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

	cube->init();

	_cube = cube;

	_mtx.rotateX(45.0f);
}

void DemoApplication::update(int deltaTime_ms)
{
	_cube->render(camera(), &_mtx);
}
