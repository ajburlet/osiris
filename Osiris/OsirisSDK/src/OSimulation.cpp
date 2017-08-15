#include "OsirisSDK/OSimulation.h"

OSimulation::OSimulation(const char * title, int argc, char ** argv, int windowPos_x, int windowPos_y, 
			 int windowWidth, int windowHeight, int targetFPS, int simulationStep_us) :
	OApplication(title, argc, argv, windowPos_x, windowPos_y, windowWidth, windowHeight, targetFPS, simulationStep_us)
{
}

OSimulation::~OSimulation()
{
}

OCollection<OBaseEntity>* OSimulation::entities()
{
	return &_entities;
}

OCollection<ORenderObject>* OSimulation::renderObjects()
{
	return &_renderObjects;
}

void OSimulation::update(const OTimeIndex & timeIndex)
{
	for (OCollection<OBaseEntity>::Iterator it = entities()->begin(); it != entities()->end(); it++) {
		it.object()->update(timeIndex);
	}
}

void OSimulation::render()
{
	for (OCollection<OBaseEntity>::Iterator it = entities()->begin(); it != entities()->end(); it++) {
		it.object()->render(camera()->transform());
	}
	for (OCollection<ORenderObject>::Iterator it = renderObjects()->begin(); it != renderObjects()->end(); it++) {
		it.object()->render(camera()->transform());
	}
}
