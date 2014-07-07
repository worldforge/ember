#ifndef SMARTBODYREPRESENTATION_H
#define SMARTBODYREPRESENTATION_H

	#include <string>
	#include <vector>

namespace Ogre
{
class Entity;
class SkeletonInstance;
}

namespace SmartBody
{
class SBCharacter;
class SBSkeleton;
class SBScene;
}

namespace Ember
{

class SmartBodyBehaviors;

/**
 @brief This class gives an interface between SmartBody and Ogre respective representation for characters.

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodyRepresentation
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyRepresentation(SmartBody::SBScene& scene, const Ogre::Entity& entity, const std::string& group, const std::string& skName,
		const std::vector<SmartBodyBehaviors*>& behaviors);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyRepresentation(void);

	/**
	 * @brief Tranfers the position of the bones from SmartBody to Ogre.
	 */
	void updateBonePositions(void);

	/**
	 * @brief Sets or unsets manual control over the skeleton of the Ogre entity.
	 */
	void setManualControl(bool mode = true);

	/**
	 * @brief Returns the value of mManualMode.
	 */
	bool isManuallyControlled(void);


private:

	/**
	 * @brief Reference over the scene (to destruct the character).
	 */
	SmartBody::SBScene& mScene;

	/**
	 * @brief The SmartBody character.
	 */
	SmartBody::SBCharacter& mCharacter;

	/**
	 * @brief The SmartBody skeleton.
	 */
	SmartBody::SBSkeleton& mSkeleton;

	/**
	 * @brief The Ogre entity.
	 */
	const Ogre::Entity& mEntity;

	/**
	 * @brief The Ogre skeleton.
	 */
	Ogre::SkeletonInstance& mOgreSkeleton;

	/**
	 * @brief Indicates if the manual control is set or not.
	 */
	bool mManualMode;

};

}

#endif