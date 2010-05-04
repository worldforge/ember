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
#include <OgreCEGUIRenderer.h>
#include <OgreCEGUITexture.h>
#include <OgreMaterialSerializer.h>
#include "../EmberOgrePrerequisites.h"

#include <CEGUIImagesetManager.h>
#include <CEGUIImageset.h>

#include "../EmberOgre.h"
#include "../GUIManager.h"

#include "framework/Exception.h"
#include <OgreTextureManager.h>
#include <OgreTexture.h>
#include <OgreString.h>
#include <OgreMeshSerializer.h>
// #include <OgreBitwise.h>


namespace EmberOgre
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
		Ogre::TexturePtr texturePtr = static_cast<Ogre::TexturePtr> (Ogre::TextureManager::getSingleton().getByName(textureName));
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

TexturePair AssetsManager::createTextureImage(Ogre::TexturePtr texturePtr, const std::string& imageSetName)
{
	// 	if (mOgreCEGUITexture) {
	// 		GUIManager::getSingleton().getGuiRenderer()->destroyTexture(mOgreCEGUITexture);
	// 		mOgreCEGUITexture = 0;
	// 	}


	CEGUI::Imageset* textureImageset;

	if (CEGUI::ImagesetManager::getSingleton().isImagesetPresent(imageSetName)) {
		textureImageset = CEGUI::ImagesetManager::getSingleton().getImageset(imageSetName);
	} else {
		///create a CEGUI texture from our Ogre texture
		S_LOG_VERBOSE("Creating new CEGUI texture from Ogre texture.");
		CEGUI::Texture* ogreCEGUITexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(texturePtr);

		///we need a imageset in order to create GUI elements from the ceguiTexture
		S_LOG_VERBOSE("Creating new CEGUI imageset with name " << imageSetName);
		textureImageset = CEGUI::ImagesetManager::getSingleton().createImageset(imageSetName, ogreCEGUITexture);

		///we only want one element: the whole texture
		textureImageset->defineImage("full_image", CEGUI::Rect(0, 0, texturePtr->getWidth(), texturePtr->getHeight()), CEGUI::Point(0, 0));
	}
	///assign our image element to the StaticImage widget
	const CEGUI::Image* textureImage = &textureImageset->getImage("full_image");

	return TexturePair(texturePtr, textureImage, textureImageset);

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
