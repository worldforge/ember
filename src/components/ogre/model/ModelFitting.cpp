/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#include "ModelFitting.h"

#include <utility>

#include "domain/EmberEntity.h"

namespace Ember
{
namespace OgreView
{
namespace Model
{

ModelFitting::ModelFitting(EmberEntity& parent, std::string mountPoint, std::string childEntityId) :
	mParent(parent),
	mChild(nullptr),
	mMountPoint(std::move(mountPoint)),
	mChildEntityId(std::move(childEntityId))
{

}

ModelFitting::~ModelFitting() = default;

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
}
