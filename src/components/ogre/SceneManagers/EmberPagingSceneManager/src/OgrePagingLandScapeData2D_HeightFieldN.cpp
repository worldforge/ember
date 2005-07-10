/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldN.cpp  -  description
  -------------------
  begin                : Mon Oct 13 2003
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

#include "OgreLogManager.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreImage.h"
#include "OgreImageCodec.h"

#include "OgreArchiveManager.h"

#include "OgreStringConverter.h"

#include "OgreException.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D_HeightFieldN.h"

#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldN::newPage( )
    {
       return new PagingLandScapeData2D_HeightFieldN();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldN::PagingLandScapeData2D_HeightFieldN()
    : PagingLandScapeData2D()
    {
        mImage = 0;
        mCoverage = 0;
        mBase = 0;
        mShadow = 0;
        mMaxheight = PagingLandScapeOptions::getSingleton().scale.y;
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldN::~PagingLandScapeData2D_HeightFieldN()
    {	
        PagingLandScapeData2D::unload ();
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldN::getBase (const Real mX, const Real mZ)
    {
        if ( mBase != 0 )
        {
            uint Pos = static_cast<uint> ((mZ * mBase->getWidth() + mX) * 4);//4 bytes (mImage is RGBA)
            if ( mBase->getSize () > Pos )
            {
                const Real divider = 1.0f / 255;
                return ColourValue((Real) mBase->getData()[ Pos + 0] * divider,
                                (Real) mBase->getData()[ Pos + 1] * divider,
                                (Real) mBase->getData()[ Pos + 2] * divider,
                                (Real) mBase->getData()[ Pos + 3] * divider);
            }
            else
            {
                return ColourValue::White;
            }
        }
        else
        {
            return ColourValue::White;
        }
    }

    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldN::getCoverage (const Real mX, const Real mZ)
    {
        if ( mCoverage != 0 )
        {
            uint Pos = static_cast<uint> ((mZ * mCoverage->getWidth()  + mX) * 4);//4 bytes (mImage is RGBA)
            if ( mCoverage->getSize () > Pos )
            {
                const Real divider = 1.0f / 255;
                return ColourValue((Real) mCoverage->getData()[ Pos + 0] * divider,
                                (Real) mCoverage->getData()[ Pos + 1] * divider,
                                (Real) mCoverage->getData()[ Pos + 2] * divider,
                                (Real) mCoverage->getData()[ Pos + 3] * divider);
            }
            else
            {
                return ColourValue::White;
            }
        }
        else
        {
            return ColourValue::White;
        }
    }

    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightFieldN::getShadow (const Real mX, const Real mZ,
                                                            const bool &positive)
    {
        if ( mShadow != 0 )
        {
            uint Pos = static_cast<uint> ((mZ * mShadow->getWidth() + mX) * 3);//3 bytes (mImage is RGBA)
            if (mShadow->getSize () > Pos)
            {
                if (positive)
                    return static_cast<Real> ( mShadow->getData()[ Pos + 0]) / 255;
                else
                    return static_cast<Real> ( mShadow->getData()[ Pos + 1]) / 255;
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
    const Vector3 PagingLandScapeData2D_HeightFieldN::getNormal (const Real x, const Real z)
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
    void PagingLandScapeData2D_HeightFieldN::_save()
    {        
        const Real scale = 256.0 / PagingLandScapeOptions::getSingleton().scale.y;
        
        uchar *img = mImage->getData();
        uint j = 0;
        const uint bpp = static_cast <uint> (mBpp);
        for (uint i = 0; i < mMax - 1;  i += bpp )
        {
            img[ i + (mBpp - 1)] =  uchar (mHeightData[j++] * scale);               
        }
        const String fname = PagingLandScapeOptions::getSingleton().landscape_filename  + ".HN." + 
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
    bool PagingLandScapeData2D_HeightFieldN::_load(const uint mX, const uint mZ)
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
			OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "PagingLandScapeData2D_HeightFieldN::_load" );
		}

        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
        if ( mBpp != 4 )
        {
        OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, "Error: Image is not a RGBA image.(4 bytes, 32 bits)",
            "PagingLandScapeData2D_HeightFieldN::_load" );
        }


		if ( mSize != mImage->getWidth() )
		{
			OGRE_EXCEPT ( Exception::ERR_INVALIDPARAMS, "Error: Declared World size <> Height Map Size.", "PagingLandScapeData2D_HeightFieldN::_load" );
		}
        mMax = static_cast <uint> (mSize * mImage->getHeight() * mBpp + 1);

        if (PagingLandScapeOptions::getSingleton().coverage_vertex_color)
        {
            mCoverage = new Image();
            mCoverage -> load( PagingLandScapeOptions::getSingleton().landscape_filename +
                                    ".Coverage." +
                                    StringConverter::toString( mZ ) + "." +
                                    StringConverter::toString( mX ) + "." +
                                    PagingLandScapeOptions::getSingleton().landscape_extension, PagingLandScapeOptions::getSingleton().groupName );

        }
        if (PagingLandScapeOptions::getSingleton().base_vertex_color)
        {
            mBase = new Image();
            mBase -> load( PagingLandScapeOptions::getSingleton().landscape_filename +
                            ".Base." +
                            StringConverter::toString( mZ ) + "." +
                            StringConverter::toString( mX ) + "." +
                            PagingLandScapeOptions::getSingleton().landscape_extension, PagingLandScapeOptions::getSingleton().groupName );
        }

        if (PagingLandScapeOptions::getSingleton().vertex_shadowed)
        {
            mShadow = new Image();
            mShadow -> load( PagingLandScapeOptions::getSingleton().landscape_filename +
                            ".HS." +
                            StringConverter::toString( mZ ) + "." +
                            StringConverter::toString( mX ) + "." +
                            PagingLandScapeOptions::getSingleton().landscape_extension, PagingLandScapeOptions::getSingleton().groupName );
        }

        mXDimension = mImage->getWidth();
        mZDimension = mImage->getHeight();
        mMaxArrayPos = static_cast <uint> (mSize * mImage->getHeight());
        mHeightData = new Real[mMaxArrayPos];
        uint j = 0;
        const double scale = PagingLandScapeOptions::getSingleton().scale.y / 256;
        mMaxheight = 0.0f;
        uchar *imagedata = mImage->getData();
        const uint bpp = static_cast <uint> (mBpp);
        for (uint i = 0; i < mMax - 1;  i += bpp )
            {
                const Real h =  imagedata[ i + (bpp - 1)] * scale;
                mMaxheight = std::max ( h, mMaxheight);
                mHeightData[j++] = h;
            }
        return true;
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldN::_load()
    {
	   
		    mImage = new Image();

    
            mImage->load ( PagingLandScapeOptions::getSingleton().landscape_filename +
                    "." + PagingLandScapeOptions::getSingleton().landscape_extension, PagingLandScapeOptions::getSingleton().groupName );

		    
		    //check to make sure it's 2^n size.
		    if (  !_checkSize( mImage->getHeight() ) ||	!_checkSize( mImage->getWidth() ) )
		    {
			    String err = "Error: Invalid heightmap size : " +
				    StringConverter::toString( static_cast <uint> (mImage->getWidth()) ) +
				    "," + StringConverter::toString( static_cast <uint> (mImage->getHeight()) ) +
				    ". Should be 2^n";
			    OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "PagingLandScapeData2D_HeightFieldN::_load" );
		    }

            mBpp = static_cast <uint> (PixelUtil::getNumElemBytes (mImage->getFormat ()));
		    if (mBpp != 1)
		    {
			    OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, "Error: Image is not a greyscale image.(1 byte, 8 bits)",
                    "PagingLandScapeData2D_HeightFieldN::_load" );
		    }

            mXDimension = mImage->getWidth();
            mZDimension = mImage->getHeight();
		    mSize = mImage->getWidth();
            mMax = static_cast <uint> (mSize * mImage->getHeight() * mBpp);

            mMaxArrayPos = static_cast <uint> (mSize * mImage->getHeight());
            mHeightData = new Real[mMaxArrayPos];

            const double scale = PagingLandScapeOptions::getSingleton().scale.y / 256;
            mMaxheight = 0.0f;
            uchar *imagedata = mImage->getData();
            for (uint i = 0; i < mMax;  i ++ )
            {
                const Real h =  imagedata[i] * scale;
                mMaxheight = std::max ( h, mMaxheight);
                mHeightData[i] = h;
            }
	   
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldN::_unload()
    {
	    delete mImage;
	    mImage = 0;
        delete mCoverage;
        mCoverage = 0;
        delete mBase;
        mBase = 0;
        delete mShadow;
        mShadow = 0;
    }
} //namespace
