//
// C++ Interface: OgreResourceLoader
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006,
// Rômulo Fernandes <abra185@gmail.com>, (C) 2008
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
#ifndef EMBEROGREOGRERESOURCELOADER_H
#define EMBEROGREOGRERESOURCELOADER_H

#include "EmberOgrePrerequisites.h"
#include "framework/ConsoleObject.h"
#include <OgreConfigFile.h>
#include <map>
namespace EmberOgre {

class FileSystemArchiveFactory;

/**
@author Erik Hjortsberg
@author Rômulo Fernandes

@brief Loads resources into the Ogre resource system.

The main role of this class is to load the resource locations as defined in resources.cfg into the Ogre resource system.

The format for the resource.cfg file is:
Sections/resource groups are defined in brackets, such as [General] or [Gui].
Each entry in a resource group first specifies the type of resource container, which for now should in pretty much all cases be "EmberFileSystem".
The value in brackets following the type of location (for example "EmberFileSystem[user]") tells Ember in which places it should look.
 "user" means that Ember will look in these locations:
   * <ember home directory>/user-media. On a standard linux install this is ~/.ember/user-media . This directory allows the user to add his/her own media, thus overriding the default media. Everytime media is saved from within Ember (for example when editing a model through the Model Editor) the media is saved here.
   * <ember home directory>/<ember media dir>. The name of the media directory is set in ember.conf. In most cases this is "ember-media-<version>". So for version 0.5.2 it's "~/.ember/ember-media-0.5.2". The contents of this directory is copied from PREFIX/share/ember/media/user at installation time, and then updated through wfut every time. The idea with this directory is to let it be updated through wfut. The user should never edit this media since it will be overwritten by wfut. Use the "user-media" directory instead.
 "shared" means that Ember will look for the media in PREFIX/shared/ember/media/shared only. This is media that is static and shared by all users on the system, and which won't be updated through wfut.
If a directory contains a file named "norecurse" (it can be empty) Ember won't recurse further into it
*/
class OgreResourceLoader : public Ember::ConsoleObject {
public:
	typedef std::multimap<std::string, std::string> ResourceLocationsMap;

	OgreResourceLoader();

	~OgreResourceLoader();

	void initialize();

	void loadBootstrap();
	void loadGui();
	void loadGeneral();

	void preloadMedia();

	unsigned int numberOfSections();

	/**
	 * @brief Gets all resource locations.
	 * @returns All resource locations defined. The keys are the resource groups and the values the locations.
	 */
	const ResourceLocationsMap& getResourceLocations() const;

	/**
	 * @brief Tells Ogre to unload all unused resources, thus freeing up memory.
	 * @note Calling this might stall the engine a little.
	 */
	void unloadUnusedResources();

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	/**
	 * @brief Allows setting of the right hand attachment's orientation. This is mainly for debugging purposes and should removed once we get a better editor in place.
	 */
	const Ember::ConsoleCommandWrapper UnloadUnusedResources;

protected:
	bool mLoadRecursive;
	Ogre::ConfigFile mConfigFile;

	/**
	 * @brief A store of extra locations, as specified in config or command line.
	 */
	ResourceLocationsMap mExtraResourceLocations;

	std::vector<std::string> mLoadedSections;

	::EmberOgre::FileSystemArchiveFactory* mFileSystemArchiveFactory;

	/**
	 * @brief A map of all resource locations.
	 * The keys are the resource groups, and the values the locations.
	 */
	ResourceLocationsMap mResourceLocations;

	void loadSection(const std::string& sectionName);

	bool addUserMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive);
	bool addSharedMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive);

	bool isExistingDir(const std::string& path) const;

	void loadAllUnloadedSections();

};

}

#endif
