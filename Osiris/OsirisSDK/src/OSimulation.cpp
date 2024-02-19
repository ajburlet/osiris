#include "OsirisSDK/OEntity.h"
#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/OCamera.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OSet.hpp"
#include "OsirisSDK/OEntity.h"
#include "OsirisSDK/ORenderable.h"
#include "OsirisSDK/ORenderingEngine.h"

#include "OsirisSDK/OSimulation.h"

// ----------------------------------------------------------------------------
// concealed implementation
// ----------------------------------------------------------------------------
struct OSimulation::Impl {
	struct VisualObjectKey {
		OVisualObject::RenderPriority	priority;
		OVisualObject*			visualObject;

		bool operator<(const VisualObjectKey& aOther) const {
			return (priority == aOther.priority) ? 
				(visualObject < aOther.visualObject) : (priority < aOther.priority);
		}
	};
	using VisualObjectSet = OSet<VisualObjectKey>;
	using EntitySet = OSet<OEntity*>;

	VisualObjectSet	visual_objects;
	EntitySet	entities;
};


// ----------------------------------------------------------------------------
// class implementation
// ----------------------------------------------------------------------------
OSimulation::OSimulation(const char * aTitle, int aArgc, char ** aArgv, GraphicsAPI aGraphicsAPI, 
			 int aWindowPosX, int aWindowPosY, int aWindowWidth, int aWindowHeight, 
			 int aTargetFPS, int aSimulationStep_us) :
	OApplication(aTitle, aArgc, aArgv, aGraphicsAPI, aWindowPosX, aWindowPosY, aWindowWidth, aWindowHeight, 
		     aTargetFPS, aSimulationStep_us)
{
	OExPointerCheck(_impl = new Impl);
}

OSimulation::~OSimulation()
{
	if (_impl) delete _impl;
}

void OSimulation::addEntity(OEntity * aEntity)
{
	_impl->entities.insert(aEntity);
	addVisualObject(aEntity);
}

void OSimulation::removeEntity(OEntity * aEntity)
{
	_impl->entities.remove(aEntity);
	removeVisualObject(aEntity);
}

void OSimulation::addVisualObject(OVisualObject * aVisualObject)
{
	_impl->visual_objects.insert({ aVisualObject->renderPriority(), aVisualObject });
}

void OSimulation::removeVisualObject(OVisualObject * aVisualObject)
{
	_impl->visual_objects.remove({ aVisualObject->renderPriority(), aVisualObject });
}

void OSimulation::update(const OTimeIndex & timeIndex, int step_us)
{
	/* first we equalize states... */
	for (auto entity : _impl->entities) entity->equalizeState();
	
	/* ...then we update each entity state... */
	for (auto entity : _impl->entities) entity->update(timeIndex, step_us);
	
	/* ...and finally we swap the states */
	for (auto entity : _impl->entities) entity->swapState(timeIndex, step_us);
}

void OSimulation::render()
{
	auto mtxTransform = camera()->transform();
	for (auto& item : _impl->visual_objects) {
		item.visualObject->render(&renderingEngine(), mtxTransform);
	}
}
