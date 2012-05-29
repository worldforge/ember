
#include <OgreFrameListener.h>

#include <sigc++/signal.h>

#include <string>
#include <map>



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
	 * @brief Constructor
	 */
	FpsUpdater();
	
	/**
	 * @brief Destructor
	 */
	~FpsUpdater();
	
	/**
	 * Method from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
	
	/**
	 * @brief Used to access the current FPS as known by this class.
	 * @return The current FPS
	 */
	float getCurrentFPS();
	
	/**
	 * @brief Signal sent out with the updated FPS value every frame
	 */
	sigc::signal<void,float> fpsUpdated;
	
	

protected:
	/**
	 * Holds the current Fps
	 */
	float mCurrentFps;	

};
	

/**
 * @brief Adaptor interface class between the central AutomaticGraphicsLevelManager class and the graphics subsystems
 * This class accepts a change in fps required and translates it into a floating change required value that the subsystems understand
 */
class IGraphicalChangeAdapter 
{
public:

	/**
	 * @brief Constructor.
	 */
	IGraphicalChangeAdapter();
	
	/**
	 * @brief Destructor.
	 */
	~IGraphicalChangeAdapter();
	
	/**
	* Signals that a change is required.
	* @param changeSize The change required in fps. A positive value means that graphical details should be improved. A negative value means that the details should be decreased.
	* @return True if further change can be performed. 
	*/
	bool fpsChangeRequired(float changeSize) = 0;
	
	sigc::signal<void,float> changeRequired;
};
  
/**
 *@brief Central class for automatic adjustment of graphics level
 *
 * This class maintains a current Graphics level. It connects to the fpsUpdated signal and thus 
 * checks the fps for a significant increase or decrease and then asks for a change in the level
 * by using the IGraphicalChangeAdapter.
 */

class AutomaticGraphicsLevelManager
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
	 * @brief Sets whether automatic adjustment is enabled
	 */
	bool setEnabled(bool newEnabled);
	
	/**
	 * @brief Used to check if automatic adjustment is enabled
	 */
	bool isEnabled();
	
	/**
	 * @brief Used to trigger a change in graphics level
	 * @param changeInFpsRequired Used to pass how much of a change in fps is required, positive for an increase in fps, negative for a decrease in fps
	 */
	void changeGraphicsLevel(float changeInFpsRequired);
	
protected:
	/**
	 * Boolean that holds whether automatic adjustment is enabled
	 */
	bool mEnabled;
	
	
	
};	

  
}
}
