/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldNTC.cpp  -  description
  -------------------
  begin                : Mon Oct 13 2003
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

#include "OgreLogManager.h"

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreImage.h"
#include "OgreStringConverter.h"

#include "OgreException.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2D_HeightFieldNTC.h"


#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldNTC::newPage()
    {
       return new PagingLandScapeData2D_HeightFieldNTC(mParent);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldNTC::PagingLandScapeData2D_HeightFieldNTC(PagingLandScapeData2DManager *dataMgr)
    : PagingLandScapeData2D(dataMgr)
    {
	    mImage = 0;
	    input_max = 3000.0f;
        input_min = 0.0f;
        mMaxheight = _decodeTC (1.0f) * mParent->getOptions()->scale.y;
    }

    //-------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightFieldNTC::getMaxAbsoluteHeight(void) const
    { 
        return _decodeTC (1.0f) * mParent->getOptions()->scale.y;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldNTC::~PagingLandScapeData2D_HeightFieldNTC()
    {
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
            if (mImage)
            {
                unsigned int Pos = static_cast<unsigned int> ((z * mSize  + x) * mBpp);//4 bytes (mImage is RGBA)

                if (mMax > Pos)
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
       
            const Real scale = 1.0f  / mParent->getOptions()->scale.y;
           
            uchar *img = mImage->getData();
            unsigned int j = 0;
            const unsigned int bpp = static_cast <unsigned int> (mBpp);
            for (unsigned int i = 0; i < mMax - 1;  i += bpp)
            {
                img[ i + (mBpp - 1)] =  uchar (_encodeTC(mHeightData[j++]) * scale);               
            }
            const String fname = mParent->getOptions()->LandScape_filename + ".HN." + 
                                        StringConverter::toString(mPageZ) + "." +
			                            StringConverter::toString(mPageX) + ".";
            const String extname = mParent->getOptions()->LandScape_extension;


        FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
                mParent->getOptions()->groupName, 
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
    bool PagingLandScapeData2D_HeightFieldNTC::_load(const unsigned int mX, const unsigned int mZ)
    { 
        const String strFileName = mParent->getOptions()->LandScape_filename + ".HN." + 
                            StringConverter::toString(mZ) + "." +
			                StringConverter::toString(mX) + ".";

        String finalName = strFileName + 
                        "modif." + 
                        mParent->getOptions()->LandScape_extension;
        if (!(mParent->getOptions()->Deformable && 
            ResourceGroupManager::getSingleton().resourceExists(mParent->getOptions()->groupName,
            finalName)))
        {
            finalName = strFileName + 
                mParent->getOptions()->LandScape_extension;  
            if (!ResourceGroupManager::getSingleton().resourceExists(mParent->getOptions()->groupName,
                finalName))
            { 
                LogManager::getSingleton().logMessage(LML_CRITICAL, String("PLSM2 : Cannot find map named ") + finalName);
                return false;
            }
        }
        mImage = new Image();
        mImage->load (finalName,  mParent->getOptions()->groupName); 
        
        //check to make sure it's 2^n + 1 size.
        if (mImage->getWidth() != mImage->getHeight() ||	!_checkSize(mImage->getWidth()))
        {
            String err = "Error: Invalid heightmap size : " +
                StringConverter::toString(static_cast <unsigned int> (mImage->getWidth())) +
                "," + StringConverter::toString(static_cast <unsigned int> (mImage->getHeight())) +
                ". Should be 2^n+1, 2^n+1";
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, err, "PagingLandScapeData2D_HeightField::_load");
        }

        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
        if (mBpp != 4)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Error: Image is not a RGBA image.(4 bytes, 32 bits)", 
                "PagingLandScapeData2D_HeightField::_load");
        }

        if (mSize != mImage->getWidth())
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS, "Error: Declared World size <> Height Map Size.", "PagingLandScapeData2D_HeightField::_load");
        }

        mXDimension = mImage->getWidth();
        mZDimension = mImage->getHeight();
        mMax = static_cast <unsigned int> (mSize * mImage->getHeight() * mBpp + 1);

        mMaxArrayPos = static_cast <unsigned int> (mSize * mImage->getHeight());
        mHeightData = new Real[mMaxArrayPos];
        unsigned int j = 0;
        const double divider = 1.0 / 255;
        const Real scale  = mParent->getOptions()->scale.y;
        uchar *data = mImage->getData();
        mMaxheight = 0.0f;
        const unsigned int bpp = static_cast <unsigned int> (mBpp);
        for (unsigned int i = 0; i < mMax - 1;  i += bpp)
            {  
                const Real h = _decodeTC(data[ i + (mBpp - 1)] * divider)* scale;
                mMaxheight = std::max (h, mMaxheight);
                mHeightData[j++] = h;
            }
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldNTC::_load()
    {
	   
		    mImage = new Image();

		    mImage->load(mParent->getOptions()->LandScape_filename + 
                "." + mParent->getOptions()->LandScape_extension, 
                    mParent->getOptions()->groupName);

		    //check to make sure it's 2^n size.
		    if (!_checkSize(mImage->getHeight()) ||	!_checkSize(mImage->getWidth()))
		    {
			    String err = "Error: Invalid heightmap size : " +
				    StringConverter::toString(static_cast <unsigned int> (mImage->getWidth())) +
				    "," + StringConverter::toString(static_cast <unsigned int> (mImage->getHeight())) +
				    ". Should be 2^n, 2^n";
			    OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, err, "PagingLandScapeData2D_HeightFieldNTC::_load");
		    }

            mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
            if (mBpp != 1)
		    {
			    OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Error: Image is not a grayscale image.(1 byte, 8 bits)", 
                    "PagingLandScapeData2D_HeightFieldNTC::_load");
		    }

            mSize = static_cast <unsigned int> (mImage->getWidth());
            mMax = static_cast <unsigned int> (mSize * mImage->getHeight() * mBpp + 1);

            mMaxArrayPos = static_cast <unsigned int> (mSize * mImage->getHeight());
            mHeightData = new Real[mMaxArrayPos];
            unsigned int j = 0;
            const double divider = 1.0 / 255;
            const Real scale  = mParent->getOptions()->scale.y;
            uchar *data = mImage->getData();
            mMaxheight = 0.0f;
            const unsigned int bpp = static_cast <unsigned int> (mBpp);
            for (unsigned int i = 0; i < mMax - 1;  i += bpp)
            {  
                const Real h = _decodeTC(data[ i + (mBpp - 1)] * divider)* scale;
                mMaxheight = std::max (h, mMaxheight);
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
