//
// C++ Interface: Sky
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#ifndef DIMEOGRESKY_H
#define DIMEOGRESKY_H

#include "components/ogre/EmberOgrePrerequisites.h"

// ------------------------------
// Include sigc header files
// ------------------------------
// #include <sigc++/object.h>
// #include <sigc++/connection.h>
#include <sigc++/trackable.h>

#include <OgreColourValue.h>


namespace EmberOgre {

namespace Environment {


/**
@author Erik Hjortsberg
*/
class Sky:  public sigc::trackable
{
public:
    Sky(::Ogre::Camera* camera, Ogre::SceneManager* sceneMgr);

    ~Sky();
	void updateFogValuesFromConfig();
private:
	
	void createSimpleSky(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr);	
	
	void setFogValues(float start, float end, Ogre::ColourValue colour);
	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);

};

}

}

#endif
