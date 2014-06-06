#ifndef BONHOMME_H
#define BONHOMME_H

	#include <Ogre.h>

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
};

#endif