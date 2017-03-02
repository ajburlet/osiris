#pragma once

#include "defs.h"
#include "GLdefs.h" 

#include "OMemoryPoolObject.hpp"

#ifndef OEVENT_MP_BLOCKSIZE
#define OEVENT_MP_BLOCKSIZE	16
#endif

#ifndef OEVENT_MP_SEGMENTSIZE
#define OEVENT_MP_SEGMENTSIZE	32
#endif

/**
 \brief Base event class.
 */
class OAPI OEvent
{
public:
	enum EventType {
		KeyboardPressEvent=1000, 
		MouseClickEvent,
		ResizeEvent
	};

	/**
	 \brief Class constructor.
	 \param type Event type.
	 */
	OEvent(EventType type);
	
	/**
	 \brief Class destructor.
	 */
	virtual ~OEvent();

	/**
	 \brief Returns event type.
	 */
	EventType type() const;

private:
	EventType _type;
};

/**
 \brief OEvent class with special memory management.

 OEvent class that also derives from OMemoryPoolObject template class, which means that the 
 the memory management will be done by an OMemoryPool class object. Since events can be frequently
 created, and in order to avoid heap fragmentation, it is important to employ a special memory
 mechanism that will not resort to malloc() and free() calls all the time.
 */
class OAPI OMemoryPoolEvent : public OEvent, public OMemoryPoolObject<OEVENT_MP_BLOCKSIZE, OEVENT_MP_SEGMENTSIZE>
{
public:
	/**
	 \brief Class constructor.
	 \param type Event type.
	 */
	OMemoryPoolEvent(OEvent::EventType type);
	
	/**
	 \brief Class destructor.
	 */
	virtual ~OMemoryPoolEvent();
};

/**
 \brief Class for keyboard press events.
 */
class OAPI OKeyboardPressEvent : public OMemoryPoolEvent
{
public:
	enum KeyCode {
		OKey_Backspace=8,
		OKey_Tab=9,
		OKey_Enter=13,
		OKey_Escape=27,
		OKey_Space=32,
		OKey_Delete=GLUT_KEY_DELETE,

		OKey_PageUp=GLUT_KEY_PAGE_UP, OKey_PageDown=GLUT_KEY_PAGE_DOWN,
		OKey_End=GLUT_KEY_END, Okey_Home=GLUT_KEY_HOME,
		OKey_Up=GLUT_KEY_UP, OKey_Down=GLUT_KEY_DOWN, OKey_Left=GLUT_KEY_LEFT, OKey_Right=GLUT_KEY_RIGHT,

		OKey_ShiftLeft=GLUT_KEY_SHIFT_L, OKey_ShiftRight=GLUT_KEY_SHIFT_R,
		OKey_CtrlLeft=GLUT_KEY_CTRL_L, OKey_CtrlRight=GLUT_KEY_CTRL_R,
		OKey_AltLeft=GLUT_KEY_ALT_L, OKey_AltRight=GLUT_KEY_ALT_R,

		OKey_F1=GLUT_KEY_F1, OKey_F2=GLUT_KEY_F2, OKey_F3=GLUT_KEY_F3, OKey_F4=GLUT_KEY_F4, OKey_F5=GLUT_KEY_F5,
		OKey_F6=GLUT_KEY_F6, OKey_F7=GLUT_KEY_F7, OKey_F8=GLUT_KEY_F8, OKey_F9=GLUT_KEY_F9, OKey_F10=GLUT_KEY_F10,
		OKey_F11=GLUT_KEY_F11, OKey_F12=GLUT_KEY_F12,

		OKey_0='0', OKey_1, OKey_2, OKey_3, OKey_4, OKey_5, OKey_6, OKey_7, OKey_8, OKey_9,

		OKey_A='A', OKey_B, OKey_C, OKey_D, OKey_E, OKey_F, OKey_G, OKey_H, OKey_I, OKey_J,
		OKey_K,     OKey_L, OKey_M, OKey_N, OKey_O, OKey_P, OKey_Q, OKey_R, OKey_S, OKey_T,
		OKey_U,     OKey_V, OKey_W, OKey_X, OKey_Y, OKey_Z,

		OKey_a='a', OKey_b, Okey_c, OKey_d, OKey_e, OKey_f, OKey_g, OKey_h, OKey_i, Okey_j,
		OKey_k,     OKey_l, OKey_m, OKey_n, OKey_o, OKey_p, OKey_q, OKey_r, OKey_s, OKey_t,
		OKey_u,     OKey_v, OKey_w, OKey_x, OKey_y, OKey_z
	};

	/**
	 \brief Class constructor.
	 \param code Key code.
	 \param mouse_x Mouse position on the window on the X-axis.
	 \param mouse_y Mouse position on the window on the Y-axis.
	 */
	OKeyboardPressEvent(KeyCode code, int mouse_x, int mouse_y);

	/**
	 \brief Class destructor.
	 */
	virtual ~OKeyboardPressEvent();

	/**
	 \brief Key code.
	 */
	KeyCode code() const;
	
	/**
	 \brief Mouse position on the window on the X-axis.
	 */
	int mouseX() const;
	
	/**
	 \brief Mouse position on the window on the Y-axis.
	 */
	int mouseY() const;

private:
	KeyCode _code;
	int _mouse_x;
	int _mouse_y;
};

/**
 \brief Class for mouse click events.
 */
class OAPI OMouseClickEvent : public OMemoryPoolEvent
{
public:
	enum MouseButton {
		RightButton=GLUT_RIGHT_BUTTON, 
		MiddleButton=GLUT_MIDDLE_BUTTON, 
		LeftButton=GLUT_LEFT_BUTTON
	};

	enum MouseStatus {
		Release=GLUT_UP,
		Press=GLUT_DOWN
	};

	/**
	 \brief Class constructor.
	 \param btn Mouse button that generated the event.
	 \param status Status of the mouse button: released or pressed.
	 \param x Window X-axis component in pixels.
	 \param y Window Y-axis component in pixels.
	 */
	OMouseClickEvent(MouseButton btn, MouseStatus status, int x, int y);
	
	/**
	 \brief Class destructor.
	 */
	virtual ~OMouseClickEvent();

	/**
	 \brief Mouse button that generated the event.
	 */
	MouseButton button() const;
	
	/**
	 \brief Mouse button status: pressed or released.
	 */
	MouseStatus status() const;
	
	/**
	 \brief Window X-axis component in pixels.
	 */
	int x() const;
	
	/**
	 \brief Window Y-axis component in pixels.
	 */
	int y() const;
	
private:
	MouseButton _btn;
	MouseStatus _status;
	int _x;
	int _y;
};

/**
 \brief Class for window resize events.
 */
class OAPI OResizeEvent : public OMemoryPoolEvent
{
public:
	
	/**
	 \brief Class constructor.
	 \param width Window width in pixels.
	 \param height Window height in pixels.
	 */
	OResizeEvent(int width, int height);
	
	/**
	 \brief Class destructor.
	 */
	virtual ~OResizeEvent();

	/**
	 \brief Window width in pixels.
	 */
	int width() const;
	
	/**
	 \brief Window height in pixels.
	 */
	int height() const;

private:
	int _width;
	int _height;
};
