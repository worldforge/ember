/***************************************************************************
OgrePagingLandScapeTexture_Splatting.cpp  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
	email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
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
#include "OgrePagingLandScapeTexture_Splatting.h"
#include "OgrePagingLandScapeData2DManager.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
	void PagingLandScapeTexture_Splatting::setOptions(void)
    {
        mParent->getOptions()->VertexCompression = false;
        mParent->getOptions()->lodMorph = false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting::newTexture()
    {
        return new PagingLandScapeTexture_Splatting(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting::isMaterialSupported()
    {              
		const PagingLandScapeOptions * const opt = mParent->getOptions();
            
		if (opt->NumMatHeightSplat < 3)
			return false;
        if (opt->numTextureUnits < 2)
            return false;

        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting::PagingLandScapeTexture_Splatting(PagingLandScapeTextureManager *textureMgr) 
		: 
		PagingLandScapeTexture(textureMgr, "Splatting", 4, true)
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting::~PagingLandScapeTexture_Splatting()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {
			const PagingLandScapeOptions * const opt = mParent->getOptions();
            const String filename (opt->LandScape_filename);
            const String commonName (StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX));
            const String matname (String("Splatting.") + commonName + filename);
		    mMaterial = MaterialManager::getSingleton().getByName(matname);

            if (mMaterial.isNull())
	        {
		        mMaterial = MaterialManager::getSingleton().getByName("Splatting");
                assert (!mMaterial.isNull());
		        // Create a new texture using the base image
		        mMaterial = mMaterial->clone(matname);

		        const String Basetexname (String("Splatting.") + commonName + filename);
		        TextureManager * const texMgr = TextureManager::getSingletonPtr();
                TexturePtr tex = texMgr->getByName (Basetexname);
                if (tex.isNull())
	            {
                    const unsigned int pageSize = opt->PageSize;
                    const unsigned int size = pageSize * pageSize;
                    const unsigned int pageMemorySize = size * 4;

                    uchar * const ogre_restrict BaseData = new uchar [pageMemorySize];
                                     
                    unsigned int alpha_pass = 0;
                    
                    const unsigned int numSplats = opt->NumMatHeightSplat;
                    
		            std::vector <uchar * ogre_restrict> alphaData;
		            alphaData.reserve(numSplats);
		            alphaData.resize(numSplats);        
                    alpha_pass = 0; 
                    while  (alpha_pass < numSplats)
                    { 
                       alphaData[alpha_pass] = new uchar [size]; 
                       alpha_pass++;
                    }
                    
                    std::vector <bool> bAlphaNotUsed;
		            bAlphaNotUsed.reserve(numSplats);
		            bAlphaNotUsed.resize(numSplats); 
                    alpha_pass = 0; 
                    while  (alpha_pass < numSplats)
                    {                      
		                bAlphaNotUsed[alpha_pass] = true;
		                alpha_pass++;
                    } 
                           
                    std::vector <Real> alpha;
		            alpha.reserve(numSplats);
		            alpha.resize(numSplats); 
                    
		            ColourValue color;
                    const uchar maxuchar = 255;
                    unsigned int posRGB = 0;  
                    unsigned int posG = 0;
		            for (unsigned int i = 0; i < pageSize; ++i)
		            {                             
		                for (unsigned int j = 0; j < pageSize; ++j)
		                {            
                            const unsigned int c_posRGB = posRGB;
                            posRGB  += 4;
                               
			                // Generate the base texture
			                _BuildPoint(i, j, color, alpha);
    			            
			                // R G B A is the format to add the colors
			                BaseData[c_posRGB+0] = static_cast <uchar> (color.r * maxuchar);
			                BaseData[c_posRGB+1] = static_cast <uchar> (color.g * maxuchar);
			                BaseData[c_posRGB+2] = static_cast <uchar> (color.b * maxuchar);
			                BaseData[c_posRGB+3] = static_cast <uchar> ((1 - (alpha[0] + alpha[1] + alpha[2] + alpha[3])) * maxuchar); // Opaque
                                    
                            alpha_pass = 0; 
                            const unsigned int c_posG = posG;   
                            posG += 1;
                            while  (alpha_pass < numSplats)
                            {  
			                    // Generate the alpha map
			                    if (!bAlphaNotUsed[alpha_pass]) 
			                    {
			                        alphaData[alpha_pass][c_posG] = static_cast <uchar> (alpha[alpha_pass] * maxuchar);
			                    } 
			                    else if (alpha[alpha_pass] - 0.05f > 0.0f)
                                {
                                   bAlphaNotUsed[alpha_pass]  = false;    
			                       alphaData[alpha_pass][c_posG] = static_cast <uchar> (alpha[alpha_pass] * maxuchar);
			                    }                        
                                alpha_pass++;
                            }    
		                }
		            }

		            // Assign the texture to the base map                    
		            // This texture will be used as a base color for the terrain, 
                    // it will fake the splat for distant renderables.
		            Image tmpImage;
					tmpImage.loadDynamicImage (BaseData, 
												pageSize, 
												pageSize, 
												1, 
												PF_R8G8B8A8, 
												true);
					tmpImage.resize(pageSize - 1, pageSize - 1);						
		            texMgr->loadImage (Basetexname, 
										opt->groupName, 
										tmpImage, TEX_TYPE_2D);

		            // assign this texture to the material
		            
		            // distant technique    
		            mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(Basetexname);
                    
                    // near technique
                    Technique * const t = mMaterial->getTechnique(0);
                    t->getPass(0)->getTextureUnitState(0)->setTextureName(Basetexname);
                      
		            const String endName ("." + commonName + filename);  
		            const String beginName("SplattingAlpha");   
                    alpha_pass = 0;       
                    unsigned int splat_pass = 0;  // if we remove a pass, splat still increases
                    while  (splat_pass < numSplats)
                    {  
                        if (!bAlphaNotUsed[splat_pass])
                        {           
                            Image tmpImage2;  
                            tmpImage2.loadDynamicImage (alphaData[splat_pass], 
													pageSize, 
													pageSize, 
													1, 
													PF_A8, 
													true);  
					        tmpImage2.resize(pageSize - 1, pageSize - 1);
					
                            // Create a new texture using the 1st alpha map
                            const String texName =  beginName + StringConverter::toString(alpha_pass + 1) + endName;
                            texMgr->loadImage(texName, 
                                opt->groupName, 
                                tmpImage2, 
                                TEX_TYPE_2D);// Generate the alpha map
                                
                                
                            // Create a new texture using the 1st alpha map
                            // assign this texture to the material
                            Pass * const p = t->getPass(alpha_pass + 1);   // base TEXture is first pass.	
                            
                            p->getTextureUnitState(0)->setTextureName(opt->SplatDetailMapNames[splat_pass]);
                            
                            p->getTextureUnitState(1)->setTextureName(texName); 		  
                            alpha_pass++;
                            
                        }
						else
						{
						    t->removePass (alpha_pass + 1);   
						    delete [] alphaData[splat_pass]; 
						} 			               	
						splat_pass++;
                    }  					  
                }
					
                
                
                
                 
            }
	    }
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting::_BuildPoint( const unsigned int x, const int z,
                                                    ColourValue& out, std::vector<Real> &alpha)
    {
        PagingLandScapeData2DManager * const dataPageManager = mParent->getSceneManager()->getData2DManager();
                	
	    // Ask for the current height value and the 8 surrounding values
	                       
		const Real currHeight = dataPageManager->getHeightAtPage(mDataX, mDataZ, x, z);
		const Real height[] =
		{
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x - 1, z - 1),		// Top-Left
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x, z - 1),			// Top-Center
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x + 1, z - 1),		// Top-Right
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x - 1, z),			// Left
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x, z),				// Current Point
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x + 1, z),			// Right
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x - 1, z + 1),		// Botton-Left
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x, z + 1),			// Botton-Center
			dataPageManager->getHeightAtPage(mDataX, mDataZ, x + 1, z + 1)		// Botton-Right
		};
	    // Weight(pt1 , pt2) = 1 - DistanceSquared(pt1,pt2) / (1.75)^2
             
        const PagingLandScapeOptions *  const  options = mParent->getOptions();
        
	    //The slope test
	    const Real dx = options->scale.x;
	    const Real dz = options->scale.z;
        const Real inv_dxdz = 1 / (dx+dz);
        const Real inv_dx = 1 / dx;
        const Real inv_dz = 1 / dz;

		const Real sloppy[] =
		{
			Math::Abs ( height[0] - currHeight ) * inv_dxdz,
			Math::Abs ( height[1] - currHeight ) * inv_dz,
			Math::Abs ( height[2] - currHeight ) * inv_dxdz,
			Math::Abs ( height[3] - currHeight ) * inv_dx, 
			0.0f,
			Math::Abs ( height[5] - currHeight ) * inv_dx,
			Math::Abs ( height[6] - currHeight ) * inv_dxdz,
			Math::Abs ( height[7] - currHeight ) * inv_dz,
			Math::Abs ( height[8] - currHeight ) * inv_dxdz
		};

	    // Init the colour    
	    out = ColourValue(0.0f, 0.0f, 0.0f, 0.0f);

	    alpha[0] = 0.0f;
	    alpha[1] = 0.0f;
	    alpha[2] = 0.0f;
	    alpha[3] = 0.0f;
           
        const Real matHeight0 = options->matHeight[1];
        const Real matHeight1 = options->matHeight[2];
                          
        #define   _MixColour(out, color)  ((out + color) * 0.5f)    
        #define   _InterpolateColour(F, color1, color2)  (F * options->matColor[color1] + (1.0f - F) * options->matColor[color2])
	         
        for (unsigned int i = 0; i < 9; i++)
        {        
		    const Real h = height[i];
		    const Real slope = sloppy[i];
            if (h < matHeight0)
            {
                if (slope < 0.2f)
                {
                    // grass-grass
                    out = _MixColour (out, options->matColor[GRASS]);
                    _InterpolateAlpha(alpha, 1.0f, GRASS, GRASS);
                }
                if (slope > 0.15f && slope < 0.4f)
                {
                    // sand-grass
                    out = _MixColour (out, _InterpolateColour (0.25f, SAND, GRASS));
                    _InterpolateAlpha(alpha, 0.25f, SAND, GRASS);
                }
                if (slope > 0.3f && slope < 0.65f)
                {
                    // sand-sand
                    out = _MixColour (out, options->matColor[SAND]);
                    _InterpolateAlpha(alpha, 1.0f, SAND, SAND);
                }
                if (slope > 0.55f && slope < 0.75f)
                {
                    // sand-rock
                    out = _MixColour(out, _InterpolateColour(0.75f, SAND, ROCK));
                    _InterpolateAlpha(alpha, 0.75f, SAND, ROCK);
                }
                if (slope > 0.70f)
                {
                    // rock-rock
                    out = _MixColour (out, options->matColor[ROCK]);
                    _InterpolateAlpha(alpha, 1.0f, ROCK, ROCK);
                }
            }
            else if (h < matHeight1)
            {
                if (slope < 0.15f)
                {
                    // grass-snow
                    out = _MixColour(out, _InterpolateColour(0.25f, GRASS, SNOW));
                    _InterpolateAlpha(alpha, 0.25f, GRASS, SNOW);
                }
                if (slope > 0.10f && slope < 0.45f)
                {
                    // snow-sand
                    out = _MixColour(out, _InterpolateColour(0.65f, SNOW, SAND));
                    _InterpolateAlpha(alpha, 0.65f, SNOW, SAND);
                }
                if (slope > 0.25f && slope < 0.65f)
                {
                    // snow-rock
                    out = _MixColour(out, _InterpolateColour(0.5f, SNOW, ROCK));
                    _InterpolateAlpha(alpha, 0.5f, SNOW, ROCK);
                }
                if (slope > 0.50f && slope < 0.75f)
                {
                    // snow-rock
                    out = _MixColour(out, _InterpolateColour(0.75f, SNOW, ROCK));
                    _InterpolateAlpha(alpha,  0.75f, SNOW, ROCK);
                }
                if (slope > 0.7f)
                {
                    // rock-rock
                    out = _MixColour (out, options->matColor[ROCK]);
                    _InterpolateAlpha(alpha, 1.0f, ROCK, ROCK);
                }
            }
            else
            {
                if (slope < 0.15f)
                {
                    // snow-snow  
                    out = _MixColour (out, options->matColor[SNOW]);
                    _InterpolateAlpha(alpha,   1.0f, SNOW, SNOW);
                }
                if (slope > 0.1f && slope < 0.45f)
                {
                    // snow-sand
                    out = _MixColour(out, _InterpolateColour(0.35f, SNOW, SAND));
                    _InterpolateAlpha(alpha,  0.35f, SNOW, SAND);
                }
                if (slope > 0.25f && slope < 0.65f)
                {
                    // snow-rock
                    out = _MixColour(out, _InterpolateColour(0.5f, SNOW, ROCK));
                    _InterpolateAlpha(alpha, 0.5f, SNOW, ROCK);
                }
                if (slope > 0.5f && slope < 0.75f)
                {
                    // snow-rock
                    out = _MixColour(out, _InterpolateColour(0.75f, SNOW, ROCK));
                    _InterpolateAlpha(alpha,  0.75f, SNOW, ROCK);
                }
                if (slope > 0.7f)
                {
                    // rock-rock
                    out = _MixColour (out, options->matColor[ROCK]);
                    _InterpolateAlpha(alpha, 1.0f, ROCK, ROCK);
                }
            }
        }
        #undef _InterpolateColour  
        #undef _MixColour
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting::_InterpolateAlpha(std::vector<Real> &alpha, 
                                                            const Real percentaje, 
                                                            const int index1, 
                                                            const int index2)
    {
	    if (index1 == index2)
	    {
			alpha[index1] += 1;
	    }
	    else
	    {
			alpha[index2] += percentaje; 
			alpha[index1] += 1 - percentaje;
	    }
	    
        const Real oneonfive = 0.2f;
        
	    alpha[0] *= (4 * oneonfive);// 4/5
	    alpha[1] *= (3 * oneonfive);// 3/5
	    alpha[2] *= (4 * oneonfive);// 4/5
	    alpha[3] *= (2 * oneonfive);// 2/5
	    
	    // Normalize
	    const Real total = 1 / ((alpha[0] + alpha[1] + alpha[2] + alpha[3]) * 2);
	    
	    alpha[0] = alpha[0] * total;
	    alpha[1] = alpha[1] * total;
	    alpha[2] = alpha[2] * total;
	    alpha[3] = alpha[3] * total;
    }                                   

} //namespace
