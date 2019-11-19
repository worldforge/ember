/*
-------------------------------------------------------------------------------
	This source file is part of Cataclysmos
	For the latest info, see http://www.cataclysmos.org/

	Copyright (c) 2005 The Cataclysmos Team
    Copyright (C) 2005  Erik Ogenvik

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
-------------------------------------------------------------------------------
*/
#ifndef SUBMODELPART_H
#define SUBMODELPART_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "ModelDefinition.h"

#include <set>
#include <vector>
#include <boost/optional.hpp>

namespace Ogre {
class SubEntity;
}


namespace Ember {
namespace OgreView {
namespace Model {

class SubModel;

class Model;

class ModelDefinition;

struct SubModelPartEntity {
	Ogre::SubEntity* SubEntity;
	boost::optional<SubEntityDefinition> Definition;
	unsigned short subEntityIndex;
};


class SubModelPart {
	friend class ModelDefinition;

public:

	SubModelPart(std::string name, SubModel& subModel);

	virtual ~SubModelPart();


	void addSubEntity(SubModelPartEntity subModelPartEntity);
	bool addSubEntity(Ogre::SubEntity* subentity, SubEntityDefinition definition, unsigned short subEntityIndex);

	bool removeSubEntity(const Ogre::SubEntity* subentity);

	void show();

	void hide();

	const std::string& getName() const;

	const std::vector<SubModelPartEntity>& getSubentities() const;

	void destroy();

protected:
	std::string mName;
	std::vector<SubModelPartEntity> mSubEntities;
	SubModel& mSubModel;

	std::vector<Ogre::InstancedEntity*> mInstancedEntities;

	bool createInstancedEntities();

	void showSubEntities();
};

}
}
}
#endif // SUBMODELPART_H
