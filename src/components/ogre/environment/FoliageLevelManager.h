#include <sigc++/signal.h>
#include <sigc++/connection.h>

#include <string>
#include <map>

namespace Ember
{
namespace OgreView
{
namespace Environment
{

class AutomaticGraphicsLevelManager;
class Foliage;
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
	FoliageLevelManager(Foliage& foliage);

	/**
	 * @brief Destructor.
	 */
	~FoliageLevelManager();

	/**
	 * @brief Signal sent out when this component makes a change in the detail level of foliage.
	 */
	sigc::signal<void, int> foliageLevelChanged;

	/**
	 * @brief Signal sent when this component changes foliage density levels.
	 */
	sigc::signal<void, float> foliageDensityChanged;

	/**
	 * @brief Used to change the density of all foliage.
	 * @param density The new density percentage of all foliage in float, where 1 implies normal or full density and 0 implies no density or no grass. 
	 * @return True if foliage change was possible.
	 */
	bool changeFoliageDensity(float density);

protected:

	/**
	 * @brief Steps the density of foliage up by value passed.
	 * @param step The amount by which the density of the foliage is stepped up.
	 * @returns True if step up was possible.
	 */
	bool stepUpFoliageDensity(float step);

	/**
	 * @brief Steps the density of foliage down by value passed.
	 * @param step The amount by which the density of the foliage is stepped down.
	 * @returns True if step down was possible.
	 */
	bool stepDownFoliageDensity(float step);

	/**
	 * @brief Changes the detail level of foliage if the asked level is above threshold level of this class.
	 * @return True if a change is possible and is made.
	 */
	bool changeLevel(float level);

	/**
	 * @brief Updates foliage densities.
	 * Sends out a signal with the updated foliage density.
	 */
	void updateFoliageDensity();

	/**
	 * The threshold level of this subcomponent passing which it responds by changing the detail level of foliage.
	 */
	float mThresholdLevel;

	/**
	 * The value by which density of a foliage is stepped up or down while adjusting foliage detail.
	 */
	float mDefaultDensityStep;

	/**
	 * Holds an updated percentage density value of all foliage.
	 */
	float mUpdatedDensity;

	/**
	 * Holds the reference to the connection to the changeRequired signal. Used to disconnect the signal on destruction of this class.
	 */
	sigc::connection mChangeRequiredConnection;

	/**
	 * Reference to the foliage class that owns this class.
	 */
	Foliage& mFoliage;

};

}
}
}
