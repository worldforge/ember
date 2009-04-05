//
// C++ Interface: EmberModelMappingManager
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


/**
@brief Uses the Ogre::ResourceManager framework to provide managing of model mapping definitions.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class EmberModelMappingManager : 
public Ogre::ResourceManager, 
public Ember::Singleton<EmberModelMappingManager>,  
public sigc::trackable
{
public:
	EmberModelMappingManager();

	~EmberModelMappingManager();
    
	/**
	Accessor for the main ModelMappingManager instance.
	*/
	ModelMappingManager& getManager();

	/**
	Method called by Ogre. Will parse the script supplied in the stream object.
	*/
	virtual void parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName);

protected:
	/**
	Internal instance of the ModelMappingManager.
	*/
	ModelMappingManager mModelMappingManager;
	
	/**
	Serializer for xml.
	*/
	XMLModelMappingDefinitionSerializer mXmlSerializer;
	
	/**
	Internal method called by Ogre.
	*/
	virtual Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
		const Ogre::NameValuePairList* createParams);

	/**
	When we get a connection, set the TypeService on the mModelMappingManager instance.
	*/
	void ServerService_GotConnection(Eris::Connection* connection);

};

inline ModelMappingManager& EmberModelMappingManager::getManager()
{
	return mModelMappingManager;
}

}
}
}

#endif
