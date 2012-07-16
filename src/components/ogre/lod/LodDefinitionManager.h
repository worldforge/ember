/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef LODDEFINITIONMANAGER_H
#define LODDEFINITIONMANAGER_H

#include "LodDefinition.h"
#include "XMLLodDefinitionSerializer.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/Singleton.h"

#include <OgreResourceManager.h>

#include <string>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

/**
 * @brief LodDefinitionManager is managing all *.loddef resource files.
 */
class LodDefinitionManager :
	public Ember::Singleton<LodDefinitionManager>,
	public Ogre::ResourceManager
{
public:
	/**
	 * @brief Ctor.
	 * @param exportDirectory Specifies the directory, where the changed *.loddef files will be saved.
	 */
	LodDefinitionManager(const std::string& exportDirectory);

	/**
	 * @brief Dtor.
	 */
	virtual ~LodDefinitionManager();

	/**
	 * @brief Instantiates the LodDefinition. Parameters are passed directly to LodDefinition ctor.
	 *
	 * This is called from Ogre::ResourceManager. You should not call this!
	 */
	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
	                           const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
	                           const Ogre::NameValuePairList* createParams);

	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);

	/**
	 * @brief Exports a LodDefinition to a file.
	 * 
	 * The definition will be serialized and saved to a file by the same name of the definition.
	 * 
	 * @param definition The definition to export.
	 * @return The path to the exported script. If the export failed, the string will be empty.
	 */
	void exportScript(std::string meshName, LodDefinitionPtr definition);

	/**
	 * @brief Returns an instance of the default Serializer.
	 */
	const XMLLodDefinitionSerializer& getSerializer() const;

private:
	const XMLLodDefinitionSerializer mLodDefinitionSerializer;
};

inline const XMLLodDefinitionSerializer& LodDefinitionManager::getSerializer() const
{
	return mLodDefinitionSerializer;
}

}
}
}
#endif // ifndef LODDEFINITIONMANAGER_H
