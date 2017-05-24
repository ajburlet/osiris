#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OApplication.h"

#include <glload/gl_load.hpp>

#ifndef OAPPLICATION_DEFAULT_STATSSAMPLE
#define OAPPLICATION_DEFAULT_STATSSAMPLE	100
#endif

using namespace std;

OApplication* OApplication::_activeInstance = NULL;

OApplication::OApplication(const char* title, int argc, char **argv, int windowPos_x, int windowPos_y, 
			   int windowWidth, int windowHeight, int targetFPS, int simulationStep_us) :
	_targetFPS(targetFPS),
	_simulationStep_us(simulationStep_us),
	_fpsStats(OAPPLICATION_DEFAULT_STATSSAMPLE),
	_idleTimeStats(OAPPLICATION_DEFAULT_STATSSAMPLE)
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
	glutKeyboardUpFunc(keyboardUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseActiveMoveCallback);
	glutPassiveMotionFunc(mousePassiveMoveCallback);
	glutReshapeFunc(resizeCallback);

	/* z-buffer */
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);

	/* initializing simulation time frame */
	OTimeIndex::init();
	_simulationTimeIndex = OTimeIndex::current();
	_lastRenderTimeIndex = 0;
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
	bool exists = false;
	for (list<OObject*>::iterator it = _eventRecipients[eventType].begin(); it != _eventRecipients[eventType].end(); it++) {
		if (*it == recipient) {
			exists = true;
			break;
		}
	}
	if (!exists) _eventRecipients[eventType].push_back(recipient);
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

const OStats<float>& OApplication::fpsStats() const
{
	return _fpsStats;
}

const OStats<int>& OApplication::idleTimeStats() const
{
	return _idleTimeStats;
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

int OApplication::eventRecipientCount(OEvent::EventType type)
{
	map<OEvent::EventType, list<OObject*> >::iterator it = _eventRecipients.find(type);
	if (it == _eventRecipients.end()) return 0;
	return it->second.size();
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

void OApplication::loopInteraction()
{
	OTimeIndex currTime = OTimeIndex::current();

	/* getting started on the iteration */
	clearScreen();
	processEvents();

	/* running the simulation */
	while ((currTime - _simulationTimeIndex).toInt() > _simulationStep_us) {
		_simulationTimeIndex += _simulationStep_us;
		update(_simulationTimeIndex);
	}

	/* rendering */
	currTime = OTimeIndex::current();
	_fpsStats.add(1.0f/((float)(currTime - _lastRenderTimeIndex).toInt()/1000000));
	_lastRenderTimeIndex = currTime;
	render();
	
	deleteObjects();

	glutSwapBuffers();
	glutPostRedisplay();
}

void OApplication::keyboardCallback(unsigned char key, int mouse_x, int mouse_y)
{
	if (_activeInstance->eventRecipientCount(OEvent::KeyboardPressEvent)) {
		OKeyboardPressEvent *evt = new OKeyboardPressEvent((OKeyboardPressEvent::KeyCode)key, mouse_x, mouse_y, true);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::keyboardUpCallback(unsigned char key, int mouse_x, int mouse_y)
{
	if (_activeInstance->eventRecipientCount(OEvent::KeyboardReleaseEvent)) {
		OKeyboardPressEvent *evt = new OKeyboardPressEvent((OKeyboardPressEvent::KeyCode)key, mouse_x, mouse_y, false);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::mouseCallback(int button, int state, int x, int y)
{
	if (_activeInstance->eventRecipientCount(OEvent::MouseClickEvent) > 0) {
		OMouseClickEvent *evt = new OMouseClickEvent((OMouseClickEvent::MouseButton)button,
						   (OMouseClickEvent::MouseStatus)state,
						   x, y);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::mouseActiveMoveCallback(int x, int y)
{
	if (_activeInstance->eventRecipientCount(OEvent::MouseActiveMoveEvent) > 0) {
		OMouseMoveEvent *evt = new OMouseMoveEvent(OMouseMoveEvent::ActiveMove, x, y);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::mousePassiveMoveCallback(int x, int y)
{
	if (_activeInstance->eventRecipientCount(OEvent::MousePassiveMoveEvent) > 0) {
		OMouseMoveEvent *evt = new OMouseMoveEvent(OMouseMoveEvent::PassiveMove, x, y);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::resizeCallback(int width, int height)
{
	if (_activeInstance->eventRecipientCount(OEvent::ResizeEvent) > 0) {
		OResizeEvent *evt = new OResizeEvent(width, height);

		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
		_activeInstance->camera()->setAspectRatio((float)width / height);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::displayCallback()
{
	_activeInstance->loopInteraction();
}
