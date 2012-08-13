#include "OgreIncludes.h"

namespace Ember
{
namespace OgreView
{
namespace Environment
{
class IFog;
}

class IGraphicalChangeAdapter;

/**
 * @brief Handles change far render distance in factors of absolute far render distance using the main camera.
 * By calling setCompensatedFarRenderDistance() with a factor value, the fog density is also affected to compensate for the missing render distance.
 * To affect only the render distance, call setFarRenderDistance().
 */
class RenderDistanceManager
{
public:
	/**
	 * @brief Constructor.
	 */
	RenderDistanceManager(IGraphicalChangeAdapter& iGraphicalChangeAdapter, Environment::IFog& fog, Ogre::Camera mainCamera);

	/**
	 * @brief Destructor.
	 */
	~RenderDistanceManager();

	/**
	 * @brief Initializes the render distance manager.
	 * The manager starts listening for graphics detail change requests.
	 */
	void initialize();

	/**
	 * @brief Sets the absolute far render distance in a factor of the original value.
	 * @param factor The factor that is multiplied with the original value of absolute far render distance to get the new absolute render distance.
	 * Note: Uses setFarClipDistance on the main camera.
	 */
	bool setFarRenderDistance(float factor);

	/**
	 * @brief Sets the absolute render distance in a factor of the original value and compensates for missing rendered areas using fog.
	 * @param factor The factor that is multiplied with the original value of absolute far render distance to get the new absolute render distance. In this case, it also affects the fog density.
	 */
	bool setCompensatedFarRenderDistance(float factor);

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
	 * @brief Checks level against this component's threshold value to determine change in absolute render distance.
	 * This function is used to listen to the changes requested by the automatic graphics detail system.
	 */
	bool changeLevel(float level);

	/**
	 * @brief Steps down the maximum absolute render distance by provided step.
	 * @param step The value to step the distance factor down by in float.
	 * @see setCompensatedRenderDistance
	 * @note Also affects the fog density.
	 */
	bool stepUpFarRenderDistance(float step);

	/**
	 * @brief Steps up the maximum absolute render distance by provided step.
	 * @param step The value to step the distance factor down by in float.
	 * @see setCompensatedRenderDistance
	 * @note Also affects the fog density.
	 */
	bool stepDownFarRenderDistance(float step);

	/**
	 * The default maximum render distance that the main camera uses.
	 */
	float mDefaultFarRenderDistance;

	/**
	 * The current far render distance that is set by this manager.
	 * @note Changing this value does not guarantee change in render distance. Use setRenderDistance or setCompensatedRenderDistance
	 */
	float mFarRenderDistance;

	/**
	 * The current far render distance factor that is multiplied with the default far render distance to get the new far render distance.
	 */
	float mFarRenderDistanceFactor;

	/**
	 * The maximum factor value that the render distance factor can be set to.
	 * @see setFarRenderDistance
	 * @see setCompensatedFarRenderDistance
	 */
	float mMaxFarRenderDistanceFactor;

	/**
	 * The minimum factor value that the render distance factor can be set to.
	 * @see setFarRenderDistance
	 * @see setCompensatedFarRenderDistance
	 */
	float mMinFarRenderDistanceFactor;

	/**
	 * The threshold fps change that when requested causes a change in maximum render distance.
	 */
	float mRenderDistanceThreshold;

	/**
	 * The step change in the maximum render distance factor that is taken while stepping up or down.
	 */
	float mDefaultRenderDistanceStep;

	/**
	 * Reference to the fog reference used to edit fog density.
	 */
	Environment::IFog& mFog;

	/**
	 * Reference to the graphical change adapter through which graphics detail changes are requested.
	 */
	IGraphicalChangeAdapter& mIGraphicalChangeAdapter;

	/**
	 * Reference to the main camera reference used to affect the far render distance.
	 */
	Ogre::Camera& mMainCamera;

	/**
	 * Holds the reference to the connection to the changeRequired signal. Used to disconnect the signal on destruction of this class or to pause the functioning of this component.
	 */
	sigc::connection mChangeRequiredConnection;
};

}
}
