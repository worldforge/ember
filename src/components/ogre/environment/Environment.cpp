//
// C++ Implementation: Environment
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

#include "Environment.h"
#include "Forest.h"
#include "framework/Tokeniser.h"
#include <OgreStringConverter.h>
#include <OgreColourValue.h>
#include <OgreSceneManager.h>

namespace Ember
{
namespace OgreView
{

namespace Environment
{

Environment::Environment(Ogre::SceneManager& sceneMgr, Terrain::TerrainManager& terrainManager, IEnvironmentProvider* provider, IEnvironmentProvider* fallbackProvider) :
		SetTime("set_time", this, "Sets the time. parameters: <hour> <minute>"), SetFogDensity("set_fogdensity", this, "Sets the fog density."), SetAmbientLight("setambientlight", this, "Set the ambient light of the world: <red> <green> <blue>"), mProvider(provider), mFallbackProvider(fallbackProvider), mEnabledFirmamentProvider(0), mForest(new Forest(terrainManager))
{
	//Set some default ambient light
	sceneMgr.setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
}

Environment::~Environment()
{
	delete mProvider;
	delete mFallbackProvider;
	delete mForest;
}

void Environment::runCommand(const std::string &command, const std::string &args)
{
// 	if (SetTime == command) {
// 		Tokeniser tokeniser;
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
		if (getFog()) {
			Tokeniser tokeniser;
			tokeniser.initTokens(args);
			std::string densityString = tokeniser.nextToken();

			float density = ::Ogre::StringConverter::parseReal(densityString);
			getFog()->setDensity(density);
		}

	} else if (SetAmbientLight == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string r = tokeniser.nextToken();
		std::string b = tokeniser.nextToken();
		std::string g = tokeniser.nextToken();

		if (r == "" || b == "" || g == "") {
			return;
		} else {
			Ogre::ColourValue colour(Ogre::StringConverter::parseReal(r), Ogre::StringConverter::parseReal(b), Ogre::StringConverter::parseReal(g));
			setAmbientLight(colour);
		}

	}

}

void Environment::setFirmamentEnabled(bool enabled)
{
	if (enabled && !mEnabledFirmamentProvider) {
		try {
			mProvider->createFirmament();
			mEnabledFirmamentProvider = mProvider;
			EventEnvironmentSetup.emit();
		} catch (...) {
			if (mFallbackProvider) {
				S_LOG_FAILURE("Error when creating environment, trying with fallback provider.");
				delete mProvider;
				mProvider = mFallbackProvider;
				mFallbackProvider = 0;
				mProvider->createFirmament();
				mEnabledFirmamentProvider = mProvider;
			} else {
				S_LOG_FAILURE("Error when creating environment. There's no fallback provider to use however, so we have to abort.");
				throw;
			}
		}
	} else if (!enabled && mEnabledFirmamentProvider) {
		mEnabledFirmamentProvider->destroyFirmament();
		mEnabledFirmamentProvider = 0;
	}
}

void Environment::setTime(int hour, int minute, int second)
{
	mProvider->setTime(hour, minute, second);
}

void Environment::setTime(int seconds)
{
	mProvider->setTime(seconds);
}

void Environment::setTimeMultiplier(float multiplier)
{
	mProvider->setTimeMultiplier(multiplier);
}

float Environment::getTimeMultiplier() const
{
	return mProvider->getTimeMultiplier();
}

void Environment::setAmbientLight(const Ogre::ColourValue& colour)
{
	if (getSun()) {
		getSun()->setAmbientLight(colour);
		EventUpdatedAmbientLight.emit(colour);
	}
}

void Environment::setWorldPosition(float longitudeDegrees, float latitudeDegrees)
{
	mProvider->setWorldPosition(longitudeDegrees, latitudeDegrees);
}

void Environment::setWaterEnabled(bool enabled)
{
	mProvider->setWaterEnabled(enabled);
}

WFMath::Vector<3> Environment::getMainLightDirection() const
{
	if (mEnabledFirmamentProvider && mEnabledFirmamentProvider->getSun()) {
		return mEnabledFirmamentProvider->getSun()->getMainLightDirection();
	} else if (mProvider && mProvider->getSun()) {
		return mProvider->getSun()->getMainLightDirection();
	}
	//If no provider, just point downwards.
	return WFMath::Vector<3>(0, 0, -1);
}

Ogre::ColourValue Environment::getAmbientLightColour() const
{
	if (mEnabledFirmamentProvider && mEnabledFirmamentProvider->getSun()) {
		return mEnabledFirmamentProvider->getSun()->getAmbientLightColour();
	} else if (mProvider && mProvider->getSun()) {
		return mProvider->getSun()->getAmbientLightColour();
	}
	//If no provider, use white
	return Ogre::ColourValue::White;
}

}

}
}
