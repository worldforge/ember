/***************************************************************************
	OgrePagingLandScapeTextureManager.cpp  -  description
	-------------------
	begin                : Fri Apr 16 2004
	copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
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

#include "OgrePagingLandScapePrecompiledHeaders.h"


#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreMaterialManager.h"
#include "OgreGpuProgramManager.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeData2DManager.h"

// Texture loaders implementations
#include "OgrePagingLandScapeTexture.h"

// #include "OgrePagingLandScapeTexture_Image.h"
// 
// #include "OgrePagingLandScapeTexture_BaseTexture.h"
// #include "OgrePagingLandScapeTexture_Splatting.h"

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
    }
	//-----------------------------------------------------------------------
	void	PagingLandScapeTextureManager::registerTextureFormats()
	{ 
		const std::vector<String> TextureFormatSupported = mOptions->TextureFormatSupported;

		// for all terrain configuration file texture format not in mTextureTypeMap allocate
		{
			std::vector<String>::const_iterator itTexFormatSupp = TextureFormatSupported.begin();
			while (itTexFormatSupp != TextureFormatSupported.end())
			{
				const String &textureFormat = (*itTexFormatSupp);
				if (!textureFormat.empty ())
				{
					PagingLandScapeTextureMap::iterator iTexFormatCurr = mTextureTypeMap.begin();
					while (iTexFormatCurr != mTextureTypeMap.end())
					{
						if ((*iTexFormatCurr)->getName () == textureFormat)
							break;
						++iTexFormatCurr;
					}
				}
				++itTexFormatSupp;
			}
		}


		// for all mTextureTypeMap[i] not in terrain configuration file, delete
		{
			PagingLandScapeTextureMap::iterator iTexFormatCurr = mTextureTypeMap.begin();
			while (iTexFormatCurr != mTextureTypeMap.end())
			{
				const String &textureFormat = (*iTexFormatCurr)->getName ();

				if (TextureFormatSupported.end() == std::find(TextureFormatSupported.begin(), 
													TextureFormatSupported.end(), 
													textureFormat))
				{
					delete (*iTexFormatCurr);
					iTexFormatCurr = mTextureTypeMap.erase (iTexFormatCurr);
				}
				else
				{
					++iTexFormatCurr;
				}
			}
		}


		// Add default texture Types.
		// dynamically create maps.
		//registerTextureType (new PagingLandScapeTexture_BaseTexture(this));
		//registerTextureType (new PagingLandScapeTexture_Splatting(this));
	}

	//-----------------------------------------------------------------------
	void	PagingLandScapeTextureManager::clearTextureFormats()
	{
		// for all in map delete.
		PagingLandScapeTextureMap::iterator i = mTextureTypeMap.begin();
		while (i != mTextureTypeMap.end())
		{
			delete (*i);
			++i;
		}
	}
    //-----------------------------------------------------------------------
    String PagingLandScapeTextureManager::getNextTextureFormat()
    {
        assert (!mTextureTypeMap.empty());
        const size_t numTextureTypes = mTextureTypeMap.size();
        unsigned int nextTextureType =  mTextureType + 1;
        if (nextTextureType == numTextureTypes)
            nextTextureType = 0;
        unsigned int numLoop = 0;
        // find Next supported texture format.
        while (numLoop != numTextureTypes)
        { 
#ifdef _DEBUG
            std::cout << "Trying " << mTextureTypeMap[nextTextureType]->getName () << '\n';
#endif //_DEBUG

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
	void PagingLandScapeTextureManager::setPageSize ()
	{
		PagingLandScapeOptions * const opt = mOptions;
		mPageSize = opt->PageSize - 1;
		Real Texturescale = opt->TextureStretchFactor;
		const unsigned int textureSize = (unsigned int) (mPageSize * Texturescale);
		uchar *data = new uchar[textureSize * textureSize * 3];
		mImage.loadDynamicImage (data, textureSize, textureSize, PF_R8G8B8A8);

		const unsigned int numColors = opt->NumMatHeightSplat;

		unsigned int i;
		colors.reserve (numColors);
		colors.resize (numColors);
		for (i = 0; i < numColors; i++)
		{
			colors[i] = opt->matColor[i];
		}
		heights.reserve (numColors);
		heights.resize (numColors);
		for (i = 0; i < numColors; i++)
		{
			heights[i] = opt->matHeight[i];
		}

		//  slope[] ??

		if (numColors  && heights[numColors - 1] == 0.0f) 
			heights[numColors - 1] = getSceneManager()->getData2DManager()->getMaxHeight ();

		dividers.reserve (numColors);
		dividers.resize (numColors);
		if (numColors)
		{
			dividers[0] = 1.0f;

			for (i = 1; i < numColors; i++) 
			{   
				if ((heights[i]  - heights[i - 1])  > 0)
					dividers[i] = 1 / (heights[i]  - heights[i - 1]);
				else 
					dividers[i] = 0.0f;
			}		
		}

	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTextureManager::clearData ()
	{
		delete [] mImage.getData ();    
		mImage.loadDynamicImage (0, 0, 0, PF_R8G8B8A8);    
	}
    //-----------------------------------------------------------------------
    String PagingLandScapeTextureManager::getCurrentTextureFormat()
    {
        if (!mTextureTypeMap.empty())
            return mTextureTypeMap[mTextureType]->getName();
        return StringUtil::BLANK;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTextureManager::~PagingLandScapeTextureManager()
	{
		reset ();
		clearTextureFormats ();
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
	PagingLandScapeTexture *PagingLandScapeTextureManager::getNewTexture(const unsigned int x, const unsigned int z)
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
	PagingLandScapeTexture *PagingLandScapeTextureManager::getTexture(const unsigned int x, const unsigned int z,
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
		registerTextureFormats ();        
        {
            reset ();
            mTextureFormat = mOptions->textureFormat; 
            unsigned int i = 0;

            assert (!mTextureTypeMap.empty());

            const size_t numTextureTypes = mTextureTypeMap.size();
            for (; i != numTextureTypes; ++i)
            {
                const String &modeName = mTextureTypeMap[i]->getName ();
                if (modeName == mTextureFormat)
                {
					mTextureType = i;
                    break;
                }
            }
            if (i == numTextureTypes)
            {
                OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                        "TextureMode not supplied or wrong (check case) !",
		                " PagingLandScapeTextureManager::load ");
            }
            mTexturePageSize = mOptions->PageSize - 1;
        }
        setPageSize();      
        WorldDimensionChange();
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::clear ()
    {
       // member de-init. 
       clearData();
       mMapMaterial.setNull();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::WorldDimensionChange()
    {
        const unsigned int newWidth = mOptions->world_width; 
        const unsigned int newHeight = mOptions->world_height;    

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
            const String texname(opt->image_filename + ".Small." + 
                                opt->TextureExtension);
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
    void PagingLandScapeTextureManager::load(const unsigned int texX, const unsigned int texZ)
    {
	    PagingLandScapeTexture* tex = getTexture (texX , texZ);
	    if (!tex->isLoaded ())
        {
		    tex->load (texX, texZ);
	    }
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::unload(const unsigned int texX, const unsigned int texZ)
    {
	    PagingLandScapeTexture* tex = getTexture (texX , texZ, false);
	    if (tex && tex->isLoaded ())
	    {
		    tex->unload ();
			releaseTexture (tex);
	    }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::reload(const unsigned int texX, const unsigned int texZ)
    {
        PagingLandScapeTexture* tex = getTexture (texX , texZ);
	    tex->unload();
	    tex->load(texX, texZ);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTextureManager::isLoaded(const unsigned int texX, const unsigned int texZ)
    {
		const PagingLandScapeTexture * const tex = getTexture (texX , texZ, false);
	    return tex && tex->isLoaded();
    }
    //-----------------------------------------------------------------------
    const MaterialPtr&  PagingLandScapeTextureManager::getMaterial(const unsigned int texX, const unsigned int texZ)
	{
		PagingLandScapeTexture * const tex = getTexture (texX , texZ, false);
		assert (tex);
		return tex->getMaterial();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::paint (const Vector3 &currpoint, 
												const Real paintForce, 
												const PagingLandScapeTileInfo *info)
    {

    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::deformHeight (const Vector3 &currpoint,
        const PagingLandScapeTileInfo *info)
    {

    } 
} //namespace
