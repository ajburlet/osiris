#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OApplication.h"

#include <glload/gl_load.hpp>

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
	glutMouseFunc(mouseCallback);
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

int OApplication::windowWidth() const
{
	return glutGet(GLUT_WINDOW_WIDTH);
}

int OApplication::windowHeight() const
{
	return glutGet(GLUT_WINDOW_HEIGHT);
}

void OApplication::addEventRecipient(OEvent::EventType eventType, OObject * recipient)
{
	_eventRecipients[eventType].push_back(recipient);
}

void OApplication::removeEventRecipient(OEvent::EventType eventType, OObject * recipient)
{
	_eventRecipients[eventType].remove(recipient);
}

void OApplication::start()
{
	init();
	glutMainLoop();
}

void OApplication::scheduleDelete(OObject * obj)
{
	map<OObject*, int>::iterator it;
	if ((it = _deleteList.find(obj)) != _deleteList.end()) _deleteList[obj] = 1;
}

OApplication * OApplication::activeInstance()
{
	return _activeInstance;
}

void OApplication::clearScreen()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OApplication::queueEvent(OEvent * evt)
{
	_eventQueue.push(evt);
}

void OApplication::processEvents()
{
	while (_eventQueue.empty() == false) {
		OEvent *cur = _eventQueue.front();
		list<OObject*>::iterator it;
		for (it = _eventRecipients[cur->type()].begin(); it != _eventRecipients[cur->type()].end(); it++) {
			(*it)->processEvent(cur);
		}
		delete cur;
		_eventQueue.pop();
	}
}

void OApplication::deleteObjects()
{
	map<OObject*, int>::iterator it;
	for (it = _deleteList.begin(); it != _deleteList.end(); it++) delete it->first;
	_deleteList.clear();
}

void OApplication::keyboardCallback(unsigned char key, int mouse_x, int mouse_y)
{
	OKeyboardPressEvent *evt = new OKeyboardPressEvent((OKeyboardPressEvent::KeyCode)key, mouse_x, mouse_y);
	_activeInstance->queueEvent(evt);
}

void OApplication::mouseCallback(int button, int state, int x, int y)
{
	OMouseClickEvent *evt = new OMouseClickEvent((OMouseClickEvent::MouseButton)button, 
						     (OMouseClickEvent::MouseStatus)state, 
						     x, y);
	_activeInstance->queueEvent(evt);
}

void OApplication::resizeCallback(int width, int height)
{
	OResizeEvent *evt = new OResizeEvent(width, height);

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	_activeInstance->camera()->setAspectRatio((float)width / height);
	_activeInstance->queueEvent(evt);
}

void OApplication::displayCallback()
{
	_activeInstance->clearScreen();
	_activeInstance->processEvents();
	_activeInstance->update(glutGet(GLUT_ELAPSED_TIME));
	_activeInstance->deleteObjects();

	glutSwapBuffers();
	glutPostRedisplay();
}
