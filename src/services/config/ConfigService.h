/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]

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

#include "../Service.h"
#include <string>
#include <hash_map>
#include <algorithm>

namespace dime {
	namespace services {

/**
 * Dime Configuration Service
 *
 * @author Miguel Guzman Miranda [Aglanor]
 *
 * @see dime::Service
 */
class ConfigService: public Service
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:

		/** struct for comparation of two strings */
		struct eqstr
		{
  			bool operator()(const char* s1, const char* s2) const
  			{
    			return strcmp(s1, s2) == 0;
  			}
		};

  		std::hash<const char*> H;
		std::hash_map<const char*, const char*, hash<const char*>, eqstr> inputMap;

		
    //----------------------------------------------------------------------
    // Constructors & Destructor

	public:
	
    /** Creates a new ConfigService using default values. */
    ConfigService();


    /** Deletes a ConfigService instance. */
    ~ConfigService()
    {
    }


    //----------------------------------------------------------------------
    // Getters & Setters

    //----------------------------------------------------------------------
    // Methods
	
   int start();

   void stop(int code) ;
	
	const char* getParam( const char* );

	/**
	 * Saves the configuration parameters to a file
	 */
   	bool saveConfig();

}; //ConfigService

	} // namespace services
} // namespace dime

#endif


