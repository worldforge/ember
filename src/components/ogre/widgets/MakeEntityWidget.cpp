//
// C++ Implementation: MakeEntityWidget
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

#include "MakeEntityWidget.h"
#include "ColouredListItem.h"
#include "ModelRenderer.h"
#include "adapters/eris/TypeTreeAdapter.h"

#include "../EmberOgre.h"
#include "../Convert.h"
#include "../Avatar.h"
#include "../EmberEntity.h"

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

namespace EmberOgre
{
namespace Gui
{

MakeEntityWidget::MakeEntityWidget() :
	Widget(), CreateEntity("createentity", this, "Create an entity."), Make("make", this, "Create an entity."), mIsReady(false), mModelPreviewRenderer(0), mTypeTreeAdapter(0)
{

	Ember::ConsoleBackend::getSingletonPtr()->registerCommand("testarea", this);

}

MakeEntityWidget::~MakeEntityWidget()
{
	delete mModelPreviewRenderer;
	delete mTypeTreeAdapter;
}

void MakeEntityWidget::buildWidget()
{

	loadMainSheet("MakeEntityWidget.layout", "MakeEntity/");

	mTypeTree = static_cast<CEGUI::Tree*> (getWindow("TypeList"));
	mTypeTree->setItemTooltipsEnabled(true);
	mTypeTree->setSortingEnabled(true);

	mName = static_cast<CEGUI::Editbox*> (getWindow("Name"));

	CEGUI::PushButton* button = static_cast<CEGUI::PushButton*> (getWindow("CreateButton"));

	BIND_CEGUI_EVENT(mTypeTree, CEGUI::Tree::EventSelectionChanged,MakeEntityWidget::typeTree_ItemSelectionChanged );
	BIND_CEGUI_EVENT(button, CEGUI::PushButton::EventClicked,MakeEntityWidget::createButton_Click );

	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &MakeEntityWidget::connectedToServer));
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotAvatar.connect(sigc::mem_fun(*this, &MakeEntityWidget::gotAvatar));

	createPreviewTexture();

	registerConsoleVisibilityToggleCommand("entitycreator");
	enableCloseButton();

}

void MakeEntityWidget::show()
{
	if (mIsReady) {
		if (mMainWindow) {
			if (!mTypeTreeAdapter) {
				mTypeTreeAdapter = new Adapters::Eris::TypeTreeAdapter(*mConn->getTypeService(), *mTypeTree);
				mTypeTreeAdapter->initialize("game_entity");
			}
			Widget::show();
		}
		S_LOG_INFO("Showing entity creator window.");
	} else {
		S_LOG_FAILURE("Can't show entity creator window before we have taken an avatar.");
	}
}

void MakeEntityWidget::gotAvatar(Eris::Avatar* avatar)
{
	mIsReady = true;
}

void MakeEntityWidget::connectedToServer(Eris::Connection* conn)
{
	mConn = conn;
}

void MakeEntityWidget::runCommand(const std::string &command, const std::string &args)
{
	if (CreateEntity == command || Make == command) {
		Eris::TypeService* typeService = mConn->getTypeService();
		Eris::TypeInfo* typeinfo = typeService->getTypeByName(args);
		if (typeinfo) {
			createEntityOfType(typeinfo);
		}
	} else {
		Widget::runCommand(command, args);
	}

}

void MakeEntityWidget::updatePreview()
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

bool MakeEntityWidget::typeTree_ItemSelectionChanged(const CEGUI::EventArgs& args)
{
	updatePreview();
	return true;
}

bool MakeEntityWidget::createButton_Click(const CEGUI::EventArgs& args)
{
	if (mTypeTreeAdapter) {

		Eris::TypeInfo* typeInfo = mTypeTreeAdapter->getSelectedTypeInfo();
		if (typeInfo) {
			createEntityOfType(typeInfo);
		}
	}
	return true;
}

void MakeEntityWidget::createPreviewTexture()
{
	CEGUI::GUISheet* imageWidget = static_cast<CEGUI::GUISheet*> (getWindow("ModelPreviewImage"));
	if (!imageWidget) {
		S_LOG_FAILURE("Could not find ModelPreviewImage, aborting creation of preview texture.");
	} else {
		mModelPreviewRenderer = new ModelRenderer(imageWidget);
	}

}

void MakeEntityWidget::createEntityOfType(Eris::TypeInfo* typeinfo)
{
	Atlas::Objects::Operation::Create c;
	EmberEntity& avatar = EmberOgre::getSingleton().getAvatar()->getEmberEntity();
	c->setFrom(avatar.getId());
	///if the avatar is a "creator", i.e. and admin, we will set the TO property
	///this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
	if (avatar.getType()->isA(mConn->getTypeService()->getTypeByName("creator"))) {
		c->setTo(avatar.getId());
	}

	Atlas::Message::MapType msg;
	msg["loc"] = avatar.getLocation()->getId();

	Ogre::Vector3 o_vector(2, 0, 0);
	Ogre::Vector3 o_pos = Convert::toOgre(avatar.getPredictedPos()) + (Convert::toOgre(avatar.getOrientation()) * o_vector); //TODO: remove conversions

	// 	WFMath::Vector<3> vector(0,2,0);
	// 	WFMath::Point<3> pos = avatar->getPosition() + (avatar->getOrientation() * vector);
	WFMath::Point<3> pos = Convert::toWF<WFMath::Point<3> >(o_pos);
	WFMath::Quaternion orientation = avatar.getOrientation();

	msg["pos"] = pos.toAtlas();
	if (mName->getText().length() > 0) {
		msg["name"] = mName->getText().c_str();
	} else {
		msg["name"] = typeinfo->getName();
	}
	msg["parents"] = Atlas::Message::ListType(1, typeinfo->getName());
	msg["orientation"] = orientation.toAtlas();

	c->setArgsAsList(Atlas::Message::ListType(1, msg));
	mConn->send(c);
	std::stringstream ss;
	ss << pos;
	S_LOG_INFO("Try to create entity of type " << typeinfo->getName() << " at position " << ss.str() );

}
}

}
;
