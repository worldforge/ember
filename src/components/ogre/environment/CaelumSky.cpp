//
// C++ Implementation: CaelumSky
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CaelumSky.h"
#include "caelum/include/Caelum.h"
#include "framework/Tokeniser.h"


namespace EmberOgre {

namespace Environment {

CaelumSky::CaelumSky(CaelumEnvironment& environment, caelum::SkyColourModel *caelumModel, caelum::SkyDome *dome)
: CaelumEnvironmentComponent( environment), mCaelumModel(caelumModel), mDome(dome)
{
}


CaelumSky::~CaelumSky()
{
}

void CaelumSky::setDensity(float density)
{
	static_cast<caelum::StoredImageSkyColourModel *>(mCaelumModel)->setFogDensity (density);
}
float CaelumSky::getDensity() const
{
	//TODO: make it get the time
	return static_cast<caelum::StoredImageSkyColourModel *>(mCaelumModel)->getFogDensity (0, Ogre::Vector3::ZERO);
}

bool CaelumSky::frameEnded(const Ogre::FrameEvent & event)
{
	return true;
}
	


}

}
