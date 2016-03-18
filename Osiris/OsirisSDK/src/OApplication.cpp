#include <gl/freeglut.h>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OApplication.h"

using namespace std;

OApplication* OApplication::_activeInstance = NULL;

OApplication::OApplication(const string& title, int argc, char **argv, int windowPos_x, int windowPos_y, int windowWidth, int windowHeight)
{
	if (_activeInstance != NULL) throw OException("There is already an OApplication instance created.");
	_activeInstance = this;

	/* GLUT init */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPos_x, windowPos_y);
	glutCreateWindow(title.c_str());
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	init();

	/* setup callbacks */
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutReshapeFunc(resizeCallback);
}

OApplication::~OApplication()
{
	_activeInstance = NULL;
}

void OApplication::start()
{
	glutMainLoop();
}

void OApplication::onKeyboardPress(unsigned char key, int mouse_x, int mouse_y)
{
}

void OApplication::onWindowResize(int width, int height)
{
}

void OApplication::keyboardCallback(unsigned char key, int mouse_x, int mouse_y)
{
	_activeInstance->onKeyboardPress(key, mouse_x, mouse_y);
}

void OApplication::resizeCallback(int width, int height)
{
	_activeInstance->onWindowResize(width, height);
}

void OApplication::displayCallback()
{
	_activeInstance->update(glutGet(GLUT_ELAPSED_TIME));
}
