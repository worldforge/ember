//
// C++ Implementation: ShadowCameraSetup
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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

#include "ShadowCameraSetup.h"
#include "framework/Tokeniser.h"
#include "framework/LoggingInstance.h"
#include <OgreRoot.h>
#include <OgreRenderSystem.h>

namespace EmberOgre {

ShadowCameraSetup::ShadowCameraSetup(Ogre::SceneManager& sceneMgr)
: mSceneMgr(sceneMgr)
{
	setup();
	registerConfigListenerWithDefaults("shadows", "texturesize", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowTextureSize), 1024);
	registerConfigListenerWithDefaults("shadows", "splitpoints", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowSplitPoints), "1 15 50 200");
	registerConfigListenerWithDefaults("shadows", "splitpadding", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowSplitPadding), 10.0);
	registerConfigListenerWithDefaults("shadows", "optimaladjustfactors", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowOptimalAdjustFactors), "1 1 1");
	registerConfigListenerWithDefaults("shadows", "useaggressivefocusregion", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowUseAggressiveFocusRegion), true);
	registerConfigListenerWithDefaults("shadows", "fardistance", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowFarDistance), 200.0);
}


ShadowCameraSetup::~ShadowCameraSetup()
{
}

bool ShadowCameraSetup::setup()
{
	// Need to detect D3D or GL for best depth shadowmapping
	bool isOpenGL;
	if (Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != Ogre::String::npos) {
		isOpenGL = true;
	} else {
		isOpenGL = false;
	}

	mSceneMgr.setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);

	mSceneMgr.setShadowTextureCount(5);
	mSceneMgr.setShadowTextureSelfShadow(true);

	mSceneMgr.setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
	mSceneMgr.setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 1);
	mSceneMgr.setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 1);

	//if (isOpenGL) {
	//	// GL performs much better if you pick half-float format
	//	mSceneMgr.setShadowTexturePixelFormat(Ogre::PF_FLOAT16_R);
	//} else {
	//	// D3D is the opposite - if you ask for PF_FLOAT16_R you
	//	// get an integer format instead! You can ask for PF_FLOAT16_GR
	//	// but the precision doesn't work well
		mSceneMgr.setShadowTexturePixelFormat(Ogre::PF_FLOAT32_R);
	//}

	mSceneMgr.setShadowTextureCasterMaterial("Ogre/DepthShadowmap/Caster/Float/NoAlpha");
	mSceneMgr.setShadowCasterRenderBackFaces(true);

	mPssmSetup = OGRE_NEW Ogre::PSSMShadowCameraSetup();
	mSharedCameraPtr = Ogre::ShadowCameraSetupPtr(mPssmSetup);
	mSceneMgr.setShadowCameraSetup(mSharedCameraPtr);

	return true;
}

void ShadowCameraSetup::Config_ShadowTextureSize(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_int()) {
			mSceneMgr.setShadowTextureSize(static_cast<int>(variable));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow texture size." << ex);
	}
}
void ShadowCameraSetup::Config_ShadowSplitPoints(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_string()) {
			Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = mPssmSetup->getSplitPoints();;
			Ember::Tokeniser tokeniser(variable);
			splitPointList[0] = atof(tokeniser.nextToken().c_str());
			splitPointList[1] = atof(tokeniser.nextToken().c_str());
			splitPointList[2] = atof(tokeniser.nextToken().c_str());
			splitPointList[3] = atof(tokeniser.nextToken().c_str());
			S_LOG_VERBOSE("Setting split points: " << splitPointList[0] << " " << splitPointList[1] << " " << splitPointList[2] << " " << splitPointList[3]);
			mPssmSetup->setSplitPoints(splitPointList);
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow split points." << ex);
	}
}
void ShadowCameraSetup::Config_ShadowSplitPadding(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_double()) {
			mPssmSetup->setSplitPadding(static_cast<double>(variable));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow split padding." << ex);
	}
}

void ShadowCameraSetup::Config_ShadowOptimalAdjustFactors(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_string()) {
			Ember::Tokeniser tokeniser(variable);
			mPssmSetup->setOptimalAdjustFactor(0, atof(tokeniser.nextToken().c_str()));
			mPssmSetup->setOptimalAdjustFactor(1, atof(tokeniser.nextToken().c_str()));
			mPssmSetup->setOptimalAdjustFactor(2, atof(tokeniser.nextToken().c_str()));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow optimal adjust factors." << ex);
	}
}

void ShadowCameraSetup::Config_ShadowUseAggressiveFocusRegion(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_bool()) {
			mPssmSetup->setUseAggressiveFocusRegion(static_cast<bool>(variable));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow use aggressive focus region." << ex);
	}
}

void ShadowCameraSetup::Config_ShadowFarDistance(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_bool()) {
			S_LOG_VERBOSE("Setting shadow far distance: " << static_cast<double>(variable));
			mSceneMgr.setShadowFarDistance(static_cast<double>(variable));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow far distance." << ex);
	}
}

}
