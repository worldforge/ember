/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]
                        Joel Schander         [nullstar]
                        Erik Ogenvik

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef CONFIGSERVICE_H
#define CONFIGSERVICE_H

#include "framework/Service.h"
#include "framework/ConsoleObject.h"
#include <varconf/varconf.h>

#include "../EmberServices.h"

#include <boost/filesystem/path.hpp>
#include <string>
#include <unordered_map>

namespace Ember {

/**
 * @brief An enum of XDG base directory types.
 *
 * This lists the types of possible XDG base directories. Ember currently does not use RUNTIME.
 */
enum BaseDirType {
	BaseDirType_DATA, BaseDirType_CONFIG, BaseDirType_CACHE, BaseDirType_RUNTIME
};

/**
* @brief Ember Configuration Service.
* Provides methods for accessing configuration information. This wraps around the Varconf system.
*
* If you want to provide functionality in your class for dynamically reacting to changes to the config you should use the ConfigLister classes instead of interfacing with this service directly.
*
* The service keeps three different config instances: global, user and instance. Instance settings takes precedence over user settings, which take precedence over global settings.
*
* @author Erik Ogenvik (erik@ogenvik.org)
* @author Miguel Guzman Miranda [Aglanor]
* @author Joel Schander         [nullstar]
*
* @see Ember::Service
* @see varconf
*/
class ConfigService : public Service, public ConsoleObject {
private:
	//----------------------------------------------------------------------
	// Class Variables
	//----------------------------------------------------------------------
#ifdef __WIN32__
	boost::filesystem::path baseDir;
#endif

	boost::filesystem::path mSharedDataDir;
	boost::filesystem::path mEtcDir;

	/**
	The home directory. If this is not set, the default one will be used. See getHomeDirectory(const BaseDirType baseDirType)
	*/
	boost::filesystem::path mHomeDir;

	/**
	 * @brief The prefix, i.e. the root directory where Ember is installed.
	 */
	std::string mPrefix;

	void registerConsoleCommands();

	void deregisterConsoleCommands();

	/**
	 * just a facade for the underlying varconf::Config::sigv
	 * @param  the section of the item
	 * @param  the key of the item
	 */
	void updatedConfig(const std::string& section, const std::string& key);

	void configError(const char* error);

	/**
	 * @brief The global configuration store.
	 *
	 * This contains values read in from either the global config.
	 */
	varconf::Config* mGlobalConfig;

	/**
	 * @brief The user configuration store.
	 *
	 * This contains values read in from the user config file (for example ~/.config/ember/ember.conf).
	 */
	varconf::Config* mUserConfig;

	/**
	 * @brief The command line configuration store.
	 *
	 * This contains values read from the command line. These should not factor in when saving the configuration.
	 */
	varconf::Config* mCommandLineConfig;

	/**
	 * @brief The instance configuration store.
	 *
	 * This contains values altered at run time.
	 */
	varconf::Config* mInstanceConfig;


protected:
public:
	typedef std::map<std::string, varconf::Variable> SectionMap;
	typedef std::unordered_map<std::string, std::map<std::string, std::string>> StringConfigMap;

	static const std::string SETVALUE;
	static const std::string GETVALUE;

	//----------------------------------------------------------------------
	// Constructors & Destructor
	//----------------------------------------------------------------------

	/**
	 * Ctor for Ember::service::ConfigService.
	 *
	 */
	ConfigService();

	/**
	 * Dtor for Ember::service::ConfigService.
	 *
	 */
	~ConfigService() override;


	//----------------------------------------------------------------------
	// Getters & Setters
	//----------------------------------------------------------------------

	/**
	 * Returns value stored in key in appropriate section.
	 *
	 * @param Section of config space to look in.
	 * @param Key to return value of.
	 */
	varconf::Variable getValue(const std::string& section, const std::string& key) const;


	bool getValue(const std::string& section, const std::string& key, varconf::Variable& value) const;

	/**
	 * Sets value of key in appropriate section.
	 *
	 * @param Section of config space to look in.
	 * @param Key to store value in.
	 * @param Value to store.
	 * @param scope The underlying configuration to save the value to. In almost all cases varconf::INSTANCE is appropriate.
	 */
	void setValue(const std::string& section, const std::string& key, const varconf::Variable& value, int scope = varconf::INSTANCE);

	//----------------------------------------------------------------------
	// Methods
	//----------------------------------------------------------------------

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string& command, const std::string& args) override;


	/**
	 * Starts ConfigService.  Returns status.
	 *
	 */
	bool start() override;

	/**
	 * Stops ConfigService.
	 *
	 */
	void stop() override;

	/**
	 * Returns true if the key exists in the section given.
	 *
	 * @param Section of config space to look in.
	 * @param Key to look for.
	 * @deprecated Use hasItem instead.
	 */
	bool itemExists(const std::string& section, const std::string& key) const;

	/**
	* Returns true if the key exists in the section given.
	*
	* @param section
	* @param key
	* @return true if the item exists
	*/
	bool hasItem(const std::string& section, const std::string& key) const;

	/**
	Returns true if the item is set to the supplied value.
	*/
	bool isItemSet(const std::string& section, const std::string& key, const std::string& value) const;

	/**
	 * Returns true if the key exists in the section give but is successfully
	 * removed.
	 *
	 * @param Section of config space to look in.
	 * @param Key to remove.
	 */
	bool deleteItem(const std::string& section, const std::string& key);


	/**
	 *    Returns a map of the specified section.
	 * @param sectionName
	 * @return
	 */
	SectionMap getSection(const std::string& sectionName);

	/**
	 * Loads config space from given file.
	 *
	 * @param Name of file to read from.
	 * @param commandLineSettings Settings from the command line.
	 */
	bool loadSavedConfig(const std::string& filename, const StringConfigMap& commandLineSettings = StringConfigMap());

	/**
	 * Saves config space to given file.
	 *
	 * @param filename of file to save to.
	 * @param scopeMask we will only write out variables which have their scope in this mask
	 */
	bool saveConfig(const boost::filesystem::path& filename, int scopeMask = varconf::GLOBAL | varconf::USER | varconf::INSTANCE);


	/**
	* returns the path to the home directory, where all configuration is stored
	*/
	const boost::filesystem::path& getHomeDirectory(BaseDirType baseDirType) const;

	/**
	* returns the path to the shared data directory, where common media is
	*/
	const boost::filesystem::path& getSharedDataDirectory() const;

	/**
	* returns the path to the ember data directory, where ember media is
	*/
	const boost::filesystem::path& getEmberDataDirectory() const;

	/**
	 *    returns the path to the media directory specific to a user, but synced with the main server, which would normally be ~/.local/share/ember/ember-media
	 * @return
	 */
	const boost::filesystem::path& getEmberMediaDirectory() const;

	/**
	 *    returns the path to the media directory specific to a user, containing media created by the user, which would normally be ~/.local/share/ember/user-media
	 * @return
	 */
	const boost::filesystem::path& getUserMediaDirectory() const;

	/**
	 *    returns the path to the media directory shared between users, which would normally be $prefix/ember/media
	 * @return
	 */
	const boost::filesystem::path& getSharedMediaDirectory() const;

	/**
	 *    returns the path to the shared config directory where all the original configuration files are stored
	 * @return
	 */
	const boost::filesystem::path& getSharedConfigDirectory() const;

	/**
	*	Emitted when a config item is changed.
	*	@param the section of the config item
	*	@param the key of the config item
	*/
	sigc::signal<void, const std::string&, const std::string&> EventChangedConfigItem;

	/**
	 * Sets the prefix for all the configuration and resources. Call this before other services have initialized.
	 * @param prefix
	 */
	void setPrefix(const std::string& prefix);

	/**
	 * @brief Gets the prefix, i.e. the root directory where Ember is installed.
	 * @return The prefix, i.e. the root directory where Ember is installed.
	 */
	const std::string& getPrefix() const;

	/**
	 *    Sets the home directory, i.e. where all configuration and media is stored. If this is not set, a default directory will be user (XDG standard on *NIX systems)
	 * @param path
	 */
	void setHomeDirectory(const std::string& path);

}; //ConfigService

} // namespace Ember

#endif
