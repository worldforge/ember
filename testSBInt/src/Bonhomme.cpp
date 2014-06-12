#include "Bonhomme.h"
#include "App.h"
#include "Sb.h"

/* TODO

 <(*w*)> add the different behaviors for my bonhomme
 <(*w*)> pray a lot and test, and debug, because I have no faith, and praying won't be efficient
 <($_$)> test updateBonePositions:
 			+ check that the joint positions are relative to their initial state.

*/


/* This skeleton is used for each instance of Bonhomme */
static bool firstCall(true);



Bonhomme::Bonhomme(Ogre::String const &name)
{
	Ogre::SceneManager *sceneManager = App::getSceneManagerSingleton();

	if (sceneManager->hasEntity(name))
	{
		mEntity = sceneManager->getEntity(name);
		mCharacter = App::getSbManagerSingleton()->getScene()->getCharacter(name);
	}

	else
	{
		/* Create Ogre entity. */
		mEntity = App::getSceneManagerSingleton()->createEntity(name, BONHOMME_MESH_PATH);

		/* Create SB character. */
		mCharacter = SbManager::getSbScene()->createCharacter(name, "Bonhomme");
		SmartBody::SBSkeleton *skeleton = getSbSkeleton();
		mCharacter->setSkeleton(skeleton);

		/* Set manually controled mode on the skeleton */
		setManualControl();
	}
}

Bonhomme::~Bonhomme(void)
{
}

/* Return the corresponding SmartBody skeleton. */
SmartBody::SBSkeleton* Bonhomme::getSbSkeleton()
{
	/* If this is the first time, then the skeleton has to be maped. */
	if (firstCall)
	{
		/* We set the joint map for the skeleton. */
		mapSkeleton();

		firstCall = false;
	}

	/* We create a new instance of the skeleton. */
	SmartBody::SBSkeleton *skeleton = SbManager::getSbScene()->createSkeleton(BONHOMME_SB_SK_PATH);
	return skeleton;	
}

/* Map the joints of the given skeleton and return the map. */
void Bonhomme::mapSkeleton(void)
{
	SmartBody::SBJointMapManager *jointMapManager = SbManager::getSbScene()->getJointMapManager();

	/* We create the joint map. */
	SmartBody::SBJointMap *jointMap = jointMapManager->createJointMap(BONHOMME_JOINTMAP_NAME);

#if SINBAD

	jointMap->setMapping("Root", "base");
	jointMap->setMapping("Neck", "spine4");
	jointMap->setMapping("Head", "spine5");
	jointMap->setMapping("Jaw", "skullbase");
	jointMap->setMapping("Clavicle.L", "l_sternoclavicular");
	jointMap->setMapping("Humerus.L", "l_shoulder");
	jointMap->setMapping("Ulna.L", "l_elbow");
	jointMap->setMapping("Hand.L", "l_wrist");
	jointMap->setMapping("ThumbProx.L", "l_thumb1");
	jointMap->setMapping("ThumbMed.L", "l_thumb2");
	jointMap->setMapping("ThumbDist.L", "l_thumb3");
	jointMap->setMapping("IndexFingerProx.L", "l_index1");
	jointMap->setMapping("IndexFingerMed.L", "l_index2");
	jointMap->setMapping("IndexFingerDist.L", "l_index3");
	jointMap->setMapping("MiddleFingerProx.L", "l_middle1");
	jointMap->setMapping("MiddleFingerMed.L", "l_middle2");
	jointMap->setMapping("MiddleFingerDist.L", "l_middle3");
	jointMap->setMapping("RingFingerProx.L", "l_ring1");
	jointMap->setMapping("RingFingerMed.L", "l_ring2");
	jointMap->setMapping("RingFingerDist.L", "l_ring3");
	jointMap->setMapping("PinkyProx.L", "l_pinky1");
	jointMap->setMapping("PinkyMed.L", "l_pinky2");
	jointMap->setMapping("PinkyDist.L", "l_pinky3");
	jointMap->setMapping("Clavicle.R", "r_sternoclavicular");
	jointMap->setMapping("Humerus.R", "r_shoulder");
	jointMap->setMapping("Ulna.R", "r_elbow");
	jointMap->setMapping("Hand.R", "r_wrist");
	jointMap->setMapping("ThumbProx.R", "r_thumb1");
	jointMap->setMapping("ThumbMed.R", "r_thumb2");
	jointMap->setMapping("ThumbDist.R", "r_thumb4");
	jointMap->setMapping("IndexFingerProx.R", "r_index1");
	jointMap->setMapping("IndexFingerMed.R", "r_index2");
	jointMap->setMapping("IndexFingerDist.R", "r_index4");
	jointMap->setMapping("MiddleFingerProx.R", "r_middle1");
	jointMap->setMapping("MiddleFingerMed.R", "r_middle2");
	jointMap->setMapping("MiddleFingerDist.R", "r_middle4");
	jointMap->setMapping("RingFingerProx.R", "r_ring1");
	jointMap->setMapping("RingFingerMed.R", "r_ring2");
	jointMap->setMapping("RingFingerDist.R", "r_ring4");
	jointMap->setMapping("PinkyProx.R", "r_pinky1");
	jointMap->setMapping("PinkyMed.R", "r_pinky2");
	jointMap->setMapping("PinkyDist.R", "r_pinky3");
	jointMap->setMapping("Thigh.L", "l_hip");
	jointMap->setMapping("Calf.L", "l_knee");
	jointMap->setMapping("Foot.L", "l_ankle");
	jointMap->setMapping("Toe.L", "l_forefoot");
	jointMap->setMapping("LeftToe_End", "l_toe");
	jointMap->setMapping("Thigh.R", "r_hip");
	jointMap->setMapping("Calf.R", "r_knee");
	jointMap->setMapping("Foot.R", "r_ankle");
	jointMap->setMapping("Toe.R", "r_forefoot");
	jointMap->setMapping("RightToe_End", "r_toe");
	jointMap->setMapping("Eye.L", "eyeball_left");
	jointMap->setMapping("Eye.R", "eyeball_right");
	jointMap->setMapping("Chest", "spine3");
	jointMap->setMapping("Stomach", "spine2");
	jointMap->setMapping("Waist", "spine1");

#else

	jointMap->setMapping("Hips", "base");
	jointMap->setMapping("LowerBack", "spine1");
	jointMap->setMapping("Spine", "spine2");
	jointMap->setMapping("Spine1", "spine3"); 
	jointMap->setMapping("Neck", "spine4");
	jointMap->setMapping("Head", "skullbase");
	jointMap->setMapping("FaceParent", "face_top_parent");
	jointMap->setMapping("LeftEye", "eyeball_left");
	jointMap->setMapping("RightEye", "eyeball_right");
	jointMap->setMapping("LeftShoulder", "l_acromioclavicular");
	jointMap->setMapping("LeftArm", "l_shoulder");
	jointMap->setMapping("LeftForeArm", "l_elbow");
	jointMap->setMapping("LeftHand", "l_wrist");
	jointMap->setMapping("RightShoulder", "r_acromioclavicular");
	jointMap->setMapping("RightArm", "r_shoulder");
	jointMap->setMapping("RightForeArm", "r_elbow");
	jointMap->setMapping("RightHand", "r_wrist");
	jointMap->setMapping("LeftUpLeg", "l_hip");
	jointMap->setMapping("LeftLeg", "l_knee");
	jointMap->setMapping("LeftFoot", "l_ankle");
	jointMap->setMapping("RightUpLeg", "r_hip");
	jointMap->setMapping("RightLeg", "r_knee");
	jointMap->setMapping("RightFoot", "r_ankle");

#endif
	
	/* We retrieve the skeleton. */
	SmartBody::SBSkeleton *skeleton = SbManager::getSbScene()->getAssetManager()->getSkeleton(BONHOMME_SB_SK_PATH);

	/* Application of the joint map to the skeleton. */
	jointMap->applySkeleton(skeleton);
}

/* Add the Bonhomme to the scene. */
void Bonhomme::addToScene(void)
{
	Ogre::SceneNode *node = App::getSceneManagerSingleton()->getRootSceneNode()->createChildSceneNode();
	node->attachObject(mEntity);
}

/* Start Bonhomme's animation. */
void Bonhomme::animate(Ogre::String const &animName, bool loop)
{
	Ogre::AnimationState *animState = mEntity->getAnimationState(animName);
	animState->setEnabled(true);
	animState->setLoop(loop);
}

/* Stop Bonhomme's animation. */
void Bonhomme::stop(Ogre::String const &animName)
{
	Ogre::AnimationState *animState = mEntity->getAnimationState(animName);
	animState->setEnabled(false);
	animState->setLoop(false);
}

/* Has to be called each frame. */
void Bonhomme::updateAnimation(Ogre::String const &animName, Ogre::Real offset)
{
	Ogre::AnimationState *animState = mEntity->getAnimationState(animName);
	animState->addTime(offset);
}

/* Set manual control mode to the ogre skeleton. */
void Bonhomme::setManualControl(void)
{
	Ogre::Skeleton* skeleton = mEntity->getSkeleton();
	Ogre::Skeleton::BoneIterator i = skeleton->getBoneIterator(); 

	while (i.hasMoreElements()) 
	{ 
		i.getNext()->setManuallyControlled(true);
	}
}

/* Must be called each frame in order to set the bone positions of Ogre skeleton to the coordinates of the joints in SB skeleton. */
void Bonhomme::updateBonePositions(void)
{
	/* We get the skeleton instance of the Ogre entity. */
	Ogre::SkeletonInstance *ogreSk = mEntity->getSkeleton();
	
	/* We get SmartBody skeleton. */ 
	SmartBody::SBSkeleton *sbSk = mCharacter->getSkeleton();
	
	/* Each bone is set to the same position than its reference in SmartBody. */
	int numJoints = sbSk->getNumJoints();
	for (int j = 0; j < numJoints; j++)
	{
		/* We get the joint by its index. */
		SmartBody::SBJoint *joint = sbSk->getJoint(j);
		SrQuat orientation = joint->quat()->value();

		/* The position vector are relative to the original position. */
		Ogre::Vector3 position(joint->getPosition().x, joint->getPosition().y, joint->getPosition().z);
		Ogre::Quaternion quaternion(orientation.w, orientation.x, orientation.y, orientation.z);

		/* We get the corresponding bone by the name of the joint (which is the one in Ogre, due to our jointMap). */
		Ogre::Bone* bone = ogreSk->getBone(joint->getName());
		bone->setPosition(bone->getInitialPosition() + position);
		bone->setOrientation(quaternion);
	}
}