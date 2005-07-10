/***************************************************************************
OgrePagingLandScapeTexture_BaseTexture.cpp  -  description
-------------------
begin                : Mon Apr 26 2004
copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
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



#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreStringConverter.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTexture_BaseTexture.h"
#include "OgrePagingLandScapeData2DManager.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::_setPagesize( void )
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::_clearData( void )
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_BaseTexture::newTexture( )
    {
        return new PagingLandScapeTexture_BaseTexture();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_BaseTexture::TextureRenderCapabilitesFullfilled()
    {
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_BaseTexture::PagingLandScapeTexture_BaseTexture() : PagingLandScapeTexture()
    {
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture_BaseTexture::~PagingLandScapeTexture_BaseTexture()
    {
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::_loadMaterial()
    {
	    if ( mMaterial.isNull() )
	    {    		
            const String filename = PagingLandScapeOptions::getSingleton().landscape_filename;
            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("BaseMaterial.") + commonName + "." + filename;
		    mMaterial = MaterialManager::getSingleton().getByName(matname);

            if (mMaterial.isNull() )
	        {
		        // Create a new texture using the base image
		        mMaterial = MaterialManager::getSingleton().getByName("BaseMaterial");

		        mMaterial = mMaterial->clone(matname);

		        const String texname = filename + commonName +  "." + String("BaseBuildPoint.");
                TexturePtr tex = TextureManager::getSingleton().getByName (texname);
                if ( tex.isNull() )
	            {
                    const uint pageSize = PagingLandScapeOptions::getSingleton().PageSize - 1;
            	
		            // This texture will be used as a base color for the terrain, it will fake the splat for distant renderables.
                    const uint size = pageSize * pageSize;
		            uchar *BaseData = new uchar [size * 3];

		            // Assign the texture to the alpha map
                    BaseImage.loadDynamicImage (BaseData, pageSize, 
                                    pageSize, 1, PF_BYTE_RGB, true);

                    const uchar maxuchar = 255;
		            ColourValue color;
		            Real alpha1, alpha2, alpha3, alpha4;
                    uint curr_image_pos = 0;        
		            for (uint i = 0; i < pageSize; i++)
                    {
		                for (uint k = 0; k < pageSize; k++)
		                {
			                // Generate the base Color 
			                _BuildPoint( color, k, i, alpha1, alpha2, alpha3, alpha4 );

        			        const uint pos = curr_image_pos + k*3;

                            assert (pos+2 < (size * 3));

                            // R G B  is the color  formats
			                BaseData[pos+0] = static_cast <uchar> (color.r * maxuchar);
			                BaseData[pos+1] = static_cast <uchar> (color.g * maxuchar);
			                BaseData[pos+2] = static_cast <uchar> (color.b * maxuchar);
                        }   
                        curr_image_pos += pageSize*3;
                    }
		            TextureManager::getSingleton().loadImage (texname, 
                        PagingLandScapeOptions::getSingleton().groupName, 
                        BaseImage, 
                        TEX_TYPE_2D, 7, 1.0f);
                }   
		        // assign this texture to the material
                assert  (mMaterial->getTechnique(0) && mMaterial->getTechnique(0)->getPass(0) && mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0));
                assert  (mMaterial->getTechnique(1) && mMaterial->getTechnique(1)->getPass(0) && mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0));

		        mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
		        mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);

                // Now that we have all the resources in place, we load the material
		        mMaterial->load(); 
                mMaterial->setLightingEnabled( PagingLandScapeOptions::getSingleton().lit );
            }
	    }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::_unloadMaterial()
    {
	    if ( !mMaterial.isNull() )
	    {
		    mMaterial->unload();
	    }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::_BuildPoint(ColourValue& out, const int x, const int z, 
                                                        Real& alpha1, Real& alpha2, Real& alpha3, Real& alpha4) const
    {
        	
	    // Ask for the current height value and the 8 surrounding values
	    Real height[9];

        const PagingLandScapeData2DManager * const dataPageManager = PagingLandScapeData2DManager::getSingletonPtr();

        height[0] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x - 1, z - 1 );		// Top-Left
	    height[1] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x, z - 1 );			// Top-Center
	    height[2] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x + 1, z - 1 );		// Top-Right
	    height[3] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x - 1, z );			// Left
	    height[4] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x, z );				// Current Point
	    height[5] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x + 1, z );			// Right
	    height[6] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x - 1, z + 1 );		// Botton-Left
	    height[7] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x, z + 1 );			// Botton-Center
	    height[8] = dataPageManager->getHeightAtPage( mDataX, mDataZ, x + 1, z + 1 );		// Botton-Right

	    // Weight( pt1 , pt2 ) = 1 - DistanceSquared(pt1,pt2) / (1.75)^2

        const PagingLandScapeOptions *  const  options = PagingLandScapeOptions::getSingletonPtr();
	    //The slope test
	    const Real dx = options->scale.x;
	    const Real dz = options->scale.z;
        const Real inv_dxdz = 1 / (dx+dz);
        const Real inv_dx = 1 / dx;
        const Real inv_dz = 1 / dz;
	    Real sloppy[8];
	    sloppy[0] = Math::Abs ( height[0] - height[4] ) * inv_dxdz;
	    sloppy[1] = Math::Abs ( height[1] - height[4] ) * inv_dz;
	    sloppy[2] = Math::Abs ( height[2] - height[4] ) * inv_dxdz;
	    sloppy[3] = Math::Abs ( height[3] - height[4] ) * inv_dx;
	    sloppy[4] = Math::Abs ( height[5] - height[4] ) * inv_dx;
	    sloppy[5] = Math::Abs ( height[6] - height[4] ) * inv_dxdz;
	    sloppy[6] = Math::Abs ( height[7] - height[4] ) * inv_dz;
	    sloppy[7] = Math::Abs ( height[8] - height[4] ) * inv_dxdz;

    	
    #define SNOW  0
    #define SAND  1
    #define GRASS 2
    #define ROCK  3

	    // Init the colour    
	    out = ColourValue(0.0f, 0.0f, 0.0f, 0.0f);
	    alpha1 = 0.0f;
	    alpha2 = 0.0f;
	    alpha3 = 0.0f;
	    alpha4 = 0.0f;

        const Real matHeight0 = options->matHeight[1];
        const Real matHeight1 = options->matHeight[2];
       
        for ( uint i = 0; i < 8; i++ )
        {    
            const Real h = height[i];
            const Real slope = sloppy[i];

            if ( h < matHeight0 )
            {
                if ( slope < 0.2f )
                {
                    // grass-grass
                    _InterpolateColour( out, 1.0f, GRASS, GRASS );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, GRASS, GRASS );
                }
                if ( slope >= 0.15f && slope < 0.4f )
                {
                    // sand-grass
                    _InterpolateColour( out, 0.25f, SAND, GRASS );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.25f, SAND, GRASS );
                }
                if ( slope >= 0.3f && slope < 0.65f )
                {
                    // sand-sand
                    _InterpolateColour( out, 1.0f, SAND, SAND );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, SAND, SAND );
                }
                if ( slope >= 0.55f && slope < 0.75f )
                {
                    // sand-rock
                    _InterpolateColour( out, 0.75f, SAND, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, SAND, ROCK );
                }
                if ( slope >= 0.70f )
                {
                    // rock-rock
                    _InterpolateColour( out, 1.0f, ROCK, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, ROCK, ROCK );
                }
            }
            else if ( h < matHeight1 )
            {
                if ( slope < 0.15f )
                {
                    // grass-snow
                    _InterpolateColour( out, 0.25f, GRASS, SNOW );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.25f, GRASS, SNOW );
                }
                if ( slope >= 0.10f && slope < 0.45f )
                {
                    // snow-sand
                    _InterpolateColour( out, 0.65f, SNOW, SAND );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.65f, SNOW, SAND );
                }
                if ( slope >= 0.25f && slope < 0.65f )
                {
                    // snow-rock
                    _InterpolateColour( out, 0.5f, SNOW, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.5f, SNOW, ROCK );
                }
                if ( slope >= 0.50f && slope < 0.75f )
                {
                    // snow-rock
                    _InterpolateColour( out, 0.75f, SNOW, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, SNOW, ROCK );
                }
                if ( slope >= 0.7f )
                {
                    // rock-rock
                    _InterpolateColour( out, 1.0f, ROCK, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, ROCK, ROCK );
                }
            }
            else
            {
                if ( slope < 0.15f )
                {
                    // snow-snow
                    _InterpolateColour( out, 1.0f, SNOW, SNOW );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, 0, 0 );
                }
                if ( slope >= 0.1f && slope < 0.45f )
                {
                    // snow-sand
                    _InterpolateColour( out, 0.35f, SNOW, SAND );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.35f, 0, 1 );
                }
                if ( slope >= 0.25f && slope < 0.65f )
                {
                    // snow-rock
                    _InterpolateColour( out, 0.5f, SNOW, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.5f, 0, 3 );
                }
                if ( slope >= 0.5f && slope < 0.75f )
                {
                    // snow-rock
                    _InterpolateColour( out, 0.75f, SNOW, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, 0, 3 );
                }
                if ( slope >= 0.7f )
                {
                    // rock-rock
                    _InterpolateColour( out, 1.0f, ROCK, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, 3, 3 );
                }
            }
        }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::_InterpolateColour(ColourValue& out, 
                                                                const Real percentaje, 
                                                                const int index1, const int index2) const
    {
        const PagingLandScapeOptions *  const  options = PagingLandScapeOptions::getSingletonPtr();
        const ColourValue color1 = options->matColor[index1];
	    if (percentaje < 1.0f)
        {   
	        const Real tmp = 1 - percentaje;
            const ColourValue color2 = options->matColor[index2];

            out.r = ( out.r + ( percentaje * color1.r + tmp * color2.r ) ) * 0.5f;
            out.g = ( out.g + ( percentaje * color1.g + tmp * color2.g ) ) * 0.5f;
            out.b = ( out.b + ( percentaje * color1.b + tmp * color2.b ) ) * 0.5f;
        }
        else if (index1 == index2)
        {
            out.r = color1.r;
            out.g = color1.g;
            out.b = color1.b;
        }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture::_InterpolateAlpha(Real& alpha1, 
                                                            Real& alpha2, 
                                                            Real& alpha3, 
                                                            Real& alpha4, 
                                                            const Real percentaje, 
                                                            const int index1, 
                                                            const int index2) const
    {
	    if ( index1 == index2 )
	    {
		    if ( index1 == 0 )
		    {
			    alpha1 += 1.0f;
		    }
		    else if ( index1 == 1 )
		    {
			    alpha2 += 1.0f;
		    }
		    else if ( index1 == 2 )
		    {
			    alpha3 += 1.0f;
		    }
		    else
		    {
			    alpha4 += 1.0f;
		    }
	    }
	    else
	    {
		    if ( index2 == 0 )
		    {
			    alpha1 += percentaje;
		    }
		    else if ( index2 == 1 )
		    {
			    alpha2 += percentaje;
		    }
		    else if ( index2 == 2 )
		    {
			    alpha3 += percentaje;
		    }
		    else
		    {
			    alpha4 += percentaje;
		    }

		    if ( index1 == 0 )
		    {
			    alpha1 += 1.0f - percentaje;
		    }
		    else if ( index1 == 1 )
		    {
			    alpha2 += 1.0f - percentaje;
		    }
		    else if ( index1 == 2 )
		    {
			    alpha3 += 1.0f - percentaje;
		    }
		    else
		    {
			    alpha4 += 1.0f - percentaje;
		    }
	    }
	    //snow  = 0
	    //sand  = 1
	    //grass = 2
	    //rock  = 3
        const Real fifth = 0.2f;
	    alpha1 *= ( 4 * fifth );
	    alpha2 *= ( 3 * fifth );
	    alpha3 *= ( 4 * fifth );
	    alpha4 *= ( 2 * fifth );
	    // Normalize
        assert ((alpha1 + alpha2 + alpha3 + alpha4) > 0.0f);
	    const Real total = 1 / (( alpha1 + alpha2 + alpha3 + alpha4 ) * 2);
	    alpha1 = alpha1 * total;
	    alpha2 = alpha2 * total;
	    alpha3 = alpha3 * total;
	    alpha4 = alpha4 * total;
    }

} //namespace
