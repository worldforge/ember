/*
 -------------------------------------------------------------------------------
 This source file is part of Cataclysmos
 For the latest info, see http://www.cataclysmos.org/

 Copyright (c) 2005 The Cataclysmos Team
 Copyright (C) 2005  Erik Hjortsberg

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 -------------------------------------------------------------------------------
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SubModelPart.h"

#include "ModelDefinition.h"

#include <OgreSubEntity.h>
#include <OgreSubMesh.h>

namespace EmberOgre
{
namespace Model
{

SubModelPart::SubModelPart(const std::string& name) :
	mName(name)
{
}

SubModelPart::~SubModelPart()
{
	//no need to try to delete the Ogre::Subentities in the mSubEntities store, since Ogre will take care of this
}

bool SubModelPart::addSubEntity(Ogre::SubEntity* subentity, SubEntityDefinition* definition)
{
	mSubEntities.push_back(SubModelPartEntity(subentity, definition));
	return true;
}

bool SubModelPart::removeSubEntity(const Ogre::SubEntity* subentity)
{
	for (SubModelPartEntityStore::iterator I = mSubEntities.begin(); I != mSubEntities.end(); ++I) {
		if (I->SubEntity == subentity) {
			mSubEntities.erase(I);
			return true;
		}
	}
	return false;
}

const std::string& SubModelPart::getName() const
{
	return mName;
}

void SubModelPart::show()
{
	for (SubModelPartEntityStore::const_iterator I = mSubEntities.begin(); I != mSubEntities.end(); ++I) {
		if (I->Definition && I->Definition->getMaterialName() != "") {
			I->SubEntity->setMaterialName(I->Definition->getMaterialName());
		} else {
			///if no material name is set in the ModelDefinition, use the default one from the mesh
			I->SubEntity->setMaterialName(I->SubEntity->getSubMesh()->getMaterialName());
		}

		I->SubEntity->setVisible(true);
	}
}

void SubModelPart::hide()
{
	for (SubModelPartEntityStore::const_iterator I = mSubEntities.begin(); I != mSubEntities.end(); ++I) {
		I->SubEntity->setVisible(false);
	}
}

const SubModelPart::SubModelPartEntityStore& SubModelPart::getSubentities() const
{
	return mSubEntities;
}

}
}
