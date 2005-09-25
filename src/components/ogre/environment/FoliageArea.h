//
// C++ Interface: FoliageArea
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBEROGREFOLIAGEAREA_H
#define EMBEROGREFOLIAGEAREA_H
#include "components/ogre/EmberOgrePrerequisites.h"

namespace EmberOgre {

class Foliage;
/**
@author Erik Hjortsberg
Represents an area of foliage, normally mapped to one TerrainPage.
Don't create this class directly, instead use the Foliage class.

Start by placing the foliage through the placeGrass methods.
Once that's done, call the build method, which will create the static geometry. Once build has been called, calls to placeGrass won't do anything unless build is called again.
*/
class FoliageArea  {
public:
    FoliageArea();

    ~FoliageArea();

	void init(Foliage* foliage, Ogre::SceneManager* sceneManager, const std::string& name);

	
	/**
	 *    sets the visibility of the area
	 * @param visible 
	 */
	void setVisible(bool visible);

	void frameStarted(const Ogre::Real & timeElapsed);
	
	void placeGrass(const std::string& type, const TerrainPosition& position);
	void placeGrass(const std::string& type, const TerrainPosition& position, const Ogre::Vector3& scale);
	
	
	/**
	 * generates and compiles the undervegetation 
	 * this might take a while
	 */
	void build();
	
	

protected:
	static Ogre::Real mXinc;
	static Ogre::Real mZinc;
	Ogre::Real mXpos;
	Ogre::Real mZpos;
	
	Ogre::SceneManager* mSceneMgr;

	Foliage* mFoliage;

	bool mVisible;

	void waveGrass(Ogre::Real timeElapsed);

	TerrainPosition mExtentMin, mExtentMax;

	Ogre::StaticGeometry* mStaticGeom;

};

}

#endif
