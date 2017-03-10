//
// C++ Implementation: OgreInfo
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreInfo.h"
#include <OgreSceneManagerEnumerator.h>
#include <OgreRoot.h>
#include <OgreEntity.h>

#ifdef HAVE_OPENGL
#if defined(__WIN32__)
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif

namespace Ember
{
namespace OgreView
{

long long int OgreInfo::sResourceCounter(1);

bool OgreInfo::isIndirect()
{
#ifdef WIN32
	//TODO: add checks for win32 too
	return false;
#else
#ifdef HAVE_OPENGL
	const GLubyte* pcRenderer = glGetString(GL_RENDERER);
	const std::string renderer((const char*)pcRenderer);

	return renderer.find("Indirect") != std::string::npos;
#else
	return false;
#endif
#endif

}

std::string OgreInfo::createUniqueResourceName(const std::string& resourceName)
{
	std::stringstream ss;
	ss << resourceName << "_" << sResourceCounter++;
	return ss.str();
}

unsigned int countNodes(Ogre::Node* node)
{
	unsigned int counter = 1;
	for (unsigned short i = 0; i < node->numChildren(); ++i) {
		counter += countNodes(node->getChild(i));
	}
	return counter;
}

void OgreInfo::diagnose(std::ostream& outputStream)
{
	Ogre::SceneManagerEnumerator::SceneManagerIterator sceneManagerI = Ogre::Root::getSingleton().getSceneManagerIterator();
	while (sceneManagerI.hasMoreElements()) {
		Ogre::SceneManager* sceneManager = sceneManagerI.getNext();
		outputStream << "Scenemanager(" << sceneManager->getTypeName() << ") " << sceneManager->getName() << std::endl;
		outputStream << " Number of scene nodes: " << countNodes(sceneManager->getRootSceneNode()) << std::endl;
		outputStream << " Movable objects:" << std::endl;
		unsigned int movableObjectCounter = 0;
		Ogre::Root::MovableObjectFactoryIterator movableObjectFactoryI = Ogre::Root::getSingleton().getMovableObjectFactoryIterator();
		while (movableObjectFactoryI.hasMoreElements()) {
			Ogre::MovableObjectFactory* factory = movableObjectFactoryI.getNext();
			std::string type(factory->getType());
			{
				Ogre::SceneManager::MovableObjectIterator I = sceneManager->getMovableObjectIterator(type);
				while (I.hasMoreElements()) {
					movableObjectCounter++;
					Ogre::MovableObject* movable = I.getNext();
					if (movable->getMovableType() == "Light") {
						Ogre::Light* light = static_cast<Ogre::Light*> (movable);
						outputStream << "  * Light " << light->getName() << "(" << (light->isInScene() ? "in scene" : "not in scene") << ")" << std::endl;
						outputStream << "   Pos: " << light->getDerivedPosition() << std::endl;
						outputStream << "   Direction: " << light->getDerivedDirection() << std::endl;

					} else {
						std::stringstream ssPosAndOrientation;
						if (movable->getParentSceneNode() && movable->isInScene()) {
							ssPosAndOrientation << " pos: " << movable->getParentSceneNode()->getPosition() << " orientation: " << movable->getParentSceneNode()->getOrientation();
						}
						outputStream << "  * " << type << " " << movable->getName() << "(" << (movable->isInScene() ? "in scene" : "not in scene") << ")" << ssPosAndOrientation.str() << std::endl;
						//					outputStream << "  Pos: " << light->getDerivedPosition() << std::endl;
						//					outputStream << "  Direction: " << light->getDerivedDirection() << std::endl;
					}
				}
			}
		}

		outputStream << " Number of movable objects: " << movableObjectCounter << std::endl;

		outputStream << " Cameras:" << std::endl;
		{
			Ogre::SceneManager::CameraIterator I = sceneManager->getCameraIterator();
			while (I.hasMoreElements()) {
				Ogre::Camera* camera = I.getNext();
				outputStream << "  Camera " << camera->getName() << "(" << (camera->isInScene() ? "in scene" : "not in scene") << ")" << std::endl;
				outputStream << "  Pos: " << camera->getDerivedPosition() << std::endl;
				outputStream << "  Direction: " << camera->getDerivedDirection() << std::endl;
				outputStream << "  Clip distances: " << camera->getNearClipDistance() << " - " << camera->getFarClipDistance() << std::endl;
			}
		}

	}

	size_t resourceMemoryUsage = 0;
	outputStream << "Resource Managers:" << std::endl;
	Ogre::ResourceGroupManager::ResourceManagerIterator I = Ogre::ResourceGroupManager::getSingleton().getResourceManagerIterator();
	while (I.hasMoreElements()) {
		std::string name = I.peekNextKey();
		Ogre::ResourceManager* manager = I.getNext();
		outputStream << " Resource Manager: " << name << std::endl;
		if (manager->getMemoryBudget() == std::numeric_limits<size_t>::max()) {
			outputStream << "  Memory budget: not set" << std::endl;
		} else {
			outputStream << "  Memory budget: " << manager->getMemoryBudget() << " bytes" << std::endl;
		}
		outputStream << "  Memory usage:  " << manager->getMemoryUsage() << " bytes" << std::endl;
		resourceMemoryUsage += manager->getMemoryUsage();

		Ogre::ResourceManager::ResourceMapIterator resourceI = manager->getResourceIterator();
		if (resourceI.hasMoreElements()) {
			outputStream << "  Resources: " << std::endl;
			int resourceCount = 0;
			int loadedResourceCount = 0;
			while (resourceI.hasMoreElements()) {
				Ogre::ResourcePtr resource = resourceI.getNext();
				if (resource->isLoaded()) {
					std::string reloadable = resource->isReloadable() ? " reloadable" : "";
					outputStream << "   " << resource->getName() << " ( " << resource->getSize() << " bytes)" << reloadable;
					Ogre::Texture* texture = dynamic_cast<Ogre::Texture*>(resource.get());
					if (texture) {
						outputStream << texture->getWidth() << "x" << texture->getHeight() << " ";
					}
					outputStream << std::endl;
					loadedResourceCount++;
				}
				resourceCount++;
			}
			outputStream << "  Total number of resources: " << resourceCount << std::endl;
			outputStream << "  Number of loaded resources: " << loadedResourceCount << std::endl;
		}
	}

	outputStream << "Total memory usage for all resource manager: " << resourceMemoryUsage << " bytes" << std::endl;

	outputStream << std::flush;
}

}
}
