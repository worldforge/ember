#include <sigc++/signal.h>

#include <string>
#include <map>

namespace Ember {
namespace OgreView {

class AutomaticGraphicsLevelManager;	
	
/**
 * @brief This class manages the detail level of foliage by responding to the changeRequired signal from IGraphicalManager.
 * Acts as a sub-component of the automatic handling of graphics system.
 */
class FoliageLevelManager
{
public:
	/**
	 * @brief Constructor.
	 */
	FoliageLevelManager(AutomaticGraphicsLevelManager &automaticGraphicsLevelManager);
	
	/**
	 * @brief Destructor.
	 */
	~FoliageLevelManager();
	
	/**
	 * @brief Signal sent out when this component makes a change in the detail level of foliage.
	 */
	sigc::signal <void,int> foliageLevelChanged;
	
	
	/**
	 * @brief Used to change the density of a particular type of foliage.
	 * @param foliage The plant type, eg. grass, grass_green etc.
	 * @param density The new density for the plant type.
	 * @return True if foliage change was possible.
	 */
	bool changeFoliageDensity(std::string foliage, float density);

protected:
	
	/**
	 * @brief Steps the density of the particular foliage up by value passed.
	 * @param foliage The plant type, eg. grass, grass_green etc.
	 * @param step The amount by which the density of the plant type is stepped up.
	 * @returns True if step up was possible.
	 */
	bool stepUpFoliageDensity(std::string foliage, float step);
	
	/**
	 * @brief Steps the density of the particular foliage down by value passed.
	 * @param foliage The plant type, eg. grass, grass_green etc.
	 * @param step The amount by which the density of the plant type is stepped down.
	 * @returns True if step down was possible.
	 */
	bool stepDownFoliageDensity(std::string foliage, float step);
	
	/**
	 * @brief Changes the detail level of foliage if the asked level is above threshold level of this class.
	 * @return True if a change is possible and is made.
	 */
	bool changeLevel(float level);
	
	/**
	 * @brief Used to find the default density of a particular plant type.
	 * @return The default density of plantType passed to this function.
	 * Uses the map defaultDensityStore to find the matching density of the plantType.
	 */
	float findDefaultDensity(std::string plantType);
	
	/**
	 * @brief Updates foliage densities.
	 * Changes the foliage densities of the main foliage definitions at TerrainLayerDefinitionManager and then reloads the foliage.
	 */
	void updateFoliageDensity();
	
	/**
	 * The threshold level of this subcomponent passing which it responds by changing the detail level of foliage.
	 */
	float mThresholdLevel;

	/**
	 * Holds a map of the original density levels of each plant type as retrieved by the TerrainLayerDefinitionManager.
	 */
	std::map <std::string,float> mDefaultDensityStore;
	
	/**
	 * The value by which density of a foliage is stepped up or down while adjusting foliage detail.
	 */
	float mDefaultDensityStep;
	
	/**
	 * Holds an updated map of plant types and their density levels,
	 */
	std::map <std::string,float> mUpdatedtDensityStore;
	
	
};
	
}
}