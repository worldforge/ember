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
#include "OgreGpuProgramManager.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTextureManager.h"

// Texture loaders implementations
#include "OgrePagingLandScapeTexture.h"

#include "OgrePagingLandScapeTexture_Image.h"

#include "OgrePagingLandScapeTexture_InstantBaseTexture.h"
#include "OgrePagingLandScapeTexture_InstantBaseTextureEdit.h"

#include "OgrePagingLandScapeTexture_BaseTexture.h"
#include "OgrePagingLandScapeTexture_BaseTexture2.h"

#include "OgrePagingLandScapeTexture_Splatting.h"
#include "OgrePagingLandScapeTexture_Splatting2.h"
#include "OgrePagingLandScapeTexture_Splatting2Edit.h"
#include "OgrePagingLandScapeTexture_Splatting3.h"
#include "OgrePagingLandScapeTexture_Splatting4.h"
#include "OgrePagingLandScapeTexture_Splatting5.h"
#include "OgrePagingLandScapeTexture_Splatting6.h"
#include "OgrePagingLandScapeTexture_Splatting7.h"
#include "OgrePagingLandScapeTexture_Splatting7Edit.h"

#include "OgrePagingLandScapeTileInfo.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    template<> PagingLandScapeTextureManager* Singleton<PagingLandScapeTextureManager>::ms_Singleton = 0;
    PagingLandScapeTextureManager* PagingLandScapeTextureManager::getSingletonPtr(void)
    {
	    return ms_Singleton;
    }
    PagingLandScapeTextureManager& PagingLandScapeTextureManager::getSingleton(void)
    {  
	    assert( ms_Singleton );  return ( *ms_Singleton );  
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTextureManager::PagingLandScapeTextureManager( ) :
        mOptions (PagingLandScapeOptions::getSingletonPtr()),
         mTextureType (0),
         mTextureFormat (""),
        mWidth (0),
        mHeight (0),
        mTexturePageSize (mOptions->PageSize -1),
        mPaintChannel(0),
        mPaintColor(ColourValue::White)
    {    
            // Add default texture Types.
            registerTextureType (new PagingLandScapeTexture_Image());

            registerTextureType (new PagingLandScapeTexture_InstantBaseTexture());
            registerTextureType (new PagingLandScapeTexture_InstantBaseTextureEdit());
            registerTextureType (new PagingLandScapeTexture_BaseTexture());
            registerTextureType (new PagingLandScapeTexture_BaseTexture2());

            registerTextureType (new PagingLandScapeTexture_Splatting());
            registerTextureType (new PagingLandScapeTexture_Splatting2());
            registerTextureType (new PagingLandScapeTexture_Splatting2Edit()); 
            registerTextureType (new PagingLandScapeTexture_Splatting3());
            registerTextureType (new PagingLandScapeTexture_Splatting4());   
            registerTextureType (new PagingLandScapeTexture_Splatting5());
            registerTextureType (new PagingLandScapeTexture_Splatting6());  
            registerTextureType (new PagingLandScapeTexture_Splatting7());
            registerTextureType (new PagingLandScapeTexture_Splatting7Edit()); 
        
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
        while (!mTextureTypeMap[nextTextureType]->TextureRenderCapabilitesFullfilled())
        {
            nextTextureType ++ ;
            if (nextTextureType == numTextureTypes)
                nextTextureType = 0;
            numLoop++;
            if (numLoop == numTextureTypes)
            {
                OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, 
                        "Cannot find a TextureMode  supported by current hardware!  (shaders/num texture units...)",
		                " getNextTextureFormat " );           
            }
        }
        return mTextureTypeMap[nextTextureType]->getName();
    }
    //-----------------------------------------------------------------------
   String PagingLandScapeTextureManager::getCurrentTextureFormat()
    {
        assert (!mTextureTypeMap.empty());
        return mTextureTypeMap[mTextureType]->getName();;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTextureManager::~PagingLandScapeTextureManager( )
    {
        // for all in map delete.
        PagingLandScapeTextureMap::iterator i = mTextureTypeMap.begin();
        while (i != mTextureTypeMap.end())
        {
            delete (*i);
            ++i;
        }
        PagingLandScapeTextureManager::reset ();
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::reset()
    {
        PagingLandScapeTexturePages::iterator iend = mTexture.end();
        for (PagingLandScapeTexturePages::iterator i = mTexture.begin(); 
            i != iend; ++i)
        {
             std::for_each( i->begin (), 
                            i->end (),  
                            delete_object());

            i->clear ();        
        }
       mTexture.clear();
       mWidth = 0; 
       mHeight = 0;
    }  
    //-----------------------------------------------------------------------
    PagingLandScapeTexture *PagingLandScapeTextureManager::allocateTexture() const
    {
       return mTextureTypeMap[mTextureType]->newTexture();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::load (  )
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
                    mTextureType = i;
                    break;
                }
            }
            if (i == numTextureTypes)
            {
                OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, 
                        "TextureMode not supplied or wrong (check case) !",
		                " PagingLandScapeTextureManager::load " );
            }
            if (!mTextureTypeMap[mTextureType]->TextureRenderCapabilitesFullfilled())
            {
                OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, 
                        "TextureMode not supported by current hardware!  (shaders/num texture units...)",
		                " PagingLandScapeTextureManager::load " );
            }
            mTexturePageSize = mOptions->PageSize - 1;
        }

        // static member init. 
        mTextureTypeMap[mTextureType]->setPagesize();

        WorldDimensionChange();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::clear (  )
    {
        // static member de-init. 
       mTextureTypeMap[mTextureType]->clearData();
       mMapMaterial.setNull();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::WorldDimensionChange()
    {
        const uint newWidth = mOptions->world_width; 
        const uint newHeight = mOptions->world_height;    

        if (newWidth != mWidth || newHeight != mHeight)
        {
            uint i,k;
            if (newWidth < mWidth )
            {
                for (i = newWidth; i <  mWidth; ++i)
	            {
                    PagingLandScapeTextureRow  *li = &mTexture[ i ];
                    for (k = 0; k < mHeight; k++ )
		            {
                        delete ((*li)[ k ]);
		            }            
                    li->clear();
	            }
                mTexture.reserve (newWidth);
                mTexture.resize (newWidth);


                if (newHeight < mHeight )
                { 
                    // clear pages
                    for (i = 0; i < newWidth; i++ )
	                {
                        PagingLandScapeTextureRow  *li = &mTexture[ i ];
                        for (k = newHeight; k < mHeight; ++k)
	                    {
                            delete (*li)[ k ];
	                    }
                        li->reserve(newHeight);
                        li->resize(newHeight);
                    }
                }
                else if (newHeight > mHeight )
                {
                    // add pages
                    for (i = 0; i < newWidth; i++ )
	                {
                        PagingLandScapeTextureRow  *li = &mTexture[ i ];
                        li->reserve(newHeight);
                        li->resize(newHeight);
	                    for (k = mHeight; k < newHeight; k++ )
		                {
			                (*li)[ k ] = allocateTexture ();
		                }
	                }
                }
            }
            else if (newWidth > mWidth )
            {
                mTexture.reserve (newWidth);
                mTexture.resize (newWidth);

                if (newHeight < mHeight )
                { 
                    // clear pages
                    for (i = 0; i < mWidth; i++ )
	                {
                        PagingLandScapeTextureRow  *li = &mTexture[ i ];
                        for (k = newHeight; k < mHeight; ++k)
	                    {
                            delete (*li)[ k ];
	                    }
                        li->reserve(newHeight);
                        li->resize(newHeight);
                    }
                }
                else if (newHeight > mHeight )
                {
                    // add pages
                    for (i = 0; i < mWidth; i++ )
	                {
                        PagingLandScapeTextureRow  *li = &mTexture[ i ];
                        li->reserve(newHeight);
                        li->resize(newHeight);
	                    for (k = mHeight; k < newHeight; k++ )
		                {
			                (*li)[ k ] = allocateTexture ();
		                }
	                }
                }

                // add pages
	            for (i = mWidth; i < newWidth; i++ )
	            {
                    mTexture[ i ].reserve (newHeight);
                    mTexture[ i ].resize (newHeight);
	            }
                for (i = mWidth; i < newWidth; i++ )
	            {
                    PagingLandScapeTextureRow  *li = &mTexture[ i ];
                    li->reserve(newHeight);
                    li->resize(newHeight);
	                for (k = 0; k < newHeight; k++ )
		            {
                        (*li)[ k ] = allocateTexture ();
		            }
	            }
            }
            else if (newHeight < mHeight )
            { 
                for (i = 0; i < newWidth; i++ )
	            {
                    PagingLandScapeTextureRow  *li = &mTexture[ i ];
                    for (k = newHeight; k < mHeight; ++k)
	                {
                        delete (*li)[ k ];
	                }
                    li->reserve(newHeight);
                    li->resize(newHeight);
                }
            }
            else if (newHeight > mHeight )
            {
                for (i = 0; i < newWidth; i++ )
	            {
                    PagingLandScapeTextureRow  *li = &mTexture[ i ];
                    li->reserve(newHeight);
                    li->resize(newHeight);
	                for (k = mHeight; k < newHeight; ++k )
		            {    
                        (*li)[ k ] = allocateTexture ();
		            }
	            }
            }
            mWidth = newWidth; 
            mHeight = newHeight;    
//            for (i = 0; i < newWidth; i++ )
//            {
//                PagingLandScapeTextureRow  *li = &mTexture[ i ];
//                for (k = 0; k < newHeight; k++ )
//                {		
//                    PagingLandScapeTexture *p = (*li)[ k ];
//                    
//                }
//            }
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::setMapMaterial()
    {
       const String mapName =  String("Small") + 
                PagingLandScapeOptions::getSingleton().image_filename;
        mMapMaterial = MaterialManager::getSingleton().getByName (mapName);
        if (mMapMaterial.isNull())
        {
            MaterialPtr matTemplate = MaterialManager::getSingleton().
                            getByName (String ("PagingLandScape.Small.Template"));
            mMapMaterial = matTemplate ->clone (mapName);
            
	        mMapMaterial->getTechnique(0)->getPass(0)->
                                getTextureUnitState(0)->setTextureName (
                                PagingLandScapeOptions::getSingleton().image_filename
                                + ".Small." + 
                                PagingLandScapeOptions::getSingleton().TextureExtension);
            mMapMaterial->load();
        }
      } 
    //-----------------------------------------------------------------------
    MaterialPtr PagingLandScapeTextureManager::getMapMaterial()
    {
        return mMapMaterial;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::load( const uint texX, const uint texZ )
    {
	    PagingLandScapeTexture* tex = mTexture[ texX ][ texZ ];
	    if ( !tex->isLoaded() )
        {
		    tex->load( texX, texZ );
	    }
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeTextureManager::unload( const uint texX, const uint texZ )
    {
	    PagingLandScapeTexture* tex = mTexture[ texX ][ texZ ];
	    if ( tex->isLoaded() )
	    {
		    tex->unload();
	    }
    }
    //-----------------------------------------------------------------------
     PagingLandScapeTexture* PagingLandScapeTextureManager::getTexture (const uint texX, const uint texZ)
    {
	    PagingLandScapeTexture* tex = mTexture[ texX ][ texZ ];
        if ( tex->isLoaded() )
	    {
		    return tex;
	    }
        return 0;
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTextureManager::isLoaded( const uint texX, const uint texZ )
    {
	    return mTexture[ texX ][ texZ ]->isLoaded();
    }
    //-----------------------------------------------------------------------
    const MaterialPtr&  PagingLandScapeTextureManager::getMaterial( const uint texX, const uint texZ ) const
    {
	    return mTexture[ texX ][ texZ ]->getMaterial();
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

        PagingLandScapeTexture* tex = mTexture[ pX ][ pZ ];
        assert (tex);
        if ( tex->isLoaded() )
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
                    tex = mTexture[ pX - 1 ][ pZ - 1 ];
                    if ( tex->isLoaded() )
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
                    tex = mTexture[ pX - 1 ][ pZ  + 1 ];
                    if ( tex->isLoaded() )
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
                    tex = mTexture[ pX - 1 ][ pZ ];
                    if ( tex->isLoaded() )
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
                    tex = mTexture[ pX + 1 ][ pZ  + 1 ];
                    if ( tex->isLoaded() )
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
                    tex = mTexture[ pX + 1 ][ pZ  - 1 ];
                    if ( tex->isLoaded() )
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
                    tex = mTexture[ pX + 1 ][ pZ ];
                    if ( tex->isLoaded() )
                    {
                        if (isAlpha)
                            tex->paint (0, z, paintForce, mPaintChannel);
                        else
                            tex->paint (0, z, paintForce, mPaintColor);
                    } // if ( tex->isLoaded() )   
                }
            }
            else if (down)
            {        
                //  lower  (down only)    
                tex = mTexture[ pX ][ pZ - 1 ];
                if ( tex->isLoaded() )
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
                tex = mTexture[ pX ][ pZ  + 1 ];
                if ( tex->isLoaded() )
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

        PagingLandScapeTexture* tex = mTexture[ pX ][ pZ ];
        assert (tex);
        if ( tex->isLoaded() )
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
                    tex = mTexture[ pX - 1 ][ pZ - 1 ];
                    if ( tex->isLoaded() )
                    {
                        tex->adjustDeformationRectangle(tSize, tSize); 
                    } 
                }
                else if (up)
                {
                    //  upper left corner 
                    tex = mTexture[ pX - 1 ][ pZ  + 1 ];
                    if ( tex->isLoaded() )
                    {
                        tex->adjustDeformationRectangle(tSize, 0); 
                    } 
                } 
                // left
                tex = mTexture[ pX - 1 ][ pZ ];
                if ( tex->isLoaded() )
                {
                    tex->adjustDeformationRectangle(tSize, z); 
                } 
            }
            else if (right)
            {
                if (up)
                {
                    //  upper right corner 
                    tex = mTexture[ pX + 1 ][ pZ  + 1 ];
                    if ( tex->isLoaded() )
                    {
                        tex->adjustDeformationRectangle(0, 0); 
                    } 
                }
                else if (down)
                {
                    //  lower right corner 
                    tex = mTexture[ pX + 1 ][ pZ  - 1 ];
                    if ( tex->isLoaded() )
                    {
                        tex->adjustDeformationRectangle(0, tSize); 
                    } 
                }  
                // right only
                tex = mTexture[ pX + 1 ][ pZ ];
                if ( tex->isLoaded() )
                {
                    tex->adjustDeformationRectangle(0, z); 
                } 
            }
            if (down)
            {        
                //  lower  (down only)    
                tex = mTexture[ pX ][ pZ - 1 ];
                if ( tex->isLoaded() )
                {
                    tex->adjustDeformationRectangle(x, tSize); 
                } 
            }
            else if (up)
            {
                // upper (up only)
                tex = mTexture[ pX ][ pZ  + 1 ];
                if ( tex->isLoaded() )
                {
                    tex->adjustDeformationRectangle(x, 0); 
                } 
            }     
        } // if ( tex->isLoaded() )          
    } 
} //namespace
