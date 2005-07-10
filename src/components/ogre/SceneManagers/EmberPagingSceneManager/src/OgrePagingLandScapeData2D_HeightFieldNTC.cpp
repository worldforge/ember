/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldNTC.cpp  -  description
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

#include "OgreException.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2D_HeightFieldNTC.h"


#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldNTC::newPage( )
    {
       return new PagingLandScapeData2D_HeightFieldNTC();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldNTC::PagingLandScapeData2D_HeightFieldNTC()
    : PagingLandScapeData2D()
    {
	    mImage = 0;
	    input_max = 3000.0f;
        input_min = 0.0f;
        mMaxheight = _decodeTC (1.0f) * PagingLandScapeOptions::getSingleton().scale.y;
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldNTC::~PagingLandScapeData2D_HeightFieldNTC()
    {
        PagingLandScapeData2D::unload ();
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldNTC::getBase (const Real mX, const Real mZ)
    {
        return ColourValue::White;
    }

    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldNTC::getCoverage (const Real mX, const Real mZ)
    {
        return ColourValue::White;
    }
    //-----------------------------------------------------------------------
    const Vector3 PagingLandScapeData2D_HeightFieldNTC::getNormalAt (const Real x, const Real z)
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
    void PagingLandScapeData2D_HeightFieldNTC::_save()
    {
       
            const Real scale = 1.0f  / PagingLandScapeOptions::getSingleton().scale.y;
           
            uchar *img = mImage->getData();
            uint j = 0;
            const uint bpp = static_cast <uint> (mBpp);
            for (uint i = 0; i < mMax - 1;  i += bpp )
            {
                img[ i + (mBpp - 1)] =  uchar (_encodeTC(mHeightData[j++]) * scale);               
            }
            const String fname = PagingLandScapeOptions::getSingleton().landscape_filename + ".HN." + 
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
            mImage->save (fname + "modif." + extname);
            //FileSystemArchive::pushDirectory();
            RetablishDir (olddir);
        }
      
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D_HeightFieldNTC::_load( const uint mX, const uint mZ )
    { 
        const String strFileName = PagingLandScapeOptions::getSingleton().landscape_filename + ".HN." + 
                            StringConverter::toString( mZ ) + "." +
			                StringConverter::toString( mX ) + ".";

        String finalName = strFileName + 
                        "modif." + 
                        PagingLandScapeOptions::getSingleton().landscape_extension;
        if (!(PagingLandScapeOptions::getSingleton().Deformable && 
            ResourceGroupManager::getSingleton().resourceExists(PagingLandScapeOptions::getSingleton().groupName,
            finalName)))
        {
            finalName = strFileName + 
                PagingLandScapeOptions::getSingleton().landscape_extension;  
            if (!ResourceGroupManager::getSingleton().resourceExists(PagingLandScapeOptions::getSingleton().groupName,
                finalName))
            { 
                LogManager::getSingleton().logMessage(String("PLSM2 : Cannot find map named ") + finalName, 
                    LML_CRITICAL,
                    true);
                return false;
            }
        }
        mImage = new Image();
        mImage->load (finalName,  PagingLandScapeOptions::getSingleton().groupName); 
        
        //check to make sure it's 2^n + 1 size.
        if ( mImage -> getWidth() != mImage->getHeight() ||	!_checkSize( mImage->getWidth() ) )
        {
            String err = "Error: Invalid heightmap size : " +
                StringConverter::toString( static_cast <uint> (mImage->getWidth()) ) +
                "," + StringConverter::toString( static_cast <uint> (mImage->getHeight()) ) +
                ". Should be 2^n+1, 2^n+1";
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "PagingLandScapeData2D_HeightField::_load" );
        }

        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
        if ( mBpp != 4 )
        {
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, "Error: Image is not a RGBA image.(4 bytes, 32 bits)", 
                "PagingLandScapeData2D_HeightField::_load" );
        }

        if ( mSize != mImage->getWidth() )
        {
            OGRE_EXCEPT ( Exception::ERR_INVALIDPARAMS, "Error: Declared World size <> Height Map Size.", "PagingLandScapeData2D_HeightField::_load" );
        }

        mXDimension = mImage->getWidth();
        mZDimension = mImage->getHeight();
        mMax = static_cast <uint> (mSize * mImage->getHeight() * mBpp + 1);

        mMaxArrayPos = static_cast <uint> (mSize * mImage->getHeight());
        mHeightData = new Real[mMaxArrayPos];
        uint j = 0;
        const double divider = 1.0 / 255;
        const Real scale  = PagingLandScapeOptions::getSingleton().scale.y;
        uchar *data = mImage->getData();
        mMaxheight = 0.0f;
        const uint bpp = static_cast <uint> (mBpp);
        for (uint i = 0; i < mMax - 1;  i += bpp )
            {  
                const Real h = _decodeTC(data[ i + (mBpp - 1)] * divider)* scale;
                mMaxheight = std::max ( h, mMaxheight);
                mHeightData[j++] = h;
            }
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldNTC::_load( )
    {
	   
		    mImage = new Image();

		    mImage -> load( PagingLandScapeOptions::getSingleton().landscape_filename + 
                "." + PagingLandScapeOptions::getSingleton().landscape_extension, 
                    PagingLandScapeOptions::getSingleton().groupName);

		    //check to make sure it's 2^n size.
		    if ( !_checkSize( mImage->getHeight() ) ||	!_checkSize( mImage->getWidth() ) )
		    {
			    String err = "Error: Invalid heightmap size : " +
				    StringConverter::toString( static_cast <uint> (mImage->getWidth() )) +
				    "," + StringConverter::toString( static_cast <uint> (mImage->getHeight()) ) +
				    ". Should be 2^n, 2^n";
			    OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "PagingLandScapeData2D_HeightFieldNTC::_load" );
		    }

            mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
            if (mBpp != 1)
		    {
			    OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, "Error: Image is not a grayscale image.(1 byte, 8 bits)", 
                    "PagingLandScapeData2D_HeightFieldNTC::_load" );
		    }

            mSize = static_cast <uint> (mImage->getWidth());
            mMax = static_cast <uint> (mSize * mImage->getHeight() * mBpp + 1);

            mMaxArrayPos = static_cast <uint> (mSize * mImage->getHeight());
            mHeightData = new Real[mMaxArrayPos];
            uint j = 0;
            const double divider = 1.0 / 255;
            const Real scale  = PagingLandScapeOptions::getSingleton().scale.y;
            uchar *data = mImage->getData();
            mMaxheight = 0.0f;
            const uint bpp = static_cast <uint> (mBpp);
            for (uint i = 0; i < mMax - 1;  i += bpp )
            {  
                const Real h = _decodeTC(data[ i + (mBpp - 1)] * divider)* scale;
                mMaxheight = std::max ( h, mMaxheight);
                mHeightData[j++] = h;
            }
	    
    }


    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldNTC::_unload()
    {	
	    delete mImage;
	    mImage = 0;
    }

    //-----------------------------------------------------------------------
    inline Real PagingLandScapeData2D_HeightFieldNTC::_decodeTC(const Real encoded) const
    {
	    return ((Real) (encoded + 0.5f)) * (input_max - input_min) + input_min;
    }

    //-----------------------------------------------------------------------
    inline uchar PagingLandScapeData2D_HeightFieldNTC::_encodeTC(const Real decoded) const
    {
        return static_cast<uchar> ((decoded - input_min) / (input_max - input_min) - 0.5f);
    }
} //namespace
