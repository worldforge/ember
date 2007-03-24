//
// C++ Implementation: EmberModelMappingManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#include "EmberModelMappingManager.h"
#include "framework/tinyxml/tinyxml.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "components/ogre/XMLHelper.h"

#include <Eris/Connection.h>

using namespace EmberOgre::Model::Mapping;


template<> EmberOgre::Model::Mapping::EmberModelMappingManager* Ember::Singleton<EmberOgre::Model::Mapping::EmberModelMappingManager>::ms_Singleton = 0;

namespace EmberOgre {

namespace Model {

namespace Mapping {

EmberModelMappingManager::EmberModelMappingManager() : mModelMappingManager(), mXmlSerializer(mModelMappingManager)
{
    mLoadOrder = 300.0f;
    mResourceType = "ModelMappingDefinition";
        
	mScriptPatterns.push_back("*.modelmap");
	mScriptPatterns.push_back("*.modelmap.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EmberModelMappingManager::ServerService_GotConnection));
	
	
}


EmberModelMappingManager::~EmberModelMappingManager()
{
}


void EmberModelMappingManager::parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
	Ember::TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return;
	}

	mXmlSerializer.parseScript(xmlDoc);
}

Ogre::Resource* EmberModelMappingManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
    const Ogre::NameValuePairList* createParams)
{
	return 0;
}


void EmberModelMappingManager::ServerService_GotConnection(Eris::Connection* connection) {
	mModelMappingManager.setTypeService(connection->getTypeService());
}




}

}

}
