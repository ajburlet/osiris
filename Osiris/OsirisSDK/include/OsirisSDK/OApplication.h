#pragma once

#include <string>
#include <list>
#include <queue>

#include "defs.h"
#include "OCamera.h"
#include "OMath.h"
#include "OObject.h"
#include "OEvent.h"

#define OAPPLICATION_DEFAULT_POSX	200
#define OAPPLICATION_DEFAULT_POSY	200
#define OAPPLICATION_DEFAULT_WIDTH	640
#define OAPPLICATION_DEFAULT_HEIGHT	480

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
	 */
	OApplication(const char* title, int argc, char **argv,
		int windowPos_x=OAPPLICATION_DEFAULT_POSX, int windowPos_y=OAPPLICATION_DEFAULT_POSY,
		int windowWidth=OAPPLICATION_DEFAULT_WIDTH, int windowHeight=OAPPLICATION_DEFAULT_HEIGHT);
	
	
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
	 */
	virtual void update(int timeIndex_ms) = 0;

private:
	static OApplication* _activeInstance;
	OCamera _cam;
	std::map<OObject*, int> _deleteList;
	std::map<OEvent::EventType, std::list<OObject*> > _eventRecipients;
	std::queue<OEvent*> _eventQueue;

	static void keyboardCallback(unsigned char key, int mouse_x, int mouse_y);
	static void mouseCallback(int button, int state, int x, int y);
	static void mouseActiveMoveCallback(int x, int y);
	static void mousePassiveMoveCallback(int x, int y);
	static void resizeCallback(int width, int height);
	static void displayCallback();
};

