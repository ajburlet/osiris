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
	OApplication(const char* title, int argc, char **argv,
		int windowPos_x=OAPPLICATION_DEFAULT_POSX, int windowPos_y=OAPPLICATION_DEFAULT_POSY,
		int windowWidth=OAPPLICATION_DEFAULT_WIDTH, int windowHeight=OAPPLICATION_DEFAULT_HEIGHT);
	virtual ~OApplication();

	OCamera* camera();

	int windowWidth() const;
	int windowHeight() const;

	void addEventRecipient(OEvent::EventType eventType, OObject* recipient);
	void removeEventRecipient(OEvent::EventType eventType, OObject* recipient);

	void start();

	void scheduleDelete(OObject* obj);

	static OApplication* activeInstance();

protected:
	virtual void init() = 0;
	virtual void clearScreen();
	void queueEvent(OEvent* evt);
	void processEvents();
	void deleteObjects();
	virtual void update(int timeIndex_ms) = 0;

private:
	static OApplication* _activeInstance;
	OCamera _cam;
	std::map<OObject*, int> _deleteList;
	std::map<OEvent::EventType, std::list<OObject*> > _eventRecipients;
	std::queue<OEvent*> _eventQueue;

	static void keyboardCallback(unsigned char key, int mouse_x, int mouse_y);
	static void mouseCallback(int button, int state, int x, int y);
	static void resizeCallback(int width, int height);
	static void displayCallback();
};

