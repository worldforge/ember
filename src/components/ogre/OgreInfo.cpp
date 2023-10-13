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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "OgreInfo.h"
#include <OgreSceneManagerEnumerator.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreResourceManager.h>

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

namespace {
unsigned int countNodes(Ogre::Node* node) {
	unsigned int counter = 1;
	for (unsigned short i = 0; i < node->numChildren(); ++i) {
		counter += countNodes(node->getChild(i));
	}
	return counter;
}
}
void OgreInfo::diagnose(std::ostream& outputStream)
{
	for (const auto& sceneManagerEntry : Ogre::Root::getSingleton().getSceneManagers()) {
		Ogre::SceneManager* sceneManager = sceneManagerEntry.second;
		outputStream << "Scenemanager(" << sceneManager->getTypeName() << ") " << sceneManager->getName() << std::endl;
		outputStream << " Number of scene nodes: " << countNodes(sceneManager->getRootSceneNode()) << std::endl;
		outputStream << " Movable objects:" << std::endl;
		unsigned int movableObjectCounter = 0;
		for (auto& movableFactoryEntry : Ogre::Root::getSingleton().getMovableObjectFactories()) {
			Ogre::MovableObjectFactory* factory = movableFactoryEntry.second;
			const std::string& type = factory->getType();
			{
				for (auto& movableObjectEntry: sceneManager->getMovableObjects(type)) {
					movableObjectCounter++;
					Ogre::MovableObject* movable = movableObjectEntry.second;
					if (movable->getMovableType() == "Light") {
						auto* light = dynamic_cast<Ogre::Light*> (movable);
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
			for (const auto& entry : sceneManager->getCameras()) {
				Ogre::Camera* camera = entry.second;
				outputStream << "  Camera " << camera->getName() << "(" << (camera->isInScene() ? "in scene" : "not in scene") << ")" << std::endl;
				outputStream << "  Pos: " << camera->getDerivedPosition() << std::endl;
				outputStream << "  Direction: " << camera->getDerivedDirection() << std::endl;
				outputStream << "  Clip distances: " << camera->getNearClipDistance() << " - " << camera->getFarClipDistance() << std::endl;
			}
		}

	}

	size_t resourceMemoryUsage = 0;
	outputStream << "Resource Managers:" << std::endl;
	for (auto& entry : Ogre::ResourceGroupManager::getSingleton().getResourceManagers()) {
		auto& name = entry.first;
		Ogre::ResourceManager* manager = entry.second;
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
					auto* texture = dynamic_cast<Ogre::Texture*>(resource.get());
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
