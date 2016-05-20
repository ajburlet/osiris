#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OApplication.h"

#include <glload/gl_load.hpp>
#include <gl/freeglut.h>

using namespace std;

OApplication* OApplication::_activeInstance = NULL;

OApplication::OApplication(const char* title, int argc, char **argv, int windowPos_x, int windowPos_y, int windowWidth, int windowHeight)
{
	if (_activeInstance != NULL) throw OException("There is already an OApplication instance created.");
	_activeInstance = this;

	/* GLUT init */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitContextVersion(OSIRIS_GL_VERSION);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPos_x, windowPos_y);
	int window = glutCreateWindow(title);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	/* GLload init */
	glload::LoadFunctions();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	if (!glload::IsVersionGEQ(OSIRIS_GL_VERSION)) {
		glutDestroyWindow(window);
		throw OException("Incorrect OpenGL version.");
	}

	/* setup callbacks */
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutReshapeFunc(resizeCallback);

	/* z-buffer */
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);
}

OApplication::~OApplication()
{
	_activeInstance = NULL;
}

OCamera * OApplication::camera()
{
	return &_cam;
}

void OApplication::start()
{
	init();
	glutMainLoop();
}

void OApplication::clearScreen()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	_activeInstance->clearScreen();
	_activeInstance->update(glutGet(GLUT_ELAPSED_TIME));
	glutSwapBuffers();
	glutPostRedisplay();
}
