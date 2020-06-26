#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OEventDefs.h"
#include "OsirisSDK/OMemoryPoolObject.hpp"

#ifndef OEVENT_MP_BLOCKSIZE
#define OEVENT_MP_BLOCKSIZE	16
#endif

#ifndef OEVENT_MP_SEGMENTSIZE
#define OEVENT_MP_SEGMENTSIZE	32
#endif

/**
 @brief Base event class.
 */
class OAPI OEvent
{
public:
	/**
	 @brief Class constructor.
	 @param aType Event type.
	 */
	OEvent(OEventType aType);
	
	/**
	 @brief Class destructor.
	 */
	virtual ~OEvent();

	/**
	 @brief Returns event type.
	 */
	OEventType type() const;

private:
	OEventType _type;
};

/**
 @brief OEvent class with special memory management.

 OEvent class that also derives from OMemoryPoolObject template class, which means that the 
 the memory management will be done by an OMemoryPool class object. Since events can be frequently
 created, and in order to avoid heap fragmentation, it is important to employ a special memory
 mechanism that will not resort to malloc() and free() calls all the time.
 */
class OAPI OMemoryPoolEvent : public OEvent, public OMemoryPoolObject<OEVENT_MP_BLOCKSIZE, OEVENT_MP_SEGMENTSIZE>
{
public:
	/**
	 @brief Class constructor.
	 @param aType Event type.
	 */
	OMemoryPoolEvent(OEventType aType);
	
	/**
	 @brief Class destructor.
	 */
	virtual ~OMemoryPoolEvent();
};

/**
 @brief Class for keyboard press events.
 */
class OAPI OKeyboardPressEvent : public OMemoryPoolEvent
{
public:
	/**
	 @brief Class constructor.
	 @param aCode Key code.
	 @param aMouseX Mouse position on the window on the X-axis.
	 @param aMouseY Mouse position on the window on the Y-axis.
	 @param aKeyPressed Key status: true if pressed, false if released.
	 */
	OKeyboardPressEvent(OKeyCode aCode, int aMouseX, int aMouseY, bool aKeyPressed);

	/**
	 @brief Class destructor.
	 */
	virtual ~OKeyboardPressEvent();

	/**
	 @brief Key code.
	 */
	OKeyCode code() const;
	
	/**
	 @brief Mouse position on the window on the X-axis.
	 */
	int mouseX() const;
	
	/**
	 @brief Mouse position on the window on the Y-axis.
	 */
	int mouseY() const;

private:
	OKeyCode _code;
	int _mouse_x;
	int _mouse_y;
};

/**
 @brief Class for mouse click events.
 */
class OAPI OMouseClickEvent : public OMemoryPoolEvent
{
public:
	/**
	 @brief Class constructor.
	 @param aBtn Mouse button that generated the event.
	 @param aSatus Status of the mouse button: released or pressed.
	 @param aX Window X-axis component in pixels.
	 @param aY Window Y-axis component in pixels.
	 */
	OMouseClickEvent(OMouseButton aBtn, OMouseButtonStatus aStatus, int aX, int aY);
	
	/**
	 @brief Class destructor.
	 */
	virtual ~OMouseClickEvent();

	/**
	 @brief Mouse button that generated the event.
	 */
	OMouseButton button() const;
	
	/**
	 @brief Mouse button status: pressed or released.
	 */
	OMouseButtonStatus status() const;
	
	/**
	 @brief Window X-axis component in pixels.
	 */
	int x() const;
	
	/**
	 @brief Window Y-axis component in pixels.
	 */
	int y() const;
	
private:
	OMouseButton _btn;
	OMouseButtonStatus _status;
	int _x;
	int _y;
};

/**
 @brief Mouse movement event.
 */
class OAPI OMouseMoveEvent : public OMemoryPoolEvent
{
public:
	/**
	 @brief Class constructor.
	 @param aType Active or passive movement.
	 @param aX Window X-axis component in pixels.
	 @param aY Window Y-axis component in pixels.
	 */
	OMouseMoveEvent(OMouseMovementType aType, int aX, int aY);

	/**
	 @brief Class destructor.
	 */
	virtual ~OMouseMoveEvent();

	/**
	 @brief Mouse movement type: active or passive.
	 */
	OMouseMovementType movementType() const;

	/**
	 @brief Window X-axis component in pixels.
	 */
	int x() const;
	
	/**
	 @brief Window Y-axis component in pixels.
	 */
	int y() const;
private:
	OMouseMovementType _type;
	int _x;
	int _y;
};

/**
 @brief Class for window resize events.
 */
class OAPI OResizeEvent : public OMemoryPoolEvent
{
public:
	/**
	 @brief Class constructor.
	 @param aWidth Window width in pixels.
	 @param aHeight Window height in pixels.
	 */
	OResizeEvent(int aWidth, int aHeight);
	
	/**
	 @brief Class destructor.
	 */
	virtual ~OResizeEvent();

	/**
	 @brief Window width in pixels.
	 */
	int width() const;
	
	/**
	 @brief Window height in pixels.
	 */
	int height() const;

private:
	int _width;
	int _height;
};
