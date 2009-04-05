//
// C++ Interface: OgreInfo
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifndef EMBEROGREOGREINFO_H
#define EMBEROGREOGREINFO_H

#include "EmberOgrePrerequisites.h"

namespace EmberOgre {

/**
	
	Provides methods for getting some basic information about the Ogre environment.

	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class OgreInfo{
public:
    OgreInfo();

    ~OgreInfo();
    
    
    /**
     * True if the rendering is indirect, for example when using Mesa drivers on Linux. This will result in _very_ bad performance, and is usually caused by the user not having vendor drivers installed.
     * @return 
     */
    bool isIndirect() const;

};

}

#endif
