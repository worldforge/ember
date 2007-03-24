//
// C++ Interface: XMLHelper
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#ifndef EMBEROGREXMLHELPER_H
#define EMBEROGREXMLHELPER_H

#include "EmberOgrePrerequisites.h"
#include "framework/tinyxml/tinyxml.h"

namespace EmberOgre {

/**
	A simple helper class for loading tinyxml documents through the ogre resource system.

	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class XMLHelper{
public:
    XMLHelper();

    ~XMLHelper();

	/**
	 Attempts to load the supplied stream into the document. Failures will be logged.
	 @param An empty xml document.
	 @param An opened and valid data stream
	 @returns true if successful, else false
	 */
    bool Load(Ember::TiXmlDocument& xmlDoc, Ogre::DataStreamPtr stream);


};

}

#endif
