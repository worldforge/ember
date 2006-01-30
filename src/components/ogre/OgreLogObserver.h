//
// C++ Interface: OgreLogObserver
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
#ifndef EMBEROGREOGRELOGOBSERVER_H
#define EMBEROGREOGRELOGOBSERVER_H

#include "EmberOgrePrerequisites.h"

namespace EmberOgre {

/**
@author Erik Hjortsberg
A log observer which writes to the Ogre log system.
*/
class OgreLogObserver: public Ember::LoggingService::Observer, 
public sigc::trackable
{
	public:
		OgreLogObserver();
		
		~OgreLogObserver();

		virtual void onNewMessage(const std::string & message, const std::string & file, const int & line,
								const Ember::LoggingService::MessageImportance & importance, const time_t & timeStamp);
		
	protected:
		
		/**
			* Updates from the config. The relevant section is "general" and the key "logginglevel". It can have the values of verbose|info|warning|failure|critical
			*/
		void updateFromConfig();
		
		
		/**
			*          React on changes to the config.
			* @param section 
			* @param key 
			*/
		void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);
		
};

}

#endif
