#pragma once

#include "defs.h"
#include "OState.h"
#include "ODoubleBuffer.hpp"
#include "OEvent.h"
#include "OTimeIndex.h"

class OMesh;

/**
  @brief Entity behavior interface.

  This template class serves as a programming interface to define entity behavior. It works with
  both entity attributes and state, but is defined neither, since it receives them as arguments 
  when called. But depends on their data structure. Ideally, behavior objects need have no data,
  relying solely on what is provided by the entities that calls them.

  It is able to receive events forwarded by entities.

  @tparam attrT Class containing the attribute data structure.
  @tparam stateT State class, by default uses the OState. 
 */
template <class attrT, class stateT=OState> class OBehavior {
public:
	/**
	 @brief Main event handler.

	 This is meant to be called by OEntity class objects. From here the specific event handlers are called. But can
	 be overriden if necessary.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Event class object.
	 */
	virtual void processEvent(attrT** attribute, ODoubleBuffer<stateT>* state, const OEvent* event) {
		switch (event->type()) {
		case OEvent::KeyboardPressEvent:	onKeyboardPress(attribute, state, (const OKeyboardPressEvent*)event);	break;
		case OEvent::KeyboardReleaseEvent:	onKeyboardRelease(attribute, state, (const OKeyboardPressEvent*)event);	break;
		case OEvent::MouseClickEvent:		onMouseClick(attribute, state, (const OMouseClickEvent*)event);		break;
		case OEvent::MouseActiveMoveEvent:
		case OEvent::MousePassiveMoveEvent:	onMouseMove(attribute, state, (const OMouseMoveEvent*)event);		break;
		case OEvent::ResizeEvent:		onScreenResize(attribute, state, (const OResizeEvent*)event);		break;
		}
	}

	/**
	 @brief Entity update method.

	 This is meant to be called by OEntity class objects. By default, it calls the update method on the state
	 object. If you choose to override this method, keep in mind to either do these operations yourself or to 
	 call the OBehavior::update() when appropriate.

	 @param attribute Pointer to entity attributes. 
	 @param state Entity state.
	 @param meshPtr Pointer to entity mesh.
	 @param timeIndex Time index.
	 */
	virtual void update(attrT** attribute, ODoubleBuffer<stateT>* state, OMesh** meshPtr, const OTimeIndex& timeIndex)
	{
		state->next()->update(timeIndex);
	}

protected:
	/**
	 @brief Keyboard press event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Keyboard event object.
	 */
	virtual void onKeyboardPress(attrT** attribute, ODoubleBuffer<stateT>* state, const OKeyboardPressEvent* evt) { }
	
	/**
	 @brief Mouse release event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Mouse click event object.
	 */
	virtual void onKeyboardRelease(attrT** attribute, ODoubleBuffer<stateT>* state, const OKeyboardPressEvent* evt) { }

	/**
	 @brief Mouse click event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Mouse click event object.
	 */
	virtual void onMouseClick(attrT** attribute, ODoubleBuffer<stateT>* state, const OMouseClickEvent* evt) { }

	/**
	 @brief Mouse active and passive move event handler.
	 
	 This handler is only called if the object is set as an event recipient. By default does nothing.
	 
	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Mouse move event object.
	 */
	virtual void onMouseMove(attrT** attribute, ODoubleBuffer<stateT>* state, const OMouseMoveEvent* evt) { }

	/**
	 @brief Screen resize event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Screen resize event object.
	 */
	virtual void onScreenResize(attrT** attribute, ODoubleBuffer<stateT>* state, const OResizeEvent* evt) { }
};