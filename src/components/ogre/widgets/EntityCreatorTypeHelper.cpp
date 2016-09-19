//
// C++ Implementation: EntityCreatorTypeHelper
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
#include "adapters/eris/RuleTreeAdapter.h"

#include "../EmberOgre.h"
#include "../Convert.h"
#include "../Avatar.h"
#include "domain/EmberEntity.h"
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

#include <CEGUI/Window.h>
#include <CEGUI/widgets/Listbox.h>
#include <CEGUI/widgets/ListboxItem.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <CEGUI/widgets/Editbox.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/widgets/Tree.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

EntityCreatorTypeHelper::EntityCreatorTypeHelper(Eris::Connection& connection, CEGUI::Tree& typeTree, CEGUI::Editbox& nameEditbox, CEGUI::PushButton& pushButton, CEGUI::Window& modelPreview) :
		mConnection(connection), mName(nameEditbox), mModelPreviewRenderer(0), mModelPreviewManipulator(0), mRuleTreeAdapter(0), mCreateButton(nullptr)
{
	buildWidget(typeTree, pushButton, modelPreview);
}

EntityCreatorTypeHelper::~EntityCreatorTypeHelper()
{
	delete mModelPreviewManipulator;
	delete mModelPreviewRenderer;
	delete mRuleTreeAdapter;
}

void EntityCreatorTypeHelper::buildWidget(CEGUI::Tree& typeTree, CEGUI::PushButton& pushButton, CEGUI::Window& modelPreview)
{

	typeTree.setItemTooltipsEnabled(true);
	typeTree.setSortingEnabled(true);

	typeTree.subscribeEvent(CEGUI::Tree::EventSelectionChanged, CEGUI::Event::Subscriber(&EntityCreatorTypeHelper::typeTree_SelectionChanged, this));
	mCreateButton = &pushButton;
	mCreateButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EntityCreatorTypeHelper::createButton_Click, this));

	mRuleTreeAdapter = new Adapters::Eris::RuleTreeAdapter(mConnection, typeTree);
	mRuleTreeAdapter->refresh("game_entity");

	mModelPreviewRenderer = new ModelRenderer(&modelPreview, "modelPreview");
	mModelPreviewManipulator = new CameraEntityTextureManipulator(modelPreview, mModelPreviewRenderer->getEntityTexture());


	mConnection.getTypeService()->BoundType.connect(sigc::mem_fun(*this, &EntityCreatorTypeHelper::typeService_BoundType));

}

void EntityCreatorTypeHelper::updatePreview()
{
	if (mModelPreviewRenderer && mRuleTreeAdapter) {
		auto typeData = mRuleTreeAdapter->getSelectedRule();
		if (typeData.isValid()) {
			//check if the type is bound
			mCurrentType = typeData->getId();
			auto type = mConnection.getTypeService()->getTypeByName(typeData->getId());
			if (type && type->isBound()) {
				//update the model preview window
				mModelPreviewRenderer->showModel(typeData->getId());
				mModelPreviewRenderer->showFull();
				//we want to zoom in a little
				mModelPreviewRenderer->setCameraDistance(0.7);
				mCreateButton->setEnabled(true);
			} else {
				mModelPreviewRenderer->showModel("");
				mCreateButton->setEnabled(false);
			}
		} else {
			mModelPreviewRenderer->showModel("");
			mCreateButton->setEnabled(false);
		}
	}
}

void EntityCreatorTypeHelper::typeService_BoundType(Eris::TypeInfo* typeInfo)
{
	//If the type that's now bound is the one that's currently handled, update the preview
	if (mModelPreviewRenderer && mRuleTreeAdapter && typeInfo->getName() == mCurrentType) {
		auto typeData = mRuleTreeAdapter->getSelectedRule();
		if (typeData.isValid()) {
			//update the model preview window
			mModelPreviewRenderer->showModel(typeData->getId());
			mModelPreviewRenderer->showFull();
			//we want to zoom in a little
			mModelPreviewRenderer->setCameraDistance(0.7);
			mCreateButton->setEnabled(true);
		}
	}
}

bool EntityCreatorTypeHelper::typeTree_SelectionChanged(const CEGUI::EventArgs& args)
{
	updatePreview();
	return true;
}

bool EntityCreatorTypeHelper::createButton_Click(const CEGUI::EventArgs& args)
{
	if (mRuleTreeAdapter) {

		auto typeData = mRuleTreeAdapter->getSelectedRule();
		if (typeData.isValid()) {
			try {
				std::string name;
				if (!mName.getText().empty()) {
					name = mName.getText().c_str();
				}
				//TODO: this has been refactored; perhaps we should also change the signal to not use a TypeInfo...

				auto typeInfo = mConnection.getTypeService()->getTypeByName(typeData->getId());
				if (typeInfo) {
					EventCreateFromType(name, *typeInfo);
				}
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
