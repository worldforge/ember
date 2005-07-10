/***************************************************************************
OgrePagingLandScapeTexture_Splatting.cpp  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
	email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
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
#include "OgrePagingLandScapeTexture_Splatting.h"
#include "OgrePagingLandScapeData2DManager.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting::_setPagesize( void )
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting::_clearData( void )
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting::newTexture( )
    {
        return new PagingLandScapeTexture_Splatting();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting::TextureRenderCapabilitesFullfilled()
    {
        
        if (PagingLandScapeOptions::getSingleton().numTextureUnits >= 2)
            return true;
        return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting::PagingLandScapeTexture_Splatting() : PagingLandScapeTexture()
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting::~PagingLandScapeTexture_Splatting()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting::_loadMaterial()
    {
	    if ( mMaterial.isNull() )
	    {
            const String filename = PagingLandScapeOptions::getSingleton().landscape_filename;
            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("SplattingMaterial.") + commonName + filename;
		    mMaterial = MaterialManager::getSingleton().getByName(matname);

            if ( mMaterial.isNull() )
	        {
		        mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial");

		        // Create a new texture using the base image
		        mMaterial = mMaterial->clone(matname);

		        const String Basetexname = String("Splatting.") + commonName + filename;
                TexturePtr tex = TextureManager::getSingleton().getByName (Basetexname);
                if ( tex.isNull() )
	            {
                    const uint pageSize = PagingLandScapeOptions::getSingleton().PageSize - 1;
                    const uint size = pageSize * pageSize;
                    const uint pageMemorySize = size * 4;
                    uchar *BaseData = new uchar [pageMemorySize];
		            // Create the base image
		            DataStreamPtr dc (new MemoryDataStream (BaseData, pageMemorySize, true));

		            // Assign the texture to the base map
                    
		            // This texture will be used as a base color for the terrain, 
                    // it will fake the splat for distant renderables.
		            Image BaseImage;
		            BaseImage.loadRawData (dc, pageSize, pageSize, PF_BYTE_RGBA);

                    uchar *AlphaData = new uchar [size];
                    memset (AlphaData, 0, size);
		            DataStreamPtr dca (new MemoryDataStream (AlphaData, size, true));
		            // Create the 1st alpha map      
		            Image AlphaMap1;
		            AlphaMap1.loadRawData(dca, pageSize, pageSize, PF_A8);
		            // Create the 2nd alpha map     
		            Image AlphaMap2;
		            AlphaMap2.loadRawData(dca, pageSize, pageSize, PF_A8);   
		            // Create the 3rd alpha map       
		            Image AlphaMap3;
		            AlphaMap3.loadRawData(dca, pageSize, pageSize, PF_A8);
		            // Create the 4th alpha map       
		            Image AlphaMap4;
		            AlphaMap4.loadRawData(dca, pageSize, pageSize, PF_A8);
            		

		            ColourValue color;

		            BaseData = BaseImage.getData();
		            uchar *AlphaData1 = AlphaMap1.getData();
		            uchar *AlphaData2 = AlphaMap2.getData();
		            uchar *AlphaData3 = AlphaMap3.getData();
		            uchar *AlphaData4 = AlphaMap4.getData();
                    
                    const uchar maxuchar = 255;
                        
                    Real alpha1, alpha2, alpha3, alpha4;
                    bAlpha1NotUsed = bAlpha2NotUsed = bAlpha3NotUsed = bAlpha4NotUsed = true;
		            for (uint j = 0; j < size; ++j)
		            {
			            // Generate the base texture
			            _BuildPoint( color, j, alpha1, alpha2, alpha3, alpha4 );
                        const uint pos = j*4;
			            // R G B A is the format to add the colors
			            BaseData[pos+0] = static_cast <uchar> (color.r * maxuchar);
			            BaseData[pos+1] = static_cast <uchar> (color.g * maxuchar);
			            BaseData[pos+2] = static_cast <uchar> (color.b * maxuchar);
			            BaseData[pos+3] = static_cast <uchar> (( 1 - ( alpha1 + alpha2 + alpha3 + alpha4 ) ) * maxuchar); // Opaque

                        const uint posa = j;
			            // Generate the alpha map 1
			            AlphaData1[posa] = static_cast <uchar> (alpha1 * maxuchar);
                        if (bAlpha1NotUsed && alpha1 - 0.05f > 0.0f)
                            bAlpha1NotUsed  = false;

			            // Generate the alpha map 2
			            AlphaData2[posa] = static_cast <uchar> (alpha2 * maxuchar);
                        if (bAlpha2NotUsed && alpha2 - 0.05f > 0.0f)
                            bAlpha2NotUsed  = false;

			            // Generate the alpha map 3
			            AlphaData3[posa] = static_cast <uchar> (alpha3 * maxuchar);
                        if (bAlpha3NotUsed && alpha3 - 0.05f > 0.0f)
                            bAlpha3NotUsed  = false;

			            // Generate the alpha map 4
			            AlphaData4[posa] = static_cast <uchar> (alpha4 * maxuchar);
                        if (bAlpha4NotUsed && alpha4 - 0.05f > 0.0f )
                            bAlpha4NotUsed  = false;
		            }

		            TextureManager::getSingleton().loadImage (
                        Basetexname, PagingLandScapeOptions::getSingleton().groupName, 
                        BaseImage, TEX_TYPE_2D, 7, 1.0f);

		            // assign this texture to the material
		            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(Basetexname);
		            mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(Basetexname);
                

                    if (!bAlpha1NotUsed)
                    {
                        // Create a new texture using the 1st alpha map
                        const String AlphaMap1texname = String("SplattingAlpha1.") + commonName + filename;
                        TextureManager::getSingleton().loadImage(
                            AlphaMap1texname, PagingLandScapeOptions::getSingleton().groupName, 
                            AlphaMap1, TEX_TYPE_2D, 7, 1.0f);
                    }
                    if (!bAlpha2NotUsed)
                    {
                        // Create a new texture using the 2nd alpha map
                        const String AlphaMap2texname = String("SplattingAlpha2.") + commonName + filename;
                        TextureManager::getSingleton().loadImage(
                            AlphaMap2texname, PagingLandScapeOptions::getSingleton().groupName, 
                            AlphaMap2, TEX_TYPE_2D, 7, 1.0f);
                    }

                    if (!bAlpha3NotUsed)
                    {  
                        // Create a new texture using the 3rd alpha map
                        const String AlphaMap3texname = String("SplattingAlpha3.") + commonName + filename;
                        TextureManager::getSingleton().loadImage(
                            AlphaMap3texname, PagingLandScapeOptions::getSingleton().groupName, 
                            AlphaMap3, TEX_TYPE_2D, 7, 1.0f);
                    }

                    if (!bAlpha4NotUsed)
                    { 
                        // Create a new texture using the 4th alpha map
                        const String AlphaMap4texname = String("SplattingAlpha4.") + commonName + filename;
                        TextureManager::getSingleton().loadImage(
                            AlphaMap4texname, PagingLandScapeOptions::getSingleton().groupName, 
                            AlphaMap4, TEX_TYPE_2D, 7, 1.0f);
                    }
                }

                uint pass_number = 1;
                if (bAlpha1NotUsed)
                {
                    mMaterial->getTechnique(0)->removePass (pass_number);
                }
                else
                {
                    // Create a new texture using the 1st alpha map
                    const String AlphaMap1texname = String("SplattingAlpha1.") + commonName + filename;
                    // assign this texture to the material
                    mMaterial->getTechnique(0)->getPass(1)->getTextureUnitState(1)->setTextureName(AlphaMap1texname);
                    pass_number++;
                }

                if (bAlpha2NotUsed)
                {
                    mMaterial->getTechnique(0)->removePass (pass_number);
                }
                else
                {
                    // Create a new texture using the 2nd alpha map
                    const String _AlphaMap2texname = String("SplattingAlpha2.") + commonName + filename;
                    // assign this texture to the material
                    mMaterial->getTechnique(0)->getPass(pass_number)->getTextureUnitState(1)->setTextureName(_AlphaMap2texname);
                    pass_number++;
                }

                if (bAlpha3NotUsed)
                {  
                    mMaterial->getTechnique(0)->removePass (pass_number);   
                }
                else
                {
                    // Create a new texture using the 3rd alpha map
                    const String _AlphaMap3texname = String("SplattingAlpha3.") + commonName + filename;
                    // assign this texture to the material
                    mMaterial->getTechnique(0)->getPass(pass_number)->getTextureUnitState(1)->setTextureName(_AlphaMap3texname);
                    pass_number++;
                }

                if (bAlpha4NotUsed)
                {
                    mMaterial->getTechnique(0)->removePass (pass_number); 
                }
                else
                {
                    // Create a new texture using the 4th alpha map
                    const String _AlphaMap4texname = String("SplattingAlpha4.") + commonName + filename;
                    // assign this texture to the material
                    mMaterial->getTechnique(0)->getPass(pass_number)->getTextureUnitState(1)->setTextureName(_AlphaMap4texname);
                    pass_number++;
                }
                // Now that we have all the resources in place, we load the material
                mMaterial->load(); 
                mMaterial->setLightingEnabled( PagingLandScapeOptions::getSingleton().lit );
            }
	    }
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting::_BuildPoint(ColourValue& out,
                                                    const int j, 
                                                    Real& alpha1, 
                                                    Real& alpha2, 
                                                    Real& alpha3, 
                                                    Real& alpha4)
    {
        const PagingLandScapeData2DManager * const dataPageManager = PagingLandScapeData2DManager::getSingletonPtr();
        const PagingLandScapeOptions *  const  options = PagingLandScapeOptions::getSingletonPtr();
        
        const uint pageSize = options->PageSize - 1;
        	
	    // Calculate the current Point position
	    const int x = j % pageSize;
	    const int z = j / pageSize;

	    // Ask for the current height value and the 8 surrounding values
	    Real height[9];

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

	    //The slope test
	    Real sloppy[8];
	    const Real dx = options->scale.x;
	    const Real dz = options->scale.z;
        const Real inv_dxdz = 1 / (dx+dz);
        const Real inv_dx = 1 / dx;
        const Real inv_dz = 1 / dz;
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
       
        for ( uint i = 0; i < 7; i++ )
        {    
            if ( height[i] < matHeight0 )
            {
                if ( sloppy[i] < 0.2f )
                {
                    // grass-grass
                    _InterpolateColour( out, 1.0f, GRASS, GRASS );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, GRASS, GRASS );
                }
                if ( sloppy[i] >= 0.15f && sloppy[i] < 0.4f )
                {
                    // sand-grass
                    _InterpolateColour( out, 0.25f, SAND, GRASS );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.25f, SAND, GRASS );
                }
                if ( sloppy[i] >= 0.3f && sloppy[i] < 0.65f )
                {
                    // sand-sand
                    _InterpolateColour( out, 1.0f, SAND, SAND );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, SAND, SAND );
                }
                if ( sloppy[i] >= 0.55f && sloppy[i] < 0.75f )
                {
                    // sand-rock
                    _InterpolateColour( out, 0.75f, SAND, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, SAND, ROCK );
                }
                if ( sloppy[i] >= 0.70f )
                {
                    // rock-rock
                    _InterpolateColour( out, 1.0, ROCK, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, ROCK, ROCK );
                }
            }
            else if ( height[i] < matHeight1 )
            {
                if ( sloppy[i] < 0.15f )
                {
                    // grass-snow
                    _InterpolateColour( out, 0.25f, GRASS, SNOW );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.25f, GRASS, SNOW );
                }
                if ( sloppy[i] >= 0.10f && sloppy[i] < 0.45f )
                {
                    // snow-sand
                    _InterpolateColour( out, 0.65f, SNOW, SAND );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.65f, SNOW, SAND );
                }
                if ( sloppy[i] >= 0.25f && sloppy[i] < 0.65f )
                {
                    // snow-rock
                    _InterpolateColour( out, 0.5, SNOW, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.5f, SNOW, ROCK );
                }
                if ( sloppy[i] >= 0.50f && sloppy[i] < 0.75f )
                {
                    // snow-rock
                    _InterpolateColour( out, 0.75f, SNOW, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, SNOW, ROCK );
                }
                if ( sloppy[i] >= 0.7f )
                {
                    // rock-rock
                    _InterpolateColour( out, 1.0f, ROCK, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, ROCK, ROCK );
                }
            }
            else
            {
                if ( sloppy[i] < 0.15f )
                {
                    // snow-snow
                    _InterpolateColour( out, 1.0f, SNOW, SNOW );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, 0, 0 );
                }
                if ( sloppy[i] >= 0.1f && sloppy[i] < 0.45f )
                {
                    // snow-sand
                    _InterpolateColour( out, 0.35f, SNOW, SAND );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.35f, 0, 1 );
                }
                if ( sloppy[i] >= 0.25f && sloppy[i] < 0.65f )
                {
                    // snow-rock
                    _InterpolateColour( out, 0.5f, SNOW, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.5f, 0, 3 );
                }
                if ( sloppy[i] >= 0.5f && sloppy[i] < 0.75f )
                {
                    // snow-rock
                    _InterpolateColour( out, 0.75f, SNOW, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, 0, 3 );
                }
                if ( sloppy[i] >= 0.7f )
                {
                    // rock-rock
                    _InterpolateColour( out, 1.0f, ROCK, ROCK );
                    _InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, 3, 3 );
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting::_InterpolateColour(ColourValue& out, const Real percentaje, const int index1, const int index2)
    {
        const PagingLandScapeOptions *  const  options = PagingLandScapeOptions::getSingletonPtr();
        
	    const ColourValue color1 = options->matColor[index1];
	    if (percentaje < 1.0)
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
    void PagingLandScapeTexture_Splatting::_InterpolateAlpha(Real& alpha1, Real& alpha2, Real& alpha3, Real& alpha4, const Real percentaje, const int index1, const int index2)
    {
	    if ( index1 == index2 )
	    {
		    if ( index1 == 0 )
		    {
			    alpha1 += 1;
		    }
		    else if ( index1 == 1 )
		    {
			    alpha2 += 1;
		    }
		    else if ( index1 == 2 )
		    {
			    alpha3 += 1;
		    }
		    else
		    {
			    alpha4 += 1;
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
			    alpha1 += 1 - percentaje;
		    }
		    else if ( index1 == 1 )
		    {
			    alpha2 += 1 - percentaje;
		    }
		    else if ( index1 == 2 )
		    {
			    alpha3 += 1 - percentaje;
		    }
		    else
		    {
			    alpha4 += 1 - percentaje;
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
	    const Real total = 1 / (( alpha1 + alpha2 + alpha3 + alpha4 ) * 2);
	    alpha1 = alpha1 * total;
	    alpha2 = alpha2 * total;
	    alpha3 = alpha3 * total;
	    alpha4 = alpha4 * total;
    }

} //namespace
