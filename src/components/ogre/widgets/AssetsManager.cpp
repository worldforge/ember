//
// C++ Implementation: AssetsManager
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include "AssetsManager.h"
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/RendererModules/Ogre/Texture.h>
#include <OgreMaterialSerializer.h>
#include "../EmberOgrePrerequisites.h"

#include "../EmberOgre.h"
#include "../GUIManager.h"

#include "framework/Exception.h"
#include "framework/osdir.h"

#include <OgreTextureManager.h>
#include <OgreTexture.h>
#include <OgreString.h>
#include <OgreMeshSerializer.h>
// #include <OgreBitwise.h>

#include <CEGUI/Image.h>
#include <CEGUI/BasicImage.h>
#include <CEGUI/ImageManager.h>
#include <CEGUI/System.h>
#include <CEGUI/Renderer.h>

namespace Ember
{
namespace OgreView
{

namespace Gui
{

AssetsManager::AssetsManager()
{
	// 	createTextureImage();
}

AssetsManager::~AssetsManager()
{
}

TexturePair AssetsManager::showTexture(const std::string textureName)
{
	// 	if (!mOgreCEGUITexture) {
	// 		S_LOG_WARNING("You must first create a valid OgreCEGUITexture instance.");
	// 		return;
	// 	}
	if (Ogre::TextureManager::getSingleton().resourceExists(textureName)) {
		Ogre::TexturePtr texturePtr = static_cast<Ogre::TexturePtr>(Ogre::TextureManager::getSingleton().getByName(textureName));
		if (!texturePtr.isNull()) {
			if (!texturePtr->isLoaded()) {
				try {
					texturePtr->load();
				} catch (...) {
					S_LOG_WARNING("Error when loading " << textureName << ". This texture will not be shown.");
					return TexturePair();
				}
			}
			std::string textureName(texturePtr->getName());
			std::string imageSetName(textureName + "_AssetsManager");

			return createTextureImage(texturePtr, imageSetName);
			// 			mOgreCEGUITexture->setOgreTexture(texturePtr);
		}
	}
	return TexturePair();

}

TexturePair AssetsManager::createTextureImage(Ogre::TexturePtr texturePtr, const std::string& imageName)
{
	// 	if (mOgreCEGUITexture) {
	// 		GUIManager::getSingleton().getGuiRenderer()->destroyTexture(mOgreCEGUITexture);
	// 		mOgreCEGUITexture = 0;
	// 	}

	auto renderer = CEGUI::System::getSingleton().getRenderer();

	CEGUI::Texture* ogreCEGUITexture;
	if (renderer->isTextureDefined(texturePtr->getName())) {
		ogreCEGUITexture = &renderer->getTexture(texturePtr->getName());
		static_cast<CEGUI::OgreTexture*>(ogreCEGUITexture)->setOgreTexture(texturePtr);
	} else {
		//create a CEGUI texture from our Ogre texture
		S_LOG_VERBOSE("Creating new CEGUI texture from Ogre texture.");
		ogreCEGUITexture = &GUIManager::getSingleton().createTexture(texturePtr);
	}

	//assign our image element to the StaticImage widget
	CEGUI::Image* textureImage;
	if (CEGUI::ImageManager::getSingleton().isDefined(imageName)) {
		textureImage = &CEGUI::ImageManager::getSingleton().get(imageName);
	} else {
		textureImage = &CEGUI::ImageManager::getSingleton().create("BasicImage", imageName);
	}

	CEGUI::BasicImage* basicImage = static_cast<CEGUI::BasicImage*>(textureImage);
	basicImage->setTexture(ogreCEGUITexture);
	auto area = CEGUI::Rectf(0, 0, ogreCEGUITexture->getSize().d_width, ogreCEGUITexture->getSize().d_height);
	basicImage->setArea(area);
	basicImage->setNativeResolution(area.getSize());
	basicImage->setAutoScaled(CEGUI::ASM_Both);

	return TexturePair(texturePtr, textureImage);

}

std::string AssetsManager::materialAsText(Ogre::MaterialPtr material)
{
	if (material.isNull()) {
		return "";
	}
	Ogre::MaterialSerializer serializer;
	serializer.queueForExport(material, true, false);
	return serializer.getQueuedAsString();
}

std::string AssetsManager::resolveResourceNameFromFilePath(const std::string& filePath)
{
	const std::multimap<std::string, std::string>& locations = EmberOgre::getSingleton().getResourceLocations();

	for (std::multimap<std::string, std::string>::const_iterator I = locations.begin(); I != locations.end(); ++I) {
		const std::string resourceLocation = I->second;
		if (Ogre::StringUtil::startsWith(filePath, resourceLocation, true)) {
			//Replace backwards slashes on windows with forwards slashes as that's what's used in the Ogre resource system.
			std::string localMeshPath = filePath.substr(resourceLocation.length(), std::string::npos);
			std::replace(localMeshPath.begin(), localMeshPath.end(), '\\', '/');
			return localMeshPath;
		}
	}
	return filePath;
}

std::string AssetsManager::resolveFilePathForMesh(Ogre::MeshPtr meshPtr)
{
	Ogre::ResourceGroupManager& manager = Ogre::ResourceGroupManager::getSingleton();
	const std::multimap<std::string, std::string>& locations = EmberOgre::getSingleton().getResourceLocations();

	for (std::multimap<std::string, std::string>::const_iterator I = locations.begin(); I != locations.end(); ++I) {
		std::string group = I->first;
		std::string fileName = meshPtr->getName();
		Ogre::FileInfoListPtr files = manager.findResourceFileInfo(group, fileName, false);
		for (Ogre::FileInfoList::const_iterator J = files->begin(); J != files->end(); ++J) {
			if (J->filename == fileName) {
				return I->second + J->filename;
			}
		}
	}
	return "";

}

bool AssetsManager::exportMesh(Ogre::MeshPtr mesh, const std::string& filePath)
{
	if (filePath != "") {
		Ogre::MeshSerializer serializer;
		try {
			std::string dirname(filePath);
			size_t end = dirname.find_last_of("/\\");
			if (end != std::string::npos) {
				dirname = dirname.substr(0, end);
			}
			oslink::directory osdir(dirname);
			if (!osdir.isExisting()) {
				oslink::directory::mkdir(dirname.c_str());
			}
			serializer.exportMesh(mesh.get(), filePath);
			S_LOG_INFO("Exported mesh " << filePath);
		} catch (const Ogre::Exception& ex) {
			S_LOG_FAILURE("Error when exporting mesh " << mesh->getName() << "to path " << filePath <<"." << ex);
			return false;
		}
		return true;
	}
	return false;
}

// bool AssetsManager::exportTexture(Ogre::TexturePtr texturePtr)
// {
//  getRenderTarget()->writeContentsToFile();
// }

}

}
}
