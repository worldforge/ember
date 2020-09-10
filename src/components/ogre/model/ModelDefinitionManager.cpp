 //
// C++ Implementation: ModelDefinitionManager
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
// Copyright (c) 2005 The Cataclysmos Team
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

#include "ModelDefinitionManager.h"
#include "Model.h"

#include "XMLModelDefinitionSerializer.h"

#include "framework/TimeFrame.h"
#include "framework/Tokeniser.h"

#include <OgreRoot.h>

#include <utility>
#include <boost/algorithm/string.hpp>


namespace Ember {
namespace OgreView {
namespace Model {

ModelDefinitionManager::ModelDefinitionManager(boost::filesystem::path exportDirectory, Eris::EventService& eventService)
		: ShowModels("showmodels", this, "Show or hide models."),
		  mShowModels(true),
		  mExportDirectory(std::move(exportDirectory)) {
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
}

ModelDefinitionManager::~ModelDefinitionManager() {
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}


void ModelDefinitionManager::addDefinition(std::string name, ModelDefinitionPtr definition) {
	mEntries[std::move(name)] = std::move(definition);
}

void ModelDefinitionManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName) {
	XMLModelDefinitionSerializer serializer;
	auto definition = serializer.parseScript(stream);
	if (definition) {
		addDefinition(stream->getName(), definition);
	}
}

std::string ModelDefinitionManager::exportScript(const std::string& name, const ModelDefinitionPtr& definition) {
	XMLModelDefinitionSerializer serializer;
	auto fileName = name;
	if (!boost::algorithm::ends_with(name, ".modeldef")) {
		fileName = name + ".modeldef";
	}

	auto path = mExportDirectory / fileName;

	bool success = serializer.exportScript(definition, path);
	if (success) {
		return path.string();
	}

	return "";

}

std::vector<std::string> ModelDefinitionManager::getAllMeshes() const {
	std::vector<std::string> meshes;
	auto meshesVector = Ogre::ResourceGroupManager::getSingleton().findResourceNames("General", "*.mesh");
	for (auto& meshName : *meshesVector) {
		meshes.emplace_back(meshName);
	}
	return meshes;
}


ModelDefinitionPtr ModelDefinitionManager::getByName(const Ogre::String& name) {
	auto I = mEntries.find(name);
	if (I != mEntries.end()) {
		return I->second;
	}
	return ModelDefinitionPtr();
}

bool ModelDefinitionManager::getShowModels() const {
	return mShowModels;
}

void ModelDefinitionManager::setShowModels(bool show) {
	mShowModels = show;
}


void ModelDefinitionManager::runCommand(const std::string& command, const std::string& args) {
	if (command == ShowModels.getCommand()) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string value = tokeniser.nextToken();
		if (value == "true") {
			S_LOG_INFO("Showing models.");
			setShowModels(true);
		} else if (value == "false") {
			S_LOG_INFO("Hiding models.");
			setShowModels(false);
		}
	}
}

const Ogre::StringVector& ModelDefinitionManager::getScriptPatterns() const {
	static Ogre::StringVector patterns{"*.modeldef", "*.modeldef.xml"};
	return patterns;
}

Ogre::Real ModelDefinitionManager::getLoadingOrder() const {
	return 300;
}

bool ModelDefinitionManager::hasDefinition(const Ogre::String& name) {
	return mEntries.find(name) != mEntries.end();
}

const std::unordered_map<std::string, ModelDefinitionPtr>& ModelDefinitionManager::getEntries() const {
	return mEntries;
}

}
}
}
