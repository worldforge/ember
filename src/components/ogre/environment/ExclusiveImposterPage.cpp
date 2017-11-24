/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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


#include <components/ogre/model/Model.h>
#include "ExclusiveImposterPage.h"

namespace Ember
{
namespace OgreView
{

namespace Environment
{

void ExclusiveImposterPage::addEntity(Ogre::Entity *ent, const Ogre::Vector3 &position, const Ogre::Quaternion &rotation, const Ogre::Vector3 &scale, const Ogre::ColourValue &color)
{
	Forests::ImpostorPage::addEntity(ent, position, rotation, scale, color);
	auto* model = Ogre::any_cast<Ember::OgreView::Model::Model*>(ent->getUserObjectBindings().getUserAny("model"));
	mModels.push_back(model);
}

void ExclusiveImposterPage::addModel(Ember::OgreView::Model::Model* model, const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const Ogre::Vector3& scale, const Ogre::ColourValue& color) {
	Forests::ImpostorPage::addModel(model, position, rotation, scale, color);
	mModels.push_back(model);
}

void ExclusiveImposterPage::setVisible(bool visible)
{
	Forests::ImpostorPage::setVisible(visible);
	for (auto model : mModels) {
		model->setVisible(!visible);
	}
}

void ExclusiveImposterPage::removeEntities()
{
	Forests::ImpostorPage::removeEntities();
	mModels.clear();
}



}

}
}
