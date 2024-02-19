#include <chrono>
#include <thread>

#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMap.hpp"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OObject.h"
#include "OsirisSDK/OTrashBin.h"
#include "OsirisSDK/OChronometer.h"
#include "OsirisSDK/OStats.hpp"
#include "OsirisSDK/OTimeIndex.h"
#include "OsirisSDK/OCamera.h"
#include "OsirisSDK/ORenderingEngine.h"
#include "OsirisSDK/OOpenGL.h"
#include "OsirisSDK/OGeometryManager.h"

#include "OsirisSDK/OApplication.h"

#ifndef OAPPLICATION_DEFAULT_STATSSAMPLE
#define OAPPLICATION_DEFAULT_STATSSAMPLE	100
#endif

using namespace std;


struct OApplication::Impl {
	using ObjectList = OList<OObject*>;
	using EventRecipientMap = OMap<OEventType, ObjectList>;
	using EventQueue = OList<OEvent*>;

	Impl(uint32_t aTargetFPS, uint32_t aSimulationStep_us) :
		targetFPS(aTargetFPS),
		simulationStep_us(aSimulationStep_us),
		fpsStats(OAPPLICATION_DEFAULT_STATSSAMPLE),
		idleTimeStats(OAPPLICATION_DEFAULT_STATSSAMPLE),
		renderTimeStats(OAPPLICATION_DEFAULT_STATSSAMPLE),
		clearColor(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}

	~Impl() {
		if (engine != nullptr) delete engine;
	}


	uint32_t		targetFPS;
	uint32_t		simulationStep_us;
	ORenderingEngine*	engine				= nullptr;
	OGeometryManager*	geometryManager			= nullptr;
	OCamera			cam;
	OTrashBin		trashBin;
	EventRecipientMap	eventRecipients;
	EventQueue		eventQueue;
	OStats<float>		fpsStats;
	OStats<int>		idleTimeStats;
	OStats<int>		renderTimeStats;
	OStats<float>		simulationPerformanceStats;
	OTimeIndex		simulationTimeIndex;
	OTimeIndex		lastRenderTimeIndex;
	float			depthBufferClearValue		= 0.0f;
	OVector4F		clearColor;
};

OApplication* OApplication::_activeInstance = NULL;

OApplication::OApplication(const char* aTitle, int aArgc, char **aArgv, GraphicsAPI aGraphicsAPI, 
			   int aWindowPosX, int aWindowPosY, int aWindowWidth, int aWindowHeight, 
			   int aTargetFPS, int aSimulationStep_us) 
{
	if (_activeInstance != NULL) throw OEx("There is already an OApplication instance created.");
	_activeInstance = this;

	OExPointerCheck(_impl = new Impl(aTargetFPS, aSimulationStep_us));

	/* GLUT init */
	glutInit(&aArgc, aArgv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitContextVersion(OSIRIS_GL_VERSION);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(aWindowWidth, aWindowHeight);
	glutInitWindowPosition(aWindowPosX, aWindowPosY);
	int window = glutCreateWindow(aTitle);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	/* init rendering engine init */
	OGraphicsAPI* graphics_api = nullptr;
	
	auto error_cb = [graphics_api, window]() {
		if (graphics_api != nullptr) delete graphics_api;
		glutDestroyWindow(window);
	};

	switch (aGraphicsAPI) {
	case GraphicsAPI::OpenGL:
		OExPointerCheckCb(graphics_api = new OOpenGL, error_cb);
		break;
	default:
		error_cb();
		throw OEx("Unknow graphics API.");
	}
	OExPointerCheckCb(_impl->engine = new ORenderingEngine(graphics_api), error_cb);

	/* resource managers */
	OExPointerCheck(_impl->geometryManager = new OGeometryManager);

	/* setup callbacks */
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseActiveMoveCallback);
	glutPassiveMotionFunc(mousePassiveMoveCallback);
	glutReshapeFunc(resizeCallback);

	/* initializing simulation time frame */
	OTimeIndex::init();
	_impl->simulationTimeIndex = OTimeIndex::current();
	_impl->lastRenderTimeIndex = 0;
}

OApplication::~OApplication()
{
	_activeInstance = NULL;
	delete _impl;
}

OApplication & OApplication::operator=(OApplication && aOther)
{
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

OCamera * OApplication::camera()
{
	return &_impl->cam;
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
	return _impl->targetFPS;
}

int OApplication::simulationStep() const
{
	return _impl->simulationStep_us;
}

void OApplication::setTargetFPS(int aTargetFPS)
{
	_impl->targetFPS = aTargetFPS;
}

void OApplication::setSimulationStep(int aSimulationStep)
{
	_impl->simulationStep_us = aSimulationStep;
}

void OApplication::addEventRecipient(OEventType aEventType, OObject * aRecipient)
{
	bool exists = false;
	for (auto recipient : _impl->eventRecipients[aEventType]) {
		if (recipient == aRecipient) {
			exists = true;
			break;
		}
	}
	if (!exists) _impl->eventRecipients[aEventType].pushBack(aRecipient);
}

void OApplication::removeEventRecipient(OEventType aEventType, OObject * aRecipient)
{
	_impl->eventRecipients[aEventType].remove(aRecipient);
}

void OApplication::start()
{
	init();
	glutMainLoop();
}

OTrashBin & OApplication::trashBin()
{
	return _impl->trashBin;
}

ORenderingEngine& OApplication::renderingEngine()
{
	return *(_impl->engine);
}

OGeometryManager& OApplication::geometryManager()
{
	return *(_impl->geometryManager);
}

const OStats<float>& OApplication::fpsStats() const
{
	return _impl->fpsStats;
}

const OStats<int>& OApplication::idleTimeStats() const
{
	return _impl->idleTimeStats;
}

const OStats<int>& OApplication::renderTimeStats() const
{
	return _impl->renderTimeStats;
}

const OStats<float>& OApplication::performanceStats() const
{
	return _impl->simulationPerformanceStats;
}

OApplication * OApplication::activeInstance()
{
	return _activeInstance;
}

void OApplication::clearScreen()
{
	_impl->engine->clearDepthBuffer(_impl->depthBufferClearValue);
	_impl->engine->clearRenderTarget(_impl->clearColor);
}

int OApplication::eventRecipientCount(OEventType aType)
{
	auto it = _impl->eventRecipients.find(aType);
	if (it == _impl->eventRecipients.end()) return 0;
	return it.value().size();
}

void OApplication::queueEvent(OEvent * evt)
{
	_impl->eventQueue.pushBack(evt);
}

void OApplication::processEvents()
{
	while (_impl->eventQueue.empty() == false) {
		OEvent *cur = _impl->eventQueue.front();
		for (auto recipient : _impl->eventRecipients[cur->type()]) {
			recipient->processEvent(cur);
		}
		delete cur;
		_impl->eventQueue.popFront();
	}
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
	while ((cron.lastPartialTime() - _impl->simulationTimeIndex).toInt() > static_cast<int>(_impl->simulationStep_us)) {
		_impl->simulationTimeIndex += _impl->simulationStep_us;
		update(_impl->simulationTimeIndex, _impl->simulationStep_us);
		stepCount++;
	}

	/* calculate mean performance indicator */
	if (stepCount > 0) _impl->simulationPerformanceStats.add((float)cron.partial() / stepCount / _impl->simulationStep_us);

	/* limit rendering frequency */
	cron.partial();
	if (_impl->targetFPS > 0) {
		int renderInterval_us = (cron.lastPartialTime() - _impl->lastRenderTimeIndex).toInt();
		int frameInterval_us = 1000000 / _impl->targetFPS;
		if (renderInterval_us < frameInterval_us && _impl->lastRenderTimeIndex > 0) {
			this_thread::sleep_for(chrono::microseconds(frameInterval_us - renderInterval_us));
		}
		_impl->idleTimeStats.add(frameInterval_us - renderInterval_us);
	} else {
		_impl->idleTimeStats.add(0);
	}

	/* calculate FPS and render */
	cron.partial();
	_impl->fpsStats.add(1000000.0f / (cron.lastPartialTime() - _impl->lastRenderTimeIndex).toInt());
	_impl->lastRenderTimeIndex = cron.lastPartialTime();

	render();
	_impl->engine->flush();
	_impl->engine->waitUntilCompleted();
	_impl->engine->trashBin().clear();
	glutSwapBuffers();
	glutPostRedisplay();
	
	_impl->renderTimeStats.add(cron.partial());

	/* clear trash bin */
	_impl->trashBin.clear();
}

void OApplication::keyboardCallback(unsigned char key, int mouse_x, int mouse_y)
{
	if (_activeInstance->eventRecipientCount(OEventType::KeyboardPressEvent)) {
		OKeyboardPressEvent *evt = new OKeyboardPressEvent((OKeyCode)key, mouse_x, mouse_y, true);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::keyboardUpCallback(unsigned char key, int mouse_x, int mouse_y)
{
	if (_activeInstance->eventRecipientCount(OEventType::KeyboardReleaseEvent)) {
		OKeyboardPressEvent *evt = new OKeyboardPressEvent((OKeyCode)key, mouse_x, mouse_y, false);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::mouseCallback(int button, int state, int x, int y)
{
	if (_activeInstance->eventRecipientCount(OEventType::MouseClickEvent) > 0) {
		OMouseClickEvent *evt = new OMouseClickEvent((OMouseButton)button, (OMouseButtonStatus)state, x, y);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::mouseActiveMoveCallback(int x, int y)
{
	if (_activeInstance->eventRecipientCount(OEventType::MouseActiveMoveEvent) > 0) {
		OMouseMoveEvent *evt = new OMouseMoveEvent(OMouseMovementType::Active, x, y);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::mousePassiveMoveCallback(int x, int y)
{
	if (_activeInstance->eventRecipientCount(OEventType::MousePassiveMoveEvent) > 0) {
		OMouseMoveEvent *evt = new OMouseMoveEvent(OMouseMovementType::Passive, x, y);
		_activeInstance->queueEvent(evt);
	}
}

void OApplication::resizeCallback(int width, int height)
{
	if (_activeInstance->eventRecipientCount(OEventType::ResizeEvent) > 0) {
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
