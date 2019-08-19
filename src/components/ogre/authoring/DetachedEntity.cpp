/*
 Copyright (C) 2008  Alexey Torkhov <atorkhov@gmail.com>

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

#include "DetachedEntity.h"

namespace Ember
{
namespace OgreView
{
namespace Authoring
{
DetachedEntity::DetachedEntity(const std::string& id, Eris::TypeInfo* ty, Eris::TypeService* typeService) :
	Eris::Entity(id, ty), mTypeService(typeService)
{
}

void DetachedEntity::setFromMessage(const Atlas::Message::MapType& attrs)
{
	beginUpdate();

	Atlas::Message::MapType::const_iterator A;

	for (A = attrs.begin(); A != attrs.end(); ++A) {
		if (A->first == "loc" || A->first == "id" || A->first == "contains")
			continue;

		// see if the value in the sight matches the existing value
		auto I = m_properties.find(A->first);
		if ((I != m_properties.end()) && (I->second == A->second))
			continue;

		setProperty(A->first, A->second);
	}

	endUpdate();
}

Eris::TypeService* DetachedEntity::getTypeService() const
{
	return mTypeService;
}

void DetachedEntity::doInit(const Atlas::Objects::Entity::RootEntity& rootEntity)
{
	init(rootEntity, true);
}


void DetachedEntity::removeFromMovementPrediction()
{
}

void DetachedEntity::addToMovementPrediction()
{
}

Eris::Entity* DetachedEntity::getEntity(const std::string&)
{
	return nullptr;
}



}
}
}
