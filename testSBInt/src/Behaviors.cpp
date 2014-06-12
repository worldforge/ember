#include "Behaviors.h"
#include "Sb.h"
#include "Retarget.h"

/* TODO 

<($_$)> setupBehavior : 
			+ encapsulate the specifical stuff for locomotion 
			+ search the purpose of these buildJointTrajectory
<($_$)> retarget : 
			+ finish to translate Python to C++
			+ search what are IK constraints

*/

static std::string locomotionMotionPath("./");


Behaviors::Behaviors(BehaviorType behaviors, std::string const &motionPath = "", std::string const &skeletonRef = "")
	: mType(behaviors), mSkelRefName(skeletonRef), mMotionPath(motionPath)
{
	/* Where to find the motion assets. */
	if (mMotionPath == "")
	{
		assignDefaultMotionPath();
	}

	/* Name of a skeleton which is bound to the behaviors. The user can specify it, namely if he changes the name of the file. */
	if (mSkelRefName == "")
	{
		assignSkeletonReference();
	}

	setupBehaviors();
}


/* Set up the different motions corresponding to the behavior set. */
void Behaviors::setupBehaviors(void)
{
	/* We get the motion paths for the behaviors. */
	std::vector<std::string> motions = getMotions();

	/* For each motion, we need to set the reference of the skeleton (and create the build joint trajectory). */ 
	int size = motions->size();
	SmartBody::SBScene *scene = SbManager::getSbScene();
	for (int i = 0; i < size; i ++)
	{
		SmartBody::SBMotion *motion = scene->getMotion(motions[i]);
		if (!motion)
		{
			scene->getAssetManager()->loadAsset(getMotionAsset(mType, motions[i]));
			motion = scene->getMotion(motions[i]);
		}

		if (motion)
		{
			motion->setMotionSkeletonName(mSkelRefName);

			// Specifical to locomotion => need to be encapsulate.
			motion->buildJointTrajectory("l_forefoot", "base");
			motion->buildJointTrajectory("r_forefoot", "base");
			motion->buildJointTrajectory("l_ankle", "base");
			motion->buildJointTrajectory("r_ankle", "base");
		}
	}		
}

/* Get the default name of the skeleton associated with the behavior set. */
void Behaviors::assignSkeletonReference(void)
{
	switch (mType)
	{
		case BEHAVIOR_LOCOMOTION:
			mSkelRefName = "Utah.sk";
			return;
	}
}

/* Get the names of the motions associated with the behavior set. */
std::vector<std::string> Behaviors::getMotions(void)
{
	switch (mType)
	{
		case BEHAVIOR_LOCOMOTION:
			return getLocomotionMotions();
	}
}

/* Get the names of the locomotion motions. */
std::vector<std::string> Behaviors::getLocomotionMotions(void)
{
	std::vector<std::string> motions;

	motions->push_back("ChrUtah_Walk001");
	motions->push_back("ChrUtah_Idle001");
	motions->push_back("ChrUtah_Idle01_ToWalk01_Turn360Lf01");
	motions->push_back("ChrUtah_Idle01_ToWalk01_Turn360Rt01");
	motions->push_back("ChrUtah_Meander01");
	motions->push_back("ChrUtah_Shuffle01");
	motions->push_back("ChrUtah_Jog001");
	motions->push_back("ChrUtah_Run001");
	motions->push_back("ChrUtah_WalkInCircleLeft001");
	motions->push_back("ChrUtah_WalkInCircleRight001");
	motions->push_back("ChrUtah_WalkInTightCircleLeft001");
	motions->push_back("ChrUtah_WalkInTightCircleRight001");
	motions->push_back("ChrUtah_RunInCircleLeft001");
	motions->push_back("ChrUtah_RunInCircleRight001");
	motions->push_back("ChrUtah_RunInTightCircleLeft01");
	motions->push_back("ChrUtah_RunInTightCircleRight01");
	motions->push_back("ChrUtah_StrafeSlowRt01");
	motions->push_back("ChrUtah_StrafeSlowLf01");
	motions->push_back("ChrUtah_StrafeFastRt01");
	motions->push_back("ChrUtah_StrafeFastLf01");
	motions->push_back("ChrUtah_Idle001");
	motions->push_back("ChrUtah_Turn90Lf01");
	motions->push_back("ChrUtah_Turn180Lf01");
	motions->push_back("ChrUtah_Turn90Rt01");
	motions->push_back("ChrUtah_Turn180Rt01");
	motions->push_back("ChrUtah_StopToWalkRt01");
	motions->push_back("ChrUtah_Idle01_ToWalk01_Turn90Lf01");
	motions->push_back("ChrUtah_Idle01_ToWalk01_Turn180Lf01");
	motions->push_back("ChrUtah_Idle01_StepBackwardRt01");
	motions->push_back("ChrUtah_Idle01_StepForwardRt01");
	motions->push_back("ChrUtah_Idle01_StepSidewaysRt01");
	motions->push_back("ChrUtah_Idle01_StepBackwardLf01");
	motions->push_back("ChrUtah_Idle01_StepForwardLf01");
	motions->push_back("ChrUtah_Idle01_StepSidewaysLf01");

	return motions;
}

/* Get the name of the file containing the motion code. */
std::string Behaviors::getMotionAsset(std::string motionName)
{
	return mMotionPath + motionName + ".skm";
}



/* Get the default path containing the motions associated with the behavior set. */
void Behaviors::assignDefaultMotionPath(void)
{
	switch (mType)
	{
		case BEHAVIOR_LOCOMOTION:
			mMotionPath = locomotionMotionPath;
			return;
	}
}

/* Static method which let the user change the default paths for each behavior set. */
void Behaviors::setDefaultMotionPath(BehaviorType behaviors, std::string const &path)
{
	std::string newPath;

	if (path == "")
	{
		newPath = ("./");
	}

	else if (path.back() != '/')
	{
		newPath = path + "/";
	}

	else
	{
		newPath = path;
	}

	switch (behaviors)
	{
		case BEHAVIOR_LOCOMOTION:
			locomotionMotionPath = newPath;
			return;
	}
}



/* Retarget the behavior set from the skeleton reference to another character. */
void Behaviors::retarget(std::string characterName)
{
	/* We get the corresponding character and the name of its skeleton. */
	SmartBody::SBCharacter *character = SbManager::getSbScene()->getCharacter(characterName);
	if (!character)
		return;

	std::string skelName = character->getSkeleton()->getName();

	Retarget retarget(mSkelName, skelName);

	#scene.run("stateMaleLocomotion.py");
	locomotionSetup('test_utah.sk', 'test_utah.sk', "base", '', 'all')


	scene.run("stateMaleStarting.py")
	startingSetup('test_utah.sk', 'test_utah.sk', "base", '', 'all')


	scene.run("stateMaleIdleTurn.py")
	idleTurnSetup('test_utah.sk', 'test_utah.sk', "base", '', 'all')


	scene.run("transitions.py")
	transitionSetup('', 'all')

	sbChar.addJointTrajectoryConstraint('l_forefoot','base')
	sbChar.addJointTrajectoryConstraint('r_forefoot','base')
	sbChar.addJointTrajectoryConstraint('l_ankle','base')
	sbChar.addJointTrajectoryConstraint('r_ankle','base')	
}

