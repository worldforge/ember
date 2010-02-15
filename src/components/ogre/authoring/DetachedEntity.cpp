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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "DetachedEntity.h"

namespace EmberOgre
{
namespace Authoring
{
DetachedEntity::DetachedEntity(const std::string& id, Eris::TypeInfo* ty, Eris::TypeService* typeService) :
	Eris::Entity(id, ty), mTypeService(typeService)
{
}

DetachedEntity::~DetachedEntity()
{
}

void DetachedEntity::setAttr(const std::string &p, const Atlas::Message::Element &v)
{
	Eris::Entity::setAttr(p, v);
}

void DetachedEntity::setFromMessage(const Atlas::Message::MapType& attrs)
{
	beginUpdate();

	Atlas::Message::MapType::const_iterator A;

	/*
	 attrs.erase("loc");
	 attrs.erase("id");
	 attrs.erase("contains");

	 if (!allowMove) filterMoveAttrs(attrs);
	 */

	for (A = attrs.begin(); A != attrs.end(); ++A) {
		if (A->first == "loc" || A->first == "id" || A->first == "contains")
			continue;

		// see if the value in the sight matches the exsiting value
		Eris::Entity::AttrMap::iterator I = m_attrs.find(A->first);
		if ((I != m_attrs.end()) && (I->second == A->second))
			continue;

		setAttr(A->first, A->second);
	}

	endUpdate();
}

Eris::TypeService* DetachedEntity::getTypeService() const
{
	return mTypeService;
}

void DetachedEntity::removeFromMovementPrediction()
{
}

void DetachedEntity::addToMovementPredition()
{
}

Eris::Entity* DetachedEntity::getEntity(const std::string& id)
{
	return 0;
}



}
}
