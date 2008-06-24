//
// C++ Implementation: Water
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Water.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "components/ogre/EmberOgre.h"

using namespace Ogre;

namespace EmberOgre {

namespace Environment {

class RefractionTextureListener : public RenderTargetListener
{
Entity* pPlaneEnt;
std::vector<Entity*> aboveWaterEnts;

public:

	void setPlaneEntity(Entity* plane)
	{
		pPlaneEnt = plane;
	}
	

	
    void preRenderTargetUpdate(const RenderTargetEvent& evt)
    {
        // Hide plane and objects above the water
        pPlaneEnt->setVisible(false);
        std::vector<Entity*>::iterator i, iend;
        iend = aboveWaterEnts.end();
        for (i = aboveWaterEnts.begin(); i != iend; ++i)
        {
            (*i)->setVisible(false);
        }

    }
    void postRenderTargetUpdate(const RenderTargetEvent& evt)
    {
        // Show plane and objects above the water
        pPlaneEnt->setVisible(true);
        std::vector<Entity*>::iterator i, iend;
        iend = aboveWaterEnts.end();
        for (i = aboveWaterEnts.begin(); i != iend; ++i)
        {
            (*i)->setVisible(true);
        }
    }

};
class ReflectionTextureListener : public RenderTargetListener
{
Plane reflectionPlane;
Entity* pPlaneEnt;
std::vector<Entity*> belowWaterEnts;
Ogre::Camera* theCam;

public:

	void setPlaneEntity(Entity* plane)
	{
		pPlaneEnt = plane;
	}
	
	void setReflectionPlane(Plane &aPlane)
	{
		reflectionPlane = aPlane;
	}
	
	void setCamera(Ogre::Camera* aCamera) {
		 theCam = aCamera;
	}
	
	
    void preRenderTargetUpdate(const RenderTargetEvent& evt)
    {
        // Hide plane and objects below the water
        pPlaneEnt->setVisible(false);
        std::vector<Entity*>::iterator i, iend;
        iend = belowWaterEnts.end();
        for (i = belowWaterEnts.begin(); i != iend; ++i)
        {
            (*i)->setVisible(false);
        }
        theCam->enableReflection(reflectionPlane);

    }
    void postRenderTargetUpdate(const RenderTargetEvent& evt)
    {
        // Show plane and objects below the water
        pPlaneEnt->setVisible(true);
        std::vector<Entity*>::iterator i, iend;
        iend = belowWaterEnts.end();
        for (i = belowWaterEnts.begin(); i != iend; ++i)
        {
            (*i)->setVisible(true);
        }
        theCam->disableReflection();
    }

};



	Water::Water(Ogre::Camera *camera, Ogre::SceneManager* mSceneMgr) : mCamera(camera)
	{
	
		Ogre::Plane waterPlane(Ogre::Vector3::UNIT_Y, 0);


		// create a water plane/scene node
/*		waterPlane.normal = ; 
		waterPlane.d = 0; */
		Ogre::MeshManager::getSingleton().createPlane(
			"WaterPlane",
			"environment",
			waterPlane,
			10000, 10000,
			5, 5,
			true, 1, 
			1000, 1000,
			Ogre::Vector3::UNIT_Z
		);
		
		mWaterNode = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode("water");
		//mWaterNode->translate(500, 0, 500);

			
		mRefractionListener = new RefractionTextureListener();
		mReflectionListener = new ReflectionTextureListener();
	
		bool canDoFresnel = true;
		// Check prerequisites first
		const RenderSystemCapabilities* caps = Root::getSingleton().getRenderSystem()->getCapabilities();
		if (!caps->hasCapability(RSC_VERTEX_PROGRAM) || !(caps->hasCapability(RSC_FRAGMENT_PROGRAM)))
		{
			canDoFresnel = false;
		/*            Except(1, "Your card does not support vertex and fragment programs, so cannot "
				"run this demo. Sorry!", 
				"Fresnel::createScene");*/
		}
		else
		{
			if (!GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1") &&
				!GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0") &&
				!GpuProgramManager::getSingleton().isSyntaxSupported("ps_1_4")
				)
			{
				canDoFresnel = false;
		/*                Except(1, "Your card does not support advanced fragment programs, "
					"so cannot run this demo. Sorry!", 
				"Fresnel::createScene");*/
			}
		}
	
		//default to normal water if "fresnelwater" not found in config
		if (canDoFresnel && Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("graphics", "fresnelwater")) {
			canDoFresnel = Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("graphics", "fresnelwater");
		} else {
			canDoFresnel = false;
		}
		
		if (canDoFresnel)
		{
			createFresnelWater(camera, mSceneMgr);
		} else {
			createSimpleWater(camera, mSceneMgr);
		}
	
	}
	
	void Water::createFresnelWater(Ogre::Camera *camera, Ogre::SceneManager* mSceneMgr)
	{
		RenderTexture* rttTex = EmberOgre::getSingleton().getOgreRoot()->getRenderSystem()->createRenderTexture( "Refraction", 512, 512 );
			
			{
				Viewport *v = rttTex->addViewport( mCamera );
				Ogre::MaterialPtr mat = MaterialManager::getSingleton().getByName("Examples/FresnelReflectionRefraction");
				if (!mat.isNull()) {
					mat->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTextureName("Refraction");
					v->setOverlaysEnabled(false);
					rttTex->addListener(mRefractionListener);
				}
			}
			
			
			rttTex = EmberOgre::getSingleton().getOgreRoot()->getRenderSystem()->createRenderTexture( "Reflection", 512, 512 );
			{
				Viewport *v = rttTex->addViewport( mCamera );
				Ogre::MaterialPtr mat = MaterialManager::getSingleton().getByName("Examples/FresnelReflectionRefraction");
				if (!mat.isNull()) {
					mat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName("Reflection");
					v->setOverlaysEnabled(false);
					rttTex->addListener(mReflectionListener);
				}
			}
			
			// Define a floor plane mesh
/*			reflectionPlane.normal = Vector3::UNIT_Y;
			reflectionPlane.d = 0;*/
/*			MeshManager::getSingleton().createPlane("ReflectPlane",reflectionPlane,
				1500,1500,10,10,true,1,5,5,Vector3::UNIT_Z);*/
		
			
			Entity* pPlaneEnt = mSceneMgr->createEntity( "plane", "WaterPlane" );
			pPlaneEnt->setMaterialName("Examples/FresnelReflectionRefraction");
			mRefractionListener->setPlaneEntity(pPlaneEnt);
			mReflectionListener->setPlaneEntity(pPlaneEnt);
			mReflectionListener->setReflectionPlane(reflectionPlane);
			mReflectionListener->setCamera(camera);
			mWaterNode->attachObject(pPlaneEnt);
	
	}
	
	void Water::createSimpleWater(Ogre::Camera *camera, Ogre::SceneManager* mSceneMgr)
	{
			Ogre::Entity *waterEntity;
/*			Ogre::Plane waterPlane;
	
	
			// create a water plane/scene node
			waterPlane.normal = Ogre::Vector3::UNIT_Y; 
			waterPlane.d = 0; 
			Ogre::MeshManager::getSingleton().createPlane(
				"WaterPlane",
				waterPlane,
				1400, 1400,
				20, 20,
				true, 1, 
				100, 100,
				Ogre::Vector3::UNIT_Z
			);*/
	
			waterEntity = mSceneMgr->createEntity("water", "WaterPlane"); 
			waterEntity->setMaterialName("/global/environment/water/simple"); 
			waterEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_6);
			waterEntity->setCastShadows(false);
			
			mWaterNode->attachObject(waterEntity); 
	
	}
	
	
	Water::~Water()
	{
		delete mRefractionListener;
		delete mReflectionListener;
	}


}

}
