#include "OsirisSDK/OEntity.h"
#include "OsirisSDK/ORenderObject.h"
#include "OsirisSDK/OMatrixStack.h"
#include "OsirisSDK/OCamera.h"

#include "OsirisSDK/OSimulation.h"

OSimulation::OSimulation(const char * aTitle, int aArgc, char ** aArgv, GraphicsAPI aGraphicsAPI, 
			 int aWindowPosX, int aWindowPosY, int aWindowWidth, int aWindowHeight, 
			 int aTargetFPS, int aSimulationStep_us) :
	OApplication(aTitle, aArgc, aArgv, aGraphicsAPI, aWindowPosX, aWindowPosY, aWindowWidth, aWindowHeight, 
		     aTargetFPS, aSimulationStep_us)
{
}

OSimulation::~OSimulation()
{
}

OCollection<OEntity>* OSimulation::entities()
{
	return &_entities;
}

OCollection<ORenderObject>* OSimulation::renderObjects()
{
	return &_renderObjects;
}

void OSimulation::update(const OTimeIndex & timeIndex, int step_us)
{
	/* first we equalize states... */
	for (OCollection<OEntity>::Iterator it = entities()->begin(); it != entities()->end(); it++) {
		it.object()->equalizeState();
	}
	/* ...then we update each entity state... */
	for (OCollection<OEntity>::Iterator it = entities()->begin(); it != entities()->end(); it++) {
		it.object()->update(timeIndex, step_us);
	}
	/* ...and finally we swap the states */
	for (OCollection<OEntity>::Iterator it = entities()->begin(); it != entities()->end(); it++) {
		it.object()->swapState(timeIndex, step_us);
	}
}

void OSimulation::render()
{
	auto mtxTransform = camera()->transform();
	/* render entities */
	for (OCollection<OEntity>::Iterator it = entities()->begin(); it != entities()->end(); it++) {
		it.object()->render(mtxTransform);
	}
	/* render other objects */
	for (OCollection<ORenderObject>::Iterator it = renderObjects()->begin(); it != renderObjects()->end(); it++) {
		it.object()->render(mtxTransform);
	}
}
