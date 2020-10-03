#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/OMesh.h"
#include "OsirisSDK/OBehavior.h"

#include "OsirisSDK/OEntity.h"

using namespace std;

OEntity::OEntity(OParameterList * attributes, OBehavior* behavior, OMesh * mesh) :
	_attributes(attributes),
	_behavior(behavior),
	_mesh(mesh),
	_disabled(false)
{ }

OEntity::~OEntity()
{
}

void OEntity::processEvent(const OEvent * evt)
{
	if (isDisabled()) return;
	if (_behavior != NULL) _behavior->processEvent(&_attributes, &_state, evt);  
}

void OEntity::update(const OTimeIndex & timeIndex, int step_us)
{
	if (isDisabled()) return;
	if (_behavior != NULL) _behavior->update(&_attributes, &_state, &_mesh, timeIndex, step_us);
}

void OEntity::equalizeState()
{
	_state.equalize();
}

void OEntity::swapState(const OTimeIndex & timeIndex, int step_us)
{
	_state.next()->update(timeIndex, step_us);
	_state.swap();
}

void OEntity::render(ORenderingEngine* aRenderingEngine, OMatrixStack * aMatrixStack)
{
	if (isHidden()) return;
	aMatrixStack->push();
	aMatrixStack->translate(_state.curr()->position());
	*aMatrixStack *= _state.curr()->orientation();
	aMatrixStack->scale(_state.curr()->scale());
	_mesh->render(aRenderingEngine, aMatrixStack);
	aMatrixStack->pop();
}

void OEntity::setBehavior(OBehavior * behavior) 
{ 
	_behavior = behavior; 
}

OBehavior * OEntity::behavior()
{ 
	return _behavior; 
}

ODoubleBuffer<OState>* OEntity::state() 
{ 
	return &_state; 
}

void OEntity::setAttributes(OParameterList * attributes)
{ 
	_attributes = attributes; 
}

OParameterList * OEntity::attributes() 
{ 
	return _attributes; 
}

OMesh * OEntity::mesh()
{
	return _mesh;
}

void OEntity::setMesh(OMesh * mesh)
{
	_mesh = mesh;
}

void OEntity::enable()
{
	_disabled = false;
}

void OEntity::disable()
{
	_disabled = true;
}

bool OEntity::isDisabled() const
{
	return _disabled;
}
