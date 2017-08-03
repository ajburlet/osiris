#pragma once

#include "OBaseEntity.h"
#include "ORenderObject.h"
#include "OBehavior.hpp"
#include "ODoubleBuffer.hpp"
#include "OState.h"
#include "OMesh.h"


/**
 @brief Simulation entity class.

 A simulation entity is composed by it's behavior, attributess, state and mesh. Here we favor composition over
 inheritance, so this class isn't supposed to be derived. If specialization is needed, then one can do so using
 the attributes, state or behavior classes.

 @tparam attrT Class containing the attributes data structure.
 @tparam stateT State class, by default uses the OState. 
 */
template <class attrT, class stateT=OState> class OAPI OEntity : public OBaseEntity, public ORenderObject {
public:
	/**
	 @brief Class constructor.
	 @param attributes Entity attributes object.
	 @param behavior Entity behavior object. 
	 @param mesh Pointer to entity mesh object.
	 */
	OEntity(attrT* attributes, OBehavior<attrT, stateT>* behavior, OMesh* mesh) :
		_attributes(attributes),
		_behavior(behavior),
		_mesh(mesh)
	{ }

	/**
	 @brief Class destructor.
	 */
	virtual ~OEntity() { }

	/**
	 @brief Main event handle.

	 This is meant to be called by OApplication class objects. This method should not me overriden, since event
	 handling should be processed by the behavior object.

	 @param evt Event class object.
	 */
	void processEvent(const OEvent* evt) 
	{
		if (isDisabled()) return;
		if (_behavior != NULL) _behavior->processEvent(&attributes, &state, evt);  
	}

	void update(const OTimeIndex& timeIndex) 
	{
		if (isDisabled()) return;
		if (_behavior != NULL) _behavior->update(&attributes, &state, &mesh, timeIndex);
	}

	void render(OMatrixStack& stack)
	{
		if (isHidden()) return;
		stack.push();
		stack.translate(_state->curr()->position());
		stack *= _state->curr()->orientation();
		_mesh->render(&stack);
		stack.pop();
	}

	/**
	 @brief Set object behavior.
	 */
	void setBehavior(OBehavior<attrT, stateT>* behavior) { _behavior = behavior; }

	/**
	 @brief Returns pointer to the behavior object.
	 */
	OBehavior* behavior() { return _behavior; }

	/**
	 @brief Returns pointer to entity state double buffer object.
	 */
	ODoubleBuffer<stateT>* state() { return &_state; }

	/**
	 @brief Set entity attributes.
	 */
	void setAttributes(attrT* attributes) { _attributes = attributes; }

	/**
	 @brief Returns pointer to entity attributes object.
	 */
	attrT* attributes() { return _attributes; }

	/**
	 @brief Returns pointer to entity mesh. 
	 */
	OMesh* mesh() { return _mesh; }

	/**
	 @brief Set entity mesh.
	 */
	void setMesh(OMesh* mesh) { _mesh = mesh; }


private:
	OBehavior<attrT, stateT>* _behavior;
	attrT* _attributes;
	ODoubleBuffer<stateT> _state;
	OMesh *_mesh;
};
