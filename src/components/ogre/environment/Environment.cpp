//
// C++ Implementation: Environment
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Environment.h"
#include "Forest.h"
#include "domain/EmberEntity.h"
#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/terrain/TerrainHandler.h"
#include "framework/Tokeniser.h"
#include <OgreStringConverter.h>
#include <OgreSceneManager.h>

namespace Ember {
namespace OgreView {

namespace Environment {

Environment::Environment(Ogre::SceneManager& sceneMgr,
						 Terrain::TerrainManager& terrainManager,
						 std::unique_ptr<IEnvironmentProvider> provider,
						 std::unique_ptr<IEnvironmentProvider> fallbackProvider) :
		SetTime("set_time", this, "Sets the time. parameters: <hour> <minute>"),
		SetFogDensity("set_fogdensity", this, "Sets the fog density."),
		SetAmbientLight("setambientlight", this, "Set the ambient light of the world: <red> <green> <blue>"),
		mProvider(std::move(provider)),
		mFallbackProvider(std::move(fallbackProvider)),
		mForest(new Forest(terrainManager)) {
	//Set some default ambient light
	sceneMgr.setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
	terrainManager.getHandler().EventTerrainEnabled.connect(sigc::mem_fun(*this, &Environment::terrainEnabled));
	terrainManager.getHandler().EventTerrainDisabled.connect(sigc::mem_fun(*this, &Environment::terrainDisabled));

}

Environment::~Environment() = default;

void Environment::terrainEnabled(EmberEntity& entity) {
	setFirmamentEnabled(true);
	if (entity.getPredictedPos().isValid()) {
		setWorldPosition(entity.getPredictedPos().x(), entity.getPredictedPos().y());
	} else {
		setWorldPosition(0, 0);
	}

}

void Environment::terrainDisabled() {
	setFirmamentEnabled(false);
}

void Environment::runCommand(const std::string& command, const std::string& args) {
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

		if (r.empty() || b.empty() || g.empty()) {
			return;
		} else {
			Ogre::ColourValue colour(Ogre::StringConverter::parseReal(r), Ogre::StringConverter::parseReal(b), Ogre::StringConverter::parseReal(g));
			setAmbientLight(colour);
		}

	}

}

void Environment::setFirmamentEnabled(bool enabled) {
	if (enabled && mProvider) {
		try {
			mProvider->createFirmament();
			EventEnvironmentSetup.emit();
		} catch (...) {
			if (mFallbackProvider) {
				S_LOG_FAILURE("Error when creating environment, trying with fallback provider.");
				mProvider = std::move(mFallbackProvider);
				setFirmamentEnabled(enabled);
			} else {
				S_LOG_FAILURE("Error when creating environment. There's no fallback provider to use however, so we have to abort.");
				throw;
			}
		}
	} else if (!enabled && mProvider) {
		mProvider->destroyFirmament();
	}
}

void Environment::setTime(int hour, int minute, int second) {
	mProvider->setTime(hour, minute, second);
}

void Environment::setTime(int seconds) {
	mProvider->setTime(seconds);
}

void Environment::setTimeMultiplier(float multiplier) {
	mProvider->setTimeMultiplier(multiplier);
}

float Environment::getTimeMultiplier() const {
	return mProvider->getTimeMultiplier();
}

void Environment::setAmbientLight(const Ogre::ColourValue& colour) {
	if (getSun()) {
		getSun()->setAmbientLight(colour);
		EventUpdatedAmbientLight.emit(colour);
	}
}

void Environment::setWorldPosition(float longitudeDegrees, float latitudeDegrees) {
	mProvider->setWorldPosition(longitudeDegrees, latitudeDegrees);
}

void Environment::setWaterEnabled(bool enabled) {
	mProvider->setWaterEnabled(enabled);
}

WFMath::Vector<3> Environment::getMainLightDirection() const {
	if (mProvider && mProvider->getSun()) {
		return mProvider->getSun()->getMainLightDirection();
	}
	//If no provider, just point downwards.
	return WFMath::Vector<3>(0, 0, -1);
}

Ogre::ColourValue Environment::getAmbientLightColour() const {
	if (mProvider && mProvider->getSun()) {
		return mProvider->getSun()->getAmbientLightColour();
	}
	//If no provider, use white
	return Ogre::ColourValue::White;
}

}

}
}
