//
// C++ Implementation: CaelumSun
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CaelumSun.h"
#include "caelum/include/CaelumSystem.h"
#include "components/ogre/Convert.h"
#include "caelum/include/Sun.h"

#include "framework/Tokeniser.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

namespace EmberOgre
{

namespace Environment
{

CaelumSun::CaelumSun(CaelumEnvironment& environment, Caelum::BaseSkyLight* sun) :
	CaelumEnvironmentComponent(environment), mSun(sun)
{
	sun->setAmbientMultiplier(Ogre::ColourValue(0.7, 0.7, 0.7));
	// 		mCaelumSystem->getSun ()->setAmbientMultiplier (Ogre::ColourValue(0.5, 0.5, 0.5));
	sun->setDiffuseMultiplier(Ogre::ColourValue(3, 3, 2.7));
	// For green terrain:
	//mCaelumSystem->getSun ()->setDiffuseMultiplier (Ogre::ColourValue(0.1, 3, 0.1));
	sun->setSpecularMultiplier(Ogre::ColourValue(5, 5, 5));

	sun->setAutoDisable(true);
	sun->setAutoDisableThreshold(0.05);

	Caelum::BaseSkyLight* moon = mCaelumSystem->getMoon();
	if (moon) {
		moon->setAutoDisable(true);
		moon->setAutoDisableThreshold(0.05);
	}

	registerConfigListener("caelum", "sunambientmultiplier", sigc::mem_fun(*this, &CaelumSun::Config_SunAmbientMultiplier));
	registerConfigListener("caelum", "sundiffusemultiplier", sigc::mem_fun(*this, &CaelumSun::Config_SunDiffuseMultiplier));
	registerConfigListener("caelum", "sunspecularmultiplier", sigc::mem_fun(*this, &CaelumSun::Config_SunSpecularMultiplier));

}

CaelumSun::~CaelumSun()
{

}

void CaelumSun::setAmbientLight(const Ogre::ColourValue& colour)
{
	mCaelumSystem->getSceneMgr()->setAmbientLight(colour);
}

Ogre::Vector3 CaelumSun::getSunDirection() const
{
	return mSun->getLightDirection();
}

WFMath::Vector<3> CaelumSun::getMainLightDirection() const
{
	return Convert::toWF<WFMath::Vector<3> >(getSunDirection());
}

Ogre::ColourValue CaelumSun::getAmbientLightColour() const
{
	return mSun->getMainLight()->getDiffuseColour();
}

void CaelumSun::Config_SunAmbientMultiplier(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	Ogre::ColourValue colour;
	if (parse(variable, colour) && mSun) {
		mSun->setAmbientMultiplier(colour);
	}
}

void CaelumSun::Config_SunDiffuseMultiplier(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	Ogre::ColourValue colour;
	if (parse(variable, colour) && mSun) {
		mSun->setDiffuseMultiplier(colour);
	}
}

void CaelumSun::Config_SunSpecularMultiplier(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	Ogre::ColourValue colour;
	if (parse(variable, colour) && mSun) {
		mSun->setSpecularMultiplier(colour);
	}
}

bool CaelumSun::parse(varconf::Variable& variable, Ogre::ColourValue& colour)
{
	if (variable.is_string()) {
		Ember::Tokeniser tokeniser(variable);
		colour.r = atof(tokeniser.nextToken().c_str());
		colour.g = atof(tokeniser.nextToken().c_str());
		colour.b = atof(tokeniser.nextToken().c_str());
		colour.a = atof(tokeniser.nextToken().c_str());
		return true;
	}
	return false;
}

}

}
