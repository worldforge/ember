/*
    Copyright (C) 2004  Erik Hjortsberg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef SUBMODEL_H
#define SUBMODEL_H
#include "components/ogre/EmberOgrePrerequisites.h"
#include <OgreEntity.h>
#include <OgreMovableObject.h>

namespace EmberOgre {
namespace Model {

class Model;
class SubModelPart;
class ModelDefinition;

class SubModel{
friend class ModelDefinition;
public:
	typedef std::map<std::string, std::set<std::string> > SubModelPartMapping;
	typedef std::map<std::string, SubModelPart*> SubModelPartMap;
	
	SubModel(Ogre::Entity* entity);
	virtual ~SubModel();
	
	//void createSubModelParts(SubModelPartMapping* submodelPartMapping);
	SubModelPart* createSubModelPart(const std::string& name);
	
	SubModelPartMap* getSubModelPartMap();

	Ogre::Entity* getEntity() const;
 
 
 	



	
protected:
	Ogre::Entity* mEntity;
	SubModelPartMap mSubModelParts;
	
//	Ogre::SceneNode* mNode;	
};

}
}

#endif // SUBMODEL_H
