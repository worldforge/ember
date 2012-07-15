#include <OgreFrameListener.h>

#include <sigc++/signal.h>

#include <string>
#include <map>

namespace varconf
{
class Variable;
}

namespace Ember
{

class ConfigListenerContainer;
namespace OgreView
{

/**
 * @brief This structure is used to accumulate the return values from the slots and then pass a result back to the signal
 * It recieves all the return values from all slots registered to this signal and passes on a calculated OR of all the values back to the signal.
 * It is used to calculate whether a further change in graphics level is possible.
 */
template<class T>
struct FurtherChangePossibleAccumulater
{
	/**
	 * The data type of the value returned to the signal. Required by sigc++
	 */
	typedef T result_type;

	/**
	 * An templated iterator to traverse through all the returns from the slots
	 */
	template<class T_iterator>

	/**
	 * @brief The overloaded () function is called by sigc++ to calculate the value to be returned to the signal from the return values of the slots.
	 * @return The value to be passed back to the signal
	 */
	result_type operator()(T_iterator first, T_iterator last) const
	{
		result_type endResult = false;
		for (; first != last; ++first) {
			endResult = endResult || *first;
		}
		return endResult;
	}
};

/**
 * @brief Class that maintains and has the current average FPS
 * 
 * This class keeps the current average FPS over a specified time frame. This time frame acts as a moving time window.
 * eg. A time frame of 5 seconds represents that the FPS should be an average of the fpses of only the last 5 seconds.
 * A time frame of 1 will represent immediate FPS and increasing the time frame will provide a smoothening factor to the calculation.
 */
class FpsUpdater: public Ogre::FrameListener
{
public:
	/**
	 * @brief Constructor
	 */
	FpsUpdater(Ogre::RenderWindow& renderWindow);

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
	sigc::signal<void, float> fpsUpdated;

protected:
	/**
	 * Ogre render window reference from which it updates it's fps.
	 */
	Ogre::RenderWindow& mRenderWindow;

	/**
	 * Holds the current Fps
	 */
	float mCurrentFps;

	/**
	 * Time in milliseconds at last fps update
	 */
	long mTimeAtLastUpdate;

	/**
	 * Time in milliseconds after which the new fps should be calculated. This can be used to induce a delay between updates for changes to take effect.
	 */
	long mTimeBetweenUpdates;

	/**
	 * The amount of time in seconds that the fps should be averaged over.
	 */
	int mRequiredTime;

	/**
	 * A list of last 'n' fpses measured every second. Used for finding the average fps in the last 'n' seconds.
	 */
	std::deque<float> mFpsStore;

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
	bool fpsChangeRequired(float);

	sigc::signal<bool, float>::accumulated<FurtherChangePossibleAccumulater<bool> > changeRequired;
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
	AutomaticGraphicsLevelManager(Ogre::RenderWindow& renderWindow);

	/**
	 * @brief Destructor
	 */
	~AutomaticGraphicsLevelManager();

	/**
	 * @brief Sets whether automatic adjustment is enabled
	 */
	void setEnabled(bool newEnabled);

	/**
	 * @brief Sets the FPS that the component tries to achieve.
	 * @param fps The fps that the manager tries to achieve.
	 */
	void setFps(float fps);

	/**
	 * @brief Used to check if automatic adjustment is enabled
	 */
	bool isEnabled();

	/**
	 * @brief Used to trigger a change in graphics level
	 * @param changeInFpsRequired Used to pass how much of a change in fps is required, positive for an increase in fps, negative for a decrease in fps
	 */
	void changeGraphicsLevel(float changeInFpsRequired);

	/**
	 * @brief Used to access the instance IGraphicalChangeAdapter owned by this class.
	 */
	IGraphicalChangeAdapter& getGraphicalAdapter();

protected:
	/**
	 * The fps this module will try to achieve once enabled
	 */
	float mDefaultFps;

	/**
	 * @brief This function is used to check if the fps is optimum, higher or lower as compared to mDefaultFps.
	 */
	void checkFps(float);

	/**
	 * Boolean that holds whether automatic adjustment is enabled.
	 */
	bool mEnabled;

	/**
	 * Instance of FpsUpdater class owned by this class to get updates on when the fps is updated.
	 */
	FpsUpdater mFpsUpdater;

	/**
	 * The interface through which this central class communicates with the graphical subcomponents.
	 */
	IGraphicalChangeAdapter mGraphicalChangeAdapter;

	/**
	 * @brief Connected to the config service to listen for derired fps settings.
	 */
	void Config_DefaultFps(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Used to listen for configuration changes.
	 */
	ConfigListenerContainer* mConfigListenerContainer;

};

}
}
