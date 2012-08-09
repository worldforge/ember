#include "OgreIncludes.h"

#include "sigc++/connection.h"

namespace Ember
{
namespace OgreView
{

class AutomaticGraphicsLevelManager;

/**
 * @brief Handles changes in shadow detail automatically.
 * Acts as a sub-component of the automatic handling of graphics system.
 */
class ShadowLevelManager
{
public:
	/**
	 * @brief Constructor.
	 */
	ShadowLevelManager(AutomaticGraphicsLevelManager& automaticGraphicsLevelManager, Ogre::SceneManager& sceneManager);

	/**
	 * @brief Destructor.
	 */
	~ShadowLevelManager();

	/**
	 * @brief Sets the maximum distance the shadows are rendered at in float.
	 * @param distance The maximum distance in float.
	 */
	bool setShadowFarDistance(float distance);

	/**
	 * @brief Sets the lod bias for the shadow camera.
	 * @param factor The bias factor applied to the shadow camera.
	 * This can be used to affect the overall quality of the shadow by affecting the level of detail of the mesh for which the shadow is rendered.
	 */
	bool setShadowCameraLodBias(float factor);

	/**
	 * @brief Initializes the shadow level manager.
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
	 * @brief Checks level against this component's threshold value to determine change in shadow detail.
	 * This function is used to listen to the changes required by the automatic graphics detail system.
	 */
	bool changeLevel(float level);

	/**
	 * @brief Steps down the maximum shadow distance by provided step.
	 * @param step The value to step the distance down by in float.
	 */
	bool stepUpShadowDistance(float step);

	/**
	 * @brief Steps up the maximum shadow distance by provided step.
	 * @param step The value to step the distance down by in float.
	 */
	bool stepDownShadowDistance(float step);

	/**
	 * @brief Steps down the maximum shadow distance by provided step.
	 * @param step The value to step the distance down by in float.
	 */
	bool stepUpShadowCameraLodBias(float step);

	/**
	 * @brief Steps up the maximum shadow distance by provided step.
	 * @param step The value to step the distance down by in float.
	 */
	bool stepDownShadowCameraLodBias(float step);

	/**
	 * Holds the distance the rendered shadows are culled. i.e. shadows beyond this distance are not rendered.
	 * @see setShadowFarDistance
	 */
	float mShadowFarDistance;

	/**
	 * The threshold fps change after which the shadow camera lod level is changed to respond to performance change requested.
	 */
	float mShadowCameraLodThreshold;

	/**
	 * The threshold fps change after which the shadow distance is changed to respond to performance change requested.
	 */
	float mShadowDistanceThreshold;

	/**
	 * The maximum distance the shadow far distance can be increased to.
	 */
	float mMaxShadowFarDistance;

	/**
	 * The minimum distance the shadow far distance can be decreased to.
	 */
	float mMinShadowFarDistance;

	/**
	 * The step taken up or down while changing shadow far distance.
	 */
	float mDefaultShadowDistanceStep;

	/**
	 * Holds the level of detail bias for the shadow camera.
	 * @see setShadowCameraLodBias
	 */
	float mShadowCameraLodBias;

	/**
	 * The maximum lod bias factor that the shadow camera can have.
	 */
	float mMaxShadowCameraLodBias;

	/**
	 * The minimum lod bias factor that the shadow camera can have.
	 */
	float mMinShadowCameraLodBias;

	/**
	 * The step taken up or down while changing the lod bias for generating shadows.
	 */
	float mDefaultShadowLodStep;

	/**
	 * Holds the reference to the connection to the changeRequired signal. Used to disconnect the signal on destruction of this class or to pause the functioning of this component.
	 */
	sigc::connection mChangeRequiredConnection;

	/**
	 * Reference to the scenemanager used to edit the shadows.
	 */
	Ogre::SceneManager& mSceneManager;

	/**
	 * Reference to AutomaticGraphicsLevelManager class that controls this manager.
	 */
	AutomaticGraphicsLevelManager& mAutomaticGraphicsLevelManager;

};
}
}
