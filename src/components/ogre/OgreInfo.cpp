//
// C++ Implementation: OgreInfo
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

#include "OgreInfo.h"
#include <OgreSceneManagerEnumerator.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreLight.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <sstream>

#ifdef HAVE_OPENGL
#if defined(__WIN32__)
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif

namespace EmberOgre
{

long long int OgreInfo::sResourceCounter(1);

bool OgreInfo::isIndirect()
{
#ifdef WIN32
	///TODO: add checks for win32 too
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
						outputStream << "  * " << type << " " << movable->getName() << "(" << (movable->isInScene() ? "in scene" : "not in scene") << ")" << std::endl;
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
			}
		}

	}

	outputStream << std::flush;
}

}
