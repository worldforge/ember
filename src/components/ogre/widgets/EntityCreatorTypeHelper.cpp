//
// C++ Implementation: EntityCreatorTypeHelper
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2004
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityCreatorTypeHelper.h"
#include "ColouredListItem.h"
#include "ModelRenderer.h"
#include "EntityTextureManipulator.h"
#include "adapters/eris/TypeTreeAdapter.h"

#include "../EmberOgre.h"
#include "../Convert.h"
#include "../Avatar.h"
#include "../EmberEntity.h"
#include "../World.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "framework/LoggingInstance.h"
#include "framework/ConsoleBackend.h"

#include <Eris/TypeService.h>
#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>
#include <Eris/View.h>

#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/Element.h>

#include <wfmath/atlasconv.h>

#include <CEGUIWindow.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIEditbox.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIGUISheet.h>
#include <elements/CEGUITree.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

EntityCreatorTypeHelper::EntityCreatorTypeHelper(Eris::Connection& connection, CEGUI::Tree& typeTree, CEGUI::Editbox& nameEditbox, CEGUI::PushButton& pushButton, CEGUI::Window& modelPreview) :
	mConnection(connection), mName(nameEditbox), mModelPreviewRenderer(0), mModelPreviewManipulator(0), mTypeTreeAdapter(0)
{
	buildWidget(typeTree, pushButton, modelPreview);
}

EntityCreatorTypeHelper::~EntityCreatorTypeHelper()
{
	delete mModelPreviewManipulator;
	delete mModelPreviewRenderer;
	delete mTypeTreeAdapter;
}

void EntityCreatorTypeHelper::buildWidget(CEGUI::Tree& typeTree, CEGUI::PushButton& pushButton, CEGUI::Window& modelPreview)
{

	typeTree.setItemTooltipsEnabled(true);
	typeTree.setSortingEnabled(true);

	typeTree.subscribeEvent(CEGUI::Tree::EventSelectionChanged, CEGUI::Event::Subscriber(&EntityCreatorTypeHelper::typeTree_ItemSelectionChanged, this));
	pushButton.subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EntityCreatorTypeHelper::createButton_Click, this));

	mTypeTreeAdapter = new Adapters::Eris::TypeTreeAdapter(*mConnection.getTypeService(), typeTree);
	mTypeTreeAdapter->initialize("game_entity");

	mModelPreviewRenderer = new ModelRenderer(&modelPreview);
	mModelPreviewManipulator = new EntityTextureManipulator(modelPreview, mModelPreviewRenderer->getEntityTexture());

}

void EntityCreatorTypeHelper::updatePreview()
{
	if (mModelPreviewRenderer && mTypeTreeAdapter) {
		Eris::TypeInfo* typeInfo = mTypeTreeAdapter->getSelectedTypeInfo();
		if (typeInfo) {
			//update the model preview window
			mModelPreviewRenderer->showModel(typeInfo->getName());
			//mModelPreviewRenderer->showFull();
			//we want to zoom in a little
			mModelPreviewRenderer->setCameraDistance(0.7);
		} else {
			mModelPreviewRenderer->showModel("");
		}
	}
}

bool EntityCreatorTypeHelper::typeTree_ItemSelectionChanged(const CEGUI::EventArgs& args)
{
	updatePreview();
	return true;
}

bool EntityCreatorTypeHelper::createButton_Click(const CEGUI::EventArgs& args)
{
	if (mTypeTreeAdapter) {

		Eris::TypeInfo* typeInfo = mTypeTreeAdapter->getSelectedTypeInfo();
		if (typeInfo) {
			try {
				std::string name;
				if (mName.getText().length() > 0) {
					name = mName.getText().c_str();
				} else {
					name = typeInfo->getName();
				}
				EventCreateFromType(name, *typeInfo);
			} catch (const std::exception& ex) {
				S_LOG_WARNING("Error when trying to create entity from type." << ex);
			}
		}
	}
	return true;
}

}

}
}
