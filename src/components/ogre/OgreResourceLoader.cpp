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
#include "OgreResourceLoader.h"
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "model/ModelDefinitionManager.h"

namespace EmberOgre {

OgreResourceLoader::OgreResourceLoader() : mLoadRecursive(false)
{
}


OgreResourceLoader::~OgreResourceLoader()
{
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
    cf.load(configPath);

}

void OgreResourceLoader::addSharedMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive)
{
	static const std::string& sharedMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedMediaDirectory();

	if (isExistingDir(sharedMediaPath + path)) {
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				sharedMediaPath + path, type, section, recursive);
		} catch (const Ogre::Exception& ex) {
			const std::string& message = ex.getFullDescription();
			S_LOG_FAILURE("Couldn't load " + sharedMediaPath + path + ". Error: "<< message);
		}
	}

}

void OgreResourceLoader::addUserMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive)
{
	static const std::string& userMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getUserMediaDirectory();
	static const std::string& emberMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getEmberMediaDirectory();
	
	if (isExistingDir(userMediaPath + path)) {
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				userMediaPath + path, type, section, recursive);
		} catch (const Ogre::Exception&) {
			///don't report anything
		}
	}
	
	///try with ember-media
	if (isExistingDir(emberMediaPath + path)) {
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				emberMediaPath + path, type, section, recursive);
		} catch (const Ogre::Exception&) {
			S_LOG_FAILURE("Couldn't load " + emberMediaPath + path + ". Continuing as if nothing happened.");
		}
	}
}


void OgreResourceLoader::loadBootstrap()
{
	loadSection("Bootstrap");
	try {
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("An error occurred when loading media. Message:\n\t"<< ex.getFullDescription());
	}
    

}

void OgreResourceLoader::loadGui()
{
	loadSection("Gui");
	try {
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Gui");
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("An error occurred when loading media. Message:\n\t"<< ex.getFullDescription());
	}
}

void OgreResourceLoader::loadGeneral()
{
	loadSection("General");
	try {
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("An error occurred when loading media. Message:\n\t"<< ex.getFullDescription());
	}
	loadSection("ModelDefinitions");
	try {
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("ModelDefinitions");
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("An error occurred when loading media. Message:\n\t"<< ex.getFullDescription());
	}
	
	///out of pure interest we'll print out how many modeldefinitions we've loaded
	Ogre::ResourceManager::ResourceMapIterator I = Model::ModelDefinitionManager::getSingleton().getResourceIterator();
	int count = 0;
	while (I.hasMoreElements()) {
		I.moveNext();
		++count;
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
	S_LOG_VERBOSE("Adding resource section " << sectionName);
// 	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(sectionName);
	
	Ogre::ConfigFile::SettingsIterator I = cf.getSettingsIterator(sectionName);
	std::string finalTypename;
	while (I.hasMoreElements()) {
		//Ogre::ConfigFile::SettingsMultiMap J = I.getNext();
		const std::string& typeName = I.peekNextKey();
		const std::string& archName = I.peekNextValue();
		I.moveNext();
		finalTypename = typeName.substr(0, typeName.find("["));
		if (Ogre::StringUtil::endsWith(typeName, "[shared]")) {
			addSharedMedia(archName, finalTypename, sectionName, mLoadRecursive);
		} else {
			addUserMedia(archName, finalTypename, sectionName, mLoadRecursive);
		}
	}

}

bool OgreResourceLoader::isExistingDir(const std::string& path) const
{
	struct stat tagStat;
	return -1 != stat( path.c_str(), &tagStat );
}


}
