#include "OsirisSDK/OEntity.h"
#include "OsirisSDK/ORenderObject.h"
#include "OsirisSDK/OMatrixStack.h"

#include "OsirisSDK/OSimulation.h"

OSimulation::OSimulation(const char * title, int argc, char ** argv, int windowPos_x, int windowPos_y, 
			 int windowWidth, int windowHeight, int targetFPS, int simulationStep_us) :
	OApplication(title, argc, argv, windowPos_x, windowPos_y, windowWidth, windowHeight, targetFPS, simulationStep_us)
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
