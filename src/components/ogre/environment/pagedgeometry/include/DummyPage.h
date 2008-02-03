//
// C++ Interface: DummyPage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
//ImpostorPage.h
//ImposterPage is an extension to PagedGeometry which displays entities as imposters.
//-------------------------------------------------------------------------------------

#ifndef __DummyPage_H__
#define __DummyPage_H__

#include "PagedGeometry.h"

#include <OgrePrerequisites.h>
#include <OgreTextureManager.h>
#include <OgreRenderTexture.h>


namespace PagedGeometry {

//-------------------------------------------------------------------------------------
/**
\brief The DummyPage class won't render anything. This might be useful if you want to render entities using normal entity rendering up to a certain distance.

To use this page type, use:
\code
PagedGeometry::addDetailLevel<DummyPage>(farRange);
\endcode

*/
class DummyPage: public GeometryPage
{

public:
	void init(PagedGeometry *geom);
	~DummyPage();
	
	void setRegion(Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom) {}
	void addEntity(Ogre::Entity *ent, const Ogre::Vector3 &position, const Ogre::Quaternion &rotation, const Ogre::Vector3 &scale, const Ogre::ColourValue &color) {}
	void build() {}
	void removeEntities() {}
	
	void setVisible(bool visible) {}
	void setFade(bool enabled, Ogre::Real visibleDist, Ogre::Real invisibleDist) {}

	void update() {}

protected:
	Ogre::SceneManager *sceneMgr;
	PagedGeometry *geom;
};

}

#endif
