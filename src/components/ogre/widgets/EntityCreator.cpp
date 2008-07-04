//
// C++ Implementation: EntityCreator
//
// Description: 
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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

#include "EntityCreator.h"
#include "../GUIManager.h"
#include "components/ogre/widgets/adapters/atlas/AdapterFactory.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include <Atlas/Message/Element.h>
#include <wfmath/atlasconv.h>
#include <Eris/TypeInfo.h>
#include "../MathConverter.h"

#include "../EmberOgre.h"
#include "../Avatar.h"

#include "../EmberPhysicalEntity.h"
#include "../AvatarEmberEntity.h"

namespace EmberOgre {

namespace Gui {

EntityCreator::EntityCreator()
{
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EntityCreator::connectedToServer));
}

EntityCreator::~EntityCreator()
{

}

void EntityCreator::showRecipe(EntityRecipe& recipe, CEGUI::Window* container)
{
	GUIManager& mGuiManager = GUIManager::getSingleton();

	GUIAdaptersStore::iterator I = recipe.getGUIAdaptersIteratorBegin();
	GUIAdaptersStore::iterator end = recipe.getGUIAdaptersIteratorEnd();
	for (; I != end; I++)
	{
		S_LOG_VERBOSE("Creating adapter " << I->first << " of type " << I->second->getType());

		CEGUI::Window* window = mGuiManager.createWindow("DefaultGUISheet");
		container->addChildWindow(window);

		I->second->attach(window);
	}
}

void EntityCreator::createEntity(EntityRecipe& recipe)
{
	Eris::TypeInfo* typeinfo = mConn->getTypeService()->getTypeByName(recipe.getEntityType());

	Atlas::Objects::Operation::Create c;
	AvatarEmberEntity* avatar = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity();
	c->setFrom(avatar->getId());
	///if the avatar is a "creator", i.e. and admin, we will set the TO property
	///this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
	if (avatar->getType()->isA(mConn->getTypeService()->getTypeByName("creator"))) {
		c->setTo(avatar->getId());
	}
	
	Atlas::Message::MapType msg = recipe.createEntity();
	msg["loc"] = avatar->getLocation()->getId();
	
	Ogre::Vector3 o_vector(2,0,0);
	Ogre::Vector3 o_pos = avatar->getSceneNode()->getPosition() + (avatar->getSceneNode()->getOrientation() * o_vector);
	
// 	WFMath::Vector<3> vector(0,2,0);
// 	WFMath::Point<3> pos = avatar->getPosition() + (avatar->getOrientation() * vector);
	WFMath::Point<3> pos = Ogre2Atlas(o_pos);
	WFMath::Quaternion orientation = avatar->getOrientation();
	
	msg["pos"] = pos.toAtlas();
	/*
	if (mName->getText().length() > 0) {
		msg["name"] = mName->getText().c_str();
	} else {
	*/
		msg["name"] = typeinfo->getName();
	/*
	}
	*/
	msg["parents"] = Atlas::Message::ListType(1, typeinfo->getName());
	msg["orientation"] = orientation.toAtlas();
	
	c->setArgsAsList(Atlas::Message::ListType(1, msg));
	mConn->send(c);
	std::stringstream ss;
	ss << pos;
	S_LOG_INFO("Trying to create entity of type " << typeinfo->getName() << " at position " << ss.str() );
}

void EntityCreator::connectedToServer(Eris::Connection* conn)
{
	mConn = conn;
}

}

}
