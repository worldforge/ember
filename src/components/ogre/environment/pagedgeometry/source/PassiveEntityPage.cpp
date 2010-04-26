/*-------------------------------------------------------------------------------------
Copyright (c) 2008 Erik Hjortsberg

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------------*/

//PassiveEntityPage.cpp
//PassiveEntityPage is an extension to PagedGeometry which displays entities as entities (no optimization techniques).
//-------------------------------------------------------------------------------------

#define __PassiveEntityPage_CPP__
#include "PassiveEntityPage.h"

#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreVector3.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
using namespace Ogre;

namespace Forests {

//-------------------------------------------------------------------------------------

unsigned long PassiveEntityPage::GUID = 0;


void PassiveEntityPage::init(::Forests::PagedGeometry *geom, const Ogre::Any &data)
{
	sceneMgr = geom->getSceneManager();
}

PassiveEntityPage::PassiveEntityPage()
	: sceneMgr(NULL)
{}

PassiveEntityPage::~PassiveEntityPage()
{
}


void PassiveEntityPage::addEntity(Ogre::Entity *ent, const Ogre::Vector3 &position, const Ogre::Quaternion &rotation, const Ogre::Vector3 &scale, const Ogre::ColourValue &color)
{
}

void PassiveEntityPage::removeEntities()
{
}

void PassiveEntityPage::setVisible(bool visible)
{

}

void PassiveEntityPage::setFade(bool enabled, Ogre::Real visibleDist, Ogre::Real invisibleDist)
{
}
}
