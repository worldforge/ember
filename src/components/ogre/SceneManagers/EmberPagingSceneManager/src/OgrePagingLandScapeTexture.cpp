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

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreStringConverter.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeTexture.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    PagingLandScapeTexture::PagingLandScapeTexture(PagingLandScapeTextureManager *textureMgr, 
													const String materialBaseName,
										   		    const unsigned int numTexture,
													const bool isSplatMode) :
        mParent(textureMgr),
		mMaterialBaseName(materialBaseName),
	    mIsLoaded (false),
        mIsModified (false),
	    mDataX (0),
	    mDataZ (0),
        mPaintRect (0, 0, 0, 0, 0, 1),
        mIsPaintRectModified(false),
        mDeformRect (0, 0, 0, 0, 0, 1),
        mIsDeformRectModified(false),
        mIsSplatMode(isSplatMode),
		mNumTexture(numTexture),
		mIsShadowed(false),
		mIsShaderShadowed(false),
		mIsBaseMode(false)
    {
        mMaterial.setNull();
    }
    //-----------------------------------------------------------------------
	PagingLandScapeTexture::~PagingLandScapeTexture()
	{
		mNumChannelperTexture.clear();			
		mImages.clear();
		mTextures.clear();
		mBuffers.clear();
		doTextureNeedUpdate.clear();
		isTextureModified.clear();
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::setNumTexture()
	{
		if (mNumTexture > 0)
		{
			mNumChannelperTexture.reserve(mNumTexture);	
			doTextureNeedUpdate.reserve(mNumTexture);
			isTextureModified.reserve(mNumTexture);	
			mTextures.reserve(mNumTexture);
			mBuffers.reserve(mNumTexture);

			mImages.reserve(mNumTexture);

			mNumChannelperTexture.resize(mNumTexture);	
			doTextureNeedUpdate.resize(mNumTexture);
			isTextureModified.resize(mNumTexture);
			mTextures.resize(mNumTexture);
			mBuffers.resize(mNumTexture);		

			mImages.resize(mNumTexture);

			for (size_t i = 0; i < mNumTexture; i++)
			{
				mImages[i].loadDynamicImage (0, 0, 0, 1, PF_BYTE_RGB, true, 1, 0);
			}

		}
	}
    //-----------------------------------------------------------------------
    const String &PagingLandScapeTexture::getMaterialName()
    {
        return mMaterial->getName ();
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
					GpuProgramParameters::RealConstantEntry * const e = params->getNamedRealConstantEntry ("splatSettings");
					if (e)
					{
						e->val[0] = static_cast <float> (opt->matHeight[1]);
						e->val[1] = static_cast <float> (opt->matHeight[2]); 
						e->val[2] = static_cast <float> (opt->maxValue); 
						e->val[3] = static_cast <float> (0.0);
						e->isSet = true;
					}
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
	void PagingLandScapeTexture::loadTexturesToModify()
	{
		if (mNumTexture > 0 || (mIsShadowed && !mIsShaderShadowed))
		{
			const String nameSep(".");
			const PagingLandScapeOptions * const opt = mParent->getOptions();
			const String filename (opt->LandScape_filename);
			const String commonName (StringConverter::toString(mDataZ) + 
						nameSep + StringConverter::toString(mDataX));

			unsigned int channel = 0;
			TextureManager* texMgr = TextureManager::getSingletonPtr();

			const unsigned short numLodLevels = 
				mMaterial->getNumLodLevels(MaterialManager::getSingleton()._getActiveSchemeIndex());
			// some texture are shared between techniques, ensure it's loaded once.
			bool imageLoaded = false;
			bool baseLoaded = false;
			bool coverageLoaded = false;
			bool lightLoaded = false;
			bool horizonLoaded = false;

			for(unsigned short k = 0; k < numLodLevels; k++)
			{
				Technique *t = mMaterial->getBestTechnique (k);
				Technique::PassIterator pIt = t->getPassIterator ();
				String texType;
				while (pIt.hasMoreElements ())
				{
					channel = 0;
					Pass *p = pIt.getNext ();
					Pass::TextureUnitStateIterator tuIt = p->getTextureUnitStateIterator ();
					while (tuIt.hasMoreElements ())
					{
						TextureUnitState *tu = tuIt.getNext ();

						const String texName (tu->getTextureName());
						if (!imageLoaded && opt->ImageNameLoad && 
							StringUtil::startsWith (texName, opt->image_filename))
						{
							// if it's an Image Texture mode
							mTextures[channel] = texMgr->getByName (texName);
							assert (!mTextures[channel].isNull() && 
								String(texName + " is missing").c_str());
							mBuffers[channel] = mTextures[channel]->getBuffer();
							loadColorTexture (texName, channel);
							channel++;
							imageLoaded = true;
						}
						else if (StringUtil::startsWith (texName, filename, false))
						{
							// if it's a dynamic texture updated by texture mode
							// subtracts filename from texname 
							String texNameInfo = texName.substr (filename.size ());
							//then split on Dot (in case of filename with dot in it.)
							const StringVector texNameInfos = 
								StringUtil::split(texNameInfo, nameSep);
							texType = texNameInfos[0];

							if (texType == "Alpha")
							{
								mTextures[channel] = texMgr->getByName (texName);
								assert (!mTextures[channel].isNull() && 
									String(texName + " is missing").c_str());
								mBuffers[channel] = mTextures[channel]->getBuffer();
								loadAlphaTexture (texName, channel);
								channel++;
							}
							else if (texType == "Coverage")
							{
								mTextures[channel] = texMgr->getByName (texName);
								assert (!mTextures[channel].isNull() && 
									String(texName + " is missing").c_str());
								mBuffers[channel] = mTextures[channel]->getBuffer();
								loadColorTexture (texName, channel);
								coverageLoaded = true;
								channel++;
							}
							else if (!lightLoaded && !mIsShaderShadowed 
								&& texType ==  "Light" )
							{
								mLightTexture = texMgr->getByName (texName);
								assert (!mLightTexture.isNull() && 
									String(texName + " is missing").c_str());
								mLightBuffer = mLightTexture->getBuffer();
								loadTexture (texName, mLightImage);

								assert (mLightBuffer->getWidth () ==  mLightImage.getWidth () && 
									mLightBuffer->getHeight () == mLightImage.getHeight ());

								String shadowTexName (filename + nameSep + "HS");
								for (size_t k = 1; k < texNameInfos.size(); k++)
									shadowTexName +=  nameSep + texNameInfos[k];
								if (ResourceGroupManager::getSingleton().
									resourceExists (opt->groupName, shadowTexName))
								{
									loadTexture (shadowTexName, mShadow);
									assert (mShadow.getWidth () ==  mLightImage.getWidth () && 
										mShadow.getHeight () == mLightImage.getHeight ());
									lightUpdate ();     
								}
								else
								{
									mIsShadowed = false;
								}
								lightLoaded = true;
							}
							else if (!coverageLoaded && !baseLoaded && texType == "Base")
							{
								mTextures[channel] = texMgr->getByName (texName);
								assert (!mTextures[channel].isNull() && 
									String(texName + " is missing").c_str());
								mBuffers[channel] = mTextures[channel]->getBuffer();
								loadColorTexture (texName, channel);
								channel++;
								baseLoaded = true;
							}
						}
					}
				}
				for (unsigned int i = 0; i < mNumTexture; i++)
				{
					mNumChannelperTexture[i] = mImages[i].getBPP () / 8;
					doTextureNeedUpdate[i] = false;
					isTextureModified[i] = false;
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::setOptions(void)
	{
		PagingLandScapeOptions * const opt  = mParent->getOptions();
		String matClassName ( 
			(opt->VertexCompression ?
			String(mMaterialBaseName + "Decompress")
			:
			mMaterialBaseName));

		MaterialPtr material = MaterialManager::getSingleton().getByName (matClassName);
		if (opt->VertexCompression && material.isNull())
		{
			matClassName = mMaterialBaseName;
			opt->VertexCompression = false;
			opt->lodMorph = false;
			material = MaterialManager::getSingleton().getByName (matClassName);
		}
		assert (!material.isNull() && 
			String(matClassName + "Must exists in the" + opt->groupName + "group").c_str ());
		
		bool hasVertexProgram = false;
		bool hasFragmentProgram = false;

		Material::TechniqueIterator tIt = material->getTechniqueIterator ();
		while (tIt.hasMoreElements ())
		{
			Technique * const t = tIt.getNext ();
			Technique::PassIterator pIt = t->getPassIterator ();
			while (pIt.hasMoreElements ())
			{
				Pass * const p = pIt.getNext ();

				// vertex shaders.
				if ( p->hasVertexProgram ())
				{
					if (hasVertexProgram == false)
					{
						hasVertexProgram = p->hasVertexProgram ();
					}
					GpuProgramParametersSharedPtr params = p->getVertexProgramParameters();	
					GpuProgramParameters::RealConstantEntry * const e = params->getNamedRealConstantEntry ("splatSettings");
					if (e)
					{
						opt->normals = true;
					}			
				}		

				// pixel shaders.
				if (hasFragmentProgram == false)
				{
					hasFragmentProgram = p->hasFragmentProgram ();	
				}
			}
		}
		if (!hasVertexProgram) 
		{
			opt->VertexCompression = false;
			opt->lodMorph = false;
		}
	}
	//-----------------------------------------------------------------------
    bool PagingLandScapeTexture::isMaterialSupported(bool recursive)
    {
		const PagingLandScapeOptions * const opt = mParent->getOptions();

		if (opt->VertexCompression && recursive)
		{
			const String MatClassName (mMaterialBaseName);
			mMaterialBaseName =  mMaterialBaseName + "Decompress";			
			const bool isOk = MaterialManager::getSingleton ().resourceExists (mMaterialBaseName) && 
					isMaterialSupported(false);
			mMaterialBaseName = MatClassName;
			if (isOk)
				return true;
		}

		MaterialPtr material = MaterialManager::getSingleton().getByName (mMaterialBaseName);
		assert (!material.isNull() && 
					String(mMaterialBaseName + " Must exists in the" + opt->groupName + "group").c_str ());

		unsigned short numPasses = 0;

		//these will store the maximum number of texture units, alpha textures, 
		//coverage textures, etc. after iterating through all passes in the material
		size_t numTextureUnits = 0;
		size_t numAlphaTexture = 0;
		size_t numCoverageTexture = 0;
		size_t numSplats = 0;
		unsigned int numDynamicTexture = 0;

		//per pass count
		size_t passNumTextureUnits = 0;
		size_t passNumAlphaTextures = 0;
		size_t passNumCoverageTextures = 0;
		size_t passNumSplats = 0;
		unsigned int passNumDynamicTextures = 0;

		bool needVertexProgram = false;
		bool needFragmentProgram = false;
		bool isImageMode = false;
		bool isSplatMode = false;
		bool isBaseMode = false;
		
		Material::TechniqueIterator tIt = material->getTechniqueIterator ();
		while (tIt.hasMoreElements ())
		{
			Technique * const t = tIt.getNext ();
			numPasses = std::max (numPasses, t->getNumPasses());
			Technique::PassIterator pIt = t->getPassIterator ();
			while (pIt.hasMoreElements ())
			{
				passNumTextureUnits = 0;
				passNumAlphaTextures = 0;
				passNumCoverageTextures = 0;
				passNumSplats = 0;
				passNumDynamicTextures = 0;

				Pass * const p = pIt.getNext ();
				if (needVertexProgram == false)
					needVertexProgram = p->hasVertexProgram ();
				if (needFragmentProgram == false)
					needFragmentProgram = p->hasFragmentProgram ();				
				numTextureUnits = std::max (numTextureUnits, p->getNumTextureUnitStates());
				Pass::TextureUnitStateIterator tuIt = p->getTextureUnitStateIterator ();
				while (tuIt.hasMoreElements ())
				{
					TextureUnitState * const tu = tuIt.getNext ();
					const String texType (tu->getTextureName());
					if (std::string::npos == texType.find("."))
					{
						// This Texture Name is A keyword,
						// check how many are dynamic in this material
						if (!isBaseMode && texType == "Base")
						{
							isBaseMode = true;
							passNumDynamicTextures++;
						}
						else if (!isImageMode && texType == "Image")
						{
							isImageMode = true;
							passNumDynamicTextures++;
						}
						else if (texType == "Alpha")
						{
							isSplatMode = true;
							passNumAlphaTextures++;
							passNumDynamicTextures++;
						}
						else if (texType == "Coverage")
						{
							isSplatMode = true;
							passNumDynamicTextures++;
							passNumCoverageTextures++;
						}
						else if (texType == "Splatting")
						{
							mIsSplatMode = true;
							passNumSplats++;
						}
						else if (texType == "Light")
						{
							//dynamic light... but in software
							mIsShadowed = true;
						}
						else if (texType == "Horizon")
						{
							//dynamic light...  but shader
							mIsShaderShadowed = true;
							mIsShadowed = true;
						}
					}
				}
				
				if(passNumTextureUnits > numTextureUnits)
					numTextureUnits = passNumTextureUnits;

				if(passNumAlphaTextures > numAlphaTexture)
					numAlphaTexture = passNumAlphaTextures;

				if(passNumCoverageTextures > numCoverageTexture)
					numCoverageTexture = passNumCoverageTextures;

				if(passNumSplats > numSplats)
					numSplats = passNumSplats;

				if(passNumDynamicTextures > numDynamicTexture)
					numDynamicTexture = passNumDynamicTextures;

			}
		}
		if (isImageMode && !opt->ImageNameLoad)
			return false;
		if (opt->numTextureUnits < numTextureUnits)
			return false;        
		if (needVertexProgram && opt->hasVertexShader == false)
			return false;
		if (needFragmentProgram && opt->hasFragmentShader == false)
			return false;

		if (isSplatMode && numAlphaTexture && opt->NumMatHeightSplat < numAlphaTexture)
			return false;

		// does all coverage must be 4 alpha ?
		//if (isSplatMode && numCoverageTexture && opt->NumMatHeightSplat < numCoverageTexture * 4)
		//	return false;
		if (isSplatMode && numCoverageTexture && opt->NumMatHeightSplat < 4)
			return false;

		if (mIsShaderShadowed && !opt->hasFragmentShader2)
			return false;

		mIsSplatMode = isSplatMode;
		mIsBaseMode = !mIsSplatMode && isBaseMode;
		mNumTexture = (mParent->getOptions()->textureModifiable)? numDynamicTexture : 0;
		return true;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::_loadMaterial()
	{
		if (mMaterial.isNull())
		{
			const PagingLandScapeOptions * const opt = mParent->getOptions();
			const String nameSep(".");
			const String commonName (StringConverter::toString(mDataZ) + 
				nameSep + StringConverter::toString(mDataX));
			if (opt->materialPerPage)
			{
				// JEFF - all material settings configured through material script
				mMaterial = MaterialManager::getSingleton().getByName(
					mMaterialBaseName + commonName);
			}
			else
			{
				const String filename (opt->LandScape_filename);
				const bool compressed = opt->VertexCompression;
				const String MatClassName ( 
					(compressed ?
								String(mMaterialBaseName + "Decompress")
								:
								mMaterialBaseName));
				const String matname (MatClassName + nameSep 
										+ commonName + nameSep  
										+ filename);
				mMaterial = MaterialManager::getSingleton().getByName(matname);
				if (mMaterial.isNull())
				{					
					mMaterial = MaterialManager::getSingleton().getByName(MatClassName);
					assert (!mMaterial.isNull() && 
							String(MatClassName + "Must exists in the" + opt->groupName + "group").c_str ());
					mMaterial = mMaterial->clone(matname);
					const String extName (opt->TextureExtension);
					const String beginName (filename + nameSep);
					const String endName (nameSep + commonName + 
										  nameSep);
					bool deformable;
					String texName, finalTexName;
					unsigned int channel = 0;
					unsigned int splat = 0;

					unsigned int alphachannel = 0;
					unsigned int coveragechannel = 0;

					Material::TechniqueIterator tIt = mMaterial->getTechniqueIterator ();
					while (tIt.hasMoreElements ())
					{
						Technique * const t = tIt.getNext ();
						Technique::PassIterator pIt = t->getPassIterator ();
						while (pIt.hasMoreElements ())
						{
							splat = 0;
							channel = 0;
							coveragechannel = 0;
							alphachannel = 0;
							Pass * const p = pIt.getNext ();
							Pass::TextureUnitStateIterator tuIt = p->getTextureUnitStateIterator ();
							while (tuIt.hasMoreElements ())
							{
								TextureUnitState * const tu = tuIt.getNext ();
								const String texType (tu->getTextureName());
								if (std::string::npos == texType.find("."))
								{
									// This Texture Name is A keyword,
									// meaning we have to dynamically replace it
									deformable = false;
									// check by what texture to replace keyword
									if (texType == "Image")
									{
										texName = opt->image_filename + endName;
										deformable = true;
									}
									else if (texType == "Splatting")
									{
										texName = opt->SplatDetailMapNames[splat];
										splat++;
									}
									else if (texType == "Base")
									{
										texName = beginName + texType + endName;
										channel++;
										deformable = true;
									}
									else if (texType == "Alpha")
									{
										texName = beginName + texType + nameSep + 
											StringConverter::toString(alphachannel) + endName;
										deformable = true;
										alphachannel++;
										channel++;
									}
									else if (texType == "Coverage")
									{
										texName = beginName + texType + nameSep + 
											StringConverter::toString(coveragechannel) + endName;
										deformable = true;
										channel++;
										coveragechannel++;
									}
									else if (texType ==  "Light")
									{
										texName = beginName +  texType + endName + extName;										
									}
									else if (texType ==  "Horizon")
									{
										texName = beginName +  "HSP" + endName + extName;
										mPositiveShadow = true;                   
									}
									if (deformable)
									{
										if(opt->Deformable &&
											ResourceGroupManager::getSingleton().resourceExists(
													opt->groupName, 
													texName + "modif." + extName))
										{
											finalTexName = texName + "modif." + extName;             
										}
										else
										{
											finalTexName = texName + extName;
										}
									}
									else
									{
										finalTexName = texName;
									}
									tu->setTextureName (finalTexName);
								}
							}
						}
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::_unloadMaterial()
	{


	}
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::load(unsigned int x, unsigned int z)
    {
        if (!mIsLoaded && isMaterialSupported())
		{
	        mDataX = x;
			mDataZ = z;
			updated();
			setNumTexture ();
	        _loadMaterial();
			const PagingLandScapeOptions * const opt = mParent->getOptions();
			mMaterial->setLightingEnabled (opt->lit);
			mMaterial->setLodLevels (opt->lodMaterialDistanceList);
			mMaterial->setReceiveShadows (true);
			// If vertex shader, have to bind parameters
			bindCompressionSettings ();
			mMaterial->load ();
			// load texture in main memory if we want to update it Real-Time
			loadTexturesToModify();

	        mIsLoaded = true;
            mIsModified = false; 
        }
    }
    //-----------------------------------------------------------------------
	void PagingLandScapeTexture::unload()
	{
        if (mIsLoaded)
        {
            if (mIsModified && mParent->getOptions()->saveDeformation)
                _save();

			if (!mMaterial.isNull())
				mMaterial->unload();

			for (unsigned int i = 0; i < mNumTexture; i++)
			{
				doTextureNeedUpdate[i] = false;
				isTextureModified[i] = false;
				mBuffers[i].setNull ();
				mTextures[i].setNull ();
				mImages[i].loadDynamicImage (0, 0, 0, 1, PF_BYTE_RGB, true, 1, 0);
			} 
			// Anyway, they're surely null already, as they're freed by delete page()
			
	        _unloadMaterial();

			const String resourceName (mMaterial->getName ());

            assert (!mMaterial.isNull() && "PagingLandScapeTexture::unload");      
			mMaterial->unload();
			mMaterial.setNull();
			MaterialManager::getSingleton().remove (resourceName);

	        mIsLoaded = false;
        }

	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::_save(void)
	{
		assert (!mMaterial.isNull() && "PagingLandScapeTexture::::_save");  
		
		const PagingLandScapeOptions * const opt = mParent->getOptions();
		if (mNumTexture > 0 && opt->textureModifiable)
		{
			const String extname (opt->TextureExtension);
			for (unsigned int i = 0; i < mNumTexture; i++)
			{
				if (isTextureModified[i])
				{
					String texName = mTextures[i]->getName ();

					FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
													opt->groupName, texName);
					FileInfoList::iterator it = finfo->begin();
					if (it != finfo->end())
					{
						char *olddir = ChangeToDir (const_cast< char * > (((it)->archive->getName()).c_str()));
							//FileSystemArchive::pushDirectory()

						assert (mImages[i].getData ());
						// check if we have to add modif to the name.
						const String baseTexName (texName, 0, texName.size () - extname.size());
						if (!StringUtil::endsWith (baseTexName, "modif."))
						{
							texName = baseTexName + "modif." + extname; 
						}
						mImages[i].save (texName); 

						RetablishDir (olddir);
							//FileSystemArchive::popDirectory();

					} // if (it != finfo->end())
				} // if (doTextureNeedUpdate[i])
			} // for (unsigned int i = 0; i < mNumTexture; i++)       
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::upload(const Image::Box& textureRect)
	{    
		assert (mNumTexture > 0);
		assert (!mMaterial.isNull() && "PagingLandScapeTexture::update()");

		for (unsigned int i = 0; i < mNumTexture; i++)
		{
			if (doTextureNeedUpdate[i])
			{
				assert (!mTextures[i].isNull() && "PagingLandScapeTexture::update()");
				assert (!mBuffers[i].isNull() && "PagingLandScapeTexture::update()");
				assert (mImages[i].getData () != 0 && "PagingLandScapeTexture::update()");

				const PixelBox srcBox = mImages[i].getPixelBox().getSubVolume(textureRect);	
				const PixelBox lock = mBuffers[i]->lock(textureRect, HardwareBuffer::HBL_DISCARD); 
				PixelUtil::bulkPixelConversion(srcBox, lock); 
				mBuffers[i]->unlock();  	

				doTextureNeedUpdate[i] = false;
				isTextureModified[i] = true;

				//#define _Missed_Spot 

				#ifdef _Missed_Spot                
					// debug can help finding missed spot.
					const PixelBox srcSpotBox = mImages[i].getPixelBox();	
					const unsigned int textureSize = (mParent->getOptions()->PageSize - 1) *
						mParent->getOptions()->TextureStretchFactor;
					const Image::Box rect (0, 0, 0, textureSize, textureSize, 1);
					const PixelBox lockSpot = mBuffers[i]->lock (rect ,HardwareBuffer::HBL_DISCARD); 
					PixelUtil::bulkPixelConversion(srcSpotBox, lockSpot); 
					mBuffers[i]->unlock();
				#endif //_Missed_Spot 	
			}
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
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::loadAlphaTexture(const String &filename, const unsigned int channel)
	{	
		assert (mNumTexture > 0);
		if (mImages[channel].getData () == 0)
		{			
			Image Imageloader;
			Imageloader.load(filename, mParent->getOptions()->groupName);
			const size_t size = Imageloader.getSize();
			uchar *data = new uchar [size];
			memcpy (data, Imageloader.getData(), size*sizeof(uchar));
			mImages[channel].loadDynamicImage(data, 
				Imageloader.getWidth(), Imageloader.getHeight(), 
				1,  PF_A8, true, 1, 0);	
		}
		
		assert (mImages[channel].getHeight() == mImages[channel].getWidth());
		assert (mImages[channel].getHeight() / (mParent->getOptions()->PageSize - 1)
			== mParent->getOptions()->TextureStretchFactor && 
			String("(texture size / (pagesize-1)) and texture stretch factor defined in terrain config file doesn't fit.").c_str());
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::loadColorTexture(const String &filename, const unsigned int channel)
	{  
		assert (mNumTexture > 0);		

		loadTexture (filename, mImages[channel]);

		assert (mImages[channel].getHeight() == mImages[channel].getWidth());
		assert (mImages[channel].getHeight() / (mParent->getOptions()->PageSize - 1)
			== mParent->getOptions()->TextureStretchFactor && 
			String("(texture size / (pagesize-1)) and texture stretch factor defined in terrain config file doesn't fit.").c_str());
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::loadTexture(const String &filename, Image &img)
	{  
		if (img.getData () == 0)
		{
			img.load(filename, mParent->getOptions()->groupName);	
		}
	}
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::updated ()
    {
        mPaintRect.left = 0;
        mPaintRect.right = 0;
        mPaintRect.top = 0;
        mPaintRect.bottom = 0;
        mIsPaintRectModified = false;
 
        mDeformRect.left = 0;
        mDeformRect.right = 0;
        mDeformRect.top = 0;
        mDeformRect.bottom = 0;
        mIsDeformRectModified = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::compute(
		PagingLandScapeData2D* data, 
        const Image::Box& dataRect,
        const Image::Box& textureRect)
    {
		assert (mNumTexture > 0);
        const size_t heightfiledsize = mParent->getOptions()->PageSize - 1;
        const Real textureScale = mParent->getOptions()->TextureStretchFactor;
        const unsigned int textureSize = heightfiledsize * textureScale;
        unsigned int curr_image_pos = static_cast <unsigned int>(textureRect.top*textureSize + textureRect.left);
        const unsigned int image_width = static_cast <unsigned int>((textureSize - (textureRect.right - textureRect.left)));
        const Real inv_scale = 1 / textureScale;

        const Real * const ogre_restrict heightData = data->getHeightData ();
        assert (heightData && "PagingLandScapeTexture::compute()");

        for (size_t k = textureRect.top; k < textureRect.bottom; ++k)
        {
            const unsigned int k_terrain = (unsigned int)(k * inv_scale);
            const size_t curr_row = k_terrain * heightfiledsize;
            for (size_t i = textureRect.left; i < textureRect.right; ++i)
            {             
                const unsigned int i_terrain = (unsigned int)(i * inv_scale);

                assert (i < textureSize && k < textureSize && 
                    "PagingLandScapeTexture::compute()");

                assert (i_terrain < heightfiledsize && k_terrain < heightfiledsize && 
                    "PagingLandScapeTexture::compute()");

				if (mIsSplatMode)
					computePointAlpha(curr_image_pos,
                            heightData[i_terrain + curr_row], 
                            1.0f - data->getNormal (i_terrain, k_terrain).y);
				if (mIsBaseMode)
					computePointColor (curr_image_pos,
							heightData[i_terrain + curr_row], 
							1.0f - data->getNormal (i_terrain, k_terrain).y);

                curr_image_pos += 1;
            }
            curr_image_pos += image_width;
        }  
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::paint (const unsigned int xParam, 
										const unsigned int zParam, 
										const Real paintForce)
	{
		assert (mNumTexture > 0);
		assert (paintForce >= 0.0f && paintForce <= 1.0f && "PagingLandScapeTexture::paint()");
		const Real blend = paintForce;
 
        const size_t psize = mParent->getOptions()->PageSize - 1;       
        const Real textureScale = mParent->getOptions ()->TextureStretchFactor;        
        const unsigned int textureScaleCount = static_cast <unsigned int> (textureScale);

        assert (((xParam + zParam *  psize) < psize*psize) && "PagingLandScapeTexture::paint()");

        const unsigned int xScaled = xParam * textureScaleCount;
        const unsigned int zScaled = zParam * textureScaleCount;
        const size_t pSizeScaled = psize * textureScaleCount;

        for (unsigned int j = 0; j < textureScaleCount; j++)
        {
			const unsigned int z = zScaled + j;
			if (z < pSizeScaled)
			{
				const unsigned int zShift = z * pSizeScaled;
				for (unsigned int k = 0; k < textureScaleCount; k++)
				{
					const unsigned int x = xScaled + k;
					if (x < pSizeScaled)
					{
						const unsigned int curr_image_pos = x + zShift;
						assert (curr_image_pos < pSizeScaled*pSizeScaled);
						paintPoint (curr_image_pos, blend);
						adjustPaintRectangle (x, z); 
					}
				}
			}
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::update()
    {       
		assert (mNumTexture > 0);
        // at least deformed once, so need to save texture if asked by user (option)
        mIsModified = true; 

        Image::Box dataRect (0, 0, 0, 0, 0, 1);
        Image::Box texturerect (0, 0, 0, 0, 0, 1);

        // computes deformation
        PagingLandScapeData2D *data = 0;
        if (mIsDeformRectModified)
        {
			dataRect = mDeformRect;
			data = mParent->getSceneManager()->getData2DManager()->getData2D(mDataX, mDataZ);

            if (dataRect.getWidth() && dataRect.getHeight ())
            {

                const Real textureScale = mParent->getOptions()->TextureStretchFactor;

                texturerect.left = dataRect.left * textureScale;
                texturerect.top  = dataRect.top * textureScale;
                texturerect.right = dataRect.right * textureScale + 1;
                texturerect.bottom = dataRect.bottom * textureScale + 1;

                dataRect.right += 1;
                dataRect.bottom += 1;
                compute(data, dataRect, texturerect);
            }
        }
        // try to upload only the smallest rectangle containing modification
        if (mIsPaintRectModified)
        {
            if (mIsDeformRectModified)
            {
                texturerect.left = std::min (mPaintRect.left, dataRect.left);
                texturerect.right = std::max (mPaintRect.right, dataRect.right);
                texturerect.top =  std::min (mPaintRect.top, dataRect.top);
                texturerect.bottom = std::max (mPaintRect.bottom, dataRect.bottom);
            } 
            else
            {
                texturerect = mPaintRect;

                texturerect.right += 1;
                texturerect.bottom += 1;
            }
        } // if (mIsRectModified)


        // Upload any changes (deformation or)
        if (texturerect.getWidth() && texturerect.getHeight ())
        {
            upload (texturerect);           
        } // if (texturerect.getWidth() && texturerect.getHeight ())

        if (mIsDeformRectModified)
            data->resetDeformationRectangle ();
        PagingLandScapeTexture::updated ();
    }
    
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture::needUpdate () const
    {
        return mIsDeformRectModified || mIsPaintRectModified;
    } 
    //-----------------------------------------------------------------------
	void PagingLandScapeTexture::adjustDeformationRectangle(unsigned int x, unsigned int z)
    {
		assert (mNumTexture > 0);
        assert (x < (mParent->getOptions ()->PageSize));
        assert (z < (mParent->getOptions ()->PageSize));

        if (mIsDeformRectModified)
        {
            if (mDeformRect.left > x)
                mDeformRect.left = x;
            if (mDeformRect.right < x)
                mDeformRect.right = x;

            if (mDeformRect.top > z)
                mDeformRect.top = z;
            if (mDeformRect.bottom < z)
                mDeformRect.bottom = z;
        }
        else
        {
            // first modification :
            // deformation rectangle is the point
            mDeformRect.left    = x;
            mDeformRect.right   = x;
            mDeformRect.top     = z;
            mDeformRect.bottom  = z;
            mIsDeformRectModified = true;
        }
    } 
    //-----------------------------------------------------------------------
	void PagingLandScapeTexture::adjustPaintRectangle(unsigned int x, unsigned int z)
    {
		assert (mNumTexture > 0);
        assert (x < ((mParent->getOptions ()->PageSize - 1) * mParent->getOptions ()->TextureStretchFactor));
        assert (z < ((mParent->getOptions ()->PageSize - 1) * mParent->getOptions ()->TextureStretchFactor));
        
        if (mIsPaintRectModified)
        {
            if (mPaintRect.left > x)
                mPaintRect.left = x;
            else if (mPaintRect.right < x)
                mPaintRect.right = x;

            if (mPaintRect.top > z)
                mPaintRect.top = z;
            else if (mPaintRect.bottom < z)
                mPaintRect.bottom = z;
        }
        else
        {
            // first modification :
            // deformation rectangle is the point
            mPaintRect.left = x;
            mPaintRect.right = x;
            mPaintRect.top = z;
            mPaintRect.bottom = z;
            mIsPaintRectModified = true;
        }
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::lightUpdate()
	{		
		PagingLandScapeOptions * const opt = mParent->getOptions();
		if (mIsShaderShadowed)
		{
			const Real SunAngle = opt->SunAngle;
			const Vector3 SunDir = opt->Sun;

			const bool positiveHorizon =  (SunDir.y > 0);// Sun is west (true), east (false);
			assert (fabs (SunAngle) < 1.1f);

			const Real LightAngle =  (positiveHorizon)? SunAngle  : -SunAngle;
			if (positiveHorizon != mPositiveShadow)
			{
				const String texname  (opt->LandScape_filename
					+
					((positiveHorizon)? String(".HSP.") : String(".HSN."))
					+
					StringConverter::toString(mDataZ) + 
					String(".") +
					StringConverter::toString(mDataX) +  "." + 
					opt->TextureExtension); 
				mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTextureName (texname);                
				mPositiveShadow = positiveHorizon;
			}
			GpuProgramParametersSharedPtr params = mMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();   
			GpuProgramParameters::RealConstantEntry * const e = params->getNamedRealConstantEntry ("HorizonSettings");
			assert (e);
			e->val[0] = static_cast <float> (SunDir.x);
			e->val[1] = static_cast <float> (SunDir.y); 
			e->val[2] = static_cast <float> (SunDir.z); 
			e->val[3] = static_cast <float> (LightAngle);
			e->isSet = true;	
		}
		else if (mIsShadowed)
		{
			assert (mLightImage.getData ());
			computeLightMap ();

			// Upload changes         
			const PixelBox srcBox = mLightImage.getPixelBox();	
			const unsigned int mTextureSize = mParent->mPageSize * opt->TextureStretchFactor;
			const Image::Box rect (0, 0, 0, mTextureSize, mTextureSize, 1);
			const PixelBox lock = mLightBuffer->lock (rect , HardwareBuffer::HBL_DISCARD); 
			PixelUtil::bulkPixelConversion(srcBox, lock); 
			mLightBuffer->unlock();
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::computeLightMap () const
	{    
		assert(mIsShadowed);
		PagingLandScapeOptions * const opt = mParent->getOptions();
		const Vector3 LightDir = opt->Sun;
		const Real SunAngle = opt->SunAngle;

		const bool positiveHorizon =  (LightDir.y > 0);// Sun is west (true), east (false);
		assert (fabs (SunAngle) < 1.1f);

		const Real LightAngle =  (positiveHorizon)? SunAngle  : -SunAngle;       
		const size_t offsetneg = (positiveHorizon)? 0: 1;
		//const Real LightAngle = SunAngle;


		unsigned int curr_rowY = 0;
		const uchar BScale = 255;
		const Real uchardivider = 1.0f / BScale;
		unsigned int curr_image_pos = 0;
		uchar * const ogre_restrict lightmap = (uchar *) (mLightImage.getData ());
		const uchar * const ogre_restrict HorizonAngle = mShadow.getData(); 

		const unsigned int mTextureSize = (unsigned int) mLightImage.getWidth ();
		assert ((mShadow.getBPP ()) / 8 == 3);
		const unsigned int rowpitch = mTextureSize*3;
		for(unsigned int nZ = 0; nZ < mTextureSize ; nZ++) 
		{
			unsigned int curr_image_posX = 0;
			for(unsigned int nX = 0; nX < mTextureSize; nX++)
			{            
				const unsigned int nVert = static_cast <unsigned int> (curr_rowY + curr_image_posX  + offsetneg);
				const Real hAngle = HorizonAngle[nVert] * uchardivider;
				if (hAngle < LightAngle) 
				{           
					const Real intensity =  1 - (LightAngle - hAngle);
					if (intensity > 0.0f) 
					{                       
						//intensity *= std::max(LightDir.dotProduct (data->getNormal (nX, nZ)), 0.0f);
						lightmap[curr_image_pos] = static_cast <uchar> (intensity * BScale);

					}
					else
					{
						// totally in shadow    
						lightmap[curr_image_pos] = 0;    
					}
				}
				else             
				{
					// if Vertex is lighted
					const Real intensity = BScale;           
					//const Real intensity = BScale * std::min(1.0f, LightDir.dotProduct (data->getNormal (nX, nZ))); 
					lightmap[curr_image_pos] = static_cast <uchar> (intensity);
				}               
				// if colored light should use a rgb map..

				curr_image_pos ++;
				curr_image_posX += 3;
			}
			curr_rowY += rowpitch;
		}    

	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::computePointAlpha(
		const unsigned int imagePos,
		const Real height, 
		const Real slope)
	{
		assert (mNumTexture > 0);
		const unsigned int numHeights = mParent->getOptions()->NumMatHeightSplat;
		unsigned int indx = 1;
		while (indx < (numHeights - 1) && height >= mParent->heights[indx])
			indx++;                                

		const unsigned int bScale = 255;
		const unsigned int up_indx = indx;
		const unsigned int down_indx = indx - 1;
		const Real interpol = (height  - mParent->heights[down_indx]) 
			* mParent->dividers[up_indx];  

		std::vector<Real> alpha;   
		alpha.reserve(numHeights);
		alpha.resize(numHeights);
		for (unsigned int ialpha = 0; ialpha < numHeights; ialpha++) 
		{    
			alpha[ialpha] = 0.0f;
		}

		if (slope < 0.05f)// speed-up as it's invisible
		{
			const Real B = (1.0f - interpol);
			const Real C = interpol;

			alpha[indx - 1] = B;
			alpha[indx] = C; 
		}
		else 
		{
			const Real A = (1.0f - slope);
			const Real B = A * (1.0f - interpol);
			const Real C = A * interpol;
			const Real D = slope;

			alpha[indx - 1] = B;
			alpha[indx] = C; 

			alpha[ 2 ] = alpha[ 2 ] + slope;
			alpha[ 2 ] = alpha[ 2 ] > 1.0f ? 1.0f : alpha[ 2 ];
		}
		// save changes in textures

		unsigned int currChannel = 0;
		for (unsigned int k = 0; k < mNumTexture; k++) 
		{    
			if (mImages[k].getBPP () == 8)
			{
				uchar * const BaseData = mImages[k].getData ();
				assert (BaseData && "PagingLandScapeTexture::computePointAlpha()");
				const unsigned int curr_image_pos = imagePos*(static_cast <unsigned int>(mNumChannelperTexture[k]));
				assert (mNumChannelperTexture[k] == (mImages[k].getBPP ()/8));
				for (unsigned int j = 0; j < mNumChannelperTexture[k]; j++) 
				{  
					BaseData[ curr_image_pos + j] = static_cast <uchar> (alpha[currChannel]*bScale);

					currChannel++;
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::computePointColor(
		const unsigned int imagePos,
		const Real height, 
		const Real slope)
	{
		assert (mNumTexture > 0);
		const unsigned int numHeights = mParent->getOptions()->NumMatHeightSplat;
		
		unsigned int indx = 1;
		while (indx < (numHeights - 1) && height >= mParent->heights[indx])
			indx++;                                

		const unsigned int bScale = 255;
		const unsigned int up_indx = indx;
		const unsigned int down_indx = indx - 1;
		const Real interpol = (height  - mParent->heights[down_indx]) 
			* mParent->dividers[up_indx];  

		//ColourValue color;
		std::vector<ColourValue> color; 
		for (unsigned int itex = 0; itex < mNumTexture; itex++) 
		{
			for (unsigned int ichan = 0; ichan < mNumChannelperTexture[itex]; ichan++) 
			{    
				color.push_back (ColourValue::Black);
			}
		}


		indx = up_indx;
		unsigned int currTexture = 0;
		while (indx != 0)
		{
			if (indx < mNumChannelperTexture[currTexture])
				break;
			indx = indx - mNumChannelperTexture[currTexture];
			currTexture++;
		}
		currTexture = currTexture < mNumTexture ? currTexture : mNumTexture - 1;

		if (slope < 0.05f)// speed-up as it's invisible
		{
			const Real B = (1.0f - interpol);
			const Real C = interpol;
			color[currTexture] = mParent->colors[down_indx] * B + mParent->colors[up_indx] * C;
		}
		else 
		{
			const Real A = (1.0f - slope);
			const Real B = A * (1.0f - interpol);
			const Real C = A * interpol;
			const Real D = slope;

			color[currTexture] = mParent->colors[down_indx] * B + mParent->colors[up_indx] * C + mParent->colors[2] * D;
		}
		// save changes in textures
		unsigned int currChannel = 0;
		for (unsigned int k = 0; k < mNumTexture; k++) 
		{    
			if (mImages[k].getBPP () > 8)
			{
				uchar * const BaseData = mImages[k].getData ();
				assert (BaseData && "PagingLandScapeTexture::computePointColor()");
				const unsigned int curr_image_pos = imagePos*(static_cast <unsigned int>(mNumChannelperTexture[k]));
				assert (mNumChannelperTexture[k] == (mImages[k].getBPP ()/8));
				 
				BaseData[ curr_image_pos + 0] = static_cast <uchar> (color[k].r * bScale);
				BaseData[ curr_image_pos + 1] = static_cast <uchar> (color[k].g * bScale);
				BaseData[ curr_image_pos + 2] = static_cast <uchar> (color[k].b * bScale);
				currChannel += 3;
			}			
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTexture::paintPoint (const unsigned int imagePos,
		const Real   paintForce)
	{
		assert (mNumTexture > 0);
		assert(mParent->channelModifList);
		const std::vector<Real>  *channelModifList = mParent->channelModifList;
		const uchar bScale = 255;
		const Real invPaintForce = 1.0f - paintForce;
		unsigned int currChannel = 0;
		for (unsigned int k = 0; k < mNumTexture; k++)
		{
			uchar * const BaseData = mImages[k].getData();
			assert (BaseData && "PagingLandScapeTexture::paint()");
			const unsigned int currImagePos = imagePos * mNumChannelperTexture[k];
			assert (currImagePos < mImages[k].getSize ());
			assert (mNumChannelperTexture[k]*8 == mImages[k].getBPP ());

			for (unsigned int j = 0; j < mNumChannelperTexture[k]; j++) 
			{    
				BaseData[ currImagePos + j ] = 
					static_cast <uchar> (
								((*channelModifList)[currChannel]) * paintForce * bScale 
								+ BaseData[ currImagePos + j ] * invPaintForce
										);
				currChannel++;
			}
			doTextureNeedUpdate[k] = true;
		}
	}
} //namespace
