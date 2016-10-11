#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OApplication.h"

#include <glload/gl_load.hpp>
#include <gl/freeglut.h>

using namespace std;

OApplication* OApplication::_activeInstance = NULL;


/**
 \brief Class constructor.

 The constructor creates the window and initializes OpenGL.

 \param title Application window title.
 \param argc Number of command line arguments
 \param argv Command line arguments.
 \param windowPos_x Window position on the X axis.
 \param windowPos_y Window position on the Y axis.
 \param windowWidth Window width.
 \param windowHeight Window height.
 */
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

/**
 \brief Class destructor.
*/
OApplication::~OApplication()
{
	_activeInstance = NULL;
}

/**
 \brief Returns the application camera object.
 \return Pointer to the application camera object.
*/
OCamera * OApplication::camera()
{
	return &_cam;
}

/**
 \brief Returns the window width in pixels.
 */
int OApplication::windowWidth() const
{
	return glutGet(GLUT_WINDOW_WIDTH);
}

/**
 \brief Returns the window height in pixels.
 */
int OApplication::windowHeight() const
{
	return glutGet(GLUT_WINDOW_HEIGHT);
}

/**
 \brief Initializes the application and starts the main loop.
*/
void OApplication::start()
{
	init();
	glutMainLoop();
}

/**
 \brief Called in the begining of the render process for each loop to clear the previous screen.
*/
void OApplication::clearScreen()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 \brief Virtual method to process key strokes.
 \param key ASCII key code.
 \param mouse_x Position in the X axis of the mouse pointer.
 \param mouse_y Position in the Y axis of the mouse pointer.
*/
void OApplication::onKeyboardPress(unsigned char key, int mouse_x, int mouse_y)
{
}

/**
 \brief Virtual method called when the window is resized.

 Camera and screen adjustments are already dealt by OApplication.

 \param width New window width.
 \param height New window height.
*/
void OApplication::onWindowResize(int width, int height)
{
}

void OApplication::keyboardCallback(unsigned char key, int mouse_x, int mouse_y)
{
	_activeInstance->onKeyboardPress(key, mouse_x, mouse_y);
}

void OApplication::resizeCallback(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	_activeInstance->camera()->setAspectRatio((float)width / height);
	_activeInstance->onWindowResize(width, height);
}

void OApplication::displayCallback()
{
	_activeInstance->clearScreen();
	_activeInstance->update(glutGet(GLUT_ELAPSED_TIME));
	glutSwapBuffers();
	glutPostRedisplay();
}
