//
// C++ Implementation: OgreResourceLoader
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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

#include "OgreResourceLoader.h"
#include "services/EmberServices.h"
#include "framework/LoggingInstance.h"
#include "framework/Tokeniser.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "model/ModelDefinitionManager.h"
#include "sound/XMLSoundDefParser.h"

#include "EmberOgreFileSystem.h"

#include "framework/osdir.h"
#include "framework/TimedLog.h"
#include <OgreArchiveManager.h>
#include <OgreResourceGroupManager.h>
#include <fstream>

namespace Ember
{
namespace OgreView
{

OgreResourceLoader::OgreResourceLoader() :
		UnloadUnusedResources("unloadunusedresources", this, "Unloads any unused resources."), mLoadRecursive(false)
{
	mFileSystemArchiveFactory = new FileSystemArchiveFactory();
	Ogre::ArchiveManager::getSingleton().addArchiveFactory(mFileSystemArchiveFactory);
}

OgreResourceLoader::~OgreResourceLoader()
{
	delete mFileSystemArchiveFactory;
}

void OgreResourceLoader::initialize()
{
	ConfigService& configSrv = EmberServices::getSingleton().getConfigService();

	//check from the config if we should load media recursively
	//this is needed for most authoring, since it allows us to find all meshes before they are loaded
	if (configSrv.itemExists("media", "loadmediarecursive")) {
		mLoadRecursive = (bool)configSrv.getValue("media", "loadmediarecursive");
	}

	if (EmberServices::getSingleton().getConfigService().itemExists("media", "extraresourcelocations")) {
		varconf::Variable resourceConfigFilesVar = EmberServices::getSingleton().getConfigService().getValue("media", "extraresourcelocations");
		std::string resourceConfigFiles = resourceConfigFilesVar.as_string();
		Tokeniser configFilesTokeniser(resourceConfigFiles, ";");
		while (configFilesTokeniser.hasRemainingTokens()) {
			std::string rawPath = configFilesTokeniser.nextToken();
			Tokeniser pathTokeniser(rawPath, "|");
			std::string group = pathTokeniser.nextToken();
			std::string path = pathTokeniser.nextToken();
			if (group != "" && path != "") {
				mExtraResourceLocations.insert(ResourceLocationsMap::value_type(group, path));
			}
		}
	}
}

void OgreResourceLoader::runCommand(const std::string &command, const std::string &args)
{
	if (UnloadUnusedResources == command) {
		unloadUnusedResources();
	}
}

void OgreResourceLoader::unloadUnusedResources()
{
	TimedLog l("Unload unused resources.");
	Ogre::ResourceGroupManager& resourceGroupManager(Ogre::ResourceGroupManager::getSingleton());

	Ogre::StringVector resourceGroups = resourceGroupManager.getResourceGroups();
	for (Ogre::StringVector::const_iterator I = resourceGroups.begin(); I != resourceGroups.end(); ++I) {
		resourceGroupManager.unloadUnreferencedResourcesInGroup(*I, false);
	}
}

bool OgreResourceLoader::addSharedMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive)
{
	static const std::string& sharedMediaPath = EmberServices::getSingleton().getConfigService().getSharedMediaDirectory();

	return addResourceDirectory(sharedMediaPath + path, type, section, recursive, true, true);
}

bool OgreResourceLoader::addUserMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive)
{
	static const std::string& userMediaPath = EmberServices::getSingleton().getConfigService().getUserMediaDirectory();
	static const std::string& emberMediaPath = EmberServices::getSingleton().getConfigService().getEmberMediaDirectory();

	bool foundDir = false;

	//try with ember-media
	foundDir = addResourceDirectory(userMediaPath + path, type, section, recursive, true);

	return addResourceDirectory(emberMediaPath + path, type, section, recursive, false) || foundDir;
}

bool OgreResourceLoader::addResourceDirectory(const std::string& path, const std::string& type, const std::string& section, bool recursive, bool reportFailure, bool throwOnFailure)
{
	if (isExistingDir(path)) {
		S_LOG_VERBOSE("Adding dir " << path);
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, type, section, recursive);
			return true;
		} catch (const std::exception&) {
			if (throwOnFailure) {
				throw Ember::Exception(std::string("Could not load from required directory '") + path + "'. This is fatal and Ember will shut down. The probable cause for this error is that you haven't properly installed all required media.");
			}
			if (reportFailure) {
				S_LOG_FAILURE("Couldn't load " << path << ". Continuing as if nothing happened.");
			}
		}
	} else {
		if (throwOnFailure) {
			throw Ember::Exception(std::string("Could not find required directory '") + path + "'. This is fatal and Ember will shut down. The probable cause for this error is that you haven't properly installed all required media.");
		}
		if (reportFailure) {
			S_LOG_VERBOSE("Couldn't find resource directory " << path);
		}
	}
	return false;
}

void OgreResourceLoader::loadBootstrap()
{
	//Add the "assets" directory, which contains most of the assets
	addUserMedia("assets", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
	addSharedMedia("assets", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
}

void OgreResourceLoader::loadGui()
{
	addUserMedia("gui", "EmberFileSystem", "Gui", true);
	addSharedMedia("gui", "EmberFileSystem", "Gui", true);
}

void OgreResourceLoader::loadGeneral()
{
	//Start with adding any extra defined locations.
	for (auto& location : mExtraResourceLocations) {
		addResourceDirectory(location.second, "EmberFileSystem", location.first, mLoadRecursive, false);
	}

	//Lua scripts
	addUserMedia("scripting", "EmberFileSystem", "Scripting", true);
	addSharedMedia("scripting", "EmberFileSystem", "Scripting", true);

	//Model definitions, terrain definitions and entity mappings
	addUserMedia("data", "EmberFileSystem", "ModelDefinitions", true);
	addSharedMedia("data", "EmberFileSystem", "ModelDefinitions", true);

	//Sound definitions
	addUserMedia("sounddefinitions", "EmberFileSystem", "SoundDefinitions", true);
	addSharedMedia("sounddefinitions", "EmberFileSystem", "SoundDefinitions", true);

	//The Caelum component
	addUserMedia("assets_external/caelum", "EmberFileSystem", "Caelum", true);
	addSharedMedia("assets_external/caelum", "EmberFileSystem", "Caelum", true);

	//Entity recipes
	addUserMedia("entityrecipes", "EmberFileSystem", "EntityRecipes", true);
	addSharedMedia("entityrecipes", "EmberFileSystem", "EntityRecipes", true);

}

void OgreResourceLoader::preloadMedia()
{
	// resource groups to be loaded
	const char* resourceGroup[] = { "General", "Gui", "ModelDefinitions" };

	for (size_t i = 0; i < (sizeof(resourceGroup) / sizeof(const char*)); ++i) {
		try {
			Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(resourceGroup[i]);
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("An error occurred when preloading media." << ex);
		}
	}
}


bool OgreResourceLoader::isExistingDir(const std::string& path) const
{
	bool exists = false;
	oslink::directory osdir(path);
	exists = osdir.isExisting();
	if (!exists) {
		//perhaps it's a file?
		std::ifstream fin(path.c_str(), std::ios::in);
		exists = !fin.fail();
	}
	return exists;
}

}
}
