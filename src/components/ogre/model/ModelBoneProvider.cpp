/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "ModelBoneProvider.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/OgreInfo.h"
#include <OgreMovableObject.h>
#include <OgreNode.h>
#include <OgreTagPoint.h>

namespace EmberOgre
{
namespace Model
{
ModelBoneProvider::ModelBoneProvider(Model& parentModel, const std::string& attachPointName, Ogre::MovableObject* movableObject) :
	mParentModel(parentModel), mAttachPointName(attachPointName), mNode(0), mAttachedObject(movableObject)
{
	if (movableObject) {
		movableObject->detatchFromParent();
		mNode = mParentModel.attachObjectToAttachPoint(attachPointName, movableObject);
	}
}

ModelBoneProvider::~ModelBoneProvider()
{
	if (mAttachedObject) {
		mParentModel.detachObjectFromBone(mAttachedObject->getName());
	}
}

Ogre::Node& ModelBoneProvider::getNode() const
{
	return *mNode;
}

Ogre::Node* ModelBoneProvider::getParentNode() const
{
	return 0;
}

INodeProvider* ModelBoneProvider::createChildProvider(Ogre::MovableObject* attachedObject)
{
	return new ModelBoneProvider(mParentModel, mAttachPointName, attachedObject);
}

void ModelBoneProvider::setVisible(bool visible)
{
	if (mAttachedObject) {
		mAttachedObject->setVisible(visible);
	}
}

void ModelBoneProvider::setVisualize(const std::string& visualization, bool visualize)
{
}

bool ModelBoneProvider::getVisualize(const std::string& visualization) const
{
	return false;
}

}
}
