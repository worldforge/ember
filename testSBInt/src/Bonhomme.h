#ifndef BONHOMME_H
#define BONHOMME_H

	#include <Ogre.h>
	#include <sb/SmartBody.h>
	#include <string>

	#define SINBAD 1

#if SINBAD
	#define BONHOMME_SK_PATH "Sinbad.skeleton"
	#define BONHOMME_MESH_PATH "Sinbad.mesh"
#else
	#define BONHOMME_SK_PATH "humanoid.skeleton"
	#define BONHOMME_MESH_PATH "male_new.mesh"
#endif
	#define BONHOMME_JOINTMAP_NAME "BonhommeJointMap"
	#define BONHOMME_SB_SK_PATH (std::string(BONHOMME_SK_PATH) + std::string(".xml"))

class Bonhomme
{
public:
	Bonhomme(Ogre::String const &name);
	~Bonhomme(void);

	void addToScene(void);
	void animate(Ogre::String const &animName, bool loop = true);
	void stop(Ogre::String const &animName);
	void updateAnimation(Ogre::String const &animName, Ogre::Real offset);
	void updateBonePositions(void);


private:
	Ogre::Entity *mEntity;
	SmartBody::SBCharacter *mCharacter;
	
	void mapSkeleton(void);
	SmartBody::SBSkeleton* getSbSkeleton(void);
	void setManualControl(void);
};

#endif