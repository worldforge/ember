/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef LODDEFINITIONMANAGER_H
#define LODDEFINITIONMANAGER_H

#include "LodDefinition.h"
#include "XMLLodDefinitionSerializer.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/Singleton.h"

#include <OgreResourceManager.h>

#include <string>
#include <boost/filesystem/path.hpp>

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
	LodDefinitionManager(const boost::filesystem::path& exportDirectory);

	/**
	 * @brief Dtor.
	 */
	~LodDefinitionManager() override;

	/// Create a new LodDefinition
	/// @see ResourceManager::createResource
	LodDefinitionPtr create (const Ogre::String& name, const Ogre::String& group,
			bool isManual = false, Ogre::ManualResourceLoader* loader = 0,
			const Ogre::NameValuePairList* createParams = 0);

	/**
	 * @brief Instantiates the LodDefinition. Parameters are passed directly to LodDefinition ctor.
	 *
	 * This is called from Ogre::ResourceManager. You should not call this!
	 */
	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
	                           const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
	                           const Ogre::NameValuePairList* createParams);

	/// Get a LodDefinition by name
	/// @see ResourceManager::getResourceByName
	LodDefinitionPtr getByName(const Ogre::String& name, const Ogre::String& groupName = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

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
