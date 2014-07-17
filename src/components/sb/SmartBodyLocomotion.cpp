#include "SmartBodyLocomotion.h"

#include "sb/SBCharacter.h"
#include "sb/SBSkeleton.h"
#include "sb/SBMotion.h"
#include "sb/SBJoint.h"
#include "sb/SBAssetManager.h"
#include "sb/SBAnimationStateManager.h"
#include "sb/SBAnimationState.h"
#include "sb/SBAnimationTransition.h"


using namespace Ember;
namespace Ember
{

SmartBodyLocomotion::SmartBodyLocomotion(const std::string& motionPath, const std::string& skeletonRef, SmartBody::SBAssetManager& assetMng,
		SmartBody::SBAnimationBlendManager& blendMng, SmartBody::SBRetargetManager& retargetMng)
	: SmartBodyBehaviors::SmartBodyBehaviors(motionPath, skeletonRef, assetMng, blendMng, retargetMng)
{
}

SmartBodyLocomotion::~SmartBodyLocomotion()
{

}


bool SmartBodyLocomotion::setup(SmartBody::SBJointMapManager& jointMapManager)
{
	if (mSetup)
	{
		return true;
	}
	
	if (SmartBodyBehaviors::setupAssets(jointMapManager))
	{
		SmartBodyBehaviors::setupBehaviors();

		locomotionSetup();
		startingSetup();
		idleTurnSetup();
		stepSetup();
		transitionSetup();

		mSetup = true;
	}

	return mSetup;
}

void SmartBodyLocomotion::applyRetargeting(SmartBody::SBCharacter& character)
{
	character.addJointTrajectoryConstraint("l_forefoot", "base");
	character.addJointTrajectoryConstraint("r_forefoot", "base");
	character.addJointTrajectoryConstraint("l_ankle", "base");
	character.addJointTrajectoryConstraint("r_ankle", "base");
}



void SmartBodyLocomotion::setupMotion(SmartBody::SBMotion& motion)
{
	motion.buildJointTrajectory("l_forefoot", "base");
	motion.buildJointTrajectory("r_forefoot", "base");
	motion.buildJointTrajectory("l_ankle", "base");
	motion.buildJointTrajectory("r_ankle", "base");
}


std::vector<std::string> SmartBodyLocomotion::getMotions()
{
	std::vector<std::string> motions;

	motions.push_back("ChrUtah_Walk001");
	motions.push_back("ChrUtah_Idle001");
	motions.push_back("ChrUtah_Idle01_ToWalk01_Turn360Lf01");
	motions.push_back("ChrUtah_Idle01_ToWalk01_Turn360Rt01");
	motions.push_back("ChrUtah_Meander01");
	motions.push_back("ChrUtah_Shuffle01");
	motions.push_back("ChrUtah_Jog001");
	motions.push_back("ChrUtah_Run001");
	motions.push_back("ChrUtah_WalkInCircleLeft001");
	motions.push_back("ChrUtah_WalkInCircleRight001");
	motions.push_back("ChrUtah_WalkInTightCircleLeft001");
	motions.push_back("ChrUtah_WalkInTightCircleRight001");
	motions.push_back("ChrUtah_RunInCircleLeft001");
	motions.push_back("ChrUtah_RunInCircleRight001");
	motions.push_back("ChrUtah_RunInTightCircleLeft01");
	motions.push_back("ChrUtah_RunInTightCircleRight01");
	motions.push_back("ChrUtah_StrafeSlowRt01");
	motions.push_back("ChrUtah_StrafeSlowLf01");
	motions.push_back("ChrUtah_StrafeFastRt01");
	motions.push_back("ChrUtah_StrafeFastLf01");
	motions.push_back("ChrUtah_Turn90Lf01");
	motions.push_back("ChrUtah_Turn180Lf01");
	motions.push_back("ChrUtah_Turn90Rt01");
	motions.push_back("ChrUtah_Turn180Rt01");
	motions.push_back("ChrUtah_StopToWalkRt01");
	motions.push_back("ChrUtah_Idle01_ToWalk01_Turn90Lf01");
	motions.push_back("ChrUtah_Idle01_ToWalk01_Turn180Lf01");
	motions.push_back("ChrUtah_Idle01_StepBackwardRt01");
	motions.push_back("ChrUtah_Idle01_StepForwardRt01");
	motions.push_back("ChrUtah_Idle01_StepSidewaysRt01");
	motions.push_back("ChrUtah_Idle01_StepBackwardLf01");
	motions.push_back("ChrUtah_Idle01_StepForwardLf01");
	motions.push_back("ChrUtah_Idle01_StepSidewaysLf01");

	return motions;
}

std::vector<std::string> SmartBodyLocomotion::getMotionsForLocomotionSetup()
{	
	std::vector<std::string> motions;

	motions.push_back("ChrUtah_Walk001");
	motions.push_back("ChrUtah_Idle001");
	motions.push_back("ChrUtah_Idle01_ToWalk01_Turn360Lf01");
	motions.push_back("ChrUtah_Idle01_ToWalk01_Turn360Rt01");
	motions.push_back("ChrUtah_Meander01");
	motions.push_back("ChrUtah_Shuffle01");
	motions.push_back("ChrUtah_Jog001");
	motions.push_back("ChrUtah_Run001");
	motions.push_back("ChrUtah_WalkInCircleLeft001");
	motions.push_back("ChrUtah_WalkInCircleRight001");
	motions.push_back("ChrUtah_WalkInTightCircleLeft001");
	motions.push_back("ChrUtah_WalkInTightCircleRight001");
	motions.push_back("ChrUtah_RunInCircleLeft001");
	motions.push_back("ChrUtah_RunInCircleRight001");
	motions.push_back("ChrUtah_RunInTightCircleLeft01");
	motions.push_back("ChrUtah_RunInTightCircleRight01");
	motions.push_back("ChrUtah_StrafeSlowRt01");
	motions.push_back("ChrUtah_StrafeSlowLf01");
	motions.push_back("ChrUtah_StrafeFastRt01");
	motions.push_back("ChrUtah_StrafeFastLf01");

	return motions;
}


void SmartBodyLocomotion::locomotionSetup()
{	
	//If the blend is already existing, we don't need to do anything. 
	if (mBlendManager.getBlend("allLocomotion") != NULL)
	{
		return;
	}

	//Creates a new stateManager refering to the original skeleton.
	SmartBody::SBAnimationBlend3D *blend = mBlendManager.createBlend3D("allLocomotion");
	blend->setBlendSkeleton(mSkelRefName);

	//We get the original set of motions.
	std::vector<std::string> motions = getMotionsForLocomotionSetup();
	size_t size = motions.size();

	//We add these motions to our blend.
	for (auto& motionName : motions)
	{
		blend->addMotion(motionName, 0, 0, 0);
	}

	//We set the correspondence points.
	float correspondencePoints[] = {
			0, 0.811475, 1.34262, 2.13333, 
			0, 0.698851, 1.37931, 2.13333, 
			0, 0.82023, 1.30207, 2.12966,
			0, 0.812874, 1.35356, 2.12966,
			0, 0.988525, 1.87377, 3.06667,
			0, 0.713115, 1.29836, 2.13333,
			0, 0.501639, 0.92459, 1.6,
			0, 0.422951, 0.772131, 1.33333, 
			0, 0.840984, 1.39672, 2.13333, 
			0, 0.840984, 1.29836, 2.13333,
			0, 0.845902, 1.30328, 2.13333,
			0, 0.880328, 1.33279, 2.13333,
			0, 0.437705, 0.811475, 1.33333,
			0, 0.452459, 0.791803, 1.33333,
			0, 0.462295, 0.757377, 1.33333,
			0, 0.452459, 0.796721, 1.33333,
			0, 0.90000, 1.41000, 2.13000,
			0.72, 1.38, 1.92, 0.72,
			0, 0.473684, 0.920079, 1.6,
			0.4, 0.893233, 1.28421, 0.4
		};

	//We add the correspondence points to the blend.
	for (size_t i = 0; i < 4; i ++)
	{
		std::vector<double> points;

		for (size_t j = 0; j < size; j ++)
		{
			points.push_back(correspondencePoints[j * 4 + i]);
		}

		blend->addCorrespondencePoints(motions, points);
	}

	//We then set the correct parameters for each motion in the blend. 
	SmartBody::SBSkeleton *skeleton = mAssetManager.getSkeleton(mSkelRefName);
	SmartBody::SBJoint *joint = skeleton->getJointByName("base");

	float directions[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 90, -90, 90, -90};
	SmartBody::SBMotion *motion;
	std::vector<double> points;
	float speed, omega, direction;

	for (size_t i = 0; i < size; i ++)
	{
		motion = mAssetManager.getMotion(motions[i]);
		motion->connect(skeleton);

		points = blend->getCorrespondencePoints(i);
		speed = motion->getJointSpeed(joint, points[0], points[3]);
		omega = motion->getJointAngularSpeed(joint, points[0], points[3]);
		direction = directions[i];

		blend->setParameter(motions[i], speed, omega, direction);

		motion->disconnect();
	}

	//Addition of the different tetrahedrons.
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_Meander01", "ChrUtah_WalkInTightCircleLeft001", "ChrUtah_StrafeSlowRt01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_Meander01", "ChrUtah_WalkInTightCircleLeft001", "ChrUtah_StrafeSlowLf01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_Meander01", "ChrUtah_WalkInTightCircleRight001", "ChrUtah_StrafeSlowRt01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_Meander01", "ChrUtah_WalkInTightCircleRight001", "ChrUtah_StrafeSlowLf01");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_Walk001", "ChrUtah_Jog001", "ChrUtah_WalkInCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_Walk001", "ChrUtah_Jog001", "ChrUtah_WalkInCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_Jog001", "ChrUtah_Run001", "ChrUtah_RunInCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_Jog001", "ChrUtah_Run001", "ChrUtah_RunInCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_WalkInCircleLeft001", "ChrUtah_RunInCircleLeft001", "ChrUtah_Jog001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_WalkInCircleRight001", "ChrUtah_RunInCircleRight001", "ChrUtah_Jog001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_WalkInCircleLeft001", "ChrUtah_WalkInTightCircleLeft001", "ChrUtah_RunInCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_WalkInCircleRight001", "ChrUtah_WalkInTightCircleRight001", "ChrUtah_RunInCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_WalkInTightCircleLeft001", "ChrUtah_RunInCircleLeft001", "ChrUtah_RunInTightCircleLeft01");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_WalkInTightCircleRight001", "ChrUtah_RunInCircleRight001", "ChrUtah_RunInTightCircleRight01");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_Meander01", "ChrUtah_Walk001", "ChrUtah_WalkInCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_Meander01", "ChrUtah_WalkInCircleLeft001", "ChrUtah_WalkInTightCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_Meander01", "ChrUtah_Walk001", "ChrUtah_WalkInCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_Meander01", "ChrUtah_WalkInCircleRight001", "ChrUtah_WalkInTightCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_Walk001", "ChrUtah_Jog001", "ChrUtah_WalkInCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_Walk001", "ChrUtah_Jog001", "ChrUtah_WalkInCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_Jog001", "ChrUtah_Run001", "ChrUtah_RunInCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_Jog001", "ChrUtah_Run001", "ChrUtah_RunInCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_WalkInCircleLeft001", "ChrUtah_RunInCircleLeft001", "ChrUtah_Jog001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_WalkInCircleRight001", "ChrUtah_RunInCircleRight001", "ChrUtah_Jog001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_WalkInCircleLeft001", "ChrUtah_WalkInTightCircleLeft001", "ChrUtah_RunInCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_WalkInCircleRight001", "ChrUtah_WalkInTightCircleRight001", "ChrUtah_RunInCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_WalkInTightCircleLeft001", "ChrUtah_RunInCircleLeft001", "ChrUtah_RunInTightCircleLeft01");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_WalkInTightCircleRight001", "ChrUtah_RunInCircleRight001", "ChrUtah_RunInTightCircleRight01");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_Meander01", "ChrUtah_Walk001", "ChrUtah_WalkInCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_Meander01", "ChrUtah_WalkInCircleLeft001", "ChrUtah_WalkInTightCircleLeft001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_Meander01", "ChrUtah_Walk001", "ChrUtah_WalkInCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_Meander01", "ChrUtah_WalkInCircleRight001", "ChrUtah_WalkInTightCircleRight001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_StrafeFastRt01", "ChrUtah_RunInCircleRight001", "ChrUtah_RunInTightCircleRight01");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_StrafeFastRt01", "ChrUtah_RunInCircleRight001", "ChrUtah_Run001");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_StrafeFastRt01", "ChrUtah_RunInCircleLeft001", "ChrUtah_RunInTightCircleLeft01");
	blend->addTetrahedron("ChrUtah_StrafeSlowRt01", "ChrUtah_StrafeFastRt01", "ChrUtah_RunInCircleLeft001", "ChrUtah_Run001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_StrafeFastLf01", "ChrUtah_RunInCircleRight001", "ChrUtah_RunInTightCircleRight01");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_StrafeFastLf01", "ChrUtah_RunInCircleRight001", "ChrUtah_Run001");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_StrafeFastLf01", "ChrUtah_RunInCircleLeft001", "ChrUtah_RunInTightCircleLeft01");
	blend->addTetrahedron("ChrUtah_StrafeSlowLf01", "ChrUtah_StrafeFastLf01", "ChrUtah_RunInCircleLeft001", "ChrUtah_Run001");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_StrafeSlowLf01", "ChrUtah_WalkInTightCircleLeft001", "ChrUtah_Idle01_ToWalk01_Turn360Lf01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_StrafeSlowLf01", "ChrUtah_WalkInTightCircleRight001", "ChrUtah_Idle01_ToWalk01_Turn360Rt01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_StrafeSlowLf01", "ChrUtah_Idle001", "ChrUtah_Idle01_ToWalk01_Turn360Lf01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_StrafeSlowLf01", "ChrUtah_Idle001", "ChrUtah_Idle01_ToWalk01_Turn360Rt01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_StrafeSlowRt01", "ChrUtah_WalkInTightCircleLeft001", "ChrUtah_Idle01_ToWalk01_Turn360Lf01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_StrafeSlowRt01", "ChrUtah_WalkInTightCircleRight001", "ChrUtah_Idle01_ToWalk01_Turn360Rt01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_StrafeSlowRt01", "ChrUtah_Idle001", "ChrUtah_Idle01_ToWalk01_Turn360Lf01");
	blend->addTetrahedron("ChrUtah_Shuffle01", "ChrUtah_StrafeSlowRt01", "ChrUtah_Idle001", "ChrUtah_Idle01_ToWalk01_Turn360Rt01");
}

void SmartBodyLocomotion::startingSetup()
{
	//Generates the mirror animations for StopToWalk, Idle_ToWalk_Turn90 and Idle_ToWalk_Turn 180.
	SmartBody::SBMotion *mirrorMotion1 = mAssetManager.getMotion("ChrUtah_StopToWalkRt01");
	mirrorMotion1->mirror("ChrUtah_StopToWalkLf01", mSkelRefName);

	SmartBody::SBMotion *mirrorMotion2 = mAssetManager.getMotion("ChrUtah_Idle01_ToWalk01_Turn90Lf01");
	mirrorMotion2->mirror("ChrUtah_Idle01_ToWalk01_Turn90Rt01", mSkelRefName);

	SmartBody::SBMotion *mirrorMotion3 = mAssetManager.getMotion("ChrUtah_Idle01_ToWalk01_Turn180Lf01");
	mirrorMotion2->mirror("ChrUtah_Idle01_ToWalk01_Turn180Rt01", mSkelRefName);

	//Gets the original set of motions.
	std::vector< std::vector<std::string> > motions;
		std::vector< std::string> m1, m2;
			m1.push_back("ChrUtah_StopToWalkLf01");
			m1.push_back("ChrUtah_Idle01_ToWalk01_Turn90Lf01");
			m1.push_back("ChrUtah_Idle01_ToWalk01_Turn180Lf01");
			m2.push_back("ChrUtah_StopToWalkRt01");
			m2.push_back("ChrUtah_Idle01_ToWalk01_Turn90Rt01");
			m2.push_back("ChrUtah_Idle01_ToWalk01_Turn180Rt01");
		motions.push_back(m1);
		motions.push_back(m2);

	//For both left and right, we set the correspondence points.
	std::string blendNames[2] = {"allStartingLeft", "allStartingRight"};
	double params[2][3] = { {0, -90, -180}, {0, 90, 180} };
	std::vector<double> points[3];
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[1].push_back(0.57541);
		points[1].push_back(1.2);
		points[1].push_back(1.35);
		points[2].push_back(0.943716);
		points[2].push_back(1.41);
		points[2].push_back(1.6);

	//Sets the blend manager for each side.
	for (size_t i = 0; i < 2; i ++)
	{
		if (mBlendManager.getBlend(blendNames[i]) != 0)
		{
			continue;
		}

		SmartBody::SBAnimationBlend1D *blend = mBlendManager.createBlend1D(blendNames[i]);
		blend->setBlendSkeleton(mSkelRefName);

		for (size_t j = 0; j < 3; j ++)
		{
			blend->addMotion(motions[i][j], params[i][j]);
		}

		for (size_t j = 0; j < 3; j ++)
		{
			blend->addCorrespondencePoints(motions[i], points[j]);
		}
	}
}

void SmartBodyLocomotion::idleTurnSetup()
{	
	if (mBlendManager.getBlend("allIdleTurn") != 0)
	{
		return;
	}

	SmartBody::SBAnimationBlend1D *blend = mBlendManager.createBlend1D("allIdleTurn");

	std::vector<std::string> motions;
	motions.push_back("ChrUtah_Idle001");
	motions.push_back("ChrUtah_Turn90Lf01");
	motions.push_back("ChrUtah_Turn180Lf01");
	motions.push_back("ChrUtah_Turn90Rt01");
	motions.push_back("ChrUtah_Turn180Rt01");
		
	double params[] = {0, -90, -180, 90, 180};

	for (int i = 0; i < 5; i ++)
	{
		blend->addMotion(motions[i], params[i]);
	}

	std::vector<double> points[3];
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[1].push_back(0.255738);
		points[1].push_back(0.762295);
		points[1].push_back(0.87541);
		points[1].push_back(0.757377);
		points[1].push_back(0.821311);
		points[2].push_back(0.633333);
		points[2].push_back(1.96667);
		points[2].push_back(2.46667);
		points[2].push_back(1.96667);
		points[2].push_back(2.46667);

	for (int i = 0; i < 3; i ++)
	{
		blend->addCorrespondencePoints(motions, points[i]);
	}
}

void SmartBodyLocomotion::stepSetup()
{
	if (mBlendManager.getBlend("allStep") != 0)
	{
		return;
	}

	SmartBody::SBAnimationBlend2D *blend = mBlendManager.createBlend2D("allStep");
	blend->setBlendSkeleton(mSkelRefName);

	std::vector<std::string> motions;
	motions.push_back("ChrUtah_Idle001");
	motions.push_back("ChrUtah_Idle01_StepBackwardRt01");
	motions.push_back("ChrUtah_Idle01_StepForwardRt01");
	motions.push_back("ChrUtah_Idle01_StepSidewaysRt01");
	motions.push_back("ChrUtah_Idle01_StepBackwardLf01");
	motions.push_back("ChrUtah_Idle01_StepForwardLf01");
	motions.push_back("ChrUtah_Idle01_StepSidewaysLf01");
	
	for (auto& motionName : motions)
	{
		blend->addMotion(motionName, 0, 0);
	}

	std::vector<double> points[3];
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[0].push_back(0);
		points[1].push_back(0.556322);
		points[1].push_back(0.556322);
		points[1].push_back(0.543678);
		points[1].push_back(0.482989);
		points[1].push_back(0.395402);
		points[1].push_back(0.531034);
		points[1].push_back(0.473563);
		points[2].push_back(1.46414);
		points[2].push_back(1.46414);
		points[2].push_back(1.46414);
		points[2].push_back(1.46414);
		points[2].push_back(1.33333);
		points[2].push_back(1.33333);
		points[2].push_back(1.33103);

	for (int i = 0; i < 3; i ++)
	{
		blend->addCorrespondencePoints(motions, points[i]);
	}

	SmartBody::SBSkeleton *skeleton = mAssetManager.getSkeleton(mSkelRefName);
	SmartBody::SBJoint *joint = skeleton->getJointByName("base");

	SmartBody::SBMotion *motion;
	std::vector<double> pts;
	std::vector<float> transition;

	for (int i = 0; i < 7; i ++)
	{
		motion = mAssetManager.getMotion(motions[i]);
		motion->connect(skeleton);

		pts = blend->getCorrespondencePoints(i);
		transition = motion->getJointTransition(joint, pts[0], pts[2]);

		blend->setParameter(motions[i], transition[0], transition[2]);

		motion->disconnect();
	}

	blend->addTriangle("ChrUtah_Idle001", "ChrUtah_Idle01_StepBackwardLf01", "ChrUtah_Idle01_StepSidewaysLf01");
	blend->addTriangle("ChrUtah_Idle001", "ChrUtah_Idle01_StepForwardLf01", "ChrUtah_Idle01_StepSidewaysLf01");
	blend->addTriangle("ChrUtah_Idle001", "ChrUtah_Idle01_StepBackwardRt01", "ChrUtah_Idle01_StepSidewaysRt01");
	blend->addTriangle("ChrUtah_Idle001", "ChrUtah_Idle01_StepForwardRt01", "ChrUtah_Idle01_StepSidewaysRt01");
}

void SmartBodyLocomotion::transitionSetup()
{	
	if (mBlendManager.getTransition("allStartingLeft", "allLocomotion") == 0)
	{
		SmartBody::SBAnimationTransition *transition = mBlendManager.createTransition("allStartingLeft", "allLocomotion");
		transition->setEaseInInterval("ChrUtah_Meander01", 0.4, 0.78);
		transition->addEaseOutInterval("ChrUtah_StopToWalkLf01", 0.54, 0.83);
	}

	if (mBlendManager.getTransition("allStartingRight", "allLocomotion") == 0)
	{
		SmartBody::SBAnimationTransition *transition = mBlendManager.createTransition("allStartingRight", "allLocomotion");
		transition->setEaseInInterval("ChrUtah_Meander01", 1.1, 1.5);
		transition->addEaseOutInterval("ChrUtah_StopToWalkRt01", 0.54, 0.83);
	}
}

}