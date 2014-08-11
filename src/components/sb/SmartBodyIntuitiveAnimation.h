/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef SMARTBODYINTUITIVEANIMATION_H
#define SMARTBODYINTUITIVEANIMATION_H

#include "SmartBodyAnimation.h"

#include <string>
#include <OgreVector3.h>
#include <OgreQuaternion.h>


namespace Ember
{

/**
 * @brief This class is used instead of SmartBodyMovingAnimation and uses blends to replace the SmartBody motions.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyIntuitiveAnimation: public SmartBodyAnimation
{
public:

	/**
	 * @brief The names of the blends used for locomotion.
	 * @see SmartBodyLocomotion.h to get an explanation for each blend.
	 */
	enum class Blend
	{
		LOCOMOTION,  						//3D blend: the three parameters are speed, angular speed, angle between the forward axis and the displacement axis.
		STARTING_LEFT, STARTING_RIGHT, 		//1D blend: the parameter is the angle.
		IDLE_TURN,							//1D blend: the parameter is the angle.
		STEP,								//2D blend.

		BLENDS_COUNT, 	//The total number of blends (used to set the size of mBlends).
	};

	/**
	 * @brief Returns the dimension of the blend, and a negative value if blend is not a blend (BLENDS_COUNT for instance).
	 */
	static int getBlendDimension(Blend blend);

	/**
	 * @brief Checks that the parameters sent for this blend are valid: concretely, if the blend is a 2D blend, then parameters must contain two strings.
	 */
	static bool areParametersValid(Blend blend, const std::vector<std::string>& parameters);

	/**
	 * @brief Ctor.
	 */
	SmartBodyIntuitiveAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetMng, 
		const std::string& locomotionBlend, const std::string& startingLfBlend, const std::string& startingRtBlend, const std::string& idleTurnBlend, const std::string& stepBlend);

	/**
	 * @brief Copy ctor.
	 */
	SmartBodyIntuitiveAnimation(const SmartBodyIntuitiveAnimation& animation);

	/** 
	 * @brief Dtor.
	 */
	~SmartBodyIntuitiveAnimation();

	/**
	 * @brief Gets the bml request that is to be sent via the BMLProcessor.
	 * @return false if blend does not correspond to any blend, or if the number of parameter does not correspond to the active blend.
	 */
	bool getBmlRequest(std::string& request, Blend blend, const std::vector<std::string>& parameters) const;

	/**
	 * @brief Gets the bml request that is to be sent via the BMLProcessor.
	 * @return false if blend does not correspond to any blend, or if the number of parameter does not correspond to the active blend.
	 */
	bool getBmlRequest(std::string& request, int blendNumber, const std::vector<std::string>& parameters) const;


private:

	/**
	 * @brief The names of the different blends.
	 * Use mBlends[(int)blend] to access the corresponding blend.
	 */
	std::vector<std::string> mBlends;

};

/**
 * @brief This is an instance for SmartBodyIntuitiveAnimation. 
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyIntuitiveAnimationInstance: public SmartBodyAnimationInstance
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyIntuitiveAnimationInstance(const SmartBodyIntuitiveAnimation& animation, SmartBody::SBBmlProcessor& bmlProcessor, const std::string& character, SmartBodyIntuitiveAnimation::Blend blend);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyIntuitiveAnimationInstance();

	/**
	 * @brief Gets the request corresponding to mBlend.
	 */
	bool getBmlRequest(std::string& request) const;

	/**
	 * @brief Changes the blend.
	 */
	void setBlend(SmartBodyIntuitiveAnimation::Blend blend);

	/**
	 * @brief Changes the parameters of the blend.
	 * @param nbParameters: the number of parameters to specify (3 for a 3D blend, 2 for a 2D blend, etc.).
	 */
	void setBlendParameters(int nbParameters, float x = 0, float y = 0, float z = 0);

	/**
	 * @brief Gets the blend currently set on the character.
	 */
	SmartBodyIntuitiveAnimation::Blend getBlend() const;

	/**
	 * @brief Notifies that a bml request has been sent.
	 * This sets mPrvPosition/mPrvOrientation to mNewPosition/mNewOrientation values.
	 */
	void notifyUpdate();

	/**
	 * @brief Initializes mPrvPosition and mPrvOrientation with position and orientation vectors.
	 */
	void initializePositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation);

	/**
	 * @brief Sets mNewPosition and mNewOrientation to the given values.
	 * @param setInitial: If setInitial is true, then mPrvPosition and mPrvOrientation will be updated too.
	 *
	 * The position and orientation that the character has on the server are required to calculate the parameters of the blend.
	 */
	void setServerPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation);

	/**
	 * @brief Calculates the appropriate parameters for the blend (this uses the position/orientation of the scene node, the ones during the last frame, and the current ones).
	 */
	void calculateBlendParameters(float timeSlice);
	

private:

	/**
	 * @brief The blend used at the moment.
	 */
	SmartBodyIntuitiveAnimation::Blend mBlend;

	/**
	 * @brief The parameters for the blend. Their signification varies from a blend to another.
	 */
	float mX, mY, mZ;

	/**
	 * @brief The number of parameters that has to be used.
	 */
	int mNbParameters;

	/**
	 * @brief Calculates the parameters associated with the LOCOMOTION blend.
	 */
	void calculateSpeedAngularSpeedAndDirectionParameters(float timeSlice);

	/**
	 * @brief Calculates the angle, that is the parameter required for STARTING_LEFT, STARTING_RIGHT and IDLE_TURN blends.
	 */
	void calculateAngleParameter(float timeSlice);

	/**
	 * @brief Calculates the base joint transitions (?) to parameterize the STEP blend.
	 */
	void calculateTransitionParameter(float timeSlice);

	/**
	 * @brief The position and orientation (previous and current) of the character this animation is bound to.
	 * They are used to calculate the parameters required for the blend.
	 */
	Ogre::Vector3 mPrvPosition, mNewPosition;
	Ogre::Quaternion mPrvOrientation, mNewOrientation;
};

}

#endif