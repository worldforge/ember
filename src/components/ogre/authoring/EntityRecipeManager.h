//
// C++ Interface: EntityRecipeManager
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
#ifndef EMBEROGREENTITYRECIPEMANAGER_H
#define EMBEROGREENTITYRECIPEMANAGER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgreResourceManager.h>
#include "framework/Singleton.h"
#include "XMLEntityRecipeSerializer.h"

namespace EmberOgre
{
namespace Authoring
{
/**
 * Resource manager for entity recipes.
 */
class EntityRecipeManager: public Ogre::ResourceManager, public Ember::Singleton<EntityRecipeManager>
{
public:
	/**
	 * Constructor
	 */
	EntityRecipeManager();

	/**
	 * Destructor
	 */
	virtual ~EntityRecipeManager();

	/**
	 * Creates new resource.
	 */
	virtual Ogre::ResourcePtr create(const Ogre::String& name, const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0, const Ogre::NameValuePairList* createParams = 0);

	/**
	 * Parse a script file.
	 */
	virtual void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);

protected:
	/**
	 Serializer for xml.
	 */
	XMLEntityRecipeSerializer mXmlSerializer;

	/**
	 * Create a new resource instance compatible with this manager.
	 */
	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* createParams);
};

}
}
#endif
