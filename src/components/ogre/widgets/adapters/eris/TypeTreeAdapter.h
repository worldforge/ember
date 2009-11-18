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

#ifndef TYPETREEADAPTER_H_
#define TYPETREEADAPTER_H_

#include <map>
#include <sigc++/trackable.h>
#include <string>

namespace Eris
{
class TypeService;
class TypeInfo;
}

namespace CEGUI
{
class Tree;
class TreeItem;
}

namespace EmberOgre
{

namespace Gui
{

namespace Adapters
{

namespace Eris
{
/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief An adapter which binds an eris type tree to a cegui tree widget.
 *
 * Make sure to call initialize() to activate the adapter.
 */
class TypeTreeAdapter: public virtual sigc::trackable
{
public:
	TypeTreeAdapter(::Eris::TypeService& typeService, CEGUI::Tree& treeWidget);

	virtual ~TypeTreeAdapter();

	/**
	 * @brief Initializes the adapter, creating initial type tree and hooking up listening for new types as they arrive.
	 * @param rootTypeName The name of the root type.
	 * @returns True if the root type was found.
	 */
	bool initialize(const std::string& rootTypeName);

	/**
	 * @brief Gets the currently selected type, if any.
	 * @returns The currently selected type, if any.
	 */
	::Eris::TypeInfo* getSelectedTypeInfo();

private:

	typedef std::map<CEGUI::TreeItem*, ::Eris::TypeInfo*> TypeTreeStore;
	typedef std::map< ::Eris::TypeInfo*, CEGUI::TreeItem*> ReverseTypeTreeStore;

	::Eris::TypeService& mTypeService;
	CEGUI::Tree& mTreeWidget;

	/**
	 * @brief Use a lookup map for the types and the corresponding tree item.
	 */
	TypeTreeStore mTypeLookup;

	/**
	 * @brief A reverse lookup map, using type info objects as keys. Mainly used to check whether a certain type already exists in the tree.
	 */
	ReverseTypeTreeStore mTreeItemLookup;

	::Eris::TypeInfo* mRootTypeInfo;

	/**
	 * @brief Recursively loads all the types in the tree.
	 */
	void loadAllTypes();

	/**
	 * @brief Adds a type info to the tree.
	 * @param typeInfo The type info to add.
	 * @param parent The parent of the type info, or 0 if we should add to the top.
	 * @param addRecursive If true, all the current children of the type will be added as well.
	 */
	void addToTree(::Eris::TypeInfo* typeInfo, CEGUI::TreeItem* parent, bool addRecursive = false);

	/**
	 * @brief Listens for types getting bound, and then updating the tree.
	 * @param typeInfo The type which has been bound.
	 */
	void boundAType(::Eris::TypeInfo* typeInfo);

};

}
}
}
}

#endif /* TYPETREEADAPTER_H_ */
