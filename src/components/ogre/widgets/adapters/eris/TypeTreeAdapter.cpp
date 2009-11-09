/*
 Copyright (C) 2009 Erik Hjortsberg

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

#include "TypeTreeAdapter.h"

#include "../../ColouredListItem.h"

#include <Eris/TypeService.h>
#include <Eris/TypeInfo.h>

#include <elements/CEGUITree.h>
#include <elements/CEGUITreeItem.h>

namespace EmberOgre
{

namespace Gui
{

namespace Adapters
{

namespace Eris
{

TypeTreeAdapter::TypeTreeAdapter(::Eris::TypeService& typeService, CEGUI::Tree& treeWidget) :
	mTypeService(typeService), mTreeWidget(treeWidget)
{

}

TypeTreeAdapter::~TypeTreeAdapter()
{
}

void TypeTreeAdapter::initialize()
{
	loadAllTypes();
	mTypeService.BoundType.connect(sigc::mem_fun(*this, &TypeTreeAdapter::boundAType));
}

void TypeTreeAdapter::loadAllTypes()
{
	::Eris::TypeInfo* typeInfo = mTypeService.getTypeByName("game_entity");
	if (typeInfo) {
		if (typeInfo->hasUnresolvedChildren())
			typeInfo->resolveChildren();
		const ::Eris::TypeInfoSet children = typeInfo->getChildren();
		::Eris::TypeInfoSet::const_iterator I = children.begin();
		::Eris::TypeInfoSet::const_iterator I_end = children.end();

		for (; I != I_end; ++I) {
			addToTree(*I, 0, true);
		}
	}
}

void TypeTreeAdapter::addToTree(::Eris::TypeInfo* typeInfo, CEGUI::TreeItem* parent, bool addRecursive)
{

	CEGUI::TreeItem* item = ColouredTreeItem::create(typeInfo->getName());
	item->toggleIsOpen();
	if (!parent) {
		mTreeWidget.addItem(item);
	} else {
		parent->addItem(item);
	}
	mTypeLookup[item] = typeInfo;

	if (addRecursive) {
		if (typeInfo->hasUnresolvedChildren())
			typeInfo->resolveChildren();
		const ::Eris::TypeInfoSet children = typeInfo->getChildren();
		::Eris::TypeInfoSet::const_iterator I = children.begin();
		::Eris::TypeInfoSet::const_iterator I_end = children.end();

		for (; I != I_end; ++I) {
			addToTree(*I, item, addRecursive);
		}
	}

}

void TypeTreeAdapter::boundAType(::Eris::TypeInfo* typeInfo)
{

	::Eris::TypeInfo* gameEntityType = mTypeService.getTypeByName("game_entity");

	if (gameEntityType != 0 && typeInfo->isA(gameEntityType)) {
		if (typeInfo->getParents().size()) {
			::Eris::TypeInfo* parentType = *typeInfo->getParents().begin();
			CEGUI::TreeItem* parent = mTreeWidget.findFirstItemWithText(parentType->getName());
			addToTree(typeInfo, parent);
		}
	}
}

::Eris::TypeInfo* TypeTreeAdapter::getSelectedTypeInfo()
{
	CEGUI::TreeItem* item = mTreeWidget.getFirstSelectedItem();
	if (item) {
		TypeTreeStore::const_iterator I = mTypeLookup.find(item);
		if (I != mTypeLookup.end()) {
			return I->second;
		}
	}
	return 0;
}

}
}
}
}
