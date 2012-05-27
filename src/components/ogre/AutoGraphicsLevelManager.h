

#include <OgreFrameListener.h>
#include <components/ogre/FpsListener.h>


#include <string>
#include <map>
#include <sigc++/signal.h>


namespace Ember {

namespace OgreView {
	
/**
 * @brief Class that maintains and has the current FPS
 * 
 * This class keeps the current FPS and throws an FPS event every 2 seconds.
 */
class FpsUpdater : public Ogre::FrameListener
{
public:
	/**
	 * Method from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
	
	/**
	 * @brief Used to access the current FPS as known by this class.
	 * @return The current FPS
	 */
	double getCurrentFPS();
	
	/**
	 * @brief Used to register an Fps listener which is called back everytime the Fps is updated
	 */
	void addFpsListener(FpsListener* );
	
	/**
	 * @brief Constructor
	 */
	FpsUpdater();
	
	/**
	 * @brief Destructor
	 */
	~FpsUpdater();

protected:
	/**
	 * Holds the current Fps
	 */
	double mCurrentFps;
	
	/**
	 * @brief Calculates the FPS and updates mCurrentFps after every frame
	 */
	void updateFPS();	
	
	/**
	 * @brief Throws an FPS event to all the listeners
	 */
	void FpsGenerated();
	
	/**
	 * List of registered Fps listeners
	 */
	std::set<FpsListener*> mFpsListeners;
};
	
	
  
/**
 *@brief Interface class for automatic adjustment of graphics level
 *
 * This class maintains a current Graphics level. It is registered as an FpsListener and therefore
 * checks the FPS for a significant increase or decrease and then increases or decreases the Required Graphics level
 * by a factor depending on how drastic the change in FPS is. This signals an adjustment event.
 */

class AutomaticGraphicsLevelManager : public FpsEvent
{
public:
	/**
	 * @brief Constructor
	 */
	AutomaticGraphicsLevelManager();
	
	/**
	 * @brief Destructor
	 */
	~AutomaticGraphicsLevelManager();
	
	/**
	 * @brief Returns the current graphics level
	 */
	double getCurrentLevel();
	
	/**
	 * @brief Sets whether automatic adjustment is enabled
	 */
	bool setEnabled(bool newEnabled);
	
	/**
	 * @brief Used to check if automatic adjustment is enabled
	 */
	bool isEnabled();
	
	/**
	 * Implementation of function from FpsListener
	 */
	void FpsUpdated(const FpsEvent);
	
	
protected:
	/**
	 * Boolean that holds whether automatic adjustment is enabled
	 */
	bool mEnabled;
	
	/**
	 * Holds the current floating graphics level
	 */
	double mGraphicsLevel;
	
	/**
	 * @brief Used to trigger a change in graphics level event
	 */
	void setGraphicsLevel(double level);
	
};	

  
}
}
