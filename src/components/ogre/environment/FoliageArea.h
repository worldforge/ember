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
*/
class FoliageArea : public Ogre::FrameListener {
public:
    FoliageArea();

    ~FoliageArea();

	void init(Foliage* foliage, Ogre::SceneManager* sceneManager, const std::string& name);

	bool frameStarted(const Ogre::FrameEvent & evt);
	
	//generates and compiles the undervegetation
	//this might take a while
	void generateUnderVegetation(TerrainPosition minExtent, TerrainPosition maxExtent);
	void setVisible(bool visible);

	
	void placeGrass(const std::string& type, const TerrainPosition& position);
	void placeGrass(const std::string& type, const TerrainPosition& position, const Ogre::Vector3& scale);
	void build();
	
	

protected:
	static Ogre::Real mXinc;
	static Ogre::Real mZinc;
	Ogre::Real mXpos;
	Ogre::Real mZpos;
	
	Ogre::SceneManager* mSceneMgr;

	void waveGrass(Ogre::Real timeElapsed);
	Foliage* mFoliage;

	bool mVisible;


	TerrainPosition mExtentMin, mExtentMax;

	Ogre::StaticGeometry* mStaticGeom;

};

}

#endif
