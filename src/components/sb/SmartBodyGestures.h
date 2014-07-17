#ifndef SMARTBODYGESTURES_H
#define SMARTBODYGESTURES_H

#include "SmartBodyBehaviors.h"


namespace SmartBody
{
class SBGestureMapManager;
}

namespace Ember
{

class SmartBodyGestures: public SmartBodyBehaviors
{

public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyGestures(const std::string& motionPath, const std::string& skeletonRef, 
		SmartBody::SBAssetManager& assetMng, SmartBody::SBAnimationBlendManager& blendMng, SmartBody::SBRetargetManager& retargetMng, 
		SmartBody::SBGestureMapManager& gestureMapMng);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyGestures();


	/**
	 * @brief Setup the gestures behavior set.
	 */
	bool setup(SmartBody::SBJointMapManager& jointMapManager);

	/**
	 * @brief Sets some of the character stats to retarget gestures him.
	 */
	void applyRetargeting(SmartBody::SBCharacter& character);


private:

	/**
	 * @brief The gesture map manager.
	 */
	SmartBody::SBGestureMapManager& mGestureMapMng;

	/**
	 * @brief Create the build joint trajectories for this motion.
	 */
	void setupMotion(SmartBody::SBMotion& motion);

	/**
	 * @brief Gets the name of all the motions contained in the gestures set.
	 */
	std::vector<std::string> getMotions();

	/**
	 * @brief Create the maps for the different gesture sets.
	 */
	void mapNeutralGestures();
	void mapAngryGestures();
	void mapSadGestures();
};

}

#endif //SMARTBODYGESTURES_H