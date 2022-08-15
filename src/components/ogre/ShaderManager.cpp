//
// C++ Implementation: ShaderManager
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2009
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

#include "ShaderManager.h"

#include "ShadowCameraSetup.h"
#include "EmberOgrePrerequisites.h"
#include "EmberOgre.h"
#include "framework/Tokeniser.h"
#include "services/config/ConfigService.h"

#include <OgreMaterialManager.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreTechnique.h>
#include <OgreViewport.h>

namespace Ember::OgreView {

/**
 * @brief A shader setup instance which envelops a scene manager and handles the shadow camera setup for that manager.
 */
class ShaderSetupInstance {
private:

	Ogre::SceneManager& mSceneManager;

	/**
	 * @brief Takes care of the setup of the pssm shadow camera. Owned by this instance.
	 */
	std::unique_ptr<ShadowCameraSetup> mShadowCameraSetup;

	/**
	 * @brief Reference to the automatic graphics manager that is to be passed on when shadow camera is set up.
	 */
	GraphicalChangeAdapter& mGraphicalChangeAdapter;

public:

	ShaderSetupInstance(Ogre::SceneManager& sceneManager, GraphicalChangeAdapter& graphicalChangeAdapter) :
			mSceneManager(sceneManager),
			mGraphicalChangeAdapter(graphicalChangeAdapter) {

	}

	~ShaderSetupInstance() = default;

	void setPSSMShadows() {
		mShadowCameraSetup = std::make_unique<ShadowCameraSetup>(mSceneManager, mGraphicalChangeAdapter);
	}

	void setNoShadows() {
		mShadowCameraSetup.reset();
		mSceneManager.setShadowTechnique(Ogre::SHADOWTYPE_NONE);
		//This will make any other camera setup delete itself (unless held by another shared pointer).
		mSceneManager.setShadowCameraSetup(Ogre::ShadowCameraSetupPtr());
	}
};

ShaderManager::ShaderManager(GraphicalChangeAdapter& graphicalChangeAdapter) :
		SetLevel("set_level", this, "Sets the graphics level. Parameters: <level>. Level is one of: high, medium, low."),
		mGraphicsLevel(LEVEL_HIGH),
		mGraphicalChangeAdapter(graphicalChangeAdapter) {
	mGraphicSchemes[LEVEL_LOW] = "Low";
	mGraphicSchemes[LEVEL_MEDIUM] = "Medium";
	mGraphicSchemes[LEVEL_HIGH] = "High";


}


void ShaderManager::init() {
	registerConfigListener("graphics", "level", sigc::mem_fun(*this, &ShaderManager::Config_Level), true);
}


ShaderManager::~ShaderManager() = default;

ShaderManager::GraphicsLevel ShaderManager::getGraphicsLevel() const {
	return mGraphicsLevel;
}

void ShaderManager::runCommand(const std::string& command, const std::string& args) {
	if (SetLevel == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string levelString = tokeniser.nextToken();
		ConfigService::getSingleton().setValue("graphics", "level", levelString);
	}
}

void ShaderManager::Config_Level(const std::string& section, const std::string& key, varconf::Variable& variable) {
	if (variable.is_string()) {
		setGraphicsLevel(getLevelByName(std::string(variable)));
	}
}

ShaderManager::GraphicsLevel ShaderManager::getLevelByName(const std::string& level) const {
	std::string levelString = level;
	std::transform(levelString.begin(), levelString.end(), levelString.begin(), (int (*)(int)) std::tolower);

	for (const auto& entry: mGraphicSchemes) {
		std::string scheme = entry.second;
		std::transform(scheme.begin(), scheme.end(), scheme.begin(), (int (*)(int)) std::tolower);
		if (levelString == scheme) {
			return entry.first;
		}
	}

	throw std::runtime_error(std::string("Not a valid graphics level: ") + level);
}

const std::map<ShaderManager::GraphicsLevel, std::string>& ShaderManager::getGraphicsScheme() const {
	return mGraphicSchemes;
}

void ShaderManager::registerSceneManager(Ogre::SceneManager* sceneManager) {
	auto instance = std::make_unique<ShaderSetupInstance>(*sceneManager, mGraphicalChangeAdapter);
	mShaderSetups.emplace(sceneManager, std::move(instance));
	setGraphicsLevel(mGraphicsLevel); //TODO: set it per new scene manager instead
}

void ShaderManager::deregisterSceneManager(Ogre::SceneManager* sceneManager) {
	auto I = mShaderSetups.find(sceneManager);
	if (I != mShaderSetups.end()) {
		mShaderSetups.erase(I);
	}
}

void ShaderManager::setGraphicsLevel(ShaderManager::GraphicsLevel newLevel) {
	std::string scheme = mGraphicSchemes[newLevel];

	S_LOG_INFO("Using graphics level " << scheme);
	Ogre::MaterialManager::getSingleton().setActiveScheme(scheme);

	Ogre::RenderWindow* window = EmberOgre::getSingleton().getRenderWindow();
	for (int i = 0; i < window->getNumViewports(); ++i) {
		window->getViewport(i)->setMaterialScheme(scheme);
	}

	switch (newLevel) {
		case LEVEL_HIGH:
			setPSSMShadows();
			break;

		case LEVEL_MEDIUM:
		case LEVEL_LOW:
			setNoShadows();
			break;
	}

	switch (newLevel) {
		case LEVEL_HIGH:
			Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
			break;
		case LEVEL_MEDIUM:
			Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_TRILINEAR);
			break;
		case LEVEL_LOW:
			Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_BILINEAR);
			break;
	}

	mGraphicsLevel = newLevel;

	EventLevelChanged.emit();
}

void ShaderManager::setPSSMShadows() {
	for (auto& entry: mShaderSetups) {
		entry.second->setPSSMShadows();
	}
}

void ShaderManager::setNoShadows() {
	for (auto& entry: mShaderSetups) {
		entry.second->setNoShadows();
	}
}

}
