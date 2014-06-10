#include "Bonhomme.h"
#include "Entity.h"
#include "App.h"

/* TODO

 <(*w*)> document a lot
 <(*w*)> get the skeleton Dean prepared or make my own for Sb
 <(*w*)> do the same with Ogre
 <(*w*)> finish mapSkeleton 
 <(*w*)> document a lot
 <(*w*)> add the different behaviors for my bonhomme
 <(*w*)> document a lot
 <(*w*)> pray a lot and test, and debug, because I have no faith, and praying won't be efficient
 <(*w*)> document a lot
 <(*w*)> finish debuging

*/


/* If the skeleton used for Bonhomme has never been maped, we can see it with this variable */
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
		mEntity = Entity::createEntityWithSkeleton(name, "male.mesh", "male.skeleton");

		/* Create SB character. */
		mCharacter = App::getSbManager()->getScene()->createCharacter(name, "Bonhomme");
		SmartBody::SBSkeleton *skeleton = getSbSkeleton();
		mCharacter->setSkeleton(skeleton);
	}
}

Bonhomme::~Bonhomme()
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
	SmartBody::SBSkeleton *skeleton = App::getSbManagerSingleton()->getScene()->createSkeleton(BONHOMME_SK_PATH);
	return skeleton;	
}

/* Map the joints of the given skeleton and return the map. */
void Bonhomme::mapSkeleton()
{
	SmartBody::SBJointMapManager *jointMapManager = App::getSbManagerSingleton()->getScene()->getJointMapManager();

	/* We create the joint map. */
	SmartBody::SBJointMap *jointMap = jointMapManager->createJointMap(BONHOMME_JOINTMAP_NAME);
	/* For each bone : do 
		jointMap.setMapping(skName, sbName);
	*/


	/* We retrieve the skeleton. */
	SmartBody::SBSkeleton *skeleton = App::getSbManagerSingleton()->getScene()->getAssetManager()->getSkeleton(BONHOMME_SK_PATH);

	/* Application of the joint map to the skeleton. */
	jointMap->applySkeleton(skeleton);
}


/* Add the Bonhomme to the scene. */
void Bonhomme::addToScene()
{
	Ogre::SceneNode *node = App::getSceneManagerSingleton()->getRootSceneNode()->createChildSceneNode();
	mEntity->setMaterialName("male");
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