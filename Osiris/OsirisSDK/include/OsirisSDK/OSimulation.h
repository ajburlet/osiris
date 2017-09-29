#pragma once

#include "defs.h"
#include "OApplication.h"
#include "OBaseEntity.h"
#include "ORenderObject.h"
#include "OCollection.hpp"

/**
 @brief An OApplication implementation, designed to ease entity handling and renderization.

 This class is an implementation of OApplication that simplifies the general application API, controlling simulation 
 entities and other objects that can be rendered. 
 */
class OAPI OSimulation : public OApplication
{
public:
	/**
	 @brief Class constructor.

	 @param title Application window title.
	 @param argc Number of command line arguments
	 @param argv Command line arguments.
	 @param windowPos_x Window position on the X axis.
	 @param windowPos_y Window position on the Y axis.
	 @param windowWidth Window width.
	 @param windowHeight Window height.
	 @param targetFPS Target FPS.
	 @param simulationStep_us Simulation step in microseconds.
	 */
	OSimulation(const char* title, int argc, char **argv,
		int windowPos_x=OAPPLICATION_DEFAULT_POSX, int windowPos_y=OAPPLICATION_DEFAULT_POSY,
		int windowWidth=OAPPLICATION_DEFAULT_WIDTH, int windowHeight=OAPPLICATION_DEFAULT_HEIGHT,
		int targetFPS=OAPPLICATION_DEFAULT_TARGETFPS, int simulationStep_us=OAPPLICATION_DEFAULT_SIMULATIONSTEP);

	/**
	 @brief Class destructor.
	 */
	virtual ~OSimulation();

	/**
	 @brief Provides the entities as an object collection.
	 */
	OCollection<OBaseEntity>* entities();
	
	/**
	 @brief Provides the entities as an object collection.
	 */
	OCollection<ORenderObject>* renderObjects();

protected:
	virtual void update(const OTimeIndex & timeIndex, int step_us) override;
	virtual void render() override;

private:
	OCollection<OBaseEntity> _entities;
	OCollection<ORenderObject> _renderObjects;

};

