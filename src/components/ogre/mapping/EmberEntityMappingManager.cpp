//
// C++ Implementation: EmberEntityMappingManager
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberEntityMappingManager.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "components/ogre/XMLHelper.h"

#include <Eris/Connection.h>

using namespace Ember::EntityMapping;



namespace Ember {
namespace OgreView {

namespace Mapping {

EmberEntityMappingManager::EmberEntityMappingManager() : mEntityMappingManager(), mXmlSerializer(mEntityMappingManager)
{
    mLoadOrder = 300.0f;
    mResourceType = "EntityMappingDefinition";

	mScriptPatterns.emplace_back("*.entitymap");
	mScriptPatterns.emplace_back("*.entitymap.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);

	EmberServices::getSingleton().getServerService().GotConnection.connect(sigc::mem_fun(*this, &EmberEntityMappingManager::ServerService_GotConnection));


}


EmberEntityMappingManager::~EmberEntityMappingManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}


void EmberEntityMappingManager::parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return;
	}

	mXmlSerializer.parseScript(xmlDoc, stream->getName());
}

Ogre::Resource* EmberEntityMappingManager::createImpl(const Ogre::String&, Ogre::ResourceHandle,
    const Ogre::String&, bool, Ogre::ManualResourceLoader*,
    const Ogre::NameValuePairList*)
{
	return nullptr;
}


void EmberEntityMappingManager::ServerService_GotConnection(Eris::Connection* connection) {
	mEntityMappingManager.setTypeService(&connection->getTypeService());
}




}

}
}
