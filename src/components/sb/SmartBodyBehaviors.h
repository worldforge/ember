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

}

namespace Ember
{

class SmartBodyManager;

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
	SmartBodyBehaviors(std::string const &motionPath, std::string const &skeletonRef, SmartBodyManager& manager);

	/**
	 * @brief Dtor.
	 */
	virtual ~SmartBodyBehaviors(void);


	/**
	 * @brief Setup the behaviors and (if wanted) check before that all the needed assets have been loaded.
	 */
	virtual bool setup(bool check = false);

	/**
	 * @brief Retarget the behavior set from the skeleton reference to another character (setup must have been called first).
	 */
	void retarget(const std::string& skName);

	/**
	 * @brief Add the necessary contraints on the character.
	 */
	virtual void addConstraints(SmartBody::SBCharacter& character) = 0;


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
	 * @brief Set up the different motions corresponding to the behavior set.
	 */
	void setupBehaviors(void);

	virtual void setupMotion(SmartBody::SBMotion& motion) = 0;

	virtual std::vector<std::string> getMotions(void) = 0;

	/**
	 * @brief Checks that each asset has been correctly loaded (skeleton + motions).
	 */
	bool assetsExist(void);
};

}

#endif
