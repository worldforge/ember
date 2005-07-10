/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldTC.cpp  -  description
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

#include "OgrePagingLandScapeData2D_HeightFieldTC.h"


#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldTC::newPage( )
    {
       return new PagingLandScapeData2D_HeightFieldTC();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldTC::PagingLandScapeData2D_HeightFieldTC()
    : PagingLandScapeData2D()
    {
	    mImage = 0;
	    input_max = 3000.0f;
        input_min = 0.0f;
        mMaxheight = _decodeTC (1.0f) * PagingLandScapeOptions::getSingleton().scale.y;
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldTC::~PagingLandScapeData2D_HeightFieldTC()
    {
	   PagingLandScapeData2D::unload ();
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldTC::getBase (const Real mX, const Real mZ)
    {
        return ColourValue::White;
    }

    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldTC::getCoverage (const Real mX, const Real mZ)
    {
        return ColourValue::White;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldTC::_save()
    {
      
            const Real scale = 1.0f  / PagingLandScapeOptions::getSingleton().scale.y;
           
            uchar *img = mImage->getData();
            uint j = 0;
            for (uint i = 0; i < mMax - 1;  i ++ )
            {
                img[ i ] =  uchar (_encodeTC(mHeightData[j++]) * scale);               
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
                mImage->save (fname + "modif." + extname);
                //FileSystemArchive::pushDirectory();
                RetablishDir (olddir);
            }
        
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D_HeightFieldTC::_load( const uint mX, const uint mZ )
    {
        const String strFileName = PagingLandScapeOptions::getSingleton().landscape_filename + "." + 
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
        if ( mImage -> getWidth() != mImage->getHeight() ||	
            !_checkSize( mImage->getWidth() ) )
        {
            String err = "Error: Invalid heightmap size : " +
                StringConverter::toString( static_cast<int>(mImage->getWidth()) ) +
                "," + StringConverter::toString( static_cast<int>(mImage->getHeight()) ) +
                ". Should be 2^n+1, 2^n+1";
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "PagingLandScapeData2D_HeightField::_load" );
        }

        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
        if ( mBpp != 1 )
        {
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, "Error: Image is not a gray image.(1 bytes, 8 bits)", 
                "PagingLandScapeData2D_HeightField::_load" );
        }

        if ( mSize != mImage->getWidth() )
        {
            OGRE_EXCEPT ( Exception::ERR_INVALIDPARAMS, "Error: Declared World size <> Height Map Size.", "PagingLandScapeData2D_HeightField::_load" );
        }

        mXDimension = mImage->getWidth();
        mZDimension = mImage->getHeight();
        mMax = static_cast <uint> (mSize * mImage->getHeight() + 1);

        mMaxArrayPos = mMax - 1;
        mHeightData = new Real[mMaxArrayPos];
        uint j = 0;
        const double divider = 1.0 / 255;
        const Real scale  = PagingLandScapeOptions::getSingleton().scale.y;
        uchar *data = mImage->getData();
        mMaxheight = 0.0f;
        for (uint i = 0; i < mMax - 1;  i++ )
            {  
                const Real h = _decodeTC(data[ i ] * divider)* scale;
                mMaxheight = std::max ( h, mMaxheight);
                mHeightData[j++] = h;
            }
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldTC::_load( )
    {
	  mImage = new Image();
        mImage->load ( PagingLandScapeOptions::getSingleton().landscape_filename +
                "." + PagingLandScapeOptions::getSingleton().landscape_extension, PagingLandScapeOptions::getSingleton().groupName );
        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
		if (mBpp != 1)
		{
			OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, "Error: Image is not a greyscale image.(1 byte, 8 bits)",
                "PagingLandScapeData2D_HeightField::_load" );
		}

        mXDimension = mImage->getWidth();
        mZDimension = mImage->getHeight();

        const size_t sourceHeight = mZDimension;
        const size_t sourceWidth = mXDimension;

        computePowerof2PlusOneSize ();     

		mSize = mXDimension;
        mMaxArrayPos = static_cast <uint> (mXDimension * mZDimension);
        mMax = static_cast <uint> (mMaxArrayPos * mBpp);
        mHeightData = new Real[mMaxArrayPos];

        const Real divider = 1.0f / 65535.0f;
        const Real scale  = PagingLandScapeOptions::getSingleton().scale.y;
        mMaxheight = 0.0f;
        const uint shift_fill = static_cast <uint> (mXDimension - sourceWidth);
        uchar *imagedata = mImage->getData();
        uint dest_pos = 0;
        for (uint i = 0; i < sourceHeight; ++i )
        {
            for (uint j = 0; j < sourceWidth; ++j )
            {   
                const Real h = _decodeTC(*imagedata++ * divider)* scale;
                mMaxheight = std::max (h, mMaxheight);
                mHeightData[dest_pos++] = h;
            }
            memset (&mHeightData[dest_pos], 0, shift_fill);
            dest_pos += shift_fill; 
        }       	    

	   
    }


    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldTC::_unload()
    {	
	    delete mImage;
	    mImage = 0;
    }

    //-----------------------------------------------------------------------
    inline Real PagingLandScapeData2D_HeightFieldTC::_decodeTC(const Real encoded) const
    {
	    return ((Real) (encoded + 0.5f)) * (input_max - input_min) + input_min;
    }

    //-----------------------------------------------------------------------
    inline uchar PagingLandScapeData2D_HeightFieldTC::_encodeTC(const Real decoded) const
    {
        return static_cast<uchar> ((decoded - input_min) / (input_max - input_min) - 0.5f);
    }
} //namespace
