/*
 * Copyright (C) 2014 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "ConsoleDevTools.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "components/ogre/GUIManager.h"
#include <CEGUI/BasicImage.h>
#include <CEGUI/ImageManager.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/DefaultWindow.h>
#include <CEGUI/widgets/FrameWindow.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <OgreGpuProgramManager.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
namespace Ember
{
namespace OgreView
{

ConsoleDevTools::ConsoleDevTools() :
	mReloadMaterial("reload_material", this, "Reloads the material. Parameters: <material name>. For example \"/reload_material /base/normalmap/specular\""),
	mShowTexture("show_texture", this, "Show given texture in a separated window. Parameters: <texture name>. For example \"/show_texture DepthBuffer\"")
{

}

void ConsoleDevTools::runCommand(const std::string& command, const std::string& args)
{
	if (mReloadMaterial == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string materialName(tokeniser.nextToken());
		reloadMaterial(materialName);

	} else if (mShowTexture == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string textureName(tokeniser.nextToken());
		showTexture(textureName);
	}
}

std::string ConsoleDevTools::genUniqueName()
{
	static int counter = 0;
	counter++;
	std::stringstream ret;
	ret << "unique" << counter;
	return ret.str();
}

void ConsoleDevTools::showTexture(const std::string& textureName)
{
	const float left = 0.0f;
	const float top = 0.0f;
	const float widthScale = 0.25f;
	const float heightScale = 0.25f;

	Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(textureName);
	if (texture.isNull()) {
		ConsoleBackend::getSingleton().pushMessage("'" + textureName + "' texture not found.", "error");
		return;
	}

	CEGUI::System* sys = CEGUI::System::getSingletonPtr();
	CEGUI::Window* root = sys->getDefaultGUIContext().getRootWindow();
	CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();
	const std::string& scheme = Ember::OgreView::GUIManager::getSingleton().getDefaultScheme();
	CEGUI::OgreRenderer* renderer = (CEGUI::OgreRenderer*) sys->getRenderer();

	//Create BasicImage wrapper for the Ogre texture.
	std::string imageName = genUniqueName();
	CEGUI::Texture& ceguitexture = renderer->createTexture(imageName, texture);
	CEGUI::BasicImage* image = (CEGUI::BasicImage*) (&CEGUI::ImageManager::getSingleton().create("BasicImage", imageName));
	image->setTexture(&ceguitexture);
	image->setArea(CEGUI::Rectf(0, 0, texture->getWidth(), texture->getHeight()));
	image->setAutoScaled(CEGUI::ASM_Both);

	// Create staticImage to show the BasicImage.
	CEGUI::DefaultWindow* staticImage = static_cast<CEGUI::DefaultWindow*>(wmgr->createWindow(scheme + "/StaticImage", genUniqueName()));
	staticImage->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	staticImage->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	staticImage->setVisible(true);
	staticImage->setEnabled(true);
	staticImage->setUpdateMode(CEGUI::WUM_VISIBLE);
	staticImage->setUsingAutoRenderingSurface(false);

	// Create scalable Frame window
	CEGUI::FrameWindow* mWindow = (CEGUI::FrameWindow*) wmgr->createWindow(scheme + "/DialogWindow", genUniqueName());
	mWindow->setText(textureName + " texture");
	mWindow->setSize(CEGUI::USize(CEGUI::UDim(0, texture->getWidth() * widthScale), CEGUI::UDim(0, texture->getHeight() * heightScale)));

	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, left), CEGUI::UDim(0, top)));
	mWindow->moveToFront();
	mWindow->setSizingBorderThickness(5);
	mWindow->setCloseButtonEnabled(true);
	mWindow->setUpdateMode(CEGUI::WUM_VISIBLE);
	mWindow->setUsingAutoRenderingSurface(false);
	mWindow->setEnabled(true);
	mWindow->setVisible(true);

	auto onCloseClicked = [&] (const CEGUI::EventArgs & e)->bool
	{
		const CEGUI::MouseEventArgs& me = static_cast<const CEGUI::MouseEventArgs&>(e);
		me.window->destroy();
		return true;
	};

	mWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, onCloseClicked);

	// Link windows together
	staticImage->setProperty("Image", imageName);
	mWindow->addChild((CEGUI::Window*) staticImage);
	root->addChild((CEGUI::Window*) mWindow);
}

void ConsoleDevTools::reloadMaterial(const std::string& materialName)
{
	// Normally multiple techniques are using the same texture,
	// so to prevent reloading the texture multiple times, I will put them in a set.
	std::set<std::string> mReloadableTextures;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);
	if (material.isNull()) {
		ConsoleBackend::getSingleton().pushMessage("'" + materialName + "' material not found.", "error");
		return;
	}
	Ogre::Material::TechniqueIterator techniqueIterator = material->getTechniqueIterator();
	while (techniqueIterator.hasMoreElements()) {
		Ogre::Technique* technique = techniqueIterator.getNext();
		Ogre::Technique::PassIterator passIterator = technique->getPassIterator();
		while (passIterator.hasMoreElements()) {
			Ogre::Pass* pass = passIterator.getNext();
			Ogre::String name;
			Ogre::GpuProgramPtr vertexProgram;
			Ogre::GpuProgramPtr fragmentProgram;
			if (pass->hasVertexProgram()) {
				vertexProgram = pass->getVertexProgram();
				name += "Vertex Program:";
				name += vertexProgram->getName();
			}
			if (pass->hasFragmentProgram()) {
				fragmentProgram = pass->getFragmentProgram();
				name += " Fragment Program:";
				name += fragmentProgram->getName();
			}
			// NOTE: Microcode name may depend on render system. This works for OpenGL!
			Ogre::GpuProgramManager::getSingleton().removeMicrocodeFromCache(name);

			if (!vertexProgram.isNull()) {
				vertexProgram->reload();
				vertexProgram->createParameters();
				ConsoleBackend::getSingleton().pushMessage("Reloaded vertex program '" + vertexProgram->getName() + "'.", "info");
			}
			if (!fragmentProgram.isNull()) {
				fragmentProgram->reload();
				fragmentProgram->createParameters();
				ConsoleBackend::getSingleton().pushMessage("Reloaded fragment program '" + fragmentProgram->getName() + "'.", "info");
			}
			Ogre::Pass::TextureUnitStateIterator it = pass->getTextureUnitStateIterator();
			for (; it.hasMoreElements(); it.moveNext()) {
				Ogre::TextureUnitState* tex = *it.current();
				std::string textureName(tex->getTextureName());
				if (!textureName.empty()) {
					mReloadableTextures.insert(textureName);
				}
			}
		}
	}
	for (std::string textureName : mReloadableTextures) {
		reloadTexture(textureName);
	}
	material->reload();
}

void ConsoleDevTools::reloadTexture(const std::string& textureName)
{
	Ogre::ResourcePtr texture = Ogre::TextureManager::getSingleton().getByName(textureName);
	if (texture.isNull()) {
		ConsoleBackend::getSingleton().pushMessage("'" + textureName + "' texture not found.", "error");
		return;
	}
	if (!texture->isReloadable()) {
		ConsoleBackend::getSingleton().pushMessage("'" + textureName + "' texture is not reloadable. Skipped!", "info");
		return;
	}
	texture->reload();
	ConsoleBackend::getSingleton().pushMessage("'" + textureName + "' texture reloaded.", "info");
}

}
}
