//
// C++ Implementation: EmberPagingSceneManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

#include "EmberPagingSceneManager.h"
#include "EmberOgrePrerequisites.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "EmberPagingLandScapeData2D_HeightField.h"
#include "EmberPagingLandScapeTexture.h"
#include "model/Model.h"

using namespace Ogre;

namespace EmberOgre {

    const Ogre::String EmberPagingSceneManagerFactory::FACTORY_TYPE_NAME = "EmberPagingSceneManager";
    //-----------------------------------------------------------------------
    void EmberPagingSceneManagerFactory::initMetaData(void) const
    {
 	   mMetaData.typeName = FACTORY_TYPE_NAME;
 	   mMetaData.description = "Uses the PagingLandscapeSceneManager.";
 	   mMetaData.sceneTypeMask = Ogre::ST_EXTERIOR_REAL_FAR; // support all types
 	   mMetaData.worldGeometrySupported = false;
    }
    //-----------------------------------------------------------------------
    Ogre::SceneManager* EmberPagingSceneManagerFactory::createInstance(
 	   const Ogre::String& instanceName)
    {
 	   return new EmberPagingSceneManager(instanceName);
    }
    //-----------------------------------------------------------------------
    void EmberPagingSceneManagerFactory::destroyInstance(Ogre::SceneManager* instance)
    {
 	   delete instance;
    }    
    
    
    EmberPagingSceneManager::EmberPagingSceneManager(const Ogre::String &name): PagingLandScapeSceneManager(name)
    {
        if (!mOptions)
            mOptions = new Ogre::PagingLandScapeOptions(this);
    }
    
    
    //-----------------------------------------------------------------------
    void EmberPagingSceneManager::InitScene ()
	{        
		PagingLandScapeSceneManager::InitScene ();
		
		getData2DManager()->registerDataType (new EmberPagingLandScapeData2D_HeightField (getData2DManager()));
		getTextureManager()->registerTextureType (new EmberPagingLandScapeTexture (getTextureManager()));

	}
	
	void EmberPagingSceneManager::registerProvider(IPageDataProvider* provider)
	{
		mProvider = provider;
	}
	
	IPageDataProvider* EmberPagingSceneManager::getProvider()
	{
		return mProvider;
	}


	Model::Model* EmberPagingSceneManager::createModel(
									const Ogre::String& modelName,
									const Ogre::String& modelDefinitionName )
	{
		// delegate to factory implementation
		Ogre::NameValuePairList params;
		params["modeldefinition"] = modelDefinitionName;
		return static_cast<Model::Model*>(
			createMovableObject(modelName, Model::ModelFactory::FACTORY_TYPE_NAME, 
				&params));
	
	}
	
	const Ogre::String& EmberPagingSceneManager::getTypeName(void) const
	{
		return EmberPagingSceneManagerFactory::FACTORY_TYPE_NAME;
	}
	
	const Ogre::Pass* EmberPagingSceneManager::deriveShadowCasterPass(const Ogre::Pass* pass)
	{
		return Ogre::SceneManager::deriveShadowCasterPass(pass);
		///This behaves very much like the original method found in Ogre::SceneManager, with the exception that we'll copy parameters from the original pass even to technique specific custom shadow material (whereas the superclass method would only to that for the scene manager general material)
		if (isShadowTechniqueTextureBased())
		{
			bool customMaterial = false;
			Pass* retPass;	
			if (!pass->getParent()->getShadowCasterMaterial().isNull())
			{
				customMaterial = true;
				retPass = pass->getParent()->getShadowCasterMaterial()->getBestTechnique()->getPass(0);
				if (!retPass) {
					return pass;
				}
			}
			else 
			{
				retPass = mShadowTextureCustomCasterPass ? 
					mShadowTextureCustomCasterPass : mShadowCasterPlainBlackPass;
			}
	
			
			// Special case alpha-blended passes
			if ((pass->getSourceBlendFactor() == SBF_SOURCE_ALPHA && 
				pass->getDestBlendFactor() == SBF_ONE_MINUS_SOURCE_ALPHA) 
				|| pass->getAlphaRejectFunction() != CMPF_ALWAYS_PASS)
			{
				// Alpha blended passes must retain their transparency
				retPass->setAlphaRejectSettings(pass->getAlphaRejectFunction(), 
					pass->getAlphaRejectValue());
				retPass->setSceneBlending(pass->getSourceBlendFactor(), pass->getDestBlendFactor());
				retPass->getParent()->getParent()->setTransparencyCastsShadows(true);
	
				// So we allow the texture units, but override the colour functions
				// Copy texture state, shift up one since 0 is shadow texture
				unsigned short origPassTUCount = pass->getNumTextureUnitStates();
				for (unsigned short t = 0; t < origPassTUCount; ++t)
				{
					TextureUnitState* tex;
					if (retPass->getNumTextureUnitStates() <= t)
					{
						tex = retPass->createTextureUnitState();
					}
					else
					{
						tex = retPass->getTextureUnitState(t);
					}
					// copy base state
					(*tex) = *(pass->getTextureUnitState(t));
					// override colour function
					tex->setColourOperationEx(LBX_SOURCE1, LBS_MANUAL, LBS_CURRENT,
						isShadowTechniqueAdditive()? ColourValue::Black : mShadowColour);
	
				}
				// Remove any extras
				while (retPass->getNumTextureUnitStates() > origPassTUCount)
				{
					retPass->removeTextureUnitState(origPassTUCount);
				}
	
			}
			else
			{
				// reset
				retPass->setSceneBlending(SBT_REPLACE);
				retPass->setAlphaRejectFunction(CMPF_ALWAYS_PASS);
				while (retPass->getNumTextureUnitStates() > 0)
				{
					retPass->removeTextureUnitState(0);
				}
			}
	
			// Propagate culling modes
			retPass->setCullingMode(pass->getCullingMode());
			retPass->setManualCullingMode(pass->getManualCullingMode());
			
	
			// Does incoming pass have a custom shadow caster program?
			if (!pass->getShadowCasterVertexProgramName().empty())
			{
				// Have to merge the shadow caster vertex program in
				retPass->setVertexProgram(
					pass->getShadowCasterVertexProgramName(), false);
				const GpuProgramPtr& prg = retPass->getVertexProgram();
				// Load this program if not done already
				if (!prg->isLoaded())
					prg->load();
				// Copy params
				retPass->setVertexProgramParameters(
					pass->getShadowCasterVertexProgramParameters());
				// Also have to hack the light autoparams, that is done later
			}
			else 
			{
				if (!customMaterial) {
					if (retPass == mShadowTextureCustomCasterPass)
					{
						// reset vp?
						if (mShadowTextureCustomCasterPass->getVertexProgramName() !=
							mShadowTextureCustomCasterVertexProgram)
						{
							mShadowTextureCustomCasterPass->setVertexProgram(
								mShadowTextureCustomCasterVertexProgram, false);
							if(mShadowTextureCustomCasterPass->hasVertexProgram())
							{
								mShadowTextureCustomCasterPass->setVertexProgramParameters(
									mShadowTextureCustomCasterVPParams);
		
							}
		
						}
		
					}
					else
					{
						// Standard shadow caster pass, reset to no vp
						retPass->setVertexProgram(StringUtil::BLANK);
					}
				}
			}
			return retPass;
		}
		else
		{
			return pass;
		}
    }
	
}

