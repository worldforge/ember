/*
 Copyright (C) 2020 Erik Ogenvik

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

#ifndef EMBER_ENTITYCREATORWIDGET_H
#define EMBER_ENTITYCREATORWIDGET_H

#include "Widget.h"
#include "WidgetPlugin.h"
#include "framework/AutoCloseConnection.h"
#include "ListHolder.h"
#include "components/ogre/widgets/adapters/StringListAdapter.h"
#include "components/ogre/authoring/RulesFetcher.h"
#include "components/ogre/authoring/EntityRecipe.h"
#include "components/ogre/authoring/DetachedEntity.h"
#include "EntityCreatorCreationInstance.h"
#include "EntityCreator.h"

#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>

namespace Ember {
namespace OgreView {
namespace Gui {

class ModelRenderer;

class EntityTextureManipulator;

class EntityCreatorWidget : public virtual sigc::trackable {
public:
	static WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager);


	EntityCreatorWidget(GUIManager& guiManager, World& world);

	~EntityCreatorWidget();

	void show();

private:
	World& mWorld;
	Widget* mWidget;
	std::unique_ptr<ListHolder> mListHolder;
	std::unique_ptr<Adapters::StringListAdapter> mListAdapter;

	/**
	 * @brief A preview renderer for creating new models.
	 */
	std::unique_ptr<ModelRenderer> mModelPreviewRenderer;

	/**
	 * @brief Handles manipulation of the entity preview.
	 */
	std::unique_ptr<EntityTextureManipulator> mModelPreviewManipulator;

	Authoring::RulesFetcher mRulesFetcher;

	std::unordered_map<std::string, Authoring::RulesFetcher::RuleEntry> mRules;

	std::shared_ptr<Authoring::EntityRecipe> mEntityRecipe;

	struct AdapterEntry {
		std::unique_ptr<Gui::Adapters::Atlas::AdapterBase> adapter;
		Authoring::GUIAdapter* guiAdapter;
		bool allowRandom;
		CEGUI::Window* container;
	};

	std::map<std::string, AdapterEntry> mAdapters;

	std::vector<Atlas::Message::MapType> mEntityMaps;

	AutoCloseConnection mBoundTypeConnection;

	AutoCloseConnection mBadTypeConnection;

	Eris::TypeInfo* mUnboundType;

	/**
	 * @brief A creation instance, which represents a preview of the entity, before it's created on the server.
	 */
	std::unique_ptr<EntityCreatorCreationInstance> mCreationInstance;


	/**
	 * @brief The last orientation used for creation.
	 *
	 * This is sent to any new creation instance, thus making sure that any new entity starts out with the same orientation as the last one.
	 * (That is unless we mRandomizeOrientation is set to True.)
	 */
	WFMath::Quaternion mLastOrientation;

	/**
	 * @brief If set to true, all new entities will have their orientation randomized around the vertical axis.
	 */
	bool mRandomizeOrientation;

	std::function<void()> mCreateNewEntityFn;

	boost::optional<std::string> mFixedParentId;

	void buildWidget();

	void addRulesToList(const Authoring::RulesFetcher::RuleEntry& entry, int level);

	void showRecipe(const std::shared_ptr<Authoring::EntityRecipe>& recipe);

	void showType(const std::string& type);

	void showPreview(Ember::OgreView::Authoring::DetachedEntity& entity);

	void refreshPreview();

	void refreshEntityMap();

	std::unique_ptr<Gui::Adapters::Atlas::AdapterBase> attachToGuiAdapter(Authoring::GUIAdapter& guiAdapter, CEGUI::Window* window);

};

}
}
}

#ifdef WF_USE_WIDGET_PLUGINS
BOOST_DLL_ALIAS(
		Ember::OgreView::Gui::EntityCreatorWidget::registerWidget,
		registerWidget
)
#endif

#endif //EMBER_ENTITYCREATORWIDGET_H
