#include "Bonhomme.h"
#include "Entity.h"
#include "App.h"

Bonhomme::Bonhomme(Ogre::String const &name)
{
	Ogre::SceneManager *sceneManager = App::getSceneManagerSingleton();

	if (sceneManager->hasEntity(name))
	{
		mEntity = sceneManager->getEntity(name);
	}

	else
	{
		mEntity = Entity::createEntityWithSkeleton(name, "male.mesh", "male.skeleton");
	}
}

Bonhomme::~Bonhomme()
{
}



void Bonhomme::addToScene()
{
	Ogre::SceneNode *node = App::getSceneManagerSingleton()->getRootSceneNode()->createChildSceneNode();
	mEntity->setMaterialName("male");
	node->attachObject(mEntity);
}

void Bonhomme::animate(Ogre::String const &animName, bool loop)
{
	Ogre::AnimationState *animState = mEntity->getAnimationState(animName);
	animState->setEnabled(true);
	animState->setLoop(loop);
}

void Bonhomme::stop(Ogre::String const &animName)
{
	Ogre::AnimationState *animState = mEntity->getAnimationState(animName);
	animState->setEnabled(false);
	animState->setLoop(false);
}

void Bonhomme::updateAnimation(Ogre::String const &animName, Ogre::Real offset)
{
	Ogre::AnimationState *animState = mEntity->getAnimationState(animName);
	animState->addTime(offset);
}