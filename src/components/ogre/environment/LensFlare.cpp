//
// C++ Implementation: LensFlare
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
#include "LensFlare.h"
#include "../EmberEntity.h"

using namespace Ogre;

namespace EmberOgre {

namespace Environment {


/* ------------------------------------------------------------------------- */
/// Constructor
/// @param LightPosition The 3D position of the Light, relative to the camera.
/// @param camera        The camera on which the lensflare effect will appear.
/// @param SceneMgr      Pointer on the SceneManager.
/* ------------------------------------------------------------------------- */
LensFlare::LensFlare(Camera* camera, SceneManager* SceneMgr) : mLightNode(0)
{
	mSceneMgr      = SceneMgr;
	mCamera        = camera;
	mHidden        = true;
}

/* ------------------------------------------------------------------------- */
/// Destructor
/* ------------------------------------------------------------------------- */
LensFlare::~LensFlare()
{
	if (mLightNode) {
		mLightNode->detachObject(mHaloSet);
		mLightNode->detachObject(mBurstSet);
		mSceneMgr->destroyBillboardSet(mHaloSet);
		mSceneMgr->destroyBillboardSet(mBurstSet);
		
/*		Ogre::SceneNode* parent = static_cast<Ogre::SceneNode*>(mNode->getParent());
		if (parent) {
			parent->removeAndDestroyChild(mNode->getName());
		} else {
			mNode->getCreator()->destroySceneNode(mNode->getName());
		}*/
	}
} 

void LensFlare::setNode(Ogre::SceneNode* node)
{
	mLightNode = node;
}

void LensFlare::initialize()
{
	createLensFlare();
}


/* ------------------------------------------------------------------------- */
/// this function creates and shows all the LensFlare graphical elements.
/* ------------------------------------------------------------------------- */
bool LensFlare::createLensFlare()
{
	Real LF_scale = 150;
	try {
		// -----------------------------------------------------
		// We create 2 sets of billboards for the lensflare
		// -----------------------------------------------------
		mHaloSet = mSceneMgr->createBillboardSet("halo");
		mHaloSet->setMaterialName("/global/environment/lensflare/halo");
		mHaloSet->setCullIndividually(true);
		mHaloSet->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	
		mBurstSet= mSceneMgr->createBillboardSet("burst");
		mBurstSet->setMaterialName("/global/environment/lensflare/burst");
		mBurstSet->setCullIndividually(true);
		mBurstSet->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	} catch (const Ogre::Exception&) {
		S_LOG_FAILURE("Couldn't load lens flare, you are probably missing the needed materials.");
		return false;
	}
	// The node is located at the light source.
	//mLightNode  = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	mLightNode->attachObject(mBurstSet);
	mLightNode->attachObject(mHaloSet);

	// -------------------------------
	// Creation of the Halo billboards
	// -------------------------------
	Billboard* LF_Halo1 = mHaloSet->createBillboard(0,0,0);
	LF_Halo1->setDimensions(LF_scale*0.5,LF_scale*0.5);
	Billboard* LF_Halo2 = mHaloSet->createBillboard(0,0,0);
	LF_Halo2->setDimensions(LF_scale,LF_scale);
	Billboard* LF_Halo3 = mHaloSet->createBillboard(0,0,0);
	LF_Halo3->setDimensions(LF_scale*0.25,LF_scale*0.25);


	// -------------------------------
	// Creation of the "Burst" billboards
	// -------------------------------
	Billboard* LF_Burst1 = mBurstSet->createBillboard(0,0,0);
	LF_Burst1->setDimensions(LF_scale*0.25,LF_scale*0.25);
	Billboard* LF_Burst2 = mBurstSet->createBillboard(0,0,0);
	LF_Burst2->setDimensions(LF_scale*0.5,LF_scale*0.5);
	Billboard* LF_Burst3 = mBurstSet->createBillboard(0,0,0);
	LF_Burst3->setDimensions(LF_scale*0.25,LF_scale*0.25);

	return true;
} 

const Ogre::Vector3& LensFlare::getLightPosition() const
{
	return mLightNode->getWorldPosition();
}


/* -------------------------------------------------------------------------- */
/// This function updates the lensflare effect. 
/** This function should be called by your frameListener.
*/
/* -------------------------------------------------------------------------- */
void LensFlare::update()
{
	if (mHidden || !mLightNode) return;

	/// If the Light is out of the Camera field Of View, the lensflare is hidden.
	if (!mCamera->isVisible(getLightPosition())) 
	{
		mHaloSet->setVisible(false);
		mBurstSet->setVisible(false);
		return;
	}

	Vector3 lightToCamera = mCamera->getDerivedPosition() - getLightPosition();
	
	Vector3 CameraVect  = lightToCamera.length() * mCamera->getDerivedDirection();
	CameraVect += mCamera->getDerivedPosition();

	// The LensFlare effect takes place along this vector.
	Vector3 LFvect = (CameraVect - getLightPosition());

//	LFvect += Vector3(-64,-64,0);  // sprite dimension (to be adjusted, but not necessary)

	// The different sprites are placed along this line.
	mHaloSet->getBillboard(0)->setPosition( LFvect);
	mHaloSet->getBillboard(1)->setPosition( LFvect*0.725);
	mHaloSet->getBillboard(2)->setPosition( LFvect*0.250);

	mBurstSet->getBillboard(0)->setPosition( LFvect*0.833);
	mBurstSet->getBillboard(1)->setPosition( LFvect*0.500);
	mBurstSet->getBillboard(2)->setPosition( LFvect*0.320);

	// We redraw the lensflare (in case it was previouly out of the camera field, and hidden)
	this->setVisible(true);	
}

/* ------------------------------------------------------------------------- */
/// This function shows (or hide) the lensflare effect.
/* ------------------------------------------------------------------------- */
void LensFlare::setVisible(bool visible)
{
	if (mLightNode) {
		mHaloSet->setVisible(visible);
		mBurstSet->setVisible(visible);
		mHidden = !visible;
	}
}


/* ------------------------------------------------------------------------- */
/// This function updates the light source position. 
/** This function can be used if the light source is moving.*/
/* ------------------------------------------------------------------------- */
// void LensFlare::setLightPosition(Vector3 pos)
// {
// 	mLightPosition = pos;
// 	if (mNode) {
// 		mNode->setPosition(mLightPosition);
// 	} 
// }


/* ------------------------------------------------------------------------- */
/// This function changes the colour of the burst. 
/* ------------------------------------------------------------------------- */
void LensFlare::setBurstColour(ColourValue color)
{
	if (mLightNode) {
		mBurstSet->getBillboard(0)->setColour(color);
		mBurstSet->getBillboard(1)->setColour(color*0.8);
		mBurstSet->getBillboard(2)->setColour(color*0.6);
	}
} 

/* ------------------------------------------------------------------------- */
/// This function changes the colour of the halos. 
/* ------------------------------------------------------------------------- */
void LensFlare::setHaloColour(ColourValue color)
{ 
	if (mLightNode) {
		mHaloSet->getBillboard(0)->setColour(color*0.8);
		mHaloSet->getBillboard(1)->setColour(color*0.6);
		mHaloSet->getBillboard(2)->setColour(color);
	}
}


}

}
