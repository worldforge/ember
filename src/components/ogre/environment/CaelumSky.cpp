//
// C++ Implementation: CaelumSky
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

#include "CaelumSky.h"
#include "caelum/include/Caelum.h"
#include "framework/Tokeniser.h"

namespace EmberOgre
{

namespace Environment
{

CaelumSky::CaelumSky(CaelumEnvironment& environment) :
	CaelumEnvironmentComponent(environment)
{
	/// Setup cloud options.
	if (mCaelumSystem->getCloudSystem()) {
		Caelum::CloudSystem* cloudSystem = mCaelumSystem->getCloudSystem();
		Caelum::FlatCloudLayer* cloudLayer = cloudSystem->createLayerAtHeight(1500);
		cloudLayer->setCloudSpeed(Ogre::Vector2(0.0005, -0.0009));
		cloudLayer->setCloudBlendTime(3600 * 24);
		cloudLayer->setCloudCover(0.3);
		cloudLayer->setFadeDistances(5000, 9000);
		cloudLayer->setFadeDistMeasurementVector(Ogre::Vector3(1, 0, 1));

		cloudLayer = cloudSystem->createLayerAtHeight(2000);
		cloudLayer->setCloudSpeed(Ogre::Vector2(0.00045, -0.00075));
		cloudLayer->setCloudBlendTime(3600 * 24);
		cloudLayer->setCloudCover(0.4);
		cloudLayer->setFadeDistances(5000, 9000);
		cloudLayer->setFadeDistMeasurementVector(Ogre::Vector3(1, 0, 1));
	}

	registerConfigListener("caelum", "cloudspeed", sigc::mem_fun(*this, &CaelumSky::Config_CloudSpeed));
	registerConfigListener("caelum", "cloudblendtime", sigc::mem_fun(*this, &CaelumSky::Config_CloudBlendTime));
	registerConfigListener("caelum", "cloudcover", sigc::mem_fun(*this, &CaelumSky::Config_CloudCover));

}

CaelumSky::~CaelumSky()
{
}

void CaelumSky::Config_CloudSpeed(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_string() && mCaelumSystem->getCloudSystem()) {
		Ogre::Vector2 vector;
		Ember::Tokeniser tokeniser(variable);
		vector.x = atof(tokeniser.nextToken().c_str());
		vector.y = atof(tokeniser.nextToken().c_str());
		mCaelumSystem->getCloudSystem()->getLayer(0)->setCloudSpeed(vector);
	}
}

void CaelumSky::Config_CloudBlendTime(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double() && mCaelumSystem->getCloudSystem()) {
		mCaelumSystem->getCloudSystem()->getLayer(0)->setCloudBlendTime(static_cast<double> (variable));
	}
}
void CaelumSky::Config_CloudCover(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double() && mCaelumSystem->getCloudSystem()) {
		mCaelumSystem->getCloudSystem()->getLayer(0)->setCloudCover(static_cast<double> (variable));
	}
}

void CaelumSky::setDensity(float density)
{
	mCaelumSystem->setGlobalFogDensityMultiplier(density);
}
float CaelumSky::getDensity() const
{
	return mCaelumSystem->getGlobalFogDensityMultiplier();
}

bool CaelumSky::frameEnded(const Ogre::FrameEvent & event)
{
	return true;
}

}

}
