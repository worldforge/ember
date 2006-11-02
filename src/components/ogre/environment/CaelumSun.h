//
// C++ Interface: CaelumSun
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
#ifndef EMBEROGRECAELUMSUN_H
#define EMBEROGRECAELUMSUN_H
#include "components/ogre/EmberOgrePrerequisites.h"
#include "CaelumEnvironment.h"

namespace EmberOgre {

namespace Environment {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class CaelumSun :  CaelumEnvironmentComponent, public ISun
{
public:
    CaelumSun(CaelumEnvironment& environment, caelum::Sun* sun);

    ~CaelumSun();
    
private:

	caelum::Sun* mSun;

};

}

}

#endif
