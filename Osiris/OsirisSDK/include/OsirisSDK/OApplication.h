#pragma once

#include <string>
#include <list>
#include <queue>

#include "defs.h"
#include "OCamera.h"
#include "OMath.h"
#include "OObject.h"
#include "OEvent.h"
#include "OTimeIndex.h"
#include "OStats.hpp"

#ifndef OAPPLICATION_DEFAULT_POSX
#define OAPPLICATION_DEFAULT_POSX	200
#endif

#ifndef OAPPLICATION_DEFAULT_POSY
#define OAPPLICATION_DEFAULT_POSY	200
#endif

#ifndef OAPPLICATION_DEFAULT_WIDTH
#define OAPPLICATION_DEFAULT_WIDTH	640
#endif

#ifndef OAPPLICATION_DEFAULT_HEIGHT
#define OAPPLICATION_DEFAULT_HEIGHT	480
#endif

#ifndef OAPPLICATION_DEFAULT_TARGETFPS
#define OAPPLICATION_DEFAULT_TARGETFPS	40
#endif

#ifndef OAPPLICATION_DEFAULT_SIMULATIONSTEP
#define OAPPLICATION_DEFAULT_SIMULATIONSTEP	20000
#endif

/**
 \brief The Osiris Application base class. 

 This is the class that represents an application based on the Osiris Framework. It handles basic 
 interaction with OpenGL, creating the window and handling main loop.
*/
class OAPI OApplication
{
public:

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
	 \param targetFPS Target FPS.
	 \param simulationStep_us Simulation step in microseconds.
	 */
	OApplication(const char* title, int argc, char **argv,
		int windowPos_x=OAPPLICATION_DEFAULT_POSX, int windowPos_y=OAPPLICATION_DEFAULT_POSY,
		int windowWidth=OAPPLICATION_DEFAULT_WIDTH, int windowHeight=OAPPLICATION_DEFAULT_HEIGHT,
		int targetFPS=OAPPLICATION_DEFAULT_TARGETFPS, int simulationStep_us=OAPPLICATION_DEFAULT_SIMULATIONSTEP);
	
	
	/**
	 \brief Class destructor.
	*/
	virtual ~OApplication();


	/**
	 \brief Returns the application camera object.
	 \return Pointer to the application camera object.
	*/
	OCamera* camera();

	/**
	 \brief Returns the window width in pixels.
	 */
	int windowWidth() const;

	/**
	 \brief Returns the window height in pixels.
	 */
	int windowHeight() const;

	/**
	 \brief Returns the target frames-per-second.
	 \param The desired FPS. If zero, the screen rendering will not be limited.
	 */
	int targetFPS() const;

	/**
	 \brief Returns the simulation step in microseconds.
	 */
	int simulationStep() const;

	/**
	 \brief Sets the target frames-per-second.
	 */
	void setTargetFPS(int targetFPS);

	/**
	 \brief Sets the simulation step.

	 This parameter must be set with caution. It shouldn't be neither too large (the simulation
	 will be sloppy) or too short (it should be at least the time it takes to process the simulation
	 iteration).

	 \param simulationStep The desired simulation step in microseconds.
	 */
	void setSimulationStep(int simulationStep);

	/**
	 \brief Adds an OObject class object as event recipient for given type.
	 \param eventType Event type.
	 \param recipient Object that will receive the events.
	 */
	void addEventRecipient(OEvent::EventType eventType, OObject* recipient);

	/**
	 \brief Removes an OObject class object as event recipient for given type.
	 \param eventType Event type.
	 \param recipient Object that will receive the events.
	 */
	void removeEventRecipient(OEvent::EventType eventType, OObject* recipient);

	/**
	 \brief Initializes the application and starts the main loop.
	*/
	void start();

	/**
	 \brief Schedule an object for deletion at the end of the current loop.
	 */
	void scheduleDelete(OObject* obj);

	/**
	 \brief Frames-per-second statistics.
	 */
	const OStats<float>& fpsStats() const;

	/**
	 \brief Simulation idle time statistics (in case target FPS is set) in microseconds.
	 */
	const OStats<int>& idleTimeStats() const;

	/**
	 \brief Renderization time statistics in microseconds.
	 */
	const OStats<int>& renderTimeStats() const;

	/**
	 \brief Performance coefficient statistics.

	 The performance coefficient is the reason between the time it takes to process a
	 simulation iteration and the time step. The closest it is to zero the better. If it
	 is greater than 1, the simulation will face problems catching up to real time, which
	 means that either the simulation step is too short, or that the iteration processing is
	 taking too long and must be optimized.
	 */
	const OStats<float>& performanceStats() const;

	/**
	 \brief Returns active OApplication instance.
	 */
	static OApplication* activeInstance();

protected:
	/**
	 \brief Method called prior to entering the main loop to initialize the OApplication object.
	 */
	virtual void init() = 0;

	/**
	 \brief Called in the begining of the render process for each loop to clear the previous screen.
	*/
	virtual void clearScreen();

	/**
	 \brief Returns the number of event recipients for a given event type.
	 */
	int eventRecipientCount(OEvent::EventType type);

	/**
	 \brief Queue event to be processed by the application and the subscribed OOBject class objects.
	 */
	void queueEvent(OEvent* evt);

	/**
	 \brief Process event queue.
	 */
	void processEvents();

	/**
	 \brief Remove objects previously scheduled for deletion.
	 */
	void deleteObjects();

	/**
	 \brief Method called on each simulation iteration.
	 \brief timeIndex Time index.
	 \brief step_us Simulation step in microseconds.
	 */
	virtual void update(const OTimeIndex& timeIndex, int step_us) = 0;

	/**
	 \brief Renderization method.
	 */
	virtual void render() = 0;

private:
	static OApplication* _activeInstance;
	OCamera _cam;
	std::map<OObject*, int> _deleteList;
	std::map<OEvent::EventType, std::list<OObject*> > _eventRecipients;
	std::queue<OEvent*> _eventQueue;
	int _targetFPS;
	int _simulationStep_us;
	OStats<float> _fpsStats;
	OStats<int> _idleTimeStats;
	OStats<int> _renderTimeStats;
	OStats<float> _simulationPerformanceStats;
	OTimeIndex _simulationTimeIndex;
	OTimeIndex _lastRenderTimeIndex;

	/**
	 Single loop iteration handling.
	 */
	void loopIteration();

	static void keyboardCallback(unsigned char key, int mouse_x, int mouse_y);
	static void keyboardUpCallback(unsigned char key, int mouse_x, int mouse_y);
	static void mouseCallback(int button, int state, int x, int y);
	static void mouseActiveMoveCallback(int x, int y);
	static void mousePassiveMoveCallback(int x, int y);
	static void resizeCallback(int width, int height);
	static void displayCallback();
};

