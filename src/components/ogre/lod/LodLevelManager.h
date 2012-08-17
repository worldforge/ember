#include "sigc++/connection.h"
#include "string"
#include "components/ogre/OgreIncludes.h"

namespace varconf
{
class Variable;
}

namespace Ember
{

class ConfigListenerContainer;
namespace OgreView
{

class ShaderManager;
class IGraphicalChangeAdapter;

namespace Lod
{
/**
 * @brief Handles level of detail changes for the main camera.
 * Acts as a sub-component of the automatic handling of graphics system.
 */
class LodLevelManager
{
public:
	/**
	 * @brief Constructor.
	 */
	LodLevelManager(IGraphicalChangeAdapter& graphicalChangeAdapter, Ogre::Camera& mainCamera);

	/**
	 * @brief Destructor.
	 */
	~LodLevelManager();

	/**
	 * @brief Sets the lod bias factor.
	 * Using this function it is possible to influence the lod behavior of all materials and meshes. It works by adjusting the 
	 * lod bias of the main camera.
	 * @param factor Proportional factor to apply to the distance at which LOD is changed. Higher values increase the distance at which higher LODs are used (eg. 2.0 is twice the normal distance, 0.5 is half).
	 */
	bool setLodBiasAll(float factor);

	/**
	 * @brief Initializes lod level manager.
	 * The manager starts listening for graphics detail change requests.
	 */
	void initialize();

	/**
	 * @brief This can be used to stop this component responding to change requests.
	 */
	void pause();

	/**
	 * @brief This can be used to restart this component responding to change requests.
	 */
	void unpause();

protected:
	/**
	 * @brief Checks level against this component's threshold value to determine change in material lod.
	 * This function is used to listen to the changes required by the automatic graphics detail system.
	 */
	bool changeLevel(float level);

	/**
	 * @brief Steps up the material lod bias by provided step.
	 * @param step The value to step the bias up by.
	 */
	bool stepUpLodBias(float step);

	/**
	 * @brief Steps down the material lod bias by provided step.
	 * @param step The value to step the bias down by.
	 */
	bool stepDownLodBias(float step);
	
	/**
	 * @brief Connected to the config service to listen for lod bias settings.
	 */
	void Config_LodBias(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * The threshold fps change after which the lod level is changed to respond to performance change requested.
	 */
	float mLodThresholdLevel;

	/**
	 * The current global lod bias. This value is used to affect the level of detail distance values 
	 * of all materials and meshes.
	 */
	float mLodFactor;

	/**
	 * The minimum material lod bias that the lod bias factor can be set to.
	 */
	float mMinLodFactor;

	/**
	 * The maximum material lod bias that the lod bias factor can be set to.
	 */
	float mMaxLodFactor;

	/**
	 * The default step that is taken when adjusting the factors of this component.
	 */
	float mDefaultStep;

	/**
	 * Holds the reference to the connection to the changeRequired signal. Used to disconnect the signal on destruction of this class or to pause the functioning of this component.
	 */
	sigc::connection mChangeRequiredConnection;

	/**
	 * Reference to graphical adapter through which this manager receives graphics detail change requests.
	 */
	IGraphicalChangeAdapter& mGraphicalChangeAdapter;

	/**
	 * Reference to the main camera object
	 */
	Ogre::Camera& mMainCamera;
	
	/**
	 * @brief Used to listen for configuration changes.
	 */
	ConfigListenerContainer* mConfigListenerContainer;
};

}
}
}
