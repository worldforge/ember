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
#ifndef SUBMODELPART_H
#define SUBMODELPART_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <OgreEntity.h>
#include <OgreMovableObject.h>

namespace EmberOgre {
namespace Model {

class SubModel;
class Model;
class ModelDefinition;


class SubModelPart{
friend class ModelDefinition;
public:
	typedef std::set<Ogre::SubEntity*> SubEntitySet;

	SubModelPart(std::string name);
	virtual ~SubModelPart();


	bool addSubEntity(Ogre::SubEntity* subentity);
 	bool removeSubEntity(Ogre::SubEntity* subentity);
 	
 	void show();
 	void hide();
 	
 	const std::string& getName() const;
	
	const SubEntitySet& getSubentities() const;

protected:
	std::string mName;
	SubEntitySet mSubEntities;
};

}
}
#endif // SUBMODELPART_H
