//
// C++ Interface: EntityCreatorTypeHelper
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_EntityCreatorTypeHelper_H
#define EMBEROGRE_EntityCreatorTypeHelper_H

#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <components/ogre/authoring/DetachedEntity.h>
#include <CEGUI/widgets/Combobox.h>

namespace CEGUI {
class Tree;

class TreeItem;

class Editbox;

class PushButton;

class Window;

class EventArgs;
}

namespace Eris {
class TypeInfo;

class Connection;

class Entity;
}

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {
namespace Eris {
class RuleTreeAdapter;
}
}

class ModelRenderer;

class EntityTextureManipulator;

/**
 * @author Erik Ogenvik
 *
 * A helper class for creating simple entities directly from entity types.
 * The helper binds to a tree widget, which will be filled with entity types.
 */
class EntityCreatorTypeHelper : public virtual sigc::trackable {
public:

	EntityCreatorTypeHelper(Eris::Avatar& avatar, CEGUI::Tree& typeTree,
							CEGUI::Editbox& nameEditbox, CEGUI::PushButton& pushButton,
							CEGUI::Window& modelPreview, CEGUI::Combobox& modeCombobox,
							CEGUI::Window& defaultModeWindow, CEGUI::Window& plantedOnWindow);

	virtual ~EntityCreatorTypeHelper();

	/**
	 * @brief Emitted when an entity should be created from a type.
	 */
	sigc::signal<void, const Atlas::Message::MapType&> EventCreateFromType;

protected:

	Eris::Avatar& mAvatar;


	CEGUI::Editbox& mName;

	/**
	 * @brief A preview renderer for creating new models.
	 */
	ModelRenderer* mModelPreviewRenderer;

	/**
	 * @brief Handles manipulation of the entity preview.
	 */
	EntityTextureManipulator* mModelPreviewManipulator;

	Adapters::Eris::RuleTreeAdapter* mRuleTreeAdapter;

	CEGUI::PushButton* mCreateButton;

	CEGUI::Combobox& mModeCombobox;

	CEGUI::Window& mDefaultModeWindow;

	CEGUI::Window& mPlantedOnWindow;

	/**
	 * @brief Keep track of the currently selected type, mainly for fast lookups in typeService_BoundType.
	 */
	std::string mCurrentType;


	void buildWidget(CEGUI::Tree& typeTree, CEGUI::PushButton& pushButton, CEGUI::Window& modelPreview);

	bool createButton_Click(const CEGUI::EventArgs& args);

	bool typeTree_SelectionChanged(const CEGUI::EventArgs& args);

	void updatePreview();

	void typeService_BoundType(Eris::TypeInfo* typeInfo);

	void showPreview(Ember::OgreView::Authoring::DetachedEntity& entity);
};
}

}
}

#endif
