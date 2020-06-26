#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OApplication.h"
#include "OsirisSDK/OCollection.hpp"

class OEntity;
class ORenderObject;

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

	 @param aTitle Application window title.
	 @param aArgc Number of command line arguments
	 @param aArgv Command line arguments.
	 @param aGraphicsAPI The graphics API used by the application.
	 @param AWindowPosX Window position on the X axis.
	 @param aWindowPosY Window position on the Y axis.
	 @param aWindowWidth Window width.
	 @param aWindowHeight Window height.
	 @param aTargetFPS Target FPS.
	 @param aSimulationStep_us Simulation step in microseconds.
	 */
	OSimulation(const char* aTitle, int aArgc, char **aArgv, GraphicsAPI aGraphicsAPI,
		int aWindowPosX=OAPPLICATION_DEFAULT_POSX, int aWindowPosY=OAPPLICATION_DEFAULT_POSY,
		int aWindowWidth=OAPPLICATION_DEFAULT_WIDTH, int aWindowHeight=OAPPLICATION_DEFAULT_HEIGHT,
		int aTargetFPS=OAPPLICATION_DEFAULT_TARGETFPS, int aSimulationStep_us=OAPPLICATION_DEFAULT_SIMULATIONSTEP);

	/**
	 @brief Class destructor.
	 */
	virtual ~OSimulation();

	/**
	 @brief Provides the entities as an object collection.
	 */
	OCollection<OEntity>* entities();
	
	/**
	 @brief Provides the entities as an object collection.
	 */
	OCollection<ORenderObject>* renderObjects();

protected:
	virtual void update(const OTimeIndex & timeIndex, int step_us) override;
	virtual void render() override;

private:
	OCollection<OEntity> _entities;
	OCollection<ORenderObject> _renderObjects;

};

