/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldRaw.cpp  -  description
  -------------------
  begin                : Mon Oct 13 2003
  copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
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

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgreLogManager.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreImage.h"
#include "OgreStringConverter.h"

#include "OgreResourceManager.h"


#include "OgreException.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2D_HeightFieldRaw.h"

#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldRaw::newPage()
    {
       return new PagingLandScapeData2D_HeightFieldRaw(mParent);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldRaw::PagingLandScapeData2D_HeightFieldRaw(PagingLandScapeData2DManager *dataMgr)
    : PagingLandScapeData2D(dataMgr)
    {
        mShadow = 0;
        mMaxheight = mParent->getOptions()->scale.y;
    }  
    //-------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightFieldRaw::getMaxAbsoluteHeight(void) const
    { 
        return mParent->getOptions()->scale.y;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldRaw::~PagingLandScapeData2D_HeightFieldRaw()
    {
    }

    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightFieldRaw::getShadow (const Real mX, const Real mZ,
                                                            const bool &positive)
    {
        if (mShadow)
        {
            unsigned int Pos = static_cast<unsigned int> ((mZ * mShadow->getWidth() + mX) * 3);//3 bytes (mImage is RGBA)
            if (mShadow->getSize () > Pos)
            {
                if (positive)
                    return static_cast<Real> (mShadow->getData()[ Pos + 0]) / 255;
                else
                    return static_cast<Real> (mShadow->getData()[ Pos + 1]) / 255;
            }
            else
            {
                return 0.0f;
            }
        }
        else
        {
            return 0.0f;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRaw::_save()
    {
        uchar *data = new uchar[ mXDimension * mZDimension * 2 ];   

        const double divider = 65535.0 / mParent->getOptions()->scale.y;
        
        unsigned int j = 0;
        for (unsigned int i = 0; i < mMaxArrayPos; i++)
        {             
            const ushort syn = ushort  (mHeightData[i] * divider);
            #if OGRE_ENDIAN == OGRE_ENDIAN_BIG
                data[j] = uchar ((syn >> 8) & 0xff);
		        data[j+ 1] = uchar (syn & 0xff);
            #else
		        data[j] = uchar (syn & 0xff);
		        data[j+ 1] = uchar ((syn >> 8) & 0xff);
            #endif
            j += 2;            
        }

        const String fname = mParent->getOptions()->LandScape_filename + "." +
                                    StringConverter::toString(mPageZ) + "." +
			                        StringConverter::toString(mPageX) + ".";
        const String extname = mParent->getOptions()->LandScape_extension;

        FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
                mParent->getOptions()->groupName, 
                fname + extname);
		//ResourceGroupManager::getSingleton().openResource ()
        FileInfoList::iterator it = finfo->begin();
        if (it != finfo->end())
            {
                //FileInfo *inf = &(*it);
                char *olddir = ChangeToDir (const_cast< char * > (((it)->archive->getName()).c_str()));
                //FileSystemArchive::pushDirectory()
                
                std::ofstream outfile;
                String FileNameRaw;
                
                DataStreamPtr image_chunk(new MemoryDataStream ((void*)data,
                            mXDimension * mZDimension * 2 * sizeof (uchar), 
                            true)); 
         outfile.open (const_cast< char * > ((fname + "modif." + extname).c_str()),
                                std::ios::binary);
                // Write out
                outfile << image_chunk->getAsString ();
                outfile.close ();


                //FileSystemArchive::pushDirectory();
                RetablishDir (olddir);
            }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D_HeightFieldRaw::_load(const unsigned int mX, const unsigned int mZ)
    {
        // Load data
        const String strFileName = mParent->getOptions()->LandScape_filename + "." + 
                            StringConverter::toString(mZ) + "." +
			                StringConverter::toString(mX) + ".";

        String finalName = strFileName + 
                        "modif." + 
                        mParent->getOptions()->LandScape_extension;

        if (!(mParent->getOptions()->Deformable && 
            ResourceGroupManager::getSingleton().resourceExists(mParent->getOptions()->groupName,finalName)))
        {    
            finalName = strFileName + mParent->getOptions()->LandScape_extension;   
            if (!ResourceGroupManager::getSingleton().resourceExists(mParent->getOptions()->groupName,finalName))
            { 
                LogManager::getSingleton().logMessage(String("PLSM2 : Cannot find map named ") + finalName, 
                    LML_CRITICAL,
                    true);
                return false;
            }
        }

        DataStreamPtr RawData;
        RawData = ResourceGroupManager::getSingleton().openResource(finalName, 
                    mParent->getOptions()->groupName);

      
        // Validate size
        // Image size comes from setting (since RAW is not self-describing)
        // here 16 bits Raw file
        mXDimension = mSize;
        mZDimension = mXDimension;
        mBpp = 2;
        mMaxArrayPos = static_cast <unsigned int> (mSize * mSize);
        const size_t numBytes = mMaxArrayPos * mBpp;
        if (RawData->size() != numBytes)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "RAW size (" + StringConverter::toString(static_cast<unsigned int> (RawData->size())) + 
                ") does not agree with configuration settings.", 
                "PagingLandScapeData2D_HeightFieldRaw::_load");
        }
		mMax = static_cast<unsigned int> (numBytes) + 1;

        mHeightData = new Real[mMaxArrayPos];
        const double scale = (double) mParent->getOptions()->scale.y / 65535;
        mMaxheight = 0.0f;

        MemoryDataStream dc (RawData, 
				                true);
        const uchar *pSrc = dc.getPtr ();   

        unsigned int j = 0;
        for (unsigned int i = 0; i < mMax - 1;  i += mBpp)
        {
            #if OGRE_ENDIAN == OGRE_ENDIAN_BIG
                        ushort val = *pSrc++ <<8;
                        val += *pSrc++;
            #else
                        ushort val = *pSrc++;
                        val += *pSrc++ <<8;
            #endif
            const Real h = val * scale;
            mMaxheight = std::max (h, mMaxheight);
            mHeightData[j++] = h;
        }

    
        if (mParent->getOptions()->vertex_shadowed)
        {
            mShadow = new Image();
            mShadow->load(mParent->getOptions()->LandScape_filename +
                            ".HS." +
                            StringConverter::toString(mZ) + "." +
                            StringConverter::toString(mX) + "." +
                            "png", 
                            mParent->getOptions()->groupName);
                            //mParent->getOptions()->LandScape_extension);
        }
        return true;
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRaw::_load()
    {
        // Load data
		const PagingLandScapeOptions * const opt =  mParent->getOptions();
		const String mName(opt->LandScape_filename + "." + opt->LandScape_extension);
		const String gName (opt->groupName);
		ResourceGroupManager *rsm = ResourceGroupManager::getSingletonPtr();
        DataStreamPtr RawData = rsm->openResource(mName, gName);

        // Validate size
        // Image size comes from setting (since RAW is not self-describing)
        // here 16 bits Raw file

        mXDimension = opt->RawWidth;
        mZDimension = opt->RawHeight;

        mBpp = 2;
        
        const size_t sourceHeight = mZDimension;
        const size_t sourceWidth = mXDimension;

        computePowerof2PlusOneSize ();       
		mSize = mXDimension;

        if (RawData->size() != sourceHeight*sourceWidth*2)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "RAW size (" + StringConverter::toString(static_cast<unsigned int> (RawData->size())) + 
                ") does not agree with configuration settings.", 
                "PagingLandScapeData2D_HeightFieldRaw::_load");
        }
    
        mMaxArrayPos = static_cast <unsigned int> (mXDimension * mZDimension);
        const size_t numBytes = mMaxArrayPos * mBpp;
		mMax = static_cast<unsigned int> (numBytes) + 1;

        mHeightData = new Real[mMaxArrayPos];
        const Real scale = opt->ScaledHeightY;
        mMaxheight = 0.0f;


        MemoryDataStream dc(RawData, 
            true);
        const uchar *pSrc = dc.getPtr ();     

        const unsigned int shift_fill = static_cast <unsigned int> (mXDimension - sourceWidth);
        unsigned int dest_pos = 0;
        //for some reason water is 65035 in SRTM files...
        const bool srtm_water = opt->SRTM_water;
        for (unsigned int i = 0; i < sourceHeight; ++i)
        {
            for (unsigned int j = 0; j < sourceWidth; ++j)
            {
                #if OGRE_ENDIAN == OGRE_ENDIAN_BIG
                            ushort val = *pSrc++ << 8;
                            val += *pSrc++;
                #else
                            ushort val = *pSrc++;
                            val += *pSrc++ << 8;
                #endif

                if (srtm_water && (val - 65035) > 0)
                    val = 0;

                const Real h = val * scale;
                mMaxheight = std::max (h, mMaxheight);
                mHeightData[dest_pos++] = h;
            }
            memset (&mHeightData[dest_pos], 0, shift_fill);
            dest_pos+= shift_fill; 
        }        
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRaw::_unload()
    {	
        delete mShadow;
        mShadow = 0;
    }
} //namespace
