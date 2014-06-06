#include "Entity.h"
#include "App.h"
#include <iostream>


Ogre::Entity* Entity::createEntityWithSkeleton(Ogre::String const& name, Ogre::String const& meshFile, Ogre::String const& skeletonFile)
{
	/* Chargement du mesh */
	Ogre::MeshManager *meshMgr = App::getRootSingleton()->getMeshManager();
	Ogre::MeshPtr mesh = meshMgr->load(meshFile, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	/* Chargement et assignement du squelette */
	mesh->setSkeletonName(skeletonFile);

	/* Création de l'entité associée */
	Ogre::Entity *ent = App::getSceneManagerSingleton()->createEntity(name, mesh);
	return ent;
}

void Entity::printEntityAnimations(Ogre::String const& name)
{
	/* On récupère l'entité et son squelette */
	Ogre::SkeletonInstance *skeleton = App::getSceneManagerSingleton()->getEntity(name)->getSkeleton();

	/* Nombre d'animations */
	int nbAnim = skeleton->getNumAnimations();

	for (int i = 0; i < nbAnim; i ++)
	{
		/* Récupération de l'animation et affichage de son nom */
		Ogre::Animation *anim = skeleton->getAnimation((unsigned short)i);
		std::cout << "Animation n°" << i << " : " << anim->getName() << std::endl;
	}
}