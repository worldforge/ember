#ifndef SMARTBODYBEHAVIORS_H
#define SMARTBODYBEHAVIORS_H

	#include <string>
	#include <vector>


namespace SmartBody
{

class SBCharacter;
class SBAssetManager;
class SBAnimationBlendManager;
class SBRetargetManager;
class SBMotion;
class SBJointMapManager;

}

namespace Ember
{

/**
 @brief A behavior set is a group of motions.

 To load a behavior set, you have to create a SmartBodyBehaviors object, then use setup(). In order to use these animations on another character.
 use retarget().
 
 This class is abstract. Right now, only SmartBodyLocomotion derived from it. 

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodyBehaviors
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyBehaviors(const std::string& motionPath, const std::string& skeletonRef, SmartBody::SBAssetManager& assetMng,
		SmartBody::SBAnimationBlendManager& blendMng, SmartBody::SBRetargetManager& retargetMng);

	/**
	 * @brief Dtor.
	 */
	virtual ~SmartBodyBehaviors();

	/**
	 * @brief Check that all assets have been correctly loaded and setup the behavior set.
	 */
	virtual bool setup(SmartBody::SBJointMapManager& jointMapManager) = 0;

	/**
	 * @brief Retarget the behavior set from the skeleton reference to another character (setup must have been called first).
	 */
	void retarget(const std::string& skName);

	/**
	 * @brief Add the necessary contraints on the character.
	 */
	virtual void applyRetargeting(SmartBody::SBCharacter& character) = 0;


protected:

	/**
	 * @brief Name of the skeleton this motion set is made for.
	 */
	std::string mSkelRefName;

	/**
	 * @brief Reference to the asset manager.
	 */
	SmartBody::SBAssetManager& mAssetManager;

	/**
	 * @brief Reference to the animation blend manager.
	 */
	SmartBody::SBAnimationBlendManager& mBlendManager;

	/**
	 * @brief Reference to the retarget manager.
	 */
	SmartBody::SBRetargetManager& mRetargetManager;

	/**
	 * @brief Stating that setup has been effectuated.
	 */
	bool mSetup;


	/**
	 * @brief Check that the assets have been correctly loaded and map the skeleton reference.
	 */
	bool setupAssets(SmartBody::SBJointMapManager& jointMapMng);

	/**
	 * @brief Setup the behaviors.
	 */
	void setupBehaviors();


	virtual void setupMotion(SmartBody::SBMotion& motion) = 0;

	virtual std::vector<std::string> getMotions() = 0;

	/**
	 * @brief Checks that each asset has been correctly loaded (skeleton + motions).
	 */
	bool assetsExist();
};

}

#endif
