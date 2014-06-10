#ifndef ENTITY_H
#define ENTITY_H

	#include <Ogre.h>

class Entity
{
public:
	static Ogre::Entity* createEntityWithSkeleton(Ogre::String const &name, Ogre::String const &meshFile, Ogre::String const &skeletonFile);
	static void printEntityAnimations(Ogre::String const &name);
	static void printSkeletonBonesName(Ogre::String const &name);
};

#endif