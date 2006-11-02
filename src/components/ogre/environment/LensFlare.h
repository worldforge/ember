//
// C++ Interface: LensFlare
//
// Description: 
//
//
// Author     : David de Lorenzo
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
#ifndef EMBEROGRELENSFLARE_H
#define EMBEROGRELENSFLARE_H

#include "components/ogre/EmberOgrePrerequisites.h"

namespace EmberOgre {

namespace Environment {


/* ------------------------------------------------------------------------- */
/// A lens Flare effect.
/**
This class will create a lensflare effect, between The light position and the 
camera position.
Some functions will allow to change the lensflare color effect, in case of coloured 
light, for instance.
*/
/* ------------------------------------------------------------------------- */
class LensFlare
{
public:
	LensFlare(Ogre::Camera* camera, Ogre::SceneManager* SceneMgr);
	virtual ~LensFlare();
	bool    createLensFlare();
	void    update();
	void    setVisible(bool visible);
// 	void    setLightPosition(Ogre::Vector3 pos);
	void	setNode(Ogre::SceneNode* node);
	void    setHaloColour(Ogre::ColourValue color);
	void    setBurstColour(Ogre::ColourValue color);
	void    initialize();


protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera*       mCamera;
	Ogre::ColourValue   mColour;
	//Ogre::SceneNode*    mNode;
   	Ogre::BillboardSet* mHaloSet;
  	Ogre::BillboardSet* mBurstSet;
	//Ogre::Vector3       mLightPosition;
	Ogre::SceneNode* 	mLightNode;
	bool          mHidden;
	
	const Ogre::Vector3& getLightPosition() const;
};

}

}

#endif
