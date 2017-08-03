#pragma once

#include "OObject.h"
#include "OBehavior.hpp"
#include "ODoubleBuffer.hpp"
#include "OState.h"
#include "OMesh.h"

/**
 @brief Simulation entity class.

 A simulation entity is composed by it's behavior, attributes, state and mesh. Here we favor composition over
 inheritance, so this class isn't supposed to be derived. If specialization is needed, then one can do so using
 the attribute, state or behavior classes.

 @tparam attrT Class containing the attribute data structure.
 @tparam stateT State class, by default uses the OState. 
 */
template <class attrT, class stateT=OState> class OEntity : public OObject {
public:
	OEntity(OBehavior<attrT, stateT>* behavior, OMesh* mesh) : _mesh(mesh) { }
	virtual ~OEntity() { }

	/**
	 @brief Main event handle.

	 This is meant to be called by OApplication class objects. This method should not me overriden, since event
	 handling should be processed by the behavior object.

	 @param evt Event class object.
	 */
	void processEvent(const OEvent* evt) 
	{ 
		if (_behavior != NULL) _behavior->processEvent(&attribute, &state, evt);  
	}

	/**
	 @brief Update method. 

	 This is meant to be called by OApplication class objects. This method should not me overriden, since
	 this must be handled by the behavior object.

	 @param timeIndex Time index.
	 */
	void update(const OTimeIndex& timeIndex) 
	{
		if (_behavior != NULL) _behavior->update(&attribute, &state, &mesh, timeIndex);
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
	ODoubleBuffer<stateT>* currentState() { return &_state; }

	/**
	 @brief Returns pointer to entity attribute double buffer object.
	 */
	ODoubleBuffer<attrT>* attribute() { return &_attribute; }

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
	ODoubleBuffer<attrT> _attribute;
	ODoubleBuffer<stateT> _state;
	OMesh *_mesh;
};