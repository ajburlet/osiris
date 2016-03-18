#pragma once

#include <string>

#include "OCamera.h"

#define OAPPLICATION_DEFAULT_POSX	200
#define OAPPLICATION_DEFAULT_POSY	200
#define OAPPLICATION_DEFAULT_WIDTH	640
#define OAPPLICATION_DEFAULT_HEIGHT	480

class OApplication
{
public:
	OApplication(const std::string &title, int argc, char **argv,
		int windowPos_x=OAPPLICATION_DEFAULT_POSX, int windowPos_y=OAPPLICATION_DEFAULT_POSY,
		int windowWidth=OAPPLICATION_DEFAULT_WIDTH, int windowHeight=OAPPLICATION_DEFAULT_HEIGHT);
	virtual ~OApplication();
	void start();

protected:
	virtual void init() = 0;
	virtual void update(int deltaTime_ms) = 0;
	virtual void onKeyboardPress(unsigned char key, int mouse_x, int mouse_y);
	void onWindowResize(int width, int height);

private:
	static OApplication* _activeInstance;
	OCamera cam;

	static void keyboardCallback(unsigned char key, int mouse_x, int mouse_y);
	static void resizeCallback(int width, int height);
	static void displayCallback();
};

