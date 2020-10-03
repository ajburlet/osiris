#pragma once

#include "OsirisSDK/OObject.h"
#include "OsirisSDK/OVisualObject.h"
#include "OsirisSDK/ODoubleBuffer.hpp"
#include "OsirisSDK/OState.h"
#include "OsirisSDK/OTimeIndex.h"

class OParameterList;
class OBehavior;
class OMesh;

/**
 @brief Simulation entity class.

 A simulation entity is composed by it's behavior, attributess, state and mesh. Here we favor composition over
 inheritance, so this class isn't supposed to be derived. If specialization is needed, then one can do so using
 the attributes, state or behavior classes.
 */
class OAPI OEntity : public OObject, public OVisualObject { 
public:
	/**
	 @brief Class constructor.
	 @param attributes Entity attributes object.
	 @param behavior Entity behavior object. 
	 @param mesh Pointer to entity mesh object.
	 */
	OEntity(OParameterList* attributes, OBehavior* behavior, OMesh* mesh);

	/**
	 @brief Class destructor.
	 */
	virtual ~OEntity();

	/**
	 @brief Main event handle.

	 This is meant to be called by OApplication class objects. This method should not me overriden, since event
	 handling should be processed by the behavior object.

	 @param evt Event class object.
	 */
	void processEvent(const OEvent* evt);

	void update(const OTimeIndex& timeIndex, int step_us);

	void equalizeState();
	
	void swapState(const OTimeIndex& timeIndex, int step_us);

	virtual void render(ORenderingEngine* aRenderingEngine, OMatrixStack* aMatrixStack) override;

	/**
	 @brief Set object behavior.
	 */
	void setBehavior(OBehavior* behavior);

	/**
	 @brief Returns pointer to the behavior object.
	 */
	OBehavior* behavior();

	/**
	 @brief Returns pointer to entity state double buffer object.
	 */
	ODoubleBuffer<OState>* state();

	/**
	 @brief Set entity attributes.
	 */
	void setAttributes(OParameterList* attributes);

	/**
	 @brief Returns pointer to entity attributes object.
	 */
	OParameterList* attributes();

	/**
	 @brief Returns pointer to entity mesh. 
	 */
	OMesh* mesh();

	/**
	 @brief Set entity mesh.
	 */
	void setMesh(OMesh* mesh);

	/**
	 @brief Enables entity processing for each update call.
	 */
	void enable();

	/**
	 @brief Disables entity processing for each update call.
	 */
	void disable();

	/**
	 @brief Returns true if object is disabled to process update calls.
	 */
	bool isDisabled() const;


private:
	OBehavior* _behavior;
	OParameterList* _attributes;
	ODoubleBuffer<OState> _state;
	OMesh *_mesh;
	bool _disabled;
};
