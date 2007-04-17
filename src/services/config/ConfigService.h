/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]
                        Joel Schander         [nullstar]

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef CONFIGSERVICE_H
#define CONFIGSERVICE_H

#include <framework/Service.h>
#include <framework/ConsoleObject.h>
#include <string>
#include <varconf/varconf.h>

// ------------------------------
// Include sigc header files
// ------------------------------
//#include <sigc++/object.h>
//#include <sigc++/connection.h>


namespace Ember {

    /**
     * Ember Configuration Service
     *
     * @author Miguel Guzman Miranda [Aglanor]
     * @author Joel Schander         [nullstar]
     *
     * @see Ember::Service
     * @see varconf
     */
class ConfigService: public Service, public Ember::ConsoleObject
{
    private:
    //----------------------------------------------------------------------
    // Class Variables
    //----------------------------------------------------------------------
#ifdef __WIN32__
	std::string baseDir;
#endif

	std::string mSharedDataDir;
	std::string mEtcDir;
	
	/**
	The home directory. If this is not set, the default one will be used. See getHomeDirectory()
	*/
	std::string mHomeDir;

    void registerConsoleCommands();
    void deregisterConsoleCommands();
    
    /**
     * just a facade for the underlying varconf::Config::sigv
     * @param  the section of the item
     * @param  the key of the item
     */
    void updatedConfig(const std::string& section, const std::string& key);
	
    void configError(const char* error);
	

    protected:
    public:
 	typedef std::map< std::string, varconf::Variable > SectionMap;
   
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
    ~ConfigService()
    {
    }

 
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

    /**
     * Sets value of key in appropriate section.
     *
     * @param Section of config space to look in.
     * @param Key to store value in.
     * @param Value to store.
     */
    void setValue(const std::string& section, const std::string& key, const varconf::Variable& value);

    //----------------------------------------------------------------------
    // Methods
    //----------------------------------------------------------------------

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);


    /**
     * Starts ConfigService.  Returns status.
     *
     */
    Service::Status start(void);

    /**
     * Stops ConfigService.
     *
     * @param stop code.
     */
    void stop(int code);

    /**
     * Returns true if the key exists in the section given.
     *
     * @param Section of config space to look in.
     * @param Key to look for.
     */
    bool itemExists(const std::string& section, const std::string& key) const;

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
	const SectionMap& getSection(const std::string& sectionName);

    /**
     * Loads config space from given file.
     *
     * @param Name of file to read from.
     */
    bool loadSavedConfig(const std::string& filename);

    /**
     * Saves config space to given file.
     *
     * @param Name of file to save to.
     */
    bool saveConfig(const std::string& filename);
	
	
	/**
	* returns the path to the home directory, where all configuration is stored
	*/
	const std::string& getHomeDirectory() const;
	
	/**
	* returns the path to the shared data directory, where common media is
	*/
	const std::string& getSharedDataDirectory() const;
	
	/**
	* returns the path to the ember data directory, where ember media is
	*/
	const std::string& getEmberDataDirectory() const;
	
	/**
	 *    returns the path to the media directory specific to a user, but synced with the main server, which would normally be ~/.ember/ember-media
	 * @return 
	 */
	const std::string& getEmberMediaDirectory() const;
	
	/**
	 *    returns the path to the media directory specific to a user, containing media created by the user, which would normally be ~/.ember/user-media
	 * @return 
	 */
	const std::string& getUserMediaDirectory() const;
	
	/**
	 *    returns the path to the media directory shared between users, which would normally be $prefix/ember/media
	 * @return 
	 */
	const std::string& getSharedMediaDirectory() const;

	/**
	 *    returns the path to the shared config directory where all the original configuration files are stored
	 * @return 
	 */
	const std::string& getSharedConfigDirectory() const;

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
	 *    Sets the home directory, i.e. where all configuration and media is stored. If this is not set, a default directory will be user (~/.ember on *NIX systems)
	 * @param path 
	 */
	void setHomeDirectory(const std::string& path);

}; //ConfigService

} // namespace Ember

#endif
