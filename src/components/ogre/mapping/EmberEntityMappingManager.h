//
// C++ Interface: EmberEntityMappingManager
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
#ifndef EMBEROGREEMBERMODELMAPPINGMANAGER_H
#define EMBEROGREEMBERMODELMAPPINGMANAGER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgreResourceManager.h>
#include "framework/Singleton.h"

#include "components/entitymapping/EntityMappingManager.h"
#include "XMLEntityMappingDefinitionSerializer.h"

namespace Ember {
namespace OgreView {

/**
 * @brief Namespace for the EmberOgre specific integration with the Entity Mapping framework.
 */
namespace Mapping {


/**
@brief Uses the Ogre::ResourceManager framework to provide managing of model mapping definitions.

@author Erik Ogenvik <erik@ogenvik.org>
*/
class EmberEntityMappingManager :
public Ogre::ResourceManager,
public Singleton<EmberEntityMappingManager>,
public virtual sigc::trackable
{
public:
	EmberEntityMappingManager();

	~EmberEntityMappingManager() override;

	/**
	Accessor for the main EntityMappingManager instance.
	*/
	EntityMapping::EntityMappingManager& getManager();

	/**
	Method called by Ogre. Will parse the script supplied in the stream object.
	*/
	void parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName) override;

protected:
	/**
	Internal instance of the EntityMappingManager.
	*/
	EntityMapping::EntityMappingManager mEntityMappingManager;

	/**
	Serializer for xml.
	*/
	XMLEntityMappingDefinitionSerializer mXmlSerializer;

	/**
	Internal method called by Ogre.
	*/
	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
		const Ogre::NameValuePairList* createParams) override;

	/**
	When we get a connection, set the TypeService on the mEntityMappingManager instance.
	*/
	void ServerService_GotConnection(Eris::Connection* connection);

};

inline EntityMapping::EntityMappingManager& EmberEntityMappingManager::getManager()
{
	return mEntityMappingManager;
}

}
}

}

#endif
