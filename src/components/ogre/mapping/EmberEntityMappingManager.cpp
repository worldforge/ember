//
// C++ Implementation: EmberEntityMappingManager
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include "EmberEntityMappingManager.h"
#include "framework/tinyxml/tinyxml.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "components/ogre/XMLHelper.h"

#include <Eris/Connection.h>

using namespace Ember::EntityMapping;


template<> EmberOgre::Mapping::EmberEntityMappingManager* Ember::Singleton<EmberOgre::Mapping::EmberEntityMappingManager>::ms_Singleton = 0;

namespace EmberOgre {

namespace Mapping {

EmberEntityMappingManager::EmberEntityMappingManager() : mEntityMappingManager(), mXmlSerializer(mEntityMappingManager)
{
    mLoadOrder = 300.0f;
    mResourceType = "EntityMappingDefinition";

	mScriptPatterns.push_back("*.entitymap");
	mScriptPatterns.push_back("*.entitymap.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);

	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EmberEntityMappingManager::ServerService_GotConnection));


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

	mXmlSerializer.parseScript(xmlDoc);
}

Ogre::Resource* EmberEntityMappingManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
    const Ogre::NameValuePairList* createParams)
{
	return 0;
}


void EmberEntityMappingManager::ServerService_GotConnection(Eris::Connection* connection) {
	mEntityMappingManager.setTypeService(connection->getTypeService());
}




}

}
