//
// C++ Implementation: TerrainEditor
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "TerrainEditor.h"
#include "EmberOgrePrerequisites.h"
#include "EmberOgre.h"
#include "TerrainGenerator.h"

namespace EmberOgre {

TerrainEditor::TerrainEditor()
{
}


TerrainEditor::~TerrainEditor()
{
}

void TerrainEditor::showOverlay()
{
	Ogre::SceneNode* overlayNode = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode();
	
	const Mercator::Terrain& terrain = EmberOgre::getSingleton().getTerrainGenerator()->getTerrain();
	const Mercator::Terrain::Pointstore &points = terrain.getPoints();
	int x, y;
	for (Mercator::Terrain::Pointstore::const_iterator I = points.begin(); I != points.end(); ++I) {
		x = I->first * 64;
		for (Mercator::Terrain::Pointcolumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			y = J->first * 64;
			const Mercator::BasePoint& basepoint = J->second;
			Ogre::SceneNode* basepointNode = overlayNode->createChildSceneNode();
			basepointNode->setPosition(Ogre::Vector3(x, basepoint.height(), y));
			std::stringstream ss;
			ss << "basepointmarker" << x << "_" << y;
			Ogre::Entity* entity = EmberOgre::getSingleton().getSceneManager()->createEntity(ss.str(), "placeholder.mesh");
			entity->setRenderingDistance(200);
			basepointNode->attachObject(entity);
		}
	}
}

}
