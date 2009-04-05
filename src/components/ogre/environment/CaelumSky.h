//
// C++ Interface: CaelumSky
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
#ifndef EMBEROGRECAELUMSKY_H
#define EMBEROGRECAELUMSKY_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "CaelumEnvironment.h"
#include "services/config/ConfigListener.h"

namespace EmberOgre {

namespace Environment {


/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CaelumSky
: CaelumEnvironmentComponent
,public IFog,public ISky, Ember::ConfigListenerContainer
{
public:
    CaelumSky(CaelumEnvironment& environment);

    virtual ~CaelumSky();
    

 	void setDensity(float density);
	float getDensity() const;
   
	
	virtual bool frameEnded(const Ogre::FrameEvent & event);
    
protected:

	void Config_CloudSpeed(const std::string& section, const std::string& key, varconf::Variable& variable);
	void Config_CloudBlendTime(const std::string& section, const std::string& key, varconf::Variable& variable);
	void Config_CloudCover(const std::string& section, const std::string& key, varconf::Variable& variable);


};


}

}

#endif
