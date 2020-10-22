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

#include "framework/TimedLog.h"
#include <OgreArchiveManager.h>
#include <Ogre.h>
#include <boost/algorithm/string.hpp>
#include <framework/FileSystemObserver.h>
#include <framework/MainLoopController.h>
#include <components/ogre/model/XMLModelDefinitionSerializer.h>


/**
 * This method is licensed under the CC BY-SA 3.0 as it's taken from this StackOverflow answer: https://stackoverflow.com/a/29221546
 * Copyright 2017 Robert Massaioli, https://stackoverflow.com/users/83446/robert-massaioli
 *
 * It's provided since boost::filesystem::relative is only available in boost 1.60, and we want to support
 * distros that don't yet have that version.
 *
 * Provides a relative path.
 */
static boost::filesystem::path relativeTo(const boost::filesystem::path& from, const boost::filesystem::path& to) {
	// Start at the root path and while they are the same then do nothing then when they first
	// diverge take the entire from path, swap it with '..' segments, and then append the remainder of the to path.
	boost::filesystem::path::const_iterator fromIter = from.begin();
	boost::filesystem::path::const_iterator toIter = to.begin();

	// Loop through both while they are the same to find nearest common directory
	while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter)) {
		++toIter;
		++fromIter;
	}

	// Replace from path segments with '..' (from => nearest common directory)
	boost::filesystem::path finalPath;
	while (fromIter != from.end()) {
		finalPath /= "..";
		++fromIter;
	}

	// Append the remainder of the to path (nearest common directory => to)
	while (toIter != to.end()) {
		finalPath /= *toIter;
		++toIter;
	}

	return finalPath;
}

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
				oldMat->load();
			});
		}

		resourceManager->remove(resource->getName(), resource->getGroup());
		return true;
	}
};

OgreResourceLoader::OgreResourceLoader() :
		UnloadUnusedResources("unloadunusedresources", this, "Unloads any unused resources."),
		mFileSystemArchiveFactory(std::make_unique<FileSystemArchiveFactory>()),
		mLoadingListener(std::make_unique<EmberResourceLoadingListener>()) {
}

OgreResourceLoader::~OgreResourceLoader() {
	//Don't deregister mLoadingListener, since this destructor needs to be called after OGRE has been shut down, and there won't be any ResourceGroupManager
	//available by then.
	for (auto& path : mResourceRootPaths) {
		Ember::FileSystemObserver::getSingleton().remove_directory(path);
	}
}

void OgreResourceLoader::initialize() {

	//We can only add an archive factory, there's no method for removing it. Thus an instance of this needs to survive longer than Ogre itself.
	Ogre::ArchiveManager::getSingleton().addArchiveFactory(mFileSystemArchiveFactory.get());

	Ogre::ResourceGroupManager::getSingleton().setLoadingListener(mLoadingListener.get());

	auto& configSrv = EmberServices::getSingleton().getConfigService();

	if (configSrv.itemExists("media", "extraresourcelocations")) {
		varconf::Variable resourceConfigFilesVar = configSrv.getValue("media", "extraresourcelocations");
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
	for (const auto& resourceGroup : resourceGroups) {
		resourceGroupManager.unloadUnreferencedResourcesInGroup(resourceGroup, false);
	}
}

bool OgreResourceLoader::addSharedMedia(const std::string& path, const std::string& type, const std::string& section) {
	auto sharedMediaPath = EmberServices::getSingleton().getConfigService().getSharedDataDirectory();
	return addResourceDirectory(sharedMediaPath / path, type, section, OnFailure::Throw);
}

bool OgreResourceLoader::addSourceRepoMedia(const std::string& path, const std::string& section) {

	//If there's processed media available, use that.
	//Otherwise, if the raw media repository is available, use that instead.

#ifdef EMBER_PROCESSEDMEDIAREPODIR
	boost::filesystem::path processedMediaRepoDir(EMBER_PROCESSEDMEDIAREPODIR);
	if (!processedMediaRepoDir.empty()) {
		boost::filesystem::path relativePath(path);
		processedMediaRepoDir /= relativePath;
		boost::system::error_code ec;
		if (boost::filesystem::is_directory(processedMediaRepoDir, ec)) {
			S_LOG_INFO("Found processed media repo at '" << processedMediaRepoDir.string() << "'.");
			return addResourceDirectory(processedMediaRepoDir.string(), "EmberFileSystem", section, OnFailure::Throw);
		}
	}
#endif


#ifdef EMBER_SOURCEMEDIAREPODIR
	boost::filesystem::path sourceMediaRepoDir(EMBER_SOURCEMEDIAREPODIR);
	if (!sourceMediaRepoDir.empty()) {
		boost::filesystem::path relativePath(path);
		sourceMediaRepoDir /= relativePath;
		boost::system::error_code ec;
		if (boost::filesystem::is_directory(sourceMediaRepoDir, ec)) {
			S_LOG_INFO("Found source media repo at '" << sourceMediaRepoDir.string() << "'.");
			return addResourceDirectory(sourceMediaRepoDir.string(), "EmberFileSystem", section, OnFailure::Throw);
		}
	}
#endif
	return false;
}


bool OgreResourceLoader::addUserMedia(const std::string& path, const std::string& type, const std::string& section) {
	auto userMediaPath = EmberServices::getSingleton().getConfigService().getUserMediaDirectory();
	auto emberMediaPath = EmberServices::getSingleton().getConfigService().getEmberMediaDirectory();

	bool foundDir = addResourceDirectory(emberMediaPath / path, type, section, OnFailure::Ignore);

	return addResourceDirectory(userMediaPath / path, type, section, OnFailure::Ignore) || foundDir;
}

bool OgreResourceLoader::addResourceDirectory(const boost::filesystem::path& path,
											  const std::string& type,
											  const std::string& section,
											  OnFailure onFailure) {
	if (boost::filesystem::is_directory(path)) {
		S_LOG_VERBOSE("Adding dir " << path.string());
		try {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path.string(), type, section, true);
			mResourceRootPaths.emplace_back(path.string());
			observeDirectory(path);

			return true;
		} catch (const std::exception&) {
			switch (onFailure) {
				case OnFailure::Ignore:
					break;
				case OnFailure::Report:
					S_LOG_FAILURE("Couldn't load " << path.string() << ". Continuing as if nothing happened.");
					break;
				case OnFailure::Throw:
					throw Ember::Exception(std::string("Could not load from required directory '") + path.string() + "'. This is fatal and Ember will shut down. The probable cause for this error is that you haven't properly installed all required media.");
			}
		}
	} else {
		switch (onFailure) {
			case OnFailure::Ignore:
				break;
			case OnFailure::Report:
				S_LOG_FAILURE("Couldn't find resource directory " << path.string());
				break;
			case OnFailure::Throw:
				throw Ember::Exception(std::string("Could not find required directory '") + path.string() + "'. This is fatal and Ember will shut down. The probable cause for this error is that you haven't properly installed all required media.");
				break;
		}
	}
	return false;
}

void OgreResourceLoader::loadBootstrap() {
	addMedia("splash", "UI");

	//Add the "assets" directory, which contains most of the assets
	addMedia("assets", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	addSharedMedia("OGRE/Media/RTShaderLib/materials", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	addSharedMedia("OGRE/Media/RTShaderLib/GLSL", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	addSharedMedia("OGRE/Media/ShadowVolume", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	addUserMedia("media/assets", "EmberFileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

void OgreResourceLoader::loadGui() {
	addMedia("ui", "UI");
	addSharedMedia("gui", "EmberFileSystem", "UI");
	addUserMedia("gui", "EmberFileSystem", "UI");
}

void OgreResourceLoader::loadGeneral() {

	//Lua scripts
	addSharedMedia("scripting", "EmberFileSystem", "Scripting");
	addUserMedia("scripting", "EmberFileSystem", "Scripting");

	//Model definitions, terrain definitions, sound definitions and entity mappings
	addSharedMedia("data", "EmberFileSystem", "Data");
	addUserMedia("data", "EmberFileSystem", "Data");

	//The Caelum component
	addMedia("assets_external/caelum", "Caelum");
	addUserMedia("media/assets_external/caelum", "EmberFileSystem", "Caelum");

	//Entity recipes
	addSharedMedia("entityrecipes", "EmberFileSystem", "EntityRecipes");
	addUserMedia("entityrecipes", "EmberFileSystem", "EntityRecipes");

	//End with adding any extra defined locations.
	for (auto& location : mExtraResourceLocations) {
		addResourceDirectory(location, "EmberFileSystem", "Extra", OnFailure::Report);
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

void OgreResourceLoader::observeDirectory(const boost::filesystem::path& path) {
	try {
		FileSystemObserver::getSingleton().add_directory(path, [this](const FileSystemObserver::FileSystemEvent& event) {
			auto& ev = event.ev;
			//Skip if it's not a file. This also means that we won't catch deletion of files. That's ok for now; but perhaps we need to revisit this.
			if (!boost::filesystem::is_regular_file(ev.path)) {
				return;
			}
			S_LOG_VERBOSE("Resource changed " << ev.path.string() << " " << ev.type_cstr());

			if (ev.type == boost::asio::dir_monitor_event::modified) {
				try {
					if (boost::filesystem::file_size(ev.path) == 0) {
						return;
					}
				} catch (...) {
					//Could not find file, just return
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

				auto startsWith = [](const boost::filesystem::path& root, boost::filesystem::path aPath) {
					while (!aPath.empty()) {
						if (aPath == root) {
							return true;
						}
						aPath = aPath.parent_path();
					}
					return false;
				};

				auto locations = Ogre::ResourceGroupManager::getSingleton().listResourceLocations(group);
				for (auto& location : *locations) {
					boost::filesystem::path locationDirectory(location);
					if (startsWith(locationDirectory, ev.path)) {
						auto relative = relativeTo(locationDirectory, ev.path);
						auto extension = ev.path.extension();

						if (extension == ".modeldef") {
							if (event.ev.type == boost::asio::dir_monitor_event::event_type::added ||
								event.ev.type == boost::asio::dir_monitor_event::event_type::modified ||
								event.ev.type == boost::asio::dir_monitor_event::event_type::renamed_old_name ||
								event.ev.type == boost::asio::dir_monitor_event::event_type::renamed_new_name) {
								refreshModelDefinition(ev.path, relative);
							}
						} else if (extension == ".material") {
							std::ifstream stream(ev.path.string());
							Ogre::SharedPtr<Ogre::DataStream> fileStream(OGRE_NEW Ogre::FileStreamDataStream(&stream, false));
							Ogre::MaterialManager::getSingleton().parseScript(fileStream, group);
						} else if (extension == ".dds" || extension == ".png" || extension == ".jpg") {
							reloadResource(Ogre::TextureManager::getSingleton(), relative.string());
						} else if (extension == ".mesh") {
							reloadResource(Ogre::MeshManager::getSingleton(), relative.string());
						} else if (extension == ".skeleton") {
							reloadResource(Ogre::SkeletonManager::getSingleton(), relative.string());
						} else if (extension == ".glsl" || extension == ".frag" || extension == ".vert") {
							//Reloading GLSL shaders in Render System GL doesn't seem to work. Perhaps we'll have more luck with GL3+?


//						{
//							Ogre::SharedPtr<Ogre::DataStream> stream(OGRE_NEW Ogre::MemoryDataStream(0));
//							Ogre::GpuProgramManager::getSingleton().loadMicrocodeCache(stream);
//
//							Ogre::HighLevelGpuProgramManager::getSingleton().reloadAll(true);
//							Ogre::GpuProgramManager::getSingleton().reloadAll(true);
//							Ogre::MaterialManager::getSingleton().reloadAll(true);
//						}



//						auto iterator = Ogre::HighLevelGpuProgramManager::getSingleton().getResourceIterator();
//						while (iterator.hasMoreElements()) {
//							auto resource = Ogre::static_pointer_cast<Ogre::HighLevelGpuProgram>(iterator.getNext());
//							if (resource->getSourceFile() == relative) {
//								S_LOG_VERBOSE("Reloading GLSL script " << resource->getName());
//								Ogre::SharedPtr<Ogre::DataStream> stream(OGRE_NEW Ogre::MemoryDataStream(0));
//								Ogre::GpuProgramManager::getSingleton().loadMicrocodeCache(stream);
//								//Ogre::GpuProgramManager::getSingleton().cac
//								resource->reload();
//
////								auto matIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
////								while (matIterator.hasMoreElements()) {
////									bool needReload = false;
////									auto material = Ogre::static_pointer_cast<Ogre::Material>(matIterator.getNext());
////									for (auto* tech : material->getTechniques()) {
////										for (auto* pass : tech->getPasses()) {
////											if (pass->getFragmentProgramName() == resource->getName()) {
////												pass->setFragmentProgram("");
////												pass->setFragmentProgram(resource->getName());
////												needReload = true;
////											}
////											if (pass->getVertexProgramName() == resource->getName()) {
////												pass->setVertexProgram("");
////												pass->setVertexProgram(resource->getName());
////												needReload = true;
////											}
////										}
////									}
////									if (needReload) {
////										material->reload();
////									}
////								}
//							}
//						}
						}
					}
				}
			}
		});
	} catch (...) {
		//Ignore errors
	}

}

bool OgreResourceLoader::addMedia(const std::string& path, const std::string& resourceGroup) {
	if (!addSourceRepoMedia(path, resourceGroup)) {
		return addSharedMedia("media/" + path, "EmberFileSystem", resourceGroup);
	}
	return true;
}

void OgreResourceLoader::refreshModelDefinition(const boost::filesystem::path& fullPath, const boost::filesystem::path& relativePath) {
	std::ifstream stream(fullPath.string());
	if (stream) {

		Model::XMLModelDefinitionSerializer serializer;

		auto modelDef = serializer.parseScript(stream, relativePath);
		if (modelDef) {
			auto& modelDefMgr = Model::ModelDefinitionManager::getSingleton();
			auto existingDef = modelDefMgr.getByName(relativePath.string());
			//Model definition doesn't exist, just add it.
			if (!existingDef) {
				modelDefMgr.addDefinition(relativePath.string(), modelDef);
			} else {
				//otherwise update existing
				existingDef->moveFrom(std::move(*modelDef));
				existingDef->reloadAllInstances();
			}
		}
	}
}

}
}
