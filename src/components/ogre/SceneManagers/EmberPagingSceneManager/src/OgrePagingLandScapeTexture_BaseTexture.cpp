/***************************************************************************
OgrePagingLandScapeTexture_BaseTexture.cpp  -  description
-------------------
begin                : Mon Apr 26 2004
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"



#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreStringConverter.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture_BaseTexture.h"
#include "OgrePagingLandScapeData2DManager.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::setOptions(void)
    {
        mParent->getOptions()->VertexCompression = false;
        mParent->getOptions()->lodMorph = false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_BaseTexture::newTexture()
    {
        return new PagingLandScapeTexture_BaseTexture(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_BaseTexture::isMaterialSupported()
    {        
		if (mParent->getOptions()->NumMatHeightSplat > 3)
			return true;
		else
			return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_BaseTexture::PagingLandScapeTexture_BaseTexture(PagingLandScapeTextureManager *textureMgr) 
		: 
		PagingLandScapeTexture(textureMgr, "Base", 1, false)
    {
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture_BaseTexture::~PagingLandScapeTexture_BaseTexture()
    {
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {    		
			PagingLandScapeOptions * const opt = mParent->getOptions();
            const String filename = opt->LandScape_filename;
            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("Base.") + commonName + "." + filename;
		    mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull())
	        {
		        // Create a new texture using the base image
		        mMaterial = MaterialManager::getSingleton().getByName("Base");
                assert (!mMaterial.isNull());
		        mMaterial = mMaterial->clone(matname);

		        const String texname = filename + commonName +  "." + String("BaseBuildPoint.");
                TexturePtr tex = TextureManager::getSingleton().getByName (texname);
                if (tex.isNull())
	            {
                    
					const unsigned int pageSize = opt->PageSize;
		            // Assign the texture to the alpha map
                    BaseImage.loadDynamicImage (_BuildBaseTexture(), 
                                                pageSize, 
                                                pageSize, 
                                                1, 
                                                PF_R8G8B8A8, 
                                                true);

					BaseImage.resize(pageSize - 1, 
									 pageSize - 1);

		            TextureManager::getSingleton().loadImage (texname, 
                        opt->groupName, 
                        BaseImage, 
                        TEX_TYPE_2D, 7, 1.0f);
                }   
		        // assign this texture to the material
                assert  (mMaterial->getTechnique(0) && mMaterial->getTechnique(0)->getPass(0) && mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0));
                assert  (mMaterial->getTechnique(1) && mMaterial->getTechnique(1)->getPass(0) && mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0));
 
		        mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
		        mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);

                
		         
                
            }
	    }
    }
    //-----------------------------------------------------------------------
    uchar *PagingLandScapeTexture_BaseTexture::_BuildBaseTexture() const
    {
		const PagingLandScapeOptions * const opt = mParent->getOptions();
	    PagingLandScapeData2DManager * const dataMgr = mParent->getSceneManager()->getData2DManager();

	    const Real dx = opt->scale.x;
	    const Real dz = opt->scale.z;
        const Real inv_dxdz = 1 / (dx+dz);
        const Real inv_dx = 1 / dx;
        const Real inv_dz = 1 / dz;

		const unsigned int pageSize = opt->PageSize;
	
#		define  _InterpolateColour(O, F, C1, C2) \
					((O + (F * matColor[C1] + \
							(1 - F) * matColor[C2])) * 0.5f)

		const Real matHeight0 = opt->matHeight[1];
		const Real matHeight1 = opt->matHeight[2]; 

		const ColourValue matColor[] = 
		{
			opt->matColor[0],
			opt->matColor[1],
			opt->matColor[2],
			opt->matColor[3]
		};

        // This texture will be used as a base color for the terrain, it will fake the splat for distant renderables.
        const unsigned int size = pageSize * pageSize;
        uchar * const ogre_restrict BaseData = new uchar [size * 3];
        const uchar maxuchar = 255;
        ColourValue out;
        unsigned int curr_image_pos = 0; 
        for (unsigned int i = 0; i < pageSize; i++)
        {
			const int z = i;
            for (unsigned int k = 0; k < pageSize; k++)
            {
                // Generate the base Color 
				const int x = k;
				const Real currHeight = dataMgr->getHeightAtPage(mDataX, mDataZ, x, z);
				const Real height[] =
				{
					dataMgr->getHeightAtPage(mDataX, mDataZ, x - 1, z - 1 ),		// Top-Left
					dataMgr->getHeightAtPage(mDataX, mDataZ, x, z - 1 ),			// Top-Center
					dataMgr->getHeightAtPage(mDataX, mDataZ, x + 1, z - 1 ),		// Top-Right
					dataMgr->getHeightAtPage(mDataX, mDataZ, x - 1, z ),			// Left
					currHeight,				// Current Point
					dataMgr->getHeightAtPage(mDataX, mDataZ, x + 1, z ),			// Right
					dataMgr->getHeightAtPage(mDataX, mDataZ, x - 1, z + 1 ),		// Bottom-Left
					dataMgr->getHeightAtPage(mDataX, mDataZ, x, z + 1 ),			// Bottom-Center
					dataMgr->getHeightAtPage(mDataX, mDataZ, x + 1, z + 1 )		    // Bottom-Right
				};
    
				 // Ask for the current height value and the 8 surrounding values
				// and compute each slope factor
				const Real sloppy[] =
				{
					Math::Abs ( height[0]	 - currHeight ) * inv_dxdz,
					Math::Abs ( height[1]	 - currHeight ) * inv_dz,
					Math::Abs ( height[2]	 - currHeight ) * inv_dxdz,
					Math::Abs ( height[3]	 - currHeight ) * inv_dx,
					0.0f,
					Math::Abs ( height[5]	 - currHeight ) * inv_dx,
					Math::Abs ( height[6]	 - currHeight ) * inv_dxdz,
					Math::Abs ( height[7]	 - currHeight ) * inv_dz,
					Math::Abs ( height[8]	 - currHeight ) * inv_dxdz
				};	

				// Init the colour    
				out = ColourValue(0.0f, 0.0f, 0.0f, 0.0f);

				for (unsigned int neighbor = 0; neighbor < 9; neighbor++)
				{    
					const Real h = height[neighbor];
					const Real slope = sloppy[neighbor];

					if (h < matHeight0)
					{
						if (slope < 0.2f)
							out = matColor[GRASS];// grass-grass
						if (slope >= 0.15f && slope < 0.4f)
							out = _InterpolateColour(out, 0.25f, SAND, GRASS);// sand-grass
						if (slope >= 0.3f && slope < 0.65f)
							out = matColor[SAND];// sand-sand
						if (slope >= 0.55f && slope < 0.75f)
							out = _InterpolateColour(out, 0.75f, SAND, ROCK);// sand-rock
						if (slope >= 0.70f)
							out = matColor[ROCK];// rock-rock						
					}
					else if (h < matHeight1)
					{
						if (slope < 0.15f)
							out = _InterpolateColour(out, 0.25f, GRASS, SNOW);// grass-snow

						if (slope >= 0.10f && slope < 0.45f)
							out = _InterpolateColour(out, 0.65f, SNOW, SAND);// snow-sand
						if (slope >= 0.25f && slope < 0.65f)
							out = _InterpolateColour(out, 0.5f, SNOW, ROCK);// snow-rock						
						if (slope >= 0.50f && slope < 0.75f)
							out = _InterpolateColour(out, 0.75f, SNOW, ROCK);// snow-rock					
						if (slope >= 0.7f)
							out = matColor[ROCK];// rock-rock
					}
					else
					{
						if (slope < 0.15f)
							out = matColor[SNOW];// snow-snow
						if (slope >= 0.1f && slope < 0.45f)
							out = _InterpolateColour(out, 0.35f, SNOW, SAND);// snow-sand						
						if (slope >= 0.25f && slope < 0.65f)
							out = _InterpolateColour(out, 0.5f, SNOW, ROCK);// snow-rock
						if (slope >= 0.5f && slope < 0.75f)
							out = _InterpolateColour(out, 0.75f, SNOW, ROCK);// snow-rock
						if (slope >= 0.7f)
							out = matColor[ROCK];// rock-rock
					}
				}

                // R G B  is the color  formats
                BaseData[curr_image_pos+0] = static_cast <uchar> (out.r * maxuchar);
                BaseData[curr_image_pos+1] = static_cast <uchar> (out.g * maxuchar);
                BaseData[curr_image_pos+2] = static_cast <uchar> (out.b * maxuchar);
				
				curr_image_pos += 3;
            }   
        }
		return BaseData;

#		undef  _InterpolateColour
	}
} //namespace
