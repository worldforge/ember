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

EntityCreatorTypeHelper::EntityCreatorTypeHelper(Eris::Entity& avatarEntity, Eris::Connection& connection, CEGUI::Tree& typeTree, CEGUI::Editbox& nameEditbox, CEGUI::PushButton& pushButton, CEGUI::Window& modelPreview) :
	CreateEntity("createentity", this, "Create an entity."), Make("make", this, "Create an entity."), mAvatarEntity(avatarEntity), mConnection(connection), mName(nameEditbox), mModelPreviewRenderer(0), mTypeTreeAdapter(0)
{
	buildWidget(typeTree, pushButton, modelPreview);
}

EntityCreatorTypeHelper::~EntityCreatorTypeHelper()
{
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

}

void EntityCreatorTypeHelper::runCommand(const std::string &command, const std::string &args)
{
	if (CreateEntity == command || Make == command) {
		Eris::TypeService* typeService = mConnection.getTypeService();
		Eris::TypeInfo* typeinfo = typeService->getTypeByName(args);
		if (typeinfo) {
			createEntityOfType(typeinfo);
		}
	}

}

void EntityCreatorTypeHelper::updatePreview()
{
	if (mModelPreviewRenderer && mTypeTreeAdapter) {
		Eris::TypeInfo* typeInfo = mTypeTreeAdapter->getSelectedTypeInfo();
		if (typeInfo) {
			///update the model preview window
			mModelPreviewRenderer->showModel(typeInfo->getName());
			//mModelPreviewRenderer->showFull();
			///we want to zoom in a little
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

void EntityCreatorTypeHelper::createEntityOfType(Eris::TypeInfo* typeinfo)
{
	Atlas::Objects::Operation::Create c;
	c->setFrom(mAvatarEntity.getId());
	///if the avatar is a "creator", i.e. and admin, we will set the TO property
	///this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
	if (mAvatarEntity.getType()->isA(mConnection.getTypeService()->getTypeByName("creator"))) {
		c->setTo(mAvatarEntity.getId());
	}

	Atlas::Message::MapType msg;
	msg["loc"] = mAvatarEntity.getLocation()->getId();

	Ogre::Vector3 o_vector(2, 0, 0);
	Ogre::Vector3 o_pos = Convert::toOgre(mAvatarEntity.getPredictedPos()) + (Convert::toOgre(mAvatarEntity.getOrientation()) * o_vector); //TODO: remove conversions

	// 	WFMath::Vector<3> vector(0,2,0);
	// 	WFMath::Point<3> pos = avatar->getPosition() + (avatar->getOrientation() * vector);
	WFMath::Point<3> pos = Convert::toWF<WFMath::Point<3> >(o_pos);
	WFMath::Quaternion orientation = mAvatarEntity.getOrientation();

	msg["pos"] = pos.toAtlas();
	if (mName.getText().length() > 0) {
		msg["name"] = mName.getText().c_str();
	} else {
		msg["name"] = typeinfo->getName();
	}
	msg["parents"] = Atlas::Message::ListType(1, typeinfo->getName());
	msg["orientation"] = orientation.toAtlas();

	c->setArgsAsList(Atlas::Message::ListType(1, msg));
	mConnection.send(c);
	std::stringstream ss;
	ss << pos;
	S_LOG_INFO("Try to create entity of type " << typeinfo->getName() << " at position " << ss.str() );

}

}

}
}
