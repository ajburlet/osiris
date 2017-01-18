#pragma once

#include "defs.h"
#include <gl/freeglut.h>

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

	OEvent(EventType type);
	virtual ~OEvent();

	EventType type() const;

private:
	EventType _type;
};

/**
 \brief Class for keyboard press events.
 */
class OAPI OKeyboardPressEvent : public OEvent
{
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

	OKeyboardPressEvent(KeyCode code, int mouse_x, int mouse_y);
	virtual ~OKeyboardPressEvent();

	KeyCode code() const;
	int mouseX() const;
	int mouseY() const;

private:
	KeyCode _code;
	int _mouse_x;
	int _mouse_y;
};

/**
 \brief Class for mouse click events.
 */
class OAPI OMouseClickEvent : public OEvent
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

	OMouseClickEvent(MouseButton btn, MouseStatus status, int x, int y);
	virtual ~OMouseClickEvent();

	MouseButton button() const;
	MouseStatus status() const;
	int x() const;
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
class OAPI OResizeEvent : public OEvent
{
public:
	OResizeEvent(int width, int height);
	virtual ~OResizeEvent();

	int width() const;
	int height() const;

private:
	int _width;
	int _height;
};