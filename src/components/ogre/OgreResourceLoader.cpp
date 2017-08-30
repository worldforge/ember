//
// C++ Implementation: OgreResourceLoader
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreResourceLoader.h"
#include "services/EmberServices.h"
#include "framework/Tokeniser.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "model/ModelDefinitionManager.h"
#include "sound/XMLSoundDefParser.h"

#include "EmberOgreFileSystem.h"

#include "framework/osdir.h"
#include "framework/TimedLog.h"
#include <OgreArchiveManager.h>
#include <Ogre.h>
#include <boost/algorithm/string.hpp>
#include <framework/FileSystemObserver.h>
#include <framework/MainLoopController.h>

namespace Ember {
namespace OgreView {

struct EmberResourceLoadingListener : public Ogre::ResourceLoadingListener {
	Ogre::DataStreamPtr resourceLoading(const Ogre::String& name, const Ogre::String& group, Ogre::Resource* resource) override {
		return Ogre::DataStreamPtr();
	}

	void resourceStreamOpened(const Ogre::String& name, const Ogre::String& group, Ogre::Resource* resource, Ogre::DataStreamPtr& dataStream) override {

	}

	bool resourceCollision(Ogre::Resource* resource, Ogre::ResourceManager* resourceManager) override {
		S_LOG_VERBOSE("Resource '" << resource->getName() << "' already exists in group '" << resource->getGroup() << "' for type '" << resourceManager->getResourceType() << "'.");
		if (resourceManager->getResourceType() == "Material") {

			//If a material, update the old version once the new one has been compiled (hence the need for "runOnMainThread".
			//Note that this only works the first time a material is updated.
			Ogre::MaterialPtr existingMaterial = Ogre::MaterialManager::getSingleton().getByName(resource->getName(), resource->getGroup());

			MainLoopController::getSingleton().getEventService().runOnMainThread([=]() {
				Ogre::MaterialPtr oldMat = existingMaterial;
				Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(resource->getName(), resource->getGroup());
				mat->copyDetailsTo(oldMat);
			});
		}

		resourceManager->remove(resource->getName(), resource->getGroup());
		return true;
	}
};

OgreResourceLoader::OgreResourceLoader() :
		UnloadUnusedResources("unloadunusedresources", this, "Unloads any unused resources."),
		mLoadRecursive(false) {
	mFileSystemArchiveFactory = new FileSystemArchiveFactory();
	Ogre::ArchiveManager::getSingleton().addArchiveFactory(mFileSystemArchiveFactory);
}

OgreResourceLoader::~OgreResourceLoader() {
	delete mFileSystemArchiveFactory;
	for (auto& path : mResourceRootPaths) {
		Ember::FileSystemObserver::getSingleton().remove_directory(path);
	}
}

void OgreResourceLoader::initialize() {

	mLoadingListener = new EmberResourceLoadingListener();
	Ogre::ResourceGroupManager::getSingleton().setLoadingListener(mLoadingListener);

	ConfigService& configSrv = EmberServices::getSingleton().getConfigService();

	//check from the config if we should load media recursively
	//this is needed for most authoring, since it allows us to find all meshes before they are loaded
	if (configSrv.itemExists("media", "loadmediarecursive")) {
		mLoadRecursive = (bool) configSrv.getValue("media", "loadmediarecursive");
	}

	if (EmberServices::getSingleton().getConfigService().itemExists("media", "extraresourcelocations")) {
		varconf::Variable resourceConfigFilesVar = EmberServices::getSingleton().getConfigService().getValue("media", "extraresourcelocations");
		std::string resourceConfigFiles = resourceConfigFilesVar.as_string();
		mExtraResourceLocations = Tokeniser::split(resourceConfigFiles, ";");
	}
}

void OgreResourceLoader::runCommand(const std::string& command, const std::string& args) {
	if (UnloadUnusedResources == command) {
		unloadUnusedResources();
	}
}

void OgreResourceLoader::unloadUnusedResources() {
	TimedLog l("Unload unused resources.");
	Ogre::ResourceGroupManager& resourceGroupManager(Ogre::ResourceGroupManager::getSingleton());

	Ogre::StringVector resourceGroups = resourceGroupManager.getResourceGroups();
	for (Ogre::StringVector::const_iterator I = resourceGroups.begin(); I != resourceGroups.end(); ++I) {
		resourceGroupManager.unloadUnreferencedResourcesInGroup(*I, false);
	}
}

bool OgreResourceLoader::addSharedMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive) {
	static const std::string& sharedMediaPath = EmberServices::getSingleton().getConfigService().getSharedDataDirectory();

	return addResourceDirectory(sharedMediaPath + path, type, section, recursive, true, true);
}

bool OgreResourceLoader::addUserMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive) {
	static const std::string& userMediaPath = EmberServices::getSingleton().getConfigService().getUserMediaDirectory();
	static const std::string& emberMediaPath = EmberServices::getSingleton().getConfigService().getEmberMediaDirectory();

	bool foundDir = addResourceDirectory(emberMediaPath + path, type, section, recursive, true);

	return addResourceDirectory(userMediaPath + path, type, section, recursive, false) || foundDir;
}

bool OgreResourceLoader::addResourceDirectory(const std::string& path, const std::string& type, const std::string& section, bool recursive, bool reportFailure, bool throwOnFailure) {
	if (isExistingDir(path)) {
		S_LOG_VERBOSE("Adding dir " << path);
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, type, section, recursive);
			mResourceRootPaths.emplace_back(path);
			observeDirectory(path);
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

void OgreResourceLoader::loadBootstrap() {
	//Add the "assets" directory, which contains most of the assets
	addSharedMedia("media/assets", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
	addUserMedia("media/assets", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
}

void OgreResourceLoader::loadGui() {
	addSharedMedia("gui", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
	addUserMedia("gui", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
}

void OgreResourceLoader::loadGeneral() {

	//Lua scripts
	addSharedMedia("scripting", "EmberFileSystem", "Scripting", true);
	addUserMedia("scripting", "EmberFileSystem", "Scripting", true);

	//Model definitions, terrain definitions, sound definitions and entity mappings
	addSharedMedia("data", "EmberFileSystem", "Data", true);
	addUserMedia("data", "EmberFileSystem", "Data", true);

	//The Caelum component
	addSharedMedia("media/assets_external/caelum", "EmberFileSystem", "Caelum", true);
	addUserMedia("media/assets_external/caelum", "EmberFileSystem", "Caelum", true);

	//Entity recipes
	addSharedMedia("entityrecipes", "EmberFileSystem", "EntityRecipes", true);
	addUserMedia("entityrecipes", "EmberFileSystem", "EntityRecipes", true);

	//End with adding any extra defined locations.
	for (auto& location : mExtraResourceLocations) {
		addResourceDirectory(location, "EmberFileSystem", "Extra", mLoadRecursive, false);
	}
}

void OgreResourceLoader::preloadMedia() {
	// resource groups to be loaded
	const char* resourceGroup[] = {"General", "Data"};

	for (auto& group : resourceGroup) {
		try {
			Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(group);
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("An error occurred when preloading media." << ex);
		}
	}
}


bool OgreResourceLoader::isExistingDir(const std::string& path) const {
	oslink::directory osdir(path);
	bool exists = osdir.isExisting();
	if (!exists) {
		//perhaps it's a file?
		std::ifstream fin(path.c_str(), std::ios::in);
		exists = !fin.fail();
	}
	return exists;
}

void OgreResourceLoader::observeDirectory(const std::string& path) {
	FileSystemObserver::getSingleton().add_directory(path, [this](const FileSystemObserver::FileSystemEvent& event) {
		auto& ev = event.ev;
		S_LOG_VERBOSE("Resource changed " << ev.path.string() << " " << ev.type_cstr());

		if (ev.type == boost::asio::dir_monitor_event::modified) {
			if (boost::filesystem::file_size(ev.path) == 0) {
				return;
			}
		}


		auto resourceGroups = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
		for (auto& group : resourceGroups) {

			auto reloadResource = [&](Ogre::ResourceManager& resourceManager, const std::string& resourceName) {
				if (resourceManager.resourceExists(resourceName, group)) {
					auto resource = resourceManager.getResourceByName(resourceName, group);
					if (resource->isLoaded() || resource->isPrepared()) {
						try {
							resource->reload();
						} catch (const std::exception& e) {
							S_LOG_FAILURE("Could not reload resource '" << resourceName << "' of type '" << resourceManager.getResourceType() << "'." << e);
						}
					}
				} else {
					//Add resource
					Ogre::ResourceGroupManager::getSingleton().declareResource(resourceName, resourceManager.getResourceType(), group);
					resourceManager.createResource(resourceName, group);
				}

			};

			auto locations = Ogre::ResourceGroupManager::getSingleton().listResourceLocations(group);
			for (auto& location : *locations) {
				if (boost::starts_with(ev.path.string(), location)) {
					std::string relative = ev.path.string().substr(location.length() + 1);
					auto extension = ev.path.extension();

					if (extension == ".material") {
						std::ifstream stream(ev.path.string());
						Ogre::SharedPtr<Ogre::DataStream> fileStream(OGRE_NEW Ogre::FileStreamDataStream(&stream, false));
						Ogre::MaterialManager::getSingleton().parseScript(fileStream, group);
					} else if (extension == ".dds" || extension == ".png" || extension == ".jpg") {
						reloadResource(Ogre::TextureManager::getSingleton(), relative);
					} else if (extension == ".mesh") {
						reloadResource(Ogre::MeshManager::getSingleton(), relative);
					}
				}
			}
		}
	});
}

}
}
