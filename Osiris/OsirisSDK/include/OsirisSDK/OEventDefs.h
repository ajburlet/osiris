#pragma once

#include "OsirisSDK/GLdefs.h"

/**
 @brief Event types.
 */
enum class OEventType {
	KeyboardPressEvent,		/**< Keyboard press event. Issues an OKeyboardPressEvent class object. */
	KeyboardReleaseEvent,		/**< Keyboard release event. Issues an OKeyboardPressEvent class object. */
	MouseClickEvent,		/**< Mouse click event. Issues an OMouseClickEvent. */
	MouseActiveMoveEvent,		/**< Mouse movement event with button pressed. Issues an OMouseMove class object. */
	MousePassiveMoveEvent,		/**< Passive mouse movement event. Issues an OMouseMove class object. */
	ResizeEvent			/**< Screen resize event. Issues an OResizeEvent class object. */
};

// ****************************************************************************************************
// TODO: this must be generalized, instead of being related to GLUT !!
// ****************************************************************************************************

/**
 @brief Key codes.
 */
enum class OKeyCode {
	Backspace=8,
	Tab=9,
	Enter=13,
	Escape=27,
	Space=32,
	Delete=GLUT_KEY_DELETE,

	PageUp=GLUT_KEY_PAGE_UP, PageDown=GLUT_KEY_PAGE_DOWN,
	End=GLUT_KEY_END, Okey_Home=GLUT_KEY_HOME,
	Up=GLUT_KEY_UP, Down=GLUT_KEY_DOWN, Left=GLUT_KEY_LEFT, Right=GLUT_KEY_RIGHT,

	ShiftLeft=GLUT_KEY_SHIFT_L, ShiftRight=GLUT_KEY_SHIFT_R,
	CtrlLeft=GLUT_KEY_CTRL_L, CtrlRight=GLUT_KEY_CTRL_R,
	AltLeft=GLUT_KEY_ALT_L, AltRight=GLUT_KEY_ALT_R,

	F1=GLUT_KEY_F1, F2=GLUT_KEY_F2, F3=GLUT_KEY_F3, F4=GLUT_KEY_F4, F5=GLUT_KEY_F5,
	F6=GLUT_KEY_F6, F7=GLUT_KEY_F7, F8=GLUT_KEY_F8, F9=GLUT_KEY_F9, F10=GLUT_KEY_F10,
	F11=GLUT_KEY_F11, F12=GLUT_KEY_F12,

	Num0='0', Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

	A='A', B, C, D, E, F, G, H, I, J,
	K,     L, M, N, O, P, Q, R, S, T,
	U,     V, W, X, Y, Z,

	a='a', b, c, d, e, f, g, h, i, j,
	k,     l, m, n, o, p, q, r, s, t,
	u,     v, w, x, y, z
};

/**
 @brief Mouse buttons.
 */
enum class OMouseButton {
	None=0x0,
	Right=GLUT_RIGHT_BUTTON, 
	Middle=GLUT_MIDDLE_BUTTON, 
	Left=GLUT_LEFT_BUTTON
};

/**
 @brief Mouse button status (pressed/released).
 */
enum class OMouseButtonStatus {
	Release=GLUT_UP,
	Press=GLUT_DOWN
};

/**
 @brief Mouse movement status
 */
enum OMouseMovementType {
	Active=1,		/**< Mouse is moved over the screen with button pressed. */
	Passive=2		/**< Passive mouse movement over the screen. */
};

