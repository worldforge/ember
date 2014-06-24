#ifndef SMARTBODYMANAGER_H
#define SMARTBODYMANAGER_H

	#include <string>


namespace SmartBody
{

class SBScene;
class SBCharacter;
class SBAssetManager;
class SBSimulationManager;
class SBBmlProcessor;

}

namespace Ogre
{
class Entity;
}

namespace Ember
{

class SmartBodyLocomotion;

/**
 @brief This class is here to set up the SmartBody library.

 Its main function is to bound the Ogre skeletons to SmartBody's ones, in order to animate the humanoid characters like real persons. Every call to
 SmartBody has to be done threw this class.

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodyManager
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyManager(void);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyManager(void);

	/**
	 * @brief Initializes the object, and by the way, the SmartBody library.
	 */
	void initialize(void);

	/**
	 * @brief Sets sbName to the name of the SmartBody skeleton corresponding to ogreName (returns false if not possible).
	 */
	bool setCorrespondingSkeletonName(std::string& sbName, const std::string& ogreName);

	/**
	 * @brief Returns true if this Ogre skeleton possesses its equivalent in SmartBody : 
	 				* check through the assets if it has been loaded,
	 				* if not, and if load is set to tru, try to load it,
	 				* if it still can't do it, then, returns false.
	 */
	bool hasSkeleton(const std::string& ogreName, bool load = false);

	/**
	 * @brief Creates a new SmartBody character and initialize him (skeleton, behaviors, etc.).
	 */
	SmartBody::SBCharacter* createCharacter(const Ogre::Entity& entity, const std::string& group, const std::string& sbSkName);

	/**
	 * @brief Removes a character from the scene.
	 */
	void removeCharacter(SmartBody::SBCharacter *character);

	/**
	 * @brief Returns the scene.
	 */
	SmartBody::SBScene* getScene(void) const;

	/**
	 * @brief Returns the asset manager.
	 */
	SmartBody::SBAssetManager* getAssetManager(void) const;

private:

	/**
	 * @brief SmartBody scene.
	 */
	SmartBody::SBScene *mScene;

	/**
	 * @brief SmartBody asset manager.
	 */
	SmartBody::SBAssetManager *mAssetManager;

	/**
	 * @brief SmartBody simulation object.
	 */
	SmartBody::SBSimulationManager *mSimulation;

	/**
	 * @brief SmartBody BML Processor.
	 */
	SmartBody::SBBmlProcessor *mProcessor;

	/**
	 * @brief Locomotion behavior.
	 */
	SmartBodyLocomotion *mLocomotion;

	/**
	 * @brief Boolean stating that the manager has been initialized correctly.
	 */
	bool mIsInit;


	/**
	 * @brief Adds the asset paths for SmartBody (the paths are set in SmartBodyConsts.h).
	 */
	void addAssetPaths(void);

	/**
	 * @brief Loads and set up all the behaviors.
	 */
	void loadAllBehaviors(void);

	/**
	 * @brief Retarget all the behaviors on the given character.
	 */
	void retargetAllBehaviors(SmartBody::SBCharacter& character);

	/**
	 * @brief Sets or unsets manual control over the (Ogre) skeleton of the entity given as parameter.
	 */
	void setManualControl(const Ogre::Entity& entity, bool mode = true);

};

}

#endif