//
// C++ Implementation: OgreSoundScriptingProvider
//
// Description: 
//
//
// Author: Miguel Guzman <aglanor@gmail.com>, (C) 2006
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../EmberOgrePrerequisites.h"

#include <OgreString.h>

#include "framework/Exception.h"

#include "OgreSoundProvider.h"

namespace EmberOgre {

OgreSoundProvider::OgreSoundProvider()
{

}


OgreSoundProvider::~OgreSoundProvider()
{
}

void OgreSoundProvider::loadSound(const std::string& soundName)
{
	S_LOG_INFO("Loading sound");
}

bool OgreSoundProvider::willLoadSound(const std::string& soundName)
{
	return false;
}

const std::string& OgreSoundProvider::getName() const
{
	static std::string name = "OgreSoundProvider";
	return name;
}

void OgreSoundProvider::_registerWithService(Ember::SoundService* service)
{
	this->service = service;
}

}
