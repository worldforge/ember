//
// C++ Implementation: Environment
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

#include "Environment.h"
#include "framework/Tokeniser.h"

namespace EmberOgre {

namespace Environment {

Environment::Environment(IEnvironmentProvider* provider):
 SetTime("set_time",this, "Sets the time. parameters: <hour> <minute>")
, SetFogDensity("set_fogdensity",this, "Sets the fog density.")
, mProvider(provider)
{
}


Environment::~Environment()
{
}

void Environment::runCommand(const std::string &command, const std::string &args)
{
// 	if (SetTime == command) {
// 		Ember::Tokeniser tokeniser;
// 		tokeniser.initTokens(args);
// 		std::string hourString = tokeniser.nextToken();
// 		std::string minuteString = tokeniser.nextToken();
// 		
// 		int hour = ::Ogre::StringConverter::parseInt( hourString);
// 		int minute = ::Ogre::StringConverter::parseInt( minuteString);
// 		setTime(hour, minute);
// 		
// 	} else 
	if (SetFogDensity == command) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string densityString = tokeniser.nextToken();
		
		float density = ::Ogre::StringConverter::parseReal(densityString);
		getFog()->setDensity( density);
		
	}
	
}

void Environment::initialize()
{
	mProvider->createEnvironment();
}

void Environment::setTime(int hour, int minute, int second)
{
// 	mCaelumSystem->setLocalTime (3600 * hour + 60 * minute + second);

}
}

}
