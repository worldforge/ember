#ifndef SMARTBODYMANAGER_H
#define SMARTBODYMANAGER_H

#include <SmartBody.h>


namespace Ember
{

/**
 @brief This class is here to set up the SmartBody library.

 Its main function is to bound the Ogre skeletons to SmartBody's ones, in order to animate the humanoid characters like real persons. Every call to
 SmartBody has to be done threw this class.

 It's a singleton so you can access it through
 @code
 SmartBodyManager::getSingleton()
 @endcode

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodyManager: public Singleton<SmartBodyManager>
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyManager();

	/**
	 * @brief Dtor.
	 */
	~SmartBodyManager();

	/**
	 * @brief Initializes the object, and by the way, the SmartBody library.
	 */
	void initialize();


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
	 * @brief Boolean stating that the manager has been initialized correctly.
	 */
	bool mIsInit;


	/**
	 * @brief Load the assets used by SmartBody (the paths are set in SmartBodyConsts.h).
	 */
	void loadAllAssets();

	/**
	 * @brief Load the map file corresponding to each skeleton, and assign it.
	 */
	void SmartBodyManager::mapAllSkeletons();

};

}

#endif