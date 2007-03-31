/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldTC.cpp  -  description
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

#include "OgrePagingLandScapeData2D_HeightFieldTC.h"


#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldTC::newPage()
    {
       return new PagingLandScapeData2D_HeightFieldTC(mParent);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldTC::PagingLandScapeData2D_HeightFieldTC(PagingLandScapeData2DManager *dataMgr)
    : PagingLandScapeData2D(dataMgr)
    {
	    mImage = 0;
	    input_max = 3000.0f;
        input_min = 0.0f;
        mMaxheight = _decodeTC (1.0f) * mParent->getOptions()->scale.y;
    }
    //-------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightFieldTC::getMaxAbsoluteHeight(void) const
    { 
        return _decodeTC (1.0f) * mParent->getOptions()->scale.y;
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldTC::~PagingLandScapeData2D_HeightFieldTC()
    {
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
      
            const Real scale = 1.0f  / mParent->getOptions()->scale.y;
           
            uchar *img = mImage->getData();
            unsigned int j = 0;
            for (unsigned int i = 0; i < mMax - 1;  i ++)
            {
                img[ i ] =  uchar (_encodeTC(mHeightData[j++]) * scale);               
            }
            const String fname = mParent->getOptions()->LandScape_filename + "." +
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
    bool PagingLandScapeData2D_HeightFieldTC::_load(const unsigned int mX, const unsigned int mZ)
    {
        const String strFileName = mParent->getOptions()->LandScape_filename + "." + 
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
        if (mImage->getWidth() != mImage->getHeight() ||	
            !_checkSize(mImage->getWidth()))
        {
            String err = "Error: Invalid heightmap size : " +
                StringConverter::toString(static_cast<int>(mImage->getWidth())) +
                "," + StringConverter::toString(static_cast<int>(mImage->getHeight())) +
                ". Should be 2^n+1, 2^n+1";
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, err, "PagingLandScapeData2D_HeightField::_load");
        }

        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
        if (mBpp != 1)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Error: Image is not a gray image.(1 bytes, 8 bits)", 
                "PagingLandScapeData2D_HeightField::_load");
        }

        if (mSize != mImage->getWidth())
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS, "Error: Declared World size <> Height Map Size.", "PagingLandScapeData2D_HeightField::_load");
        }

        mXDimension = mImage->getWidth();
        mZDimension = mImage->getHeight();
        mMax = static_cast <unsigned int> (mSize * mImage->getHeight() + 1);

        mMaxArrayPos = mMax - 1;
        mHeightData = new Real[mMaxArrayPos];
        unsigned int j = 0;
        const double divider = 1.0 / 255;
        const Real scale  = mParent->getOptions()->scale.y;
        uchar *data = mImage->getData();
        mMaxheight = 0.0f;
        for (unsigned int i = 0; i < mMax - 1;  i++)
            {  
                const Real h = _decodeTC(data[ i ] * divider)* scale;
                mMaxheight = std::max (h, mMaxheight);
                mHeightData[j++] = h;
            }
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldTC::_load()
    {
	  mImage = new Image();
        mImage->load (mParent->getOptions()->LandScape_filename +
                "." + mParent->getOptions()->LandScape_extension, mParent->getOptions()->groupName);
        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
		if (mBpp != 1)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Error: Image is not a greyscale image.(1 byte, 8 bits)",
                "PagingLandScapeData2D_HeightField::_load");
		}

        mXDimension = mImage->getWidth();
        mZDimension = mImage->getHeight();

        const size_t sourceHeight = mZDimension;
        const size_t sourceWidth = mXDimension;

        computePowerof2PlusOneSize ();     

		mSize = mXDimension;
        mMaxArrayPos = static_cast <unsigned int> (mXDimension * mZDimension);
        mMax = static_cast <unsigned int> (mMaxArrayPos * mBpp);
        mHeightData = new Real[mMaxArrayPos];

        const Real divider = 1.0f / 65535.0f;
        const Real scale  = mParent->getOptions()->scale.y;
        mMaxheight = 0.0f;
        const unsigned int shift_fill = static_cast <unsigned int> (mXDimension - sourceWidth);
        uchar *imagedata = mImage->getData();
        unsigned int dest_pos = 0;
        for (unsigned int i = 0; i < sourceHeight; ++i)
        {
            for (unsigned int j = 0; j < sourceWidth; ++j)
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
