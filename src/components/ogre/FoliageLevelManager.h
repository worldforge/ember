#include <sigc++/signal.h>

namespace Ember {
namespace OgreView {

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
	FoliageLevelManager();
	
	/**
	 * @brief Destructor.
	 */
	~FoliageLevelManager();
	
	/**
	 * @brief Signal sent out when this component makes a change in the detail level of foliage.
	 */
	sigc::signal <void,int> foliageLevelChanged;
	
	/**
	 * @brief Changes the detail level of foliage if the asked level is above threshold level of this class.
	 * @return True if a change is possible and is made.
	 */
	bool changeLevel(float level);

protected:
	/**
	 * The threshold level of this subcomponent passing which it responds by changing the detail level of foliage.
	 */
	float thresholdLevel;
	
};
	
}
}