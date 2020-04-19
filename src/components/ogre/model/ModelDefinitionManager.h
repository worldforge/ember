//
// C++ Interface: ModelManager
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
#ifndef EMBEROGREMODELMANAGER_H
#define EMBEROGREMODELMANAGER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include "ModelDefinition.h"
#include "framework/Singleton.h"
#include "framework/ConsoleCommandWrapper.h"

#include <OgreScriptLoader.h>
#include <boost/filesystem/path.hpp>

namespace Eris {
class EventService;
}

namespace Ember {
class TimeFrame;
namespace OgreView {
namespace Model {

class ModelFactory;
class ModelBackgroundLoader;


/**
 *
 * @brief Manages all of the ModelDefinition instances.
 *
 * Responsible for creating, managing, and destroying ModelDefinition instances.
 *
 * @author Erik Ogenvik
 */
class ModelDefinitionManager: public Ogre::ScriptLoader, public Singleton<ModelDefinitionManager>, public ConsoleObject
{
public:
	/**
	 * @brief Ctor.
	 *
	 * @param exportDirectory The path to the export directory, where exported models will be stored.
	 */
	explicit ModelDefinitionManager(boost::filesystem::path exportDirectory, Eris::EventService& eventService);

	/**
	 * @brief Dtor.
	 */
	~ModelDefinitionManager() override;


	void addDefinition(std::string name, ModelDefinitionPtr definition);


	const Ogre::StringVector& getScriptPatterns() const override;

	Ogre::Real getLoadingOrder() const override;

	/**
	 * @brief Parses the submitted script and creates ModelDefinition instances.
	 * @param stream The stream containing the script definition.
	 * @param groupName
	 */
	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName) override;

	/**
	 * @brief Exports a modeldefinition to a file.
	 * The definition will be serialized and saved to a file by the same name of the definition.
	 * @param definition The definition to export.
	 * @return The path to the exported script. If the export failed, the string will be empty.
	 */
	std::string exportScript(const std::string& name, const ModelDefinitionPtr& definition);


	/// Get a ModelDefinition by name
	ModelDefinitionPtr getByName(const Ogre::String& name);

	bool hasDefinition(const Ogre::String& name);

	const std::unordered_map<std::string, ModelDefinitionPtr>& getEntries() const;


	/**
	 * @brief Gets a vector of all mesh names in the system.
	 * This is mainly a helper method for retrieving a list of all available meshes.
	 * @return A list of all available meshes.
	 */
	std::vector<std::string> getAllMeshes() const;

	/**
	 * @brief Returns whether all models should be shown using their normal visibility settings.
	 * @return True if all models should use their normal visibility settings for determining whether they should be shown.
	 */
	bool getShowModels() const;

	/**
	 * @brief Sets whether models should be shown, using their normal visibility settings.
	 * This is useful for hiding all models in the system.
	 * @param show If false, all models will be hidden. If true, each model will use its normal visibility setting.
	 */
	void setShowModels(bool show);

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string &command, const std::string &args) override;

	/**
	 Command for setting whether models should be shown or not.
	 */
	const ConsoleCommandWrapper ShowModels;


protected:

	std::unordered_map<std::string, ModelDefinitionPtr> mEntries;


	/**
	 * @brief Determines whether models should be shown.
	 */
	bool mShowModels;


	/**
	 * @brief The path to the export directory.
	 */
	const boost::filesystem::path mExportDirectory;


};

}
}
}

#endif
