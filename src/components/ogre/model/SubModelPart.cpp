#include "Model.h"
#include "SubModelPart.h"
#include "SubModel.h"

namespace EmberOgre {

SubModelPart::SubModelPart(std::string name)
: mName(name)
{}
SubModelPart::~SubModelPart()
{}


bool SubModelPart::addSubEntity(Ogre::SubEntity* subentity)
{
	mSubEntities.insert(subentity);	
	return true;
}
bool SubModelPart::removeSubEntity(Ogre::SubEntity* subentity)
{
	mSubEntities.erase(subentity);	
	return true;
}

const std::string SubModelPart::getName() const
{
	return mName;	
}

void SubModelPart::show()
{
	SubEntitySet::const_iterator I;
	for (I = mSubEntities.begin(); I != mSubEntities.end(); I++) {
		Ogre::SubEntity* subEntity = *I;
		subEntity->setVisible(true);
	}	
}

void SubModelPart::hide()
{
	SubEntitySet::const_iterator I;
	for (I = mSubEntities.begin(); I != mSubEntities.end(); I++) {
		Ogre::SubEntity* subEntity = *I;
		subEntity->setVisible(false);
	}	
}

const SubModelPart::SubEntitySet& SubModelPart::getSubentities() const
{
	return mSubEntities;
}

}

