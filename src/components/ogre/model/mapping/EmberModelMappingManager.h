//
// C++ Interface: EmberModelMappingManager
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
#ifndef EMBEROGREEMBERMODELMAPPINGMANAGER_H
#define EMBEROGREEMBERMODELMAPPINGMANAGER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgreResourceManager.h>
#include "framework/Singleton.h"

#include "ModelMappingManager.h"
#include "XMLModelMappingDefinitionSerializer.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

class EmberDummyActionCreator : public IActionCreator
{
public:
	virtual void createActions(ModelMapping& modelMapping, CaseBase* aCase, CaseDefinition& caseDefinition);

};





/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class EmberModelMappingManager : 
public Ogre::ResourceManager, 
public Ember::Singleton<EmberModelMappingManager>,  
public sigc::trackable
{
public:
    EmberModelMappingManager();

    ~EmberModelMappingManager();
    
    inline ModelMappingManager& getManager();

/*	virtual Ogre::ResourcePtr create(const Ogre::String& name, const Ogre::String& group, 
		bool isManual = false, Ogre::ManualResourceLoader* loader = 0, 
		const Ogre::NameValuePairList* createParams = 0);*/
	
	virtual void parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName);

protected:
	ModelMappingManager mModelMappingManager;
	XMLModelMappingDefinitionSerializer mXmlSerializer;
	
	virtual Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
        const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
        const Ogre::NameValuePairList* createParams);

	void ServerService_GotConnection(Eris::Connection* connection);
// 	EmberDummyActionCreator mActionCreator;

};

ModelMappingManager& EmberModelMappingManager::getManager()
{
	return mModelMappingManager;
}

}
}
}

#endif
