/*
    Copyright (C) 2004  Erik Ogenvik

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef SUBMODEL_H
#define SUBMODEL_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include <map>

namespace Ember {
namespace OgreView {
namespace Model {

class Model;

class SubModelPart;

class ModelDefinition;

class SubModel {
	friend class ModelDefinition;

	friend class SubModelPart;

public:

	explicit SubModel(Ogre::Entity& entity, Model& model);

	virtual ~SubModel();

	SubModelPart& createSubModelPart(const std::string& name);

	std::map<std::string, SubModelPart>& getSubModelPartMap();

	Ogre::Entity* getEntity() const;

protected:
	Ogre::Entity& mEntity;
	Model& mModel;
	std::map<std::string, SubModelPart> mSubModelParts;

};

}
}

}

#endif // SUBMODEL_H
