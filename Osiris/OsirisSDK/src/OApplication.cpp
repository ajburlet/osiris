#include <chrono>
#include <thread>

#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OApplication.h"
#include "OsirisSDK/OChronometer.h"

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
	_idleTimeStats(OAPPLICATION_DEFAULT_STATSSAMPLE),
	_renderTimeStats(OAPPLICATION_DEFAULT_STATSSAMPLE)
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

int OApplication::targetFPS() const
{
	return _targetFPS;
}

int OApplication::simulationStep() const
{
	return _simulationStep_us;
}

void OApplication::setTargetFPS(int targetFPS)
{
	_targetFPS = targetFPS;
}

void OApplication::setSimulationStep(int simulationStep)
{
	_simulationStep_us = simulationStep;
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

const OStats<int>& OApplication::renderTimeStats() const
{
	return _renderTimeStats;
}

const OStats<float>& OApplication::performanceStats() const
{
	return _simulationPerformanceStats;
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

void OApplication::loopIteration()
{
	OChronometer cron;

	/* getting started on the iteration */
	clearScreen();
	processEvents();

	/* running the simulation */
	cron.partial();
	int stepCount = 0;
	while ((cron.lastPartialTime() - _simulationTimeIndex).toInt() > _simulationStep_us) {
		_simulationTimeIndex += _simulationStep_us;
		update(_simulationTimeIndex);
		stepCount++;
	}

	/* calculate mean performance indicator */
	_simulationPerformanceStats.add(cron.partial() / (float)stepCount / _simulationStep_us);

	/* limit rendering frequency */
	if (_targetFPS > 0) {
		int renderInterval_us = (cron.lastPartialTime() - _lastRenderTimeIndex).toInt();
		int frameInterval_us = 1000000 / _targetFPS;
		if (renderInterval_us < frameInterval_us && _lastRenderTimeIndex > 0) {
			this_thread::sleep_for(chrono::microseconds(frameInterval_us - renderInterval_us));
		}
		_idleTimeStats.add(frameInterval_us - renderInterval_us);
	} else {
		_idleTimeStats.add(0);
	}

	/* calculate FPS and render */
	cron.partial();
	_fpsStats.add(1000000.0f / (cron.lastPartialTime() - _lastRenderTimeIndex).toInt());
	_lastRenderTimeIndex = cron.lastPartialTime();

	render();
	glutSwapBuffers();
	glutPostRedisplay();
	
	_renderTimeStats.add(cron.partial());

	/* delete objects at the end of the iteration */
	deleteObjects();
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
	_activeInstance->loopIteration();
}
