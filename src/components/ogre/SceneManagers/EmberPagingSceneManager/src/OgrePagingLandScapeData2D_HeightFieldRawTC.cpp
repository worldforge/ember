/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldRawTC.cpp  -  description
  -------------------
  begin                : Mon Oct 13 2003
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

#include "OgreLogManager.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreImage.h"
#include "OgreStringConverter.h"

#include "OgreResourceManager.h"

#include "OgreException.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2D_HeightFieldRawTC.h"

#include "fileutils.h"


namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldRawTC::newPage( )
    {
       return new PagingLandScapeData2D_HeightFieldRawTC();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldRawTC::PagingLandScapeData2D_HeightFieldRawTC()
    : PagingLandScapeData2D()
    {
	    mImage = 0;
	    input_max =  3000.0f;
        input_min = 0.0f;
        mMaxheight = _decodeRawTC (1.0f) * PagingLandScapeOptions::getSingleton().scale.y;
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldRawTC::~PagingLandScapeData2D_HeightFieldRawTC()
    {
	   PagingLandScapeData2D::unload ();
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldRawTC::getBase (const Real mX, const Real mZ)
    {
        return ColourValue::White;
    }

    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldRawTC::getCoverage (const Real mX, const Real mZ)
    {
        return ColourValue::White;
    }
    //-----------------------------------------------------------------------
    const Vector3 PagingLandScapeData2D_HeightFieldRawTC::getNormalAt (const Real x, const Real z)
    {
        #ifndef _LOADEDNORM
            return PagingLandScapeData2D::getNormal(x,z);
        #else
            if ( mImage )
            {
                uint Pos = static_cast<uint> (( z * mSize  + x) * mBpp);//4 bytes (mImage is RGBA)

                if ( mMax > Pos )
                {
                    const Real normalscale = 1.0f / 127.0f;
                    return Vector3 (((Real)(mImage->getData()[Pos + 0]) - 128.0f) * normalscale,
                                    ((Real)(mImage->getData()[Pos + 1]) - 128.0f) * normalscale,
                                    ((Real)(mImage->getData()[Pos + 2]) - 128.0f) * normalscale);
                }
                else
                {
                    return Vector3::UNIT_Y;
                }
            }
            else
            {
                return Vector3::UNIT_Y;
            }
        #endif //_NOLOAD
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRawTC::_save()
    {
        uchar *data = new uchar[ mXDimension * mZDimension * 2 ];   

        const double divider = 65535.0 / PagingLandScapeOptions::getSingleton().scale.y;
        
        uint j = 0;
        for (uint i = 0; i < mXDimension*mZDimension; i++ )
        {             
            ushort syn = static_cast <ushort>  (_encodeRawTC (mHeightData[i]) * divider);
            #if OGRE_ENDIAN == ENDIAN_BIG
                data[j] = uchar ((syn >> 8) & 0xff);
		        data[j+ 1] = uchar (syn & 0xff);
            #else
		        data[j] = uchar (syn & 0xff);
		        data[j+ 1] = uchar ((syn >> 8) & 0xff);
            #endif
            j += 2;            
        }
        const String fname = PagingLandScapeOptions::getSingleton().landscape_filename + "." +
                                    StringConverter::toString( mPageZ ) + "." +
			                        StringConverter::toString( mPageX ) + ".";
        const String extname = PagingLandScapeOptions::getSingleton().landscape_extension;


       FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
             PagingLandScapeOptions::getSingleton().groupName, 
             fname + extname);
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
    bool PagingLandScapeData2D_HeightFieldRawTC::_load( const uint mX, const uint mZ )
    {
        // Load data
        //mRawData.clear();

        const String strFileName = PagingLandScapeOptions::getSingleton().landscape_filename + "." + 
                            StringConverter::toString( mZ ) + "." +
			                StringConverter::toString( mX ) + ".";

            
        String finalName = strFileName + 
                        "modif." + 
                        PagingLandScapeOptions::getSingleton().landscape_extension;
        if (!(PagingLandScapeOptions::getSingleton().Deformable && 
            ResourceGroupManager::getSingleton().resourceExists(PagingLandScapeOptions::getSingleton().groupName,finalName)))
        {    
            finalName = strFileName + 
                PagingLandScapeOptions::getSingleton().landscape_extension;   
            if (!ResourceGroupManager::getSingleton().resourceExists(PagingLandScapeOptions::getSingleton().groupName,finalName))
            { 
                LogManager::getSingleton().logMessage(String("PLSM2 : Cannot find map named ") + finalName, 
                    LML_CRITICAL,
                    true);
                return false;
            }
        }
        DataStreamPtr RawData;
        RawData = ResourceGroupManager::getSingleton().openResource(finalName, 
                    PagingLandScapeOptions::getSingleton().groupName);
        
        // Validate size
        // Image size comes from setting (since RAW is not self-describing)
        // here 16 bits Raw file
        mXDimension = mSize;
        mZDimension = mXDimension;
        mBpp = 2;

        size_t numBytes = mSize * mSize * mBpp;
        if (RawData->size() != numBytes)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                ") does not agree with configuration settings.", 
                "PagingLandScapeData2D_HeightFieldRaw::_load");
        }
		mMax = static_cast<uint> (numBytes) + 1;

        mMaxArrayPos = static_cast <uint> (mSize * mSize);
        mHeightData = new Real[mMaxArrayPos];            

        const Real divider = 1.0f / 65535.0f;
        const Real scale  = PagingLandScapeOptions::getSingleton().scale.y;
        
        MemoryDataStream dc (RawData, 
				                true);
        const uchar *pSrc = dc.getPtr ();     

        mMaxheight = 0.0f;
        uint j = 0;
        for (uint i = 0; i < mMax - 1;  i += mBpp )
        {                  
            #if OGRE_ENDIAN == ENDIAN_BIG
                        ushort val = *pSrc++ <<8;
                        val += *pSrc++;
            #else
                        ushort val = *pSrc++;
                        val += *pSrc++ <<8;
            #endif
            const Real h = _decodeRawTC (val * divider)* scale;
            mMaxheight = std::max (h, mMaxheight);
            mHeightData[j++] = h;
        }
    
//            if (PagingLandScapeOptions::getSingleton().vertex_shadowed)
//            {
//                mShadow = new Image();
//                mShadow -> load( PagingLandScapeOptions::getSingleton().landscape_filename +
//                                ".HS." +
//                                StringConverter::toString( mZ ) + "." +
//                                StringConverter::toString( mX ) + "." +
//                                "png");
//                                //PagingLandScapeOptions::getSingleton().landscape_extension );
//            }
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRawTC::_load( )
    {
        // Load data
        DataStreamPtr RawData = ResourceGroupManager::getSingleton().openResource(PagingLandScapeOptions::getSingleton().landscape_filename +
                    "." + 
                    PagingLandScapeOptions::getSingleton().landscape_extension, 
                    PagingLandScapeOptions::getSingleton().groupName);

        // Validate size
        // Image size comes from setting (since RAW is not self-describing)
        // here 16 bits Raw file

        mXDimension = PagingLandScapeOptions::getSingleton().RawWidth;
        mZDimension = PagingLandScapeOptions::getSingleton().RawHeight;

        mBpp = 2;
        
        const size_t sourceHeight = mZDimension;
        const size_t sourceWidth = mXDimension;

        computePowerof2PlusOneSize ();       
		mSize = mXDimension;

        if (RawData->size() != sourceHeight*sourceWidth*2)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "RAW size (" + StringConverter::toString(static_cast<uint> (RawData->size())) + 
                ") does not agree with configuration settings.", 
                "PagingLandScapeData2D_HeightFieldRaw::_load");
        }
    
        mMaxArrayPos = static_cast <uint> (mXDimension * mZDimension);
        const size_t numBytes = mMaxArrayPos * mBpp;
		mMax = static_cast<uint> (numBytes) + 1;

        mHeightData = new Real[mMaxArrayPos];

        mMaxheight = 0.0f;


        MemoryDataStream dc(RawData, 
            true);
        const uchar *pSrc = dc.getPtr ();     

        const Real divider = 1.0f / 65535.0f;
        const Real scale  = PagingLandScapeOptions::getSingleton().scale.y;
        const uint shift_fill = static_cast <uint> (mXDimension - sourceWidth);
        uint dest_pos = 0;
        //for some reason water is 65035 in SRTM files...
        const bool srtm_water = PagingLandScapeOptions::getSingleton().SRTM_water;
        for (uint i = 0; i < sourceHeight; ++i )
        {
            for (uint j = 0; j < sourceWidth; ++j )
            {
                #if OGRE_ENDIAN == ENDIAN_BIG
                            ushort val = *pSrc++ <<8;
                            val += *pSrc++;
                #else
                            ushort val = *pSrc++;
                            val += *pSrc++ <<8;
                #endif

                if (srtm_water && (val - 65035) > 0)
                    val = 0;

                const Real h = _decodeRawTC (val * divider) * scale;
                mMaxheight = std::max (h, mMaxheight);
                mHeightData[dest_pos++] = h;
            }
            memset (&mHeightData[dest_pos], 0, shift_fill);
            dest_pos+= shift_fill; 
        }
    }
  

    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRawTC::_unload()
    {
	    delete mImage;
		mImage = 0;
	 }

    //-----------------------------------------------------------------------
    inline Real PagingLandScapeData2D_HeightFieldRawTC::_decodeRawTC(const Real encoded) const
    {
	    return ((Real) (encoded + 0.5f)) * (input_max - input_min) + input_min;
    }
    //-----------------------------------------------------------------------
    inline ushort PagingLandScapeData2D_HeightFieldRawTC::_encodeRawTC(const Real decoded) const
    {
        return static_cast <short> ((decoded - input_min) / (input_max - input_min) - 0.5f);
    }

} //namespace
