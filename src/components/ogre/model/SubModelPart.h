#ifndef SUBMODELPART_H
#define SUBMODELPART_H

#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreMovableObject.h>

namespace DimeOgre {

class SubModel;
class Model;

class SubModelPart{
public:
	typedef std::set<Ogre::SubEntity*> SubEntitySet;

	SubModelPart(std::string name);
	virtual ~SubModelPart();


	bool addSubEntity(Ogre::SubEntity* subentity);
 	bool removeSubEntity(Ogre::SubEntity* subentity);
 	
 	void show();
 	void hide();
 	
 	const std::string getName() const;
	
protected:
	std::string mName;
	SubEntitySet mSubEntities;
};

}

#endif // SUBMODELPART_H
