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

#include "ModelFitting.h"

#include "components/ogre/EmberEntity.h"

namespace EmberOgre
{
namespace Model
{

ModelFitting::ModelFitting(EmberEntity& parent, const std::string& mountPoint, const std::string& childEntityId) :
	mParent(parent), mChild(0), mMountPoint(mountPoint), mChildEntityId(childEntityId)
{

}

ModelFitting::~ModelFitting()
{
}

void ModelFitting::attachChild(EmberEntity& entity)
{
	mChild = &entity;
}

EmberEntity& ModelFitting::getParent() const
{
	return mParent;
}
EmberEntity* ModelFitting::getChild() const
{
	return mChild;
}
const std::string& ModelFitting::getMountPoint() const
{
	return mMountPoint;
}

const std::string& ModelFitting::getChildEntityId() const
{
	return mChildEntityId;
}
}
}
