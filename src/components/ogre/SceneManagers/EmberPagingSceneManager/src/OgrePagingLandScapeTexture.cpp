/***************************************************************************
OgrePagingLandScapeTexture.cpp  -  description
-------------------
begin                : Fri Apr 16 2004
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

#include <OgreVector3.h>
#include <OgreColourValue.h>
#include <OgreStringConverter.h>
#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>


namespace Ogre
{
//-----------------------------------------------------------------------
PagingLandScapeTexture::PagingLandScapeTexture(PagingLandScapeTextureManager *textureMgr, 
					       const String materialBaseName,
					       const unsigned int numTexture,
					       const bool isSplatMode) :
	mParent(textureMgr),
	mIsLoaded (false),
	mDataX (0),
	mDataZ (0)
{
	mMaterial.setNull();
}

//-----------------------------------------------------------------------
PagingLandScapeTexture::~PagingLandScapeTexture()
{
}


//-----------------------------------------------------------------------
const Ogre::String& PagingLandScapeTexture::getMaterialName() const
{
        return mMaterial->getName();
}

//-----------------------------------------------------------------------
void PagingLandScapeTexture::bindCompressionSettings()
{
	const PagingLandScapeOptions * const opt = mParent->getOptions();
		
	Material::TechniqueIterator tIt = mMaterial->getTechniqueIterator ();
	while (tIt.hasMoreElements ())
	{
		Technique * const t = tIt.getNext ();
		Technique::PassIterator pIt = t->getPassIterator ();
		while (pIt.hasMoreElements ())
		{
			Pass *p = pIt.getNext ();
			if (p->hasVertexProgram ())
			{
				// vertex compression setting
				GpuProgramParametersSharedPtr params = p->getVertexProgramParameters();	
				if (opt->VertexCompression)
				{
					bindCompressionSettings (params);
					bindCompressionSettings (p->getShadowReceiverVertexProgramParameters ());
				}

				// splat settings.
#ifdef PLSM2_EIHORT
				GpuConstantDefinition const * const e = params->_findNamedConstantDefinition("splatSettings", false);
				if (e)
				{
					// use index to get RealConstantEntry
					params->_writeRawConstant(e->physicalIndex + 0, opt->matHeight[1]);
					params->_writeRawConstant(e->physicalIndex + 1, opt->matHeight[2]);
					params->_writeRawConstant(e->physicalIndex + 2, float(opt->maxValue));
					params->_writeRawConstant(e->physicalIndex + 3, 0.0f);				
				}
#else
				GpuProgramParameters::RealConstantEntry * const e = params->getNamedRealConstantEntry ("splatSettings");
				
				if (e)
				{
					e->val[0] = static_cast <float> (opt->matHeight[1]);
					e->val[1] = static_cast <float> (opt->matHeight[2]); 
					e->val[2] = static_cast <float> (opt->maxValue); 
					e->val[3] = static_cast <float> (0.0);
					e->isSet = true;
				}
#endif

			}
		}
	}
}

//-----------------------------------------------------------------------
void PagingLandScapeTexture::bindCompressionSettings(GpuProgramParametersSharedPtr params)
{
	GpuProgramParameters::AutoConstantIterator aci = params->getAutoConstantIterator();
        bool found = false;
        while (aci.hasMoreElements())
        {
		const GpuProgramParameters::AutoConstantEntry& ace = aci.getNext();
		if (ace.paramType == GpuProgramParameters::ACT_CUSTOM && 
		    ace.data == MORPH_CUSTOM_PARAM_ID)
		{
			found = true;
		}
        }
        if (!found)
        {                        
		params->setNamedAutoConstant("compressionSettings", 
					     GpuProgramParameters::ACT_CUSTOM, MORPH_CUSTOM_PARAM_ID);                       
        }
}

//-----------------------------------------------------------------------
void PagingLandScapeTexture::_loadMaterial()
{

}

//-----------------------------------------------------------------------
void PagingLandScapeTexture::_unloadMaterial()
{
}

//-----------------------------------------------------------------------
void PagingLandScapeTexture::load(unsigned int x, unsigned int z)
{
        if (!mIsLoaded)
	{
	        mDataX = x;
		mDataZ = z;
//		updated();
//		setNumTexture ();
	        _loadMaterial();
//		const PagingLandScapeOptions * const opt = mParent->getOptions();
//		mMaterial->setLightingEnabled (opt->lit);
//		mMaterial->setLodLevels (opt->lodMaterialDistanceList);
//		mMaterial->setReceiveShadows (true);
//		// If vertex shader, have to bind parameters
//		bindCompressionSettings ();
//		mMaterial->load ();
//		// load texture in main memory if we want to update it Real-Time
//		loadTexturesToModify();

	        mIsLoaded = true;

        }
}

//-----------------------------------------------------------------------
void PagingLandScapeTexture::unload()
{
        if (mIsLoaded)
        {

        	std::string resourceName = mMaterial->getName();
		if (!mMaterial.isNull())
			mMaterial->unload();


	        _unloadMaterial();

		mMaterial.setNull();
		MaterialManager::getSingleton().remove (resourceName);


		if (Ogre::Pass::getDirtyHashList().size() != 0 || Ogre::Pass::getPassGraveyard().size() != 0) {
			Ogre::SceneManagerEnumerator::SceneManagerIterator scenesIter = Ogre::Root::getSingleton().getSceneManagerIterator();

			while (scenesIter.hasMoreElements()) {
				Ogre::SceneManager* pScene = scenesIter.getNext();
				if (pScene) {
					Ogre::RenderQueue* pQueue = pScene->getRenderQueue();
					if (pQueue) {
						Ogre::RenderQueue::QueueGroupIterator groupIter = pQueue->_getQueueGroupIterator();
						while (groupIter.hasMoreElements()) {
							Ogre::RenderQueueGroup* pGroup = groupIter.getNext();
							if (pGroup)
								pGroup->clear(false);
						}//end_while(groupIter.hasMoreElements())
					}//end_if(pScene)
				}//end_if(pScene)
			}//end_while(scenesIter.hasMoreElements())

			// Now trigger the pending pass updates
			Ogre::Pass::processPendingPassUpdates();

		}//end_if(m_Root..
			
	        mIsLoaded = false;
        }

}

//-----------------------------------------------------------------------
bool PagingLandScapeTexture::isLoaded() const
{
	return mIsLoaded;
}

//-----------------------------------------------------------------------
const MaterialPtr&  PagingLandScapeTexture::getMaterial() const
{
	return mMaterial;
}



} //namespace
