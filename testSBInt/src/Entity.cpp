#include "Entity.h"
#include "App.h"
#include <iostream>


/* Reassign a skeleton for an entity (when the path indicated in the mesh is not the good one for example). */
Ogre::Entity* Entity::createEntityWithSkeleton(Ogre::String const &name, Ogre::String const &meshFile, Ogre::String const &skeletonFile)
{
	/* Loading of the mesh. */
	Ogre::MeshManager *meshMgr = App::getRootSingleton()->getMeshManager();
	Ogre::MeshPtr mesh = meshMgr->load(meshFile, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	/* Loading and assignment of the skeleton. */
	mesh->setSkeletonName(skeletonFile);

	/* Creation of the associated entity. */
	Ogre::Entity *ent = App::getSceneManagerSingleton()->createEntity(name, mesh);
	return ent;
}

/* Display the names of the animations linked to the given entity. */
void Entity::printEntityAnimations(Ogre::String const &name)
{
	/* We get the entity's skeleton. */
	Ogre::SkeletonInstance *skeleton = App::getSceneManagerSingleton()->getEntity(name)->getSkeleton();

	/* Number of animations. */
	int nbAnim = skeleton->getNumAnimations();

	for (int i = 0; i < nbAnim; i ++)
	{
		/* We get the animation by its index, and display its name. */
		Ogre::Animation *anim = skeleton->getAnimation((unsigned short)i);
		std::cout << "Animation n°" << i << " : " << anim->getName() << std::endl;
	}
}

/* Display the names of the entity's bones */
void Entity::printSkeletonBonesName(Ogre::String const &name)
{
	/* We get the entity's skeleton. */
	Ogre::SkeletonInstance *skeleton = App::getSceneManagerSingleton()->getEntity(name)->getSkeleton();

	/* We print each bone name one by one */ 
	Ogre::Skeleton::BoneIterator i = skeleton->getBoneIterator(); 
	while (i.hasMoreElements()) 
	{ 
		std::cout << i->getNext()->getName() << std::endl;
	}
}