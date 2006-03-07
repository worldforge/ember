//
// C++ Interface: OgreResourceLoader
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
#ifndef EMBEROGREOGRERESOURCELOADER_H
#define EMBEROGREOGRERESOURCELOADER_H

#include "EmberOgrePrerequisites.h"

namespace EmberOgre {

/**
@author Erik Hjortsberg
*/
class OgreResourceLoader{
public:
    OgreResourceLoader();

    ~OgreResourceLoader();
    
	void initialize();
    
    void loadBootstrap();
    void loadGui();
    void loadGeneral();
    
    void preloadMedia();
    
protected:
	bool mLoadRecursive;
	Ogre::ConfigFile cf;
	
	void loadSection(const std::string& sectionName);
	
	void addUserMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive);
	void addSharedMedia(const std::string& path, const std::string& type, const std::string& section, bool recursive);
	
	bool isExistingDir(const std::string& path) const;
};

}

#endif
