#pragma once

#include <string>

#include "defs.h"
#include "OCamera.h"
#include "OMath.h"

#define OAPPLICATION_DEFAULT_POSX	200
#define OAPPLICATION_DEFAULT_POSY	200
#define OAPPLICATION_DEFAULT_WIDTH	640
#define OAPPLICATION_DEFAULT_HEIGHT	480

class OAPI OApplication
{
public:
	OApplication(const char* title, int argc, char **argv,
		int windowPos_x=OAPPLICATION_DEFAULT_POSX, int windowPos_y=OAPPLICATION_DEFAULT_POSY,
		int windowWidth=OAPPLICATION_DEFAULT_WIDTH, int windowHeight=OAPPLICATION_DEFAULT_HEIGHT);
	virtual ~OApplication();

	OCamera* camera();

	void start();

protected:
	virtual void init() = 0;
	virtual void clearScreen();
	virtual void update(int timeIndex_ms) = 0;
	virtual void onKeyboardPress(unsigned char key, int mouse_x, int mouse_y);
	void onWindowResize(int width, int height);

private:
	static OApplication* _activeInstance;
	OCamera _cam;

	static void keyboardCallback(unsigned char key, int mouse_x, int mouse_y);
	static void resizeCallback(int width, int height);
	static void displayCallback();
};

