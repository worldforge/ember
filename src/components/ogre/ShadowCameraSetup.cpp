//
// C++ Implementation: ShadowCameraSetup
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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
#include "ShadowCameraSetup.h"
#include "ShadowDetailManager.h"
#include "framework/Tokeniser.h"
#include "framework/LoggingInstance.h"
#include <OgreRoot.h>
#include <OgreShadowCameraSetupPSSM.h>
#include <OgreShadowCameraSetup.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>

namespace Ember
{
namespace OgreView
{

ShadowCameraSetup::ShadowCameraSetup(Ogre::SceneManager& sceneMgr, GraphicalChangeAdapter& graphicalChangeAdapter) :
		mSceneMgr(sceneMgr),
		mShadowDetailManager(std::make_unique<ShadowDetailManager>(graphicalChangeAdapter, sceneMgr))
{
	//	// Need to detect D3D or GL for best depth shadowmapping
//	bool isOpenGL;
//	if (Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != Ogre::String::npos) {
//		isOpenGL = true;
//	} else {
//		isOpenGL = false;
//	}

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

	mPssmSetup = OGRE_NEW Ogre::PSSMShadowCameraSetup();
	mSharedCameraPtr = Ogre::ShadowCameraSetupPtr(mPssmSetup);
	mSceneMgr.setShadowCameraSetup(mSharedCameraPtr);

	registerConfigListenerWithDefaults("shadows", "texturesize", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowTextureSize), 1024);
	registerConfigListenerWithDefaults("shadows", "splitpoints", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowSplitPoints), "1 15 50 200");
	registerConfigListenerWithDefaults("shadows", "splitpadding", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowSplitPadding), 10.0);
	registerConfigListenerWithDefaults("shadows", "optimaladjustfactors", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowOptimalAdjustFactors), "1 1 1");
	registerConfigListenerWithDefaults("shadows", "useaggressivefocusregion", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowUseAggressiveFocusRegion), true);
	registerConfigListenerWithDefaults("shadows", "fardistance", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowFarDistance), 200.0);
	registerConfigListenerWithDefaults("shadows", "renderbackfaces", sigc::mem_fun(*this, &ShadowCameraSetup::Config_ShadowRenderBackfaces), true);

}

ShadowCameraSetup::~ShadowCameraSetup() = default;

void ShadowCameraSetup::Config_ShadowTextureSize(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_int()) {
			S_LOG_VERBOSE("Setting shadow texture size: " << static_cast<int>(variable));
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
			Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = mPssmSetup->getSplitPoints();
			Tokeniser tokeniser(variable.as_string());
			splitPointList[0] = std::stof(tokeniser.nextToken());
			splitPointList[1] = std::stof(tokeniser.nextToken());
			splitPointList[2] = std::stof(tokeniser.nextToken());
			splitPointList[3] = std::stof(tokeniser.nextToken());
			S_LOG_VERBOSE("Setting shadow split points: " << splitPointList[0] << " " << splitPointList[1] << " " << splitPointList[2] << " " << splitPointList[3]);
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
			S_LOG_VERBOSE("Setting shadow split padding: " << static_cast<double>(variable));
			mPssmSetup->setSplitPadding((float)static_cast<double>(variable));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow split padding." << ex);
	}
}

void ShadowCameraSetup::Config_ShadowOptimalAdjustFactors(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_string()) {
			S_LOG_VERBOSE("Setting shadow optimal adjust factor: " << static_cast<std::string>(variable));
			Tokeniser tokeniser(variable.as_string());
			mPssmSetup->setOptimalAdjustFactor(0, std::stof(tokeniser.nextToken()));
			mPssmSetup->setOptimalAdjustFactor(1, std::stof(tokeniser.nextToken()));
			mPssmSetup->setOptimalAdjustFactor(2, std::stof(tokeniser.nextToken()));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow optimal adjust factors." << ex);
	}
}

void ShadowCameraSetup::Config_ShadowUseAggressiveFocusRegion(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_bool()) {
			S_LOG_VERBOSE("Setting shadow use aggressive focus region: " << static_cast<bool>(variable));
			mPssmSetup->setUseAggressiveFocusRegion(static_cast<bool>(variable));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow use aggressive focus region." << ex);
	}
}

void ShadowCameraSetup::Config_ShadowFarDistance(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_double()) {
			S_LOG_VERBOSE("Setting shadow far distance: " << static_cast<double>(variable));
			mSceneMgr.setShadowFarDistance((float)static_cast<double>(variable));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow far distance." << ex);
	}
}

void ShadowCameraSetup::Config_ShadowRenderBackfaces(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	try {
		if (variable.is_bool()) {
			S_LOG_VERBOSE("Setting shadow render back faces: " << static_cast<bool>(variable));
			mSceneMgr.setShadowCasterRenderBackFaces(static_cast<bool>(variable));
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting shadow render back faces." << ex);
	}

}

}
}
