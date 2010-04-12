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

namespace EmberOgre
{

OgreResourceLoader::OgreResourceLoader() :
		UnloadUnusedResources("unloadunusedresources", this, "Unloads any unused resources."), mLoadRecursive(false)
{
	mFileSystemArchiveFactory = new ::EmberOgre::FileSystemArchiveFactory();
	Ogre::ArchiveManager::getSingleton().addArchiveFactory(mFileSystemArchiveFactory);
}

OgreResourceLoader::~OgreResourceLoader()
{
	delete mFileSystemArchiveFactory;
}

void OgreResourceLoader::initialize()
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();

	///check from the config if we should load media recursively
	///this is needed for most authoring, since it allows us to find all meshes before they are loaded
	if (configSrv->itemExists("media", "loadmediarecursive")) {
		mLoadRecursive = (bool)configSrv->getValue("media", "loadmediarecursive");
	}

	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("media", "extraresourcelocations")) {
		varconf::Variable resourceConfigFilesVar = Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("media", "extraresourcelocations");
		std::string resourceConfigFiles = resourceConfigFilesVar.as_string();
		Ember::Tokeniser configFilesTokeniser(resourceConfigFiles, ";");
		while (configFilesTokeniser.hasRemainingTokens()) {
			std::string rawPath = configFilesTokeniser.nextToken();
			Ember::Tokeniser pathTokeniser(rawPath, "|");
			std::string group = pathTokeniser.nextToken();
			std::string path = pathTokeniser.nextToken();
			if (group != "" && path != "") {
				mExtraResourceLocations.insert(ResourceLocationsMap::value_type(group, path));
			}
		}
	}

	///load the resource file
	const std::string configPath(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory() + "/resources.cfg");
	S_LOG_VERBOSE("Loading resources definitions from " << configPath);
	mConfigFile.load(configPath);
}

unsigned int OgreResourceLoader::numberOfSections()
{
	unsigned int numberOfSections = 0;
	Ogre::ConfigFile::SectionIterator I = mConfigFile.getSectionIterator();
	while (I.hasMoreElements()) {
		numberOfSections++;
		I.moveNext();
	}
	return numberOfSections - 1;
}

void OgreResourceLoader::runCommand(const std::string &command, const std::string &args)
{
	if (UnloadUnusedResources == command) {
		Ember::TimedLog l("Unload unused resources.");
		Ogre::ResourceGroupManager& resourceGroupManager(Ogre::ResourceGroupManager::getSingleton());

		Ogre::StringVector resourceGroups = resourceGroupManager.getResourceGroups();
		for (Ogre::StringVector::const_iterator I = resourceGroups.begin(); I != resourceGroups.end(); ++I) {
			resourceGroupManager.unloadUnreferencedResourcesInGroup(*I, false);
		}
	}
}


bool OgreResourceLoader::addSharedMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive)
{
	static const std::string& sharedMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedMediaDirectory();

	bool foundDir = false;
	std::string finalPath(sharedMediaPath + path);

	S_LOG_INFO("Looking for " << finalPath);
	if (isExistingDir(finalPath)) {
		S_LOG_INFO("Adding dir " << finalPath);
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(finalPath, type, section, recursive);
			foundDir = true;
			mResourceLocations.insert(std::make_pair(section, finalPath));
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Couldn't load " << sharedMediaPath << path << "." << ex);
		}
	}

	return foundDir;
}

bool OgreResourceLoader::addUserMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive)
{
	static const std::string& userMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getUserMediaDirectory();
	static const std::string& emberMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getEmberMediaDirectory();

	bool foundDir = false;
	std::string finalPath(userMediaPath + path);

	S_LOG_VERBOSE("Looking for " << finalPath);
	if (isExistingDir(finalPath)) {
		S_LOG_VERBOSE("Adding dir " << finalPath);
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(finalPath, type, section, recursive);
			foundDir = true;
			mResourceLocations.insert(std::make_pair(section, finalPath));
		} catch (const std::exception&) {
			///don't report anything
		}
	}

	///try with ember-media
	finalPath = emberMediaPath + path;
	S_LOG_VERBOSE("Looking for " << finalPath);
	if (isExistingDir(finalPath)) {
		S_LOG_VERBOSE("Adding dir " << finalPath);
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(finalPath, type, section, recursive);
			foundDir = true;
			mResourceLocations.insert(std::make_pair(section, finalPath));
		} catch (const std::exception&) {
			S_LOG_FAILURE("Couldn't load " << emberMediaPath << path << ". Continuing as if nothing happened.");
		}
	}
	return foundDir;
}

void OgreResourceLoader::loadBootstrap()
{
	loadSection("Bootstrap");
}

void OgreResourceLoader::loadGui()
{
	loadSection("Gui");
}

void OgreResourceLoader::loadGeneral()
{
	loadSection("General");
	loadSection("ModelDefinitions");
	loadSection("SoundDefinitions");

	loadAllUnloadedSections();

	///out of pure interest we'll print out how many modeldefinitions we've loaded
	Ogre::ResourceManager::ResourceMapIterator I = Model::ModelDefinitionManager::getSingleton().getResourceIterator();
	int count = 0;
	while (I.hasMoreElements()) {
		++count;
		I.moveNext();
	}

	S_LOG_INFO("Finished loading " << count << " modeldefinitions.");
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

void OgreResourceLoader::loadSection(const std::string& sectionName)
{
	if (sectionName != "" && std::find(mLoadedSections.begin(), mLoadedSections.end(), sectionName) == mLoadedSections.end()) {
		bool mediaAdded = false;

		S_LOG_VERBOSE("Adding resource section " << sectionName);
		// 	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(sectionName);

		///Start with adding any extra defined locations.
		ResourceLocationsMap::const_iterator extraI = mExtraResourceLocations.lower_bound(sectionName);
		ResourceLocationsMap::const_iterator extraIend = mExtraResourceLocations.upper_bound(sectionName);
		while (extraI != extraIend) {
			S_LOG_VERBOSE("Adding dir " << extraI->second);
			try {
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(extraI->second, "EmberFileSystem", sectionName, mLoadRecursive);
			} catch (const Ogre::Exception&) {
				S_LOG_FAILURE("Couldn't load " + extraI->second + ". Continuing as if nothing happened.");
			}
			extraI++;
		}

		///We want to make sure that "user" media always is loaded before "shared" media. Unfortunately the Ogre settings iterator doesn't return the settings in the order they are defined in resource.cfg so we can't rely on the order that the settings are defined, and instead need to first add the settings to two different vectors, and then iterate through the user vector first.
		std::vector<std::pair<std::string, std::string> > userPlaces;
		std::vector<std::pair<std::string, std::string> > sharedPlaces;

		Ogre::ConfigFile::SettingsIterator I = mConfigFile.getSettingsIterator(sectionName);
		std::string finalTypename;
		while (I.hasMoreElements()) {
			//Ogre::ConfigFile::SettingsMultiMap J = I.getNext();
			const std::string& typeName = I.peekNextKey();
			const std::string& archName = I.peekNextValue();

			finalTypename = typeName.substr(0, typeName.find("["));

			if (Ogre::StringUtil::endsWith(typeName, "[shared]")) {
				sharedPlaces.push_back(std::pair<std::string, std::string>(finalTypename, archName));
			} else {
				userPlaces.push_back(std::pair<std::string, std::string>(finalTypename, archName));
			}
			I.moveNext();
		}

		///We've now filled the vectors, start by adding all the user media.
		for (std::vector<std::pair<std::string, std::string> >::iterator I = userPlaces.begin(); I != userPlaces.end(); ++I) {
			mediaAdded |= addUserMedia(I->second, I->first, sectionName, mLoadRecursive);
		}
		for (std::vector<std::pair<std::string, std::string> >::iterator I = sharedPlaces.begin(); I != sharedPlaces.end(); ++I) {
			mediaAdded |= addSharedMedia(I->second, I->first, sectionName, mLoadRecursive);
		}

		mLoadedSections.push_back(sectionName);

		///only initialize the resource group if it has media
		if (mediaAdded) {
			try {
				Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(sectionName);
				/*			} catch (const Ogre::ItemIdentityException& ex) {
				 if (ex.getNumber() == Ogre::ERR_DUPLICATE_ITEM) {
				 const std::string& message(ex.getDescription());
				 size_t pos = std::string("Resource with the name ").length();
				 std::string resourceName = message.substr(pos, message.find_first_of(' ', pos) - pos);

				 }
				 */
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("An error occurred when loading media from section '" << sectionName << "'." << ex);
			} catch (...) {
				S_LOG_FAILURE("An unknown error occurred when loading media from section '" << sectionName << "'.");
			}
		}
	}
}

void OgreResourceLoader::loadAllUnloadedSections()
{
	S_LOG_VERBOSE("Now loading all unloaded sections.");
	Ogre::ConfigFile::SectionIterator I = mConfigFile.getSectionIterator();
	while (I.hasMoreElements()) {
		const std::string& sectionName = I.peekNextKey();
		loadSection(sectionName);
		I.moveNext();
	}

}

bool OgreResourceLoader::isExistingDir(const std::string& path) const
{
	bool exists = false;
	oslink::directory osdir(path);
	exists = osdir.isExisting();
	if (!exists) {
		///perhaps it's a file?
		std::ifstream fin(path.c_str(), std::ios::in);
		exists = !fin.fail();
	}
	return exists;
}

const OgreResourceLoader::ResourceLocationsMap& OgreResourceLoader::getResourceLocations() const
{
	return mResourceLocations;
}


}
