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
#include <string>
#include <varconf/varconf.h>

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
class ConfigService: public Service
{
    private:
    //----------------------------------------------------------------------
    // Class Variables
    //----------------------------------------------------------------------


    protected:
    public:
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
    varconf::Variable getValue(const std::string& section, const std::string& key);

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
    bool itemExists(const std::string& section, const std::string& key);

    /**
     * Returns true if the key exists in the section give but is successfully
     * removed.
     * 
     * @param Section of config space to look in.
     * @param Key to remove.
     */
    bool deleteItem(const std::string& section, const std::string& key);

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
	std::string getHomeDirectory() const;
	
	

}; //ConfigService

} // namespace Ember

#endif
