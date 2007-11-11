//
// C++ Implementation: CaelumSun
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
#include "CaelumSun.h"
#include "components/ogre/EmberOgre.h"
#include "caelum/include/Sun.h"

namespace EmberOgre {

namespace Environment {

CaelumSun::CaelumSun(CaelumEnvironment& environment, caelum::Sun* sun)
: CaelumEnvironmentComponent( environment), mSun(sun) 
{
}


CaelumSun::~CaelumSun()
{
}

void CaelumSun::setAmbientLight(const Ogre::ColourValue& colour) {
	EmberOgre::getSingleton().getSceneManager()->setAmbientLight(colour);
}

Ogre::Vector3 CaelumSun::getSunDirection() const
{
	return mSun->getSunDirection();
}

}

}
