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
	bool changeFoliageDensity(String foliage, float density);

protected:
	
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
	 * The threshold level of this subcomponent passing which it responds by changing the detail level of foliage.
	 */
	float thresholdLevel;
	
	/**
	 * Holds a map of the original density levels of each plant type as retrieved by the TerrainLayerDefinitionManager.
	 */
	std::map <std::string,float> defaultDensityStore;
	
	
};
	
}
}