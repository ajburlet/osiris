#pragma once

#include <utility>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OApplication.h"

class OEntity;
class OVisualObject;
template <typename, class> class OList;

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
	 @brief Deleted copy constructor.
	 */
	OSimulation(const OSimulation& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OSimulation(OSimulation&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OSimulation();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OSimulation& operator=(const OSimulation& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OSimulation& operator=(OSimulation&& aOther);

	/**
	 @brief Adds an entity to the simulation.
	 */
	void addEntity(OEntity* aEntity);

	/**
	 @brief Removes an entity from the simulation.
	 */
	void removeEntity(OEntity* aEntity);

	/**
	 @brief Adds a visual object to be rendered.
	 */
	void addVisualObject(OVisualObject* aVisualObject);

	/**
	 @brief Removes a visual object. 
	 */
	void removeVisualObject(OVisualObject* aVisualObject);

protected:
	virtual void update(const OTimeIndex & timeIndex, int step_us) override;
	virtual void render() override;

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OSimulation::OSimulation(OSimulation && aOther) : OApplication(std::move(aOther))
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

