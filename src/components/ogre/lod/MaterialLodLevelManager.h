namespace Ember
{
namespace OgreView
{
namespace Lod
{

class MaterialLodLevelManager
{
public:
	/**
	 * @brief Constructor.
	 */
	MaterialLodLevelManager();

	/**
	 * @brief Destructor.
	 */
	~MaterialLodLevelManager();

	/**
	 * @brief Sets the lod bias factor.
	 * Using this function it is possible to influence the lod behavior of all materials. It shifts the lod behavior of all materials
	 * currently loaded up or down.
	 * @param factor Proportional factor to apply to the distance at which LOD is changed. Higher values increase the distance at which 
	 * higher LODs are displayed (eg. 2.0 is twice the normal distance, 0.5 is half).
	 */
	bool setLodBiasAll(float factor);

	/**
	 * @brief Sets the lod bias factor for passed material categories.
	 * @see setLodBiasAll
	 * @param materialCategories List of categories the materials belong to.
	 * @param factor Proportional factor to apply to the distance at which LOD is changed.
	 * @note Current categories include: characters, creatures, environment, items, plants, primitives, structures.
	 */
	bool setLodBias(std::list<std::string> materialCategories, float factor);

protected:
	/**
	 * @brief Checks level against this component's threshold value to determine change in material lod.
	 * This function is used to listen to the changes required by the automatic graphics detail system.
	 */
	bool changeLevel(float level);

};

}
}
}
