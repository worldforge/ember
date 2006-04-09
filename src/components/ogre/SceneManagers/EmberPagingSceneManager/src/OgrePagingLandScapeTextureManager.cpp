/***************************************************************************
	OgrePagingLandScapeTextureManager.cpp  -  description
	-------------------
	begin                : Fri Apr 16 2004
	copyright            : (C) 2003-2005 by Jose A. Milan and Tuan Kuranes
	email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/


#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreMaterialManager.h"
#include "OgreGpuProgramManager.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTextureManager.h"

// Texture loaders implementations
#include "OgrePagingLandScapeTexture.h"

#include "OgrePagingLandScapeTexture_Image.h"

#include "OgrePagingLandScapeTexture_InstantBaseTexture.h"
#include "OgrePagingLandScapeTexture_InstantBaseTextureShadowed.h"
#include "OgrePagingLandScapeTexture_InstantBaseTextureEdit.h"

#include "OgrePagingLandScapeTexture_BaseTexture.h"
#include "OgrePagingLandScapeTexture_BaseTexture2.h"

#include "OgrePagingLandScapeTexture_None.h"

#include "OgrePagingLandScapeTexture_Splatting.h"
#include "OgrePagingLandScapeTexture_Splatting2.h"
#include "OgrePagingLandScapeTexture_Splatting2Edit.h"
#include "OgrePagingLandScapeTexture_Splatting3.h"
#include "OgrePagingLandScapeTexture_Splatting4.h"
#include "OgrePagingLandScapeTexture_Splatting5.h"
#include "OgrePagingLandScapeTexture_SplattingShader.h"
#include "OgrePagingLandScapeTexture_SplattingShaderEdit.h"
#include "OgrePagingLandScapeTexture_Splatting6.h"
#include "OgrePagingLandScapeTexture_Splatting7.h"
#include "OgrePagingLandScapeTexture_Splatting7Edit.h"

#include "OgrePagingLandScapeTileInfo.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    PagingLandScapeTextureManager::PagingLandScapeTextureManager(PagingLandScapeSceneManager * scnMgr) :
        mSceneManager(scnMgr),
        mOptions (scnMgr->getOptions()),
         mTextureType (0),
         mTextureFormat (""),
        mWidth (0),
        mHeight (0),
        mTexturePageSize (mOptions->PageSize -1),
        mPaintChannel(0),
        mPaintColor(ColourValue::White)
    {    
            // Add default texture Types.
            registerTextureType (new PagingLandScapeTexture_Image(this));

            registerTextureType (new PagingLandScapeTexture_InstantBaseTextureShadowed(this));
            registerTextureType (new PagingLandScapeTexture_SplattingShader(this));
            registerTextureType (new PagingLandScapeTexture_SplattingShaderEdit(this));

            registerTextureType (new PagingLandScapeTexture_InstantBaseTexture(this));
            registerTextureType (new PagingLandScapeTexture_InstantBaseTextureEdit(this));
            registerTextureType (new PagingLandScapeTexture_BaseTexture(this));
            registerTextureType (new PagingLandScapeTexture_BaseTexture2(this));
            registerTextureType (new PagingLandScapeTexture_None(this));

            registerTextureType (new PagingLandScapeTexture_Splatting(this));
            registerTextureType (new PagingLandScapeTexture_Splatting2(this));
            registerTextureType (new PagingLandScapeTexture_Splatting2Edit(this)); 
            registerTextureType (new PagingLandScapeTexture_Splatting3(this));
            registerTextureType (new PagingLandScapeTexture_Splatting4(this));   
            registerTextureType (new PagingLandScapeTexture_Splatting5(this));
            registerTextureType (new PagingLandScapeTexture_Splatting6(this));  
            registerTextureType (new PagingLandScapeTexture_Splatting7(this));
            registerTextureType (new PagingLandScapeTexture_Splatting7Edit(this)); 
            
        
    }
    //-----------------------------------------------------------------------
    String PagingLandScapeTextureManager::getNextTextureFormat()
    {
        assert (!mTextureTypeMap.empty());
        const size_t numTextureTypes = mTextureTypeMap.size();
        uint nextTextureType =  mTextureType + 1;
        if (nextTextureType == numTextureTypes)
            nextTextureType = 0;
        uint numLoop = 0;
        // find Next supported texture format.
        while (numLoop != numTextureTypes)
        { 
#ifdef _DEBUG
            std::cout << "Trying " << mTextureTypeMap[nextTextureType]->getName () << '\n';
#endif //_DEBUG
            if (mTextureTypeMap[nextTextureType]->TextureRenderCapabilitesFullfilled()
                && mOptions->TextureFormatSupported.end() != std::find(mOptions->TextureFormatSupported.begin(),
                                                                        mOptions->TextureFormatSupported.end(), 
                                                                        mTextureTypeMap[nextTextureType]->getName ()))
            {
                return mTextureTypeMap[nextTextureType]->getName();
            }

            nextTextureType ++ ;
            if (nextTextureType == numTextureTypes)
                nextTextureType = 0;
            numLoop++;
            
        }
        assert (numLoop == numTextureTypes);
        
        OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
            "Cannot find a TextureMode  supported by current hardware!  (shaders/num texture units...)",
            " getNextTextureFormat ");           
        
        return StringUtil::BLANK;
    }
    //-----------------------------------------------------------------------
   String PagingLandScapeTextureManager::getCurrentTextureFormat()
    {
        assert (!mTextureTypeMap.empty());
        return mTextureTypeMap[mTextureType]->getName();;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTextureManager::~PagingLandScapeTextureManager()
	{
		reset ();
        // for all in map delete.
        PagingLandScapeTextureMap::iterator i = mTextureTypeMap.begin();
        while (i != mTextureTypeMap.end())
        {
            delete (*i);
            ++i;
        }
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::reset()
	{
		if (!mActiveTextures.empty())
		{
			std::for_each (mActiveTextures.begin(), mActiveTextures.end(), 
				std::mem_fun (&PagingLandScapeTexture::unload));

			// Insert actives into free list
			mFreeTextures.insert (mFreeTextures.end(), mActiveTextures.begin(), mActiveTextures.end());
			// Remove all active instances
			mActiveTextures.clear(); 
		}

		// could save a delete if texture type is the same... ?
		if (!mTexturePool.empty())
		{
			std::for_each(mTexturePool.begin(), mTexturePool.end(), delete_object());
			mTexturePool.clear();
			mFreeTextures.clear();
		}
       mWidth = 0; 
       mHeight = 0;
	}  
	//-----------------------------------------------------------------------
	PagingLandScapeTexture *PagingLandScapeTextureManager::getNewTexture(const uint x, const uint z)
	{
		PagingLandScapeTexture *newTexture;
		if (mFreeTextures.empty())
		{
			const size_t pool_size = mTexturePool.size ();			
			const size_t new_pool_size = (pool_size == 0)  ? 9 : pool_size * 2;
			mTexturePool.reserve(new_pool_size);
			mTexturePool.resize(new_pool_size);

			// Create new pages
			for (size_t i = pool_size; i < new_pool_size; ++i)
			{
				newTexture = allocateTexture();
				mTexturePool[i] = newTexture;
				mFreeTextures.push_back (newTexture);
			}
		}

		newTexture = mFreeTextures.front ();
		mFreeTextures.pop_front ();
		mActiveTextures.push_back (newTexture); 

		newTexture->load(x, z);

		return newTexture;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTextureManager::releaseTexture(PagingLandScapeTexture *p)
	{
		mActiveTextures.remove (p);
		mFreeTextures.push_back (p);
	}
	//-----------------------------------------------------------------------
	PagingLandScapeTexture *PagingLandScapeTextureManager::getTexture(const uint x, const uint z,
		const bool alwaysReturn)
	{
		if (x < mWidth && z < mHeight)
		{
			PagingLandScapeTextureList::iterator l, lend = mActiveTextures.end();
			for (l = mActiveTextures.begin(); l != lend; ++l)
			{
				if ((*l)->isCoord(x, z))
					return (*l);
			}
			if (alwaysReturn)
				return getNewTexture(x, z);
		}
		assert (!alwaysReturn);
		return 0;
	}
    //-----------------------------------------------------------------------
    PagingLandScapeTexture *PagingLandScapeTextureManager::allocateTexture() const
    {
       return mTextureTypeMap[mTextureType]->newTexture();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::load ()
    {        

        if (mOptions->textureFormat != mTextureFormat)
        {
            reset ();
            mTextureFormat = mOptions->textureFormat; 
            uint i = 0;
            assert (!mTextureTypeMap.empty());
            const size_t numTextureTypes = mTextureTypeMap.size();
            for (; i != numTextureTypes; ++i)
            {
                const String &modeName = mTextureTypeMap[i]->getName ();
                if (modeName == mTextureFormat)
                {
					if (!mTextureTypeMap[i]->TextureRenderCapabilitesFullfilled())
					{
						mTextureFormat = getNextTextureFormat();// not supported, find first supported.
						for (uint k = 0; k != numTextureTypes; ++k)
						{
							const String &newmodeName = mTextureTypeMap[k]->getName ();
							if (newmodeName == mTextureFormat)
							{
								mTextureType = k;
								i = k;
								break;
							}
						}
					}
					else
					{
						mTextureType = i;
					}
                    break;
                }
            }
            if (i == numTextureTypes)
            {
                OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                        "TextureMode not supplied or wrong (check case) !",
		                " PagingLandScapeTextureManager::load ");
            }
            if (!mTextureTypeMap[mTextureType]->TextureRenderCapabilitesFullfilled())
            {
                OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                        "TextureMode not supported by current hardware!  (shaders/num texture units...)",
		                " PagingLandScapeTextureManager::load ");
            }
            mTexturePageSize = mOptions->PageSize - 1;
        }

        // static member init.      
        mTextureTypeMap[mTextureType]->setPagesize();

        WorldDimensionChange();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::clear ()
    {
       // static member de-init. 
	   assert (!mTextureTypeMap.empty() && mTextureTypeMap[mTextureType]);
       mTextureTypeMap[mTextureType]->clearData();
       mMapMaterial.setNull();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::WorldDimensionChange()
    {
        const uint newWidth = mOptions->world_width; 
        const uint newHeight = mOptions->world_height;    

		reset();

        mWidth = newWidth; 
        mHeight = newHeight;  
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::setMapMaterial()
    {
    	PagingLandScapeOptions * const opt = mOptions;
        const String mapName =  String("Small") + opt->image_filename;
        mMapMaterial = MaterialManager::getSingleton().getByName (mapName);
        if (mMapMaterial.isNull())
        {
            MaterialPtr matTemplate = MaterialManager::getSingleton().
                            getByName (String ("PagingLandScape.Small.Template"));
            mMapMaterial = matTemplate ->clone (mapName);
            const String texname(opt->image_filename
                                + ".Small." + 
                                opt->TextureExtension);
            TextureManager::getSingleton().load (texname, opt->groupName);   
	        mMapMaterial->getTechnique(0)->getPass(0)->
                                getTextureUnitState(0)->setTextureName (texname);
            mMapMaterial->load();
        }
      } 
    //-----------------------------------------------------------------------
    MaterialPtr PagingLandScapeTextureManager::getMapMaterial()
    {
        return mMapMaterial;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::load(const uint texX, const uint texZ)
    {
	    PagingLandScapeTexture* tex = getTexture (texX , texZ);
	    if (!tex->isLoaded ())
        {
		    tex->load (texX, texZ);
	    }
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::unload(const uint texX, const uint texZ)
    {
	    PagingLandScapeTexture* tex = getTexture (texX , texZ, false);
	    if (tex && tex->isLoaded ())
	    {
		    tex->unload ();
			releaseTexture (tex);
	    }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::reload(const uint texX, const uint texZ)
    {
        PagingLandScapeTexture* tex = getTexture (texX , texZ);
	    tex->unload();
	    tex->load(texX, texZ);
        const uint tsize =  mOptions->TileSize;
        const uint psize =  mOptions->PageSize;
        for (uint z = 0; z <= psize ; z += tsize)
        {
            for (uint x = 0; x <= psize ; x += tsize)
            {                       
	            tex->adjustDeformationRectangle (x, z);
            }
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTextureManager::isLoaded(const uint texX, const uint texZ)
    {
		const PagingLandScapeTexture * const tex = getTexture (texX , texZ, false);
	    return tex && tex->isLoaded();
    }
    //-----------------------------------------------------------------------
    const MaterialPtr&  PagingLandScapeTextureManager::getMaterial(const uint texX, const uint texZ)
	{
		PagingLandScapeTexture * const tex = getTexture (texX , texZ, false);
		assert (tex);
		return tex->getMaterial();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::paint (const Vector3 &currpoint, const Real paintForce, 
                const PagingLandScapeTileInfo *info, const bool isAlpha)
    {
        const uint pX = info->pageX;
        const uint pZ = info->pageZ;
        const uint pSize = mOptions->PageSize - 1;
        // adjust x and z to be local to page
        const uint x = static_cast<uint> (currpoint.x
                                            - pX * pSize
                                            + mOptions->maxUnScaledX);
        const uint z = static_cast<uint> (currpoint.z
                                            - pZ * pSize
                                            + mOptions->maxUnScaledZ);

        PagingLandScapeTexture* tex = getTexture (pX , pZ, false);
        if (tex && tex->isLoaded())
        {
            const uint tSize = pSize - 1;
            const bool rightlimit = (x >= tSize);
            const bool uplimit    = (z >= tSize);

            // If we're on a page edge
            if (!(rightlimit && uplimit))
            {
                if (isAlpha)
                    tex->paint (x, z, paintForce, mPaintChannel);
                else
                    tex->paint (x, z, paintForce, mPaintColor);
            }

            // we must duplicate the change on the 
            // neighbour page (if it has one...)    
            const uint wL = mOptions->world_width;
            const uint hL = mOptions->world_height;
            const bool right = (rightlimit && pX < wL - 1);
            const bool up    = (uplimit    && pZ < hL - 1);
            const bool left  = (x == 0     && pX != 0);
            const bool down  = (z == 0     && pZ != 0);

            assert (z <= tSize && x <= tSize);

            if (left)
            {
                if (down)
                {               
                    //  lower left corner 
                    tex = getTexture (pX - 1 , pZ - 1, false);
                    if (tex && tex->isLoaded())
                    {
                        if (isAlpha)
                            tex->paint (tSize, tSize, paintForce, mPaintChannel);
                        else
                            tex->paint (tSize, tSize, paintForce, mPaintColor);
                    } 
                }
                else if (up)
                {
					//  upper left corner 
					tex = getTexture (pX - 1 , pZ + 1, false);
					if (tex && tex->isLoaded())
                    {
                        if (isAlpha)
                            tex->paint (tSize, 0, paintForce, mPaintChannel);
                        else
                            tex->paint (tSize, 0, paintForce, mPaintColor);
                    } 
                } 
                else
                {
					// left only 
					tex = getTexture (pX - 1 , pZ, false);
					if (tex && tex->isLoaded())
                    {
                        if (isAlpha)
                            tex->paint (tSize, z, paintForce, mPaintChannel);
                        else
                            tex->paint (tSize, z, paintForce, mPaintColor);
                    } 
                }
            }
            else if (right)
            {
                if (up)
                {
					//  upper right corner 
					tex = getTexture (pX + 1 , pZ + 1, false);
					if (tex && tex->isLoaded())
                    {
                        if (isAlpha)
                            tex->paint (0, 0, paintForce, mPaintChannel);
                        else
                            tex->paint (0, 0, paintForce, mPaintColor);
                    } 
                }
                else if (down)
                {
					//  lower right corner 
					tex = getTexture (pX + 1 , pZ - 1, false);
					if (tex && tex->isLoaded())
                    {
                        if (isAlpha)
                            tex->paint (0, tSize, paintForce, mPaintChannel);
                        else
                            tex->paint (0, tSize, paintForce, mPaintColor);
                    } 
                }  
                else
                {
					// right only
					tex = getTexture (pX + 1 , pZ, false);
					if (tex && tex->isLoaded())
                    {
                        if (isAlpha)
                            tex->paint (0, z, paintForce, mPaintChannel);
                        else
                            tex->paint (0, z, paintForce, mPaintColor);
                    } // if (tex->isLoaded())   
                }
            }
            else if (down)
            {        
				//  lower  (down only)    
				tex = getTexture (pX, pZ - 1, false);
				if (tex && tex->isLoaded())
                {
                    if (isAlpha)
                        tex->paint (x, tSize, paintForce, mPaintChannel);
                    else
                        tex->paint (x, tSize, paintForce, mPaintColor);
                } 
            }
            else if (up)
            {
				// upper (up only)
				tex = getTexture (pX, pZ + 1, false);
				if (tex && tex->isLoaded())
                {
                    if (isAlpha)
                        tex->paint (x, 0, paintForce, mPaintChannel);
                    else
                        tex->paint (x, 0, paintForce, mPaintColor);
                } 
            }     
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::DeformHeight (const Vector3 &currpoint,
        const PagingLandScapeTileInfo *info)
    {
        const uint pX = info->pageX;
        const uint pZ = info->pageZ;
        const uint pSize = mOptions->PageSize - 1;
        // adjust x and z to be local to page
        const uint x = static_cast<uint> (currpoint.x
                                            - pX * pSize
                                            + mOptions->maxUnScaledX);
        const uint z = static_cast<uint> (currpoint.z
                                            - pZ * pSize
                                            + mOptions->maxUnScaledZ);

		PagingLandScapeTexture* tex = getTexture (pX , pZ, false);
		if (tex && tex->isLoaded())
        {
            const uint tSize = pSize - 1;
            const bool rightlimit = (x >= tSize);
            const bool uplimit    = (z >= tSize);

            // If we're on a page edge
            if (!(rightlimit && uplimit))
            {
                tex->adjustDeformationRectangle(x, z); 
            }

            // we must duplicate the change on the 
            // neighbour page (if it has one...)    
            const uint wL = mOptions->world_width;
            const uint hL = mOptions->world_height;
            const bool right = (rightlimit && pX < wL - 1);
            const bool up    = (uplimit    && pZ < hL - 1);
            const bool left  = (x == 0     && pX != 0);
            const bool down  = (z == 0     && pZ != 0);

            assert (z <= tSize && x <= tSize);

            if (left)
            {
                if (down)
                {               
					//  lower left corner 
					tex = getTexture (pX - 1 , pZ - 1, false);
					if (tex && tex->isLoaded())
                    {
                        tex->adjustDeformationRectangle(tSize, tSize); 
                    } 
                }
                else if (up)
                {
					//  upper left corner 
					tex = getTexture (pX - 1 , pZ + 1, false);
					if (tex && tex->isLoaded())
                    {
                        tex->adjustDeformationRectangle(tSize, 0); 
                    } 
                } 
				// left
				tex = getTexture (pX - 1 , pZ, false);
				if (tex && tex->isLoaded())
                {
                    tex->adjustDeformationRectangle(tSize, z); 
                } 
            }
            else if (right)
            {
                if (up)
                {
					//  upper right corner 
					tex = getTexture (pX + 1 , pZ + 1, false);
					if (tex && tex->isLoaded())
                    {
                        tex->adjustDeformationRectangle(0, 0); 
                    } 
                }
                else if (down)
                {
					//  lower right corner 
					tex = getTexture (pX + 1 , pZ - 1, false);
					if (tex && tex->isLoaded())
                    {
                        tex->adjustDeformationRectangle(0, tSize); 
                    } 
                }  
				// right only
				tex = getTexture (pX + 1 , pZ, false);
				if (tex && tex->isLoaded())
                {
                    tex->adjustDeformationRectangle(0, z); 
                } 
            }
            if (down)
            {        
				//  lower  (down only)    
				tex = getTexture (pX, pZ - 1, false);
				if (tex && tex->isLoaded())
                {
                    tex->adjustDeformationRectangle(x, tSize); 
                } 
            }
            else if (up)
            {
				// upper (up only)
				tex = getTexture (pX, pZ + 1, false);
				if (tex && tex->isLoaded())
                {
                    tex->adjustDeformationRectangle(x, 0); 
                } 
            }     
        } // if (tex->isLoaded())          
    } 
} //namespace
