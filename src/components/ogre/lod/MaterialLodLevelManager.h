namespace Ember
{
namespace OgreView
{
class AutomaticGraphicsLevelManager;

namespace Lod
{

class MaterialLodLevelManager
{
public:
	/**
	 * @brief Constructor.
	 */
	MaterialLodLevelManager(AutomaticGraphicsLevelManager& automaticGraphicsLevelManager);

	/**
	 * @brief Destructor.
	 */
	~MaterialLodLevelManager();

	/**
	 * @brief Sets the lod bias factor.
	 * Using this function it is possible to influence the lod behavior of all materials. It shifts the lod behavior 
	 * of all materials currently loaded up or down.
	 * @param factor Proportional factor to apply to the distance at which LOD is changed. Higher values increase the distance at which higher LODs are displayed (eg. 2.0 is twice the normal distance, 0.5 is half).
	 */
	bool setLodBiasAll(float factor);

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
	 * The threshold level after which this component starts responding to change in detail required signals.
	 */
	float mThresholdLevel;
	
	/**
	 * The current global material lod bias. This value is used to affect the level of detail distance values 
	 * of all materials.
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
	
};

}
}
}
