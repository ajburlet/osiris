#pragma once

#include "defs.h"
#include "OApplication.h"
#include "OObject.h"
#include "OEvent.h"
#include "OCamera.h"
#include "OMath.h"

#include <map>

#ifndef OSIRIS_CAMCTRL_DEFAULT_ACC
#define OSIRIS_CAMCTRL_DEFAULT_ACC		0.3
#endif

#ifndef OSIRIS_CAMCTRL_DEFAULT_MAXSPEED
#define OSIRIS_CAMCTRL_DEFAULT_MAXSPEED		1.0
#endif

/**
 \brief Camera control class.

 Objects from this class will receive input information from mouse and keyboard and move
 the camera and change it's orientation.
 */
class OAPI OCameraController : public OObject
{
public:
	/**
	 \brief Mouse orientation mode.

	 The camera orientation can be changed by mouse movement, either enabled or passive.
	 */
	enum MouseOrientationMode {
		OrientationDisabled=0,		/**< Mouse orientation disabled. */
		ActiveOrientation,		/**< Orientation by enabled mouse movement (button pressed). */
		PassiveOrientation		/**< Orientation by mouse motion over the screen, without any buttons pressed. */
	};

	/**
	 \brief Class constructor.

	 This constructor will also connect itself to the application in order to
	 receive events.
	 
	 \param app The application object.
	 \param orMode Camera orientation mode: enabled or passive mouse movement, or disabled.
	 \param orMouseBtn In case of enabled mouse motion, which button triggers camera movement.
	 \param movementAcceleration Camera movement acceleration in space units per sec^2.
	 \param movementMaxSpeed Camera maximum speed in space units per sec.
	 */
	OCameraController(OApplication* app, 
		MouseOrientationMode orMode = ActiveOrientation, OMouseClickEvent::MouseButton orMouseBtn = OMouseClickEvent::LeftButton,
		float movementAcceleration=OSIRIS_CAMCTRL_DEFAULT_ACC, float movementMaxSpeed=OSIRIS_CAMCTRL_DEFAULT_MAXSPEED);

	/**
	 \brief Class destructor.
	 */
	virtual ~OCameraController();

	/**
	 \brief Camera movement events.
	 */
	enum CameraMoveDir {
		MoveForward = 0x01,	/**< Move camera forward. */
		MoveBack,		/**< Move camera back. */
		MoveLeft,		/**< Move camera left. */
		MoveRight,		/**< Move camera right. */
		MoveUp,			/**< Move camera up. */
		MoveDown		/**< Move camera down. */
	};

	/**
	 \brief Bind keyboard event to camera movement.
	 \param key Key code.
	 \param camEvt Camera movement event.
	 */
	void setMoveEventKey(OKeyboardPressEvent::KeyCode key, CameraMoveDir camEvt);

	/**
	 \brief Updates the camera position and orientation for a given time index.
	 \param timeIndex_ms Time index in miliseconds.
	 */
	void update(int timeIndex_ms);


protected:
	/* from OObject: */
	virtual void onKeyboardPress(const OKeyboardPressEvent* evt);
	virtual void onKeyboardRelease(const OKeyboardPressEvent *evt);
	virtual void onMouseMove(const OMouseMoveEvent* evt);
	virtual void onMouseClick(const OMouseClickEvent* evt);

private:
	/* object working parameters */
	OApplication *_app;
	MouseOrientationMode _orMode;
	OMouseClickEvent::MouseButton _orMouseBtn;
	float _movementAcceleration;
	float _movementMaxSpeed;
	
	bool _activeMoveMouseBtnPressed;
	int _last_mouse_x;
	int _last_mouse_y;
	int _delta_mouse_x;
	int _delta_mouse_y;
	std::map<OKeyboardPressEvent::KeyCode, CameraMoveDir> _keyBind; /* KeyCode -> Movement direction */
	std::map<CameraMoveDir, bool> _pressedKeys;			/* Movement direction -> pressed flag (bool) */

	/**
	 \brief Updates the application adding the object as a event listener the needed mouse
	        and keyboard events.
	 */
	void updateApplication();

	/**
	 \brief Check if a key assigned to camera movement is pressed.
	 \param dir Movement direction.
	 */
	bool isMovementKeyPressed(CameraMoveDir dir);
};



