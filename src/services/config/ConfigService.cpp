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

#include "ConfigService.h"

#include <iostream>

using namespace std;

namespace dime
{

    ConfigService* ConfigService::theInstance;

    ConfigService::ConfigService()
    {
	setName("Configuration Service");
	setDescription("Service for management of dime user-defined configuration");
        setStatus(OK);
    	setRunning(true);
    	setStatusText("Configuration Service status OK.");
    }

    varconf::Variable ConfigService::getValue(const std::string& section, const std::string& key)
    {
	string value;
	value = varconf::Config::inst()->getItem(section, key);
	return value;
    }

    void ConfigService::setValue(const std::string& section, const std::string& key, const varconf::Variable& value)
    {
	varconf::Config::inst()->setItem(section, key, value);
    }

    ConfigService* ConfigService::getInstance(void)
    {
	if (theInstance == NULL) 
	{
	    theInstance = new ConfigService;
	    return theInstance;
	}
	else
	{
	    return theInstance;
	}
    }

    Service::Status ConfigService::start()
    {
	cout << "TRACE: Config service started (TODO: this should go through logging service)" << endl;
	setRunning(true);
	return Service::OK;
    }

    void ConfigService::stop(int code)
    {
	setRunning(false);
	return;
    }

    bool ConfigService::itemExists(const std::string& section, const std::string& key)
    {
	return varconf::Config::inst()->find(section, key);
    }

    bool ConfigService::deleteItem(const std::string& section, const std::string& key)
    {
	return varconf::Config::inst()->erase(section, key);
    }

    bool ConfigService::loadSavedConfig(const std::string& filename)
    {
	return varconf::Config::inst()->readFromFile(filename);
    }

    bool ConfigService::saveConfig(const std::string& filename)
    {
	return varconf::Config::inst()->writeToFile(filename);
    }

} // namespace dime
