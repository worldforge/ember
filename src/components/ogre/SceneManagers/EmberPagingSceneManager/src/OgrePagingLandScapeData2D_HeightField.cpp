/***************************************************************************
  OgrePagingLandScapeData2D_HeightField.cpp  -  description
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
#include "OgrePagingLandScapeData2D_HeightField.h"

#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightField::newPage()
    {
       return new PagingLandScapeData2D_HeightField(mParent);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightField::PagingLandScapeData2D_HeightField(PagingLandScapeData2DManager *dataMgr): 
    PagingLandScapeData2D(dataMgr)
    {
        mImage = 0;
        mCoverage = 0;
        mBase = 0;
        mShadow = 0;
        mMaxheight = mParent->getOptions()->scale.y;
    }
    //-------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightField::getMaxAbsoluteHeight(void) const
    { 
        return mParent->getOptions()->scale.y;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightField::~PagingLandScapeData2D_HeightField()
    {
        PagingLandScapeData2D::unload ();
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightField::getBase (const Real mX, const Real mZ)
    {
        if (mBase != 0)
        {
            uint Pos = static_cast<uint> ((mZ * mBase->getWidth() + mX) * 4);//4 bytes (mImage is RGBA)
            if (mBase->getSize () > Pos)
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
    const ColourValue PagingLandScapeData2D_HeightField::getCoverage (const Real mX, const Real mZ)
    {
        if (mCoverage != 0)
        {
            uint Pos = static_cast<uint> ((mZ * mCoverage->getWidth()  + mX) * 4);//4 bytes (mImage is RGBA)
            if (mCoverage->getSize () > Pos)
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
    const Real PagingLandScapeData2D_HeightField::getShadow (const Real mX, const Real mZ,
                                                            const bool &positive)
    {
        if (mShadow != 0)
        {
            uint Pos = static_cast<uint> ((mZ * mShadow->getWidth() + mX) * 3);//3 bytes (mImage is RGBA)
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
    const Vector3 PagingLandScapeData2D_HeightField::getNormal (const Real x, const Real z)
    {
        #ifndef _LOADEDNORM
            return PagingLandScapeData2D::getNormal(x,z);
        #else
            if (mImage)
            {
                uint Pos = static_cast<uint> ((z * mSize  + x) * mBpp);//4 bytes (mImage is RGBA)

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
    double PagingLandScapeData2D_HeightField::getScale() const
    {        
        double prescale;
        switch (mBpp)
        {
            case 1:
                prescale = mParent->getOptions()->scale.y / 255;
                break;
            case 2:
				prescale = mParent->getOptions()->scale.y / 65535;
                break;
            case 3:
				prescale = mParent->getOptions()->scale.y / 16777215;
                break;
            case 4:
				prescale = mParent->getOptions()->scale.y / 16777215;
                break;
            default:
                assert(0);
                OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS, 
                    "unrecongnized number of bpp for data src image.", 
                    "PagingLandScapeData2D_HeightField::getScale");
                break;
        }
        return prescale;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightField::_save()
    {
        const Real scale = 1.0 / getScale();
		const size_t bpp = mBpp;

		size_t j = 0;
		uchar * ogre_restrict data = mImage->getData();
		for (uint i = 0; i < mMaxArrayPos; i++)
		{             
			switch (bpp)
			{  
			case 1:
				data[j] = uchar (mHeightData[i] * scale);
			break;
			case 2:
			case 3:
			case 4:
				{
					const ushort syn = ushort  (mHeightData[i] * scale);
					#if OGRE_ENDIAN == ENDIAN_BIG
						data[j] = uchar ((syn >> 8) & 0xff);
						data[j+ 1] = uchar (syn & 0xff);
					#else
						data[j] = uchar (syn & 0xff);
						data[j+ 1] = uchar ((syn >> 8) & 0xff);
					#endif
				}
				break;
			default:
				assert(0);
				OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS, 
					"unrecognized bpp image.", 
					"PagingLandScapeData2D_HeightField::_save");
				break;
			}
			j += bpp;            
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
    bool PagingLandScapeData2D_HeightField::_load(const uint mX, const uint mZ)
    {
        const PagingLandScapeOptions * const opt = mParent->getOptions();
        const String pageName = "." + StringConverter::toString(mZ) + "." +
                                    StringConverter::toString(mX) + ".";
        const String &fileExt = opt->LandScape_extension;
        const String &groupName = opt->groupName;
        const String strFileName = opt->LandScape_filename + pageName;
        
		mImage = new Image();
        if (!(opt->Deformable && 
            ResourceGroupManager::getSingleton().resourceExists(groupName, strFileName + "modif." + fileExt)))
        {      
            if (!ResourceGroupManager::getSingleton().resourceExists(groupName, strFileName + fileExt))
            {
                LogManager::getSingleton().logMessage(
                                String("PLSM2 : Cannot find map named ") + strFileName + fileExt, 
                                LML_CRITICAL,
                                true);
                return false;
            }
            mImage->load (strFileName + fileExt, groupName); 
        }
        else
        {
            mImage->load (strFileName + "modif." + fileExt, groupName); 
        }

		//check to make sure it's 2^n + 1 size.
		if (mImage->getWidth() != mImage->getHeight() || 
            !_checkSize(mImage->getWidth()))
		{
			String err = "Error: Invalid height map size : " +
				StringConverter::toString(static_cast <uint> (mImage->getWidth())) +
				"," + StringConverter::toString(static_cast <uint> (mImage->getHeight())) +
				". Should be 2^n+1, 2^n+1";
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, err, 
                "PagingLandScapeData2D_HeightField::_load");
		}

        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());
        const uint bpp = static_cast <uint> (mBpp);
		if (mSize != mImage->getWidth())
		{
			OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS, 
                "Error: Declared World size <> Height Map Size.",  
                "PagingLandScapeData2D_HeightField::_load");
		}

        if (opt->coverage_vertex_color)
        {
            mCoverage = new Image();
            mCoverage->load (opt->LandScape_filename + ".Coverage" + pageName + fileExt, 
                                groupName);

        }
        if (opt->base_vertex_color)
        {
            mBase = new Image();
            mBase->load(opt->LandScape_filename + ".Base" + pageName + fileExt, 
                            groupName);
        }

        if (opt->vertex_shadowed)
        {
            mShadow = new Image();
            mShadow->load(opt->LandScape_filename + ".HS" + pageName + fileExt, 
                            groupName);
        }

        mXDimension = mImage->getWidth() * bpp;
        mZDimension = mImage->getHeight() * bpp;
        mMax = static_cast <uint> (mSize * mSize * bpp);
        mMaxArrayPos = static_cast <uint> (mSize * mSize);
        mHeightData = new Real[mMaxArrayPos];

        const double scale = getScale();

        mMaxheight = 0.0f;
        const uchar * const ogre_restrict imagedata = mImage->getData();
        const uint maxminusone = mMax;

        Real h;
        uint j = 0;
        Real * const ogre_restrict heightField = mHeightData;
        for (uint src_pos = 0; src_pos < maxminusone;  src_pos += bpp)
        {
            switch (bpp)
            {
            case 1:
                h = imagedata[src_pos] * scale;
                break;
			case 2:
			case 3:
			case 4:
                {
                    #if OGRE_ENDIAN == ENDIAN_BIG
                        ushort val = imagedata[src_pos] << 8;
                        val += imagedata[src_pos + 1];
                    #else
                        ushort val = imagedata[src_pos];
                        val += imagedata[src_pos + 1] << 8;
                    #endif
				    h = ((float)(val)) * scale;
                }
				break;
            default:
                assert(0);
                OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS, 
                    "unrecongnized bpp image.", 
                    "PagingLandScapeData2D_HeightField::_load");
                break;
            }
            mMaxheight = std::max (h, mMaxheight);
            heightField[j++] = h;
        }
        return true;
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightField::_load()
    {
		mImage = new Image();
        mImage->load (mParent->getOptions()->LandScape_filename +
                "." + mParent->getOptions()->LandScape_extension, mParent->getOptions()->groupName);
        mBpp = PixelUtil::getNumElemBytes (mImage->getFormat ());

        mXDimension = mImage->getWidth();
        mZDimension = mImage->getHeight();

        const size_t sourceHeight = mZDimension;
        const size_t sourceWidth = mXDimension;

        computePowerof2PlusOneSize ();     

		mSize = mXDimension;
        mMaxArrayPos = static_cast <uint> (mXDimension * mZDimension);
        mMax = static_cast <uint> (mMaxArrayPos * mBpp);
        mHeightData = new Real[mMaxArrayPos];


        const uint bpp = static_cast <uint> (mBpp);
        const double scale = getScale();

        mMaxheight = 0.0f;
        const uchar * ogre_restrict imagedata = mImage->getData();
        const uint maxminusone = mMax;
        const uint shift_fill = static_cast <uint> (mXDimension - sourceWidth);

        Real h; 
        uint dest_pos = 0;
        uint src_pos = 0;
        Real * const ogre_restrict heightField = mHeightData;
        for (uint i = 0; i < sourceHeight; ++i)
        {
            for (uint j = 0; j < sourceWidth; ++j)
            {  
                switch (bpp)
                {
                    case 1:
                        h = imagedata[src_pos] * scale;
                        break;
					case 2:
					case 3:
					case 4:
                        {
                            #if OGRE_ENDIAN == ENDIAN_BIG
                                ushort val = imagedata[src_pos] << 8;
                                val += imagedata[src_pos + 1];
                            #else
                                ushort val = imagedata[src_pos];
                                val += imagedata[src_pos + 1] << 8;
                            #endif
				            h = ((float)(val)) * scale;
                        }
                        break;
                    default:
                        assert(0);
                        OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS, 
                            "unrecongnized bpp image.", 
                            "PagingLandScapeData2D_HeightField::_load");
                        break;
                }
                //DEBUG_OUTPUT ( " Bpp " << StringConverter::toString (h)  << "\n";
                mMaxheight = std::max (h, mMaxheight);
                heightField[dest_pos++] = h;
                src_pos += bpp;
            }
            memset (&heightField[dest_pos], 0, shift_fill);
            dest_pos += shift_fill; 
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightField::_unload()
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
