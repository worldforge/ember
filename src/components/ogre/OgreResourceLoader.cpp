//
// C++ Implementation: OgreResourceLoader
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "model/ModelDefinitionManager.h"

#include "EmberOgreFileSystem.h"

#include "framework/osdir.h"
#include <fstream>

namespace EmberOgre {

OgreResourceLoader::OgreResourceLoader() : mLoadRecursive(false)
{
	mFileSystemArchiveFactory = new ::EmberOgre::FileSystemArchiveFactory();
	Ogre::ArchiveManager::getSingleton().addArchiveFactory( mFileSystemArchiveFactory );

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
    
// 	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());
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


bool OgreResourceLoader::addSharedMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive)
{
	static const std::string& sharedMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedMediaDirectory();

	bool foundDir = false;

	S_LOG_INFO("Looking for " << sharedMediaPath + path);
	if (isExistingDir(sharedMediaPath + path)) {
		S_LOG_INFO("Adding dir " << sharedMediaPath + path);
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				sharedMediaPath + path, type, section, recursive);
			foundDir = true;
		} catch (const Ogre::Exception& ex) {
			const std::string& message = ex.getFullDescription();
			S_LOG_FAILURE("Couldn't load " + sharedMediaPath + path + ". Error: "<< message);
		}
	}
	
	return foundDir;
}

bool OgreResourceLoader::addUserMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive)
{
	static const std::string& userMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getUserMediaDirectory();
	static const std::string& emberMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getEmberMediaDirectory();
	
	bool foundDir = false;
	
	S_LOG_VERBOSE("Looking for " << userMediaPath + path);
	if (isExistingDir(userMediaPath + path)) {
		S_LOG_VERBOSE("Adding dir " << userMediaPath + path);
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				userMediaPath + path, type, section, recursive);
			foundDir = true;
		} catch (const Ogre::Exception&) {
			///don't report anything
		}
	}
	
	///try with ember-media
	S_LOG_VERBOSE("Looking for " << emberMediaPath + path);
	if (isExistingDir(emberMediaPath + path)) {
		S_LOG_VERBOSE("Adding dir " << emberMediaPath + path);
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				emberMediaPath + path, type, section, recursive);
			foundDir = true;
		} catch (const Ogre::Exception&) {
			S_LOG_FAILURE("Couldn't load " + emberMediaPath + path + ". Continuing as if nothing happened.");
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
	try {
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("General");
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("An error occurred when preloading media. Message:\n\t"<< ex.getFullDescription());
	}
	try {
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Gui");
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("An error occurred when preloading media. Message:\n\t"<< ex.getFullDescription());
	}
	try {
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("ModelDefinitions");
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("An error occurred when preloading media. Message:\n\t"<< ex.getFullDescription());
	}


// 	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
// 	
// 
// 	std::vector<std::string> shaderTextures;
// 	
// 	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "rock")));
// 	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "sand")));
// 	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "grass")));
// 	
// 	for (std::vector<std::string>::iterator I = shaderTextures.begin(); I != shaderTextures.end(); ++I) {
// 		try {
// 			Ogre::TextureManager::getSingleton().load(*I, "General");
// 		} catch (const Ogre::Exception& e) {
// 			S_LOG_FAILURE( "Error when loading texture " << *I )
// 		}
// 	}	
}

void OgreResourceLoader::loadSection(const std::string& sectionName)
{
	if (sectionName != "" && std::find(mLoadedSections.begin(), mLoadedSections.end(), sectionName) == mLoadedSections.end())
	{
		bool mediaAdded = false;

		S_LOG_VERBOSE("Adding resource section " << sectionName);
	// 	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(sectionName);
		
		Ogre::ConfigFile::SettingsIterator I = mConfigFile.getSettingsIterator(sectionName);
		std::string finalTypename;
		while (I.hasMoreElements()) {
			//Ogre::ConfigFile::SettingsMultiMap J = I.getNext();
			const std::string& typeName = I.peekNextKey();
			const std::string& archName = I.peekNextValue();
	
			finalTypename = typeName.substr(0, typeName.find("["));
			if (Ogre::StringUtil::endsWith(typeName, "[shared]")) {
				mediaAdded |= addSharedMedia(archName, finalTypename, sectionName, mLoadRecursive);
			} else {
				mediaAdded |= addUserMedia(archName, finalTypename, sectionName, mLoadRecursive);
			}
			I.moveNext();
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
			} catch (const Ogre::Exception& ex) {
				S_LOG_FAILURE("An error occurred when loading media from section '" << sectionName << "'. Message:\n\t"<< ex.getFullDescription());
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("An error occurred when loading media from section '" << sectionName << "'. Message:\n\t"<< ex.what());
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
		std::ifstream fin(path.c_str() , std::ios::in );
		exists = !fin.fail();
	}
	return exists;
}


}
