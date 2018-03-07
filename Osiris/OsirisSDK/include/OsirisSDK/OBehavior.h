#pragma once

#include "defs.h"
#include "OTimeIndex.h"
#include "OEvent.h"

class OMesh;
class OParameterList;
template <class stateT> class ODoubleBuffer;
class OState;
class OTimeIndex;

/**
  @brief Entity behavior interface.

  This template class serves as a programming interface to define entity behavior. It works with
  both entity attributes and state, but is defined neither, since it receives them as arguments 
  when called. But depends on their data structure. Ideally, behavior objects need have no data,
  relying solely on what is provided by the entities that calls them.

  It is able to receive events forwarded by entities.

  @tparam OParameterList Class containing the attribute data structure.
  @tparam OState State class, by default uses the OState. 
 */
class OAPI OBehavior {
public:
	/**
	 @brief Main event handler.

	 This is meant to be called by OEntity class objects. From here the specific event handlers are called. But can
	 be overriden if necessary.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Event class object.
	 */
	virtual void processEvent(OParameterList** attribute, ODoubleBuffer<OState>* state, const OEvent* event);

	/**
	 @brief Entity update method.

	 This is meant to be called by OEntity class objects. By default, it calls the update method on the state
	 object. If you choose to override this method, keep in mind to either do these operations yourself or to 
	 call the OBehavior::update() when appropriate.

	 @param attribute Pointer to entity attributes. 
	 @param state Entity state.
	 @param meshPtr Pointer to entity mesh.
	 @param timeIndex Time index.
	 @param step_us Simulation step in microseconds.
	 */
	virtual void update(OParameterList** attribute, 
			    ODoubleBuffer<OState>* state,
			    OMesh** meshPtr, 
			    const OTimeIndex& timeIndex, 
			    int step_us) = 0;

protected:
	/**
	 @brief Keyboard press event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Keyboard event object.
	 */
	virtual void onKeyboardPress(OParameterList** attribute, ODoubleBuffer<OState>* state, const OKeyboardPressEvent* evt);
	
	/**
	 @brief Mouse release event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Mouse click event object.
	 */
	virtual void onKeyboardRelease(OParameterList** attribute, ODoubleBuffer<OState>* state, const OKeyboardPressEvent* evt);

	/**
	 @brief Mouse click event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Mouse click event object.
	 */
	virtual void onMouseClick(OParameterList** attribute, ODoubleBuffer<OState>* state, const OMouseClickEvent* evt);

	/**
	 @brief Mouse active and passive move event handler.
	 
	 This handler is only called if the object is set as an event recipient. By default does nothing.
	 
	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Mouse move event object.
	 */
	virtual void onMouseMove(OParameterList** attribute, ODoubleBuffer<OState>* state, const OMouseMoveEvent* evt);

	/**
	 @brief Screen resize event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 @param attribute Entity attributes. 
	 @param state Entity state.
	 @param evt Screen resize event object.
	 */
	virtual void onScreenResize(OParameterList** attribute, ODoubleBuffer<OState>* state, const OResizeEvent* evt);
};