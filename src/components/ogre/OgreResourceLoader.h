//
// C++ Interface: OgreResourceLoader
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006,
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
namespace Ember {
namespace OgreView {

class FileSystemArchiveFactory;

/**
@author Erik Ogenvik
@author Rômulo Fernandes

@brief Loads resources into the Ogre resource system.

The main role of this class is to define and load the resources into the Ogre resource system.

If a directory contains a file named "norecurse" (it can be empty) Ember won't recurse further into it
*/
class OgreResourceLoader : public ConsoleObject {
public:
	OgreResourceLoader();

	~OgreResourceLoader();

	void initialize();

	void loadBootstrap();
	void loadGui();
	void loadGeneral();

	void preloadMedia();

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
	const ConsoleCommandWrapper UnloadUnusedResources;

protected:
	bool mLoadRecursive;

	/**
	 * @brief A store of extra locations, as specified in config or command line.
	 */
	std::vector<std::string> mExtraResourceLocations;

	std::vector<std::string> mLoadedSections;

	FileSystemArchiveFactory* mFileSystemArchiveFactory;


	bool addUserMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive);
	bool addSharedMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive);

	bool isExistingDir(const std::string& path) const;

	/**
	 * @brief Adds a resource directory to the Ogre resource system.
	 * @param path File system path.
	 * @param type The type of archive.
	 * @param section The resource group to add it to.
	 * @param recursive Whether it should be searched recursively.
	 * @param reportFailure Whether any failures to find or add the path should be written to the log.
	 * @param throwOnFailure Throws an exception on failure.
	 * @return True if the path was successfully added.
	 */
	bool addResourceDirectory(const std::string& path, const std::string& type, const std::string& section, bool recursive, bool reportFailure, bool throwOnFailure = false);


};

}

}

#endif
