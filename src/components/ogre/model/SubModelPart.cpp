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
#include "SubModelPart.h"
#include "Model.h"
#include "SubModel.h"

namespace EmberOgre {
namespace Model {

SubModelPart::SubModelPart(std::string name)
: mName(name)
{}
SubModelPart::~SubModelPart()
{
	//no need to try to delete the Ogre::Subentities in the mSubEntities store, since Ogre will take care of this
}


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

const std::string& SubModelPart::getName() const
{
	return mName;	
}

void SubModelPart::show()
{
	SubEntitySet::const_iterator I;
	for (I = mSubEntities.begin(); I != mSubEntities.end(); I++) {
		(*I)->setVisible(true);
	}	
}

void SubModelPart::hide()
{
	SubEntitySet::const_iterator I;
	for (I = mSubEntities.begin(); I != mSubEntities.end(); I++) {
		(*I)->setVisible(false);
	}	
}

const SubModelPart::SubEntitySet& SubModelPart::getSubentities() const
{
	return mSubEntities;
}

}
}
