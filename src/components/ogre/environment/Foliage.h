//
// C++ Interface: Foliage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#ifndef DIMEOGREFOLIAGE_H
#define DIMEOGREFOLIAGE_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/Singleton.h"

namespace EmberOgre {

namespace Environment {

class FoliageArea;

//class GroundCover;
/**
@author Erik Hjortsberg
*/
class Foliage : public Ember::Singleton<Foliage>, public Ogre::FrameListener
{

public:

	typedef std::list<FoliageArea*> FoliageAreaStore;
    Foliage( Ogre::SceneManager* mSceneMgr);

    ~Foliage();
	
	/**
	 *    Creates a new Foliage area, ready to be populated
	 * @return 
	 */
	FoliageArea* createArea();	
	
	
	/**
	 *    destroys a foliage area
	 * @param area 
	 */
	void destroyArea(FoliageArea* area);

	bool frameStarted(const Ogre::FrameEvent & evt);

	Ogre::Entity* getEntity(const std::string& name);
	
	inline double getGrassSpacing() const {return mGrassSpacing;}

protected:
	std::map<const std::string, Ogre::Entity* > mEntities;

	//GroundCover* mGround;
	FoliageAreaStore mFoliageAreas;

	void createGrassMesh();
	

	double mGrassSpacing;	
	Ogre::SceneManager* mSceneMgr;

	Foliage* mFoliage;
	
	//defines the extent of the foliage

};

}

}

#endif
