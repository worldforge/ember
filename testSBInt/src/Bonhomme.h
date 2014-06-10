#ifndef BONHOMME_H
#define BONHOMME_H

	#include <Ogre.h>

	#define BONHOMME_SK_PATH "BonhommeSkeleton.skeleton.xml"
	#define BONHOMME_JOINTMAP_NAME "BonhommeJointMap"

class Bonhomme
{
public:
	Bonhomme(Ogre::String const &name);
	~Bonhomme();

	void addToScene();
	void animate(Ogre::String const &animName, bool loop = true);
	void stop(Ogre::String const &animName);
	void updateAnimation(Ogre::String const &animName, Ogre::Real offset);


private:
	Ogre::Entity *mEntity;
	
	SmartBody::SBJointMap* Bonhomme::mapSkeleton();
	SmartBody::SBSkeleton* Bonhomme::getSbSkeleton();
};

#endif