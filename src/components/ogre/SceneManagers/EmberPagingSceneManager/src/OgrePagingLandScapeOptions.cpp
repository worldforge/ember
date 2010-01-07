/***************************************************************************
  OgrePagingLandScapeOptions.cpp  -  description
  -------------------
  begin                : Sun Mar 02 2003
  copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
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

#include "OgreRoot.h"
#include "OgreSceneManager.h" 
#include "OgreRenderSystem.h"
#include "OgreGpuProgramManager.h"

#include "OgreStringConverter.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"
#include "OgreConfigFile.h"

#include "OgreImage.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "OgreTechnique.h"

#include "OgreCamera.h"
#include "OgreViewport.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeTileInfo.h"

#include "fileutils.h"
#include <limits>

namespace Ogre
{

    //-----------------------------------------------------------------------
    PagingLandScapeOptions::PagingLandScapeOptions(PagingLandScapeSceneManager * scnMgr):
        mScnMgr(scnMgr), mConfig(0)
    {
      cfgGroupName = "";
      primaryCamera = 0;
      isInit = false;
	  setDefault();
    };

    //-----------------------------------------------------------------------
    PagingLandScapeOptions::~PagingLandScapeOptions()
    {
		clearTileInfo();
		delete mConfig;
	}
	//-----------------------------------------------------------------------
    void PagingLandScapeOptions::setDefault()
	{

        mUseLodMapCache = false;
		materialPerPage = false;
		textureModifiable = true;
        roughnessLod = true;    

        data2DFormat = "HeightField";
	    textureFormat = "Image";
	    
		image_filename = "";
		ImageNameLoad = false;
	    LandScape_filename = "";
	    LandScape_export_filename = "";
	    ColorMapName = "";
	    
	    groupName = cfgGroupName;
	    
	    LandScape_extension = "png";
	    LandScape_export_extension = "png";
	    TextureExtension = "png";
	    
	    maxValue = 5000;
	    minValue = 0;

	    PageSize = 513;
        TileSize = 65;
		maxRenderLevel = 100;

	    world_width = 0;
	    world_height = 0;

	    maxScaledZ = 0;
	    maxScaledX = 0;
	    maxUnScaledZ = 0;
	    maxUnScaledX = 0;

	    max_adjacent_pages = 2;
	    max_preload_pages = 3;

	    change_factor = 1;
	    distanceLOD = 4;
	    LOD_factor = 10;

		scale = Ogre::Vector3::UNIT_SCALE;
		invScale = Ogre::Vector3::UNIT_SCALE;
		position = Ogre::Vector3::ZERO;

	    num_renderables = 64;
	    num_renderables_increment = 64;
	    num_tiles = 64;
	    num_tiles_increment = 64;

        visible_renderables = 75;
	    cameraThreshold = 5;

	    num_renderables_loading = 10;
		RenderableLoadInterval = 3;

		normals = false;
	    lit = false;

        colored = false;
        vertex_shadowed = false;
        base_vertex_color = false;
        coverage_vertex_color = false;
        vertex_instant_colored = false;

        RawHeight = 0;
        RawWidth = 0;

        lightmoved = false;

		Deformable = false;
        saveDeformation = true;
		VertexCompression = false;

		BigImage = false;

        lodMorphStart = 0.2f;
        lodMorph = false;

        maxPixelError = 0;
       
        TextureStretchFactor = 1.0f;
		
		NumMatHeightSplat = 0;
		matHeight.reserve (0);
		matHeight.resize (0);
		matColor.reserve (0);
		matColor.resize (0);

		NumSplatMapToSplit = 0;
		SplatMapNames.reserve (0);
		SplatMapNames.resize (0);

        NumTextureFormatSupported = 0;
        TextureFormatSupported.reserve (0);
        TextureFormatSupported.resize (0);

		MaxLodUnderCam = false;
		VisMap = false;
		BigImage = false;

		PageInvisibleUnloadFrames = 300;
		TileInvisibleUnloadFrames = 300;

        lodMaterialDistanceList.clear();
        lodMaterialDistanceList.push_back (80000.0f);

        mResourceFilesystem.clear ();
        mResourceZip.clear();

		queryNoInterpolation = false;
		queryResolutionFactor = 0.1f;

		uninitializedHeight = std::numeric_limits<float>::min();
#ifdef _MAPSPLITTER

        Blur = 0.0f;
        Amb = 0.5f;
        Diff = 0.5f;
        
        Blur = 0.0f;
        HeightMapBlurFactor = 0.0f;
		Paged = true;
        MiniMap = false;
        BaseMap = false;
        ColorMapGenerate = false;
        ColorMapSplit = false;
        LightMap = false;
        NormalMap = false;
        HeightMap = false;
        AlphaMaps = false;
        LitBaseMap = false;
        InfiniteMap = false;
        CoverageMap = false;
        LitColorMapGenerate = false;
        LitColorMapSplit = false;
        HeightNormalMap = false;   
        Equalize = false;
        ZHorizon = false;
        SRTM_water = false;
        
#endif
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::init()
    {
        // sets options that doesn't depend on maps.
        if (!isInit)
        {
            const RenderSystem *renderer = Root::getSingleton().getRenderSystem();
            if (renderer)
            {
                const RenderSystemCapabilities* caps = renderer->getCapabilities();
				hasVertexShader = caps->hasCapability(RSC_VERTEX_PROGRAM) && caps->isShaderProfileSupported ("vs_1_0");
				hasVertexShader = caps->hasCapability(RSC_BLENDING) && caps->isShaderProfileSupported("vs_1_0");
                hasFragmentShader = caps->hasCapability(RSC_FRAGMENT_PROGRAM);
//                 const String &maxShaderVersion = caps->getMaxFragmentProgramVersion ();
                hasFragmentShader2 = hasFragmentShader && caps->isShaderProfileSupported("ps_1_1") && caps->isShaderProfileSupported("ps_1_0");
                numTextureUnits = caps->getNumTextureUnits ();
                isRenderGL = StringUtil::startsWith(renderer->getName(), "OpenGL", false);
                isInit = true;
            }
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::loadcfg (const String &filename, ConfigFile& config)
    {
        std::ifstream fs;
        fs.open(filename.c_str(), std::ios::in | std::ios::binary);
        if (fs)
        {
            // Wrap as a stream
            DataStreamPtr stream(
                OGRE_NEW FileStreamDataStream(filename, &fs, false));

	        config.load(stream);

        } 
        else
        {
            // otherwise try resource system
            DataStreamPtr stream = 
                ResourceGroupManager::getSingleton().openResource(filename);

	        config.load(stream);
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::load(const String &filename)
    {
	    ConfigFile config;
        loadcfg(filename, config);
        return load(filename, config);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::load(DataStreamPtr &stream)
    {
	    ConfigFile config;
	    config.load(stream);
        return load(stream->getName (), config);
    }
    //-----------------------------------------------------------------------
    bool  PagingLandScapeOptions::load(const String &filename, ConfigFile& config)
    {
    	#if 0
        init();

	    // Set up the options :  
        // List of map associated with map names.
        // or directly a map.


        if (config.getSetting ("DefaultMap").empty())
        {
            mConfig = &config;
            mCurrentMap = filename;
            loadMapOptions (filename);
            return true;
        }
        else
        {
            // Go through all sections & settings in the file
            ConfigFile::SettingsIterator setIt = config.getSettingsIterator();
            while (setIt.hasMoreElements())
            {  
                const String name = setIt.peekNextKey();
                const String value = setIt.getNext();
                mMapList.insert(LandScapeFileNames::value_type(name, value));
            }

            LandScapeFileNames::iterator i = mMapList.find("GroupName");
            if (i == mMapList.end())
            {
                OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                    "You need to define a GroupName where to find the map definition file ",
                    "PagingLandScapeOptions::load");
            }    
            cfgGroupName = i->second;
            mMapList.erase (i);


            i = mMapList.find("BatchMode");
            if ((i != mMapList.end()) && (i->second == "yes"))
            {
                mBatchMode = true;
                mMapList.erase (i);
            }    
            else
            {
                mBatchMode = false;
            }

            i = mMapList.find("TextureFormatDebug");
            if (i != mMapList.end())
            {       
                TextureFormatDebug = config.getSetting("TextureFormatDebug") == "yes";
                mMapList.erase (i);
            }



            i = mMapList.find("DefaultMap");
            if (i == mMapList.end()) 
            {
                if( mBatchMode == false)
                {
                    OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                        "You need to define a DefaultMap= ",
                        "PagingLandScapeOptions::load");
                }
            }
            else
            {
                mCurrentMap = i->second; 
            }
            if (i != mMapList.end()) 
                mMapList.erase (i);


#ifndef _MAPSPLITTER
			ResourceGroupManager * const rgsm = ResourceGroupManager::getSingletonPtr();
			
			rgsm->initialiseResourceGroup (cfgGroupName);

            LandScapeFileNames::iterator iend = mMapList.end();
            LandScapeFileNames::iterator itCheck = mMapList.begin();
            const String cfgExt(".cfg");
            for (; itCheck != iend; )
            {
                const String mapName (itCheck->first);
                const String mapFileName (itCheck->second);
                if ( 
                    !rgsm->resourceExists(cfgGroupName, mapFileName) 
                    &&
                    !rgsm->resourceExists(cfgGroupName, mapFileName + cfgExt) 
                    &&
                    !rgsm->resourceExists(cfgGroupName, mapName + cfgExt)
                    &&
                    !rgsm->resourceExists(cfgGroupName, mapName)
                    )
                {
                    mMapList.erase (itCheck++);
                }
                else
                {
                    ++itCheck;
                }
            }
#endif //_MAPSPLITTER

            if (!mBatchMode && !StringUtil::startsWith(mCurrentMap, "none", true))
            {
                loadMap(mCurrentMap);
                return true;
            }
        }
        #endif

        return false;
    }
    //-----------------------------------------------------------------------
    const String &PagingLandScapeOptions::getCurrentTextureFormat() const
    {
        return textureFormat;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::setTextureFormat (const String &format) 
    {
        if (TextureFormatSupported.end() == std::find(TextureFormatSupported.begin(),
                                                 TextureFormatSupported.end(), 
                                                 format))
        {
            TextureFormatSupported.push_back (format);
        }
        textureFormat = format;
        // sort of debugging hack til BigImage itself becomes a chunked Lod thing.
        BigImage = BigImage && textureFormat == "Image";
	}   
	//-----------------------------------------------------------------------
	void PagingLandScapeOptions::insertTextureFormat (const String &format) 
	{
		if (TextureFormatSupported.end() == std::find(TextureFormatSupported.begin(),
			TextureFormatSupported.end(), 
			format))
		{
			TextureFormatSupported.push_back (format);
		}
	}   
	
    //-----------------------------------------------------------------------
    LandScapeFileNames& PagingLandScapeOptions::getMapList() 
    {
        return mMapList;
    }
    //-----------------------------------------------------------------------
    const String& PagingLandScapeOptions::getPreviousMapName() const
    {
        if (!mMapList.empty ())
        {
            if (StringUtil::startsWith(mCurrentMap, "none", true))
                return mMapList.rbegin()->first;
            LandScapeFileNames::const_reverse_iterator iend = mMapList.rend();
            LandScapeFileNames::const_reverse_iterator i = mMapList.rbegin();
            for (; i != iend; ++i)
            {
                if (i->first == mCurrentMap || i->second == mCurrentMap)
                {   
                    ++i;
                    if (i == iend)
                        i = mMapList.rbegin();
                    return i->first;
                }
            }
        }
        return StringUtil::BLANK;
    }
    //-----------------------------------------------------------------------
    const String& PagingLandScapeOptions::getNextMapName() const
    {  
        if (!mMapList.empty ())
        {
            if (StringUtil::startsWith(mCurrentMap, "none", true))
                return mMapList.begin()->first;

            LandScapeFileNames::const_iterator iend = mMapList.end();
            LandScapeFileNames::const_iterator i = mMapList.begin();
            for (; i != iend; ++i)
            {
                if (i->first == mCurrentMap || i->second == mCurrentMap)
                {   
                    ++i;
                    if (i == iend)
                        i = mMapList.begin();
                    return i->first;
                }
            }
        }
        return StringUtil::BLANK;
    }
    //-----------------------------------------------------------------------
    const String& PagingLandScapeOptions::getCurrentMapName() const
    {
        return mCurrentMap;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::setCurrentMapName(const String& mapName)
    {
        if (StringUtil::BLANK != mapName)
		{
			LandScapeFileNames::iterator ifind = mMapList.find (mapName);
			if (ifind == mMapList.end())
			{
				mMapList.insert(LandScapeFileNames::value_type(mapName, mapName));
			}
			mCurrentMap = mapName;
        }
		loadMap(mapName);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::loadMap(const String& mapName)
    {
    #if 0
        ConfigFile config;
        mConfig = &config;
        const String fileMapName (getMapFilename(mapName) + 
            (StringUtil::endsWith(mapName, ".cfg", true) ? StringUtil::BLANK : String(".cfg")));
        if (!cfgGroupName.empty ())
        {
            std::vector<Ogre::String>::iterator itFileSystem = mResourceFilesystem.begin();
            for(; itFileSystem != mResourceFilesystem.end(); ++itFileSystem) 
            {
                ResourceGroupManager::getSingleton().removeResourceLocation(
                    *itFileSystem, cfgGroupName);
            }
            std::vector<Ogre::String>::iterator itZip = mResourceZip.begin();
            for(; itZip != mResourceZip.end(); ++itZip) 
            {
                ResourceGroupManager::getSingleton().removeResourceLocation(
                    *itZip, cfgGroupName);
            }
            // Set up the options For a Map
            mConfig->load (fileMapName, cfgGroupName, String("="), true);
        }
        else
        {
            mConfig->load (fileMapName, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, String("="), true);
        }
        /* Set up the options For a Map*/
	    loadMapOptions (fileMapName);
	#endif
	}
	//-----------------------------------------------------------------------
	const String &PagingLandScapeOptions::getMapFilename(const String &currMapName) const
	{
		LandScapeFileNames::const_iterator i = mMapList.find(currMapName);
		LandScapeFileNames::const_iterator iEnd = mMapList.end();
		if (i != iEnd)
			return i->second;


		for (i = mMapList.begin(); i != iEnd; ++i)
		{
			if (StringUtil::match(i->second, currMapName, false) || 
				StringUtil::match(i->first, currMapName, false))
			{
				return i->second;
			}
		}			
        return currMapName;
	}
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::insertMap(const String& mapName)
    {        
        LandScapeFileNames::iterator ifind = mMapList.find (mapName);
        if (ifind == mMapList.end())
        {
            mMapList.insert(LandScapeFileNames::value_type(mapName, mapName));
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::setUint (unsigned int &u, const String &ValuetoGet)
	{	
		const String valString = mConfig->getSetting(ValuetoGet);
		if (!valString.empty())
		{
            u = static_cast<unsigned int> (StringConverter::parseReal(valString));
            return true;
        }
        return false;
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::setBool (bool &b, const String &ValuetoGet)
	{
		const String valString = mConfig->getSetting (ValuetoGet);
		if (!valString.empty())
		{
            b = StringUtil::startsWith(valString, "yes", true);
            return true;
        }
        return false;
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::setReal (Real &r, const String &ValuetoGet)
	{
		const String valString = mConfig->getSetting(ValuetoGet);
		if (!valString.empty())
		{
			const Ogre::Real val = StringConverter::parseReal(valString);
            r = val;
            return true;
        }
        return false;
	}
	//-----------------------------------------------------------------------
	bool PagingLandScapeOptions::setColourValue(ColourValue &r, const String &ValuetoGet)
	{
		const String valString = mConfig->getSetting(ValuetoGet);
		if (!valString.empty())
		{
			const ColourValue val = StringConverter::parseColourValue(valString);
            r = val;
            return true;
        }
        return false;
	}
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::setString (String &s, const String &ValuetoGet)
    {
        const String val = mConfig->getSetting (ValuetoGet);
        if (!val.empty())
        {
            s = val;
            return true;
        }
        return false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::loadMapOptions(DataStreamPtr &stream)
    {
		unsigned int i;
		
        init ();
		setDefault ();

		///By Ember start
	    // Set up the options For a Map
	    mConfig = new ConfigFile();
	    mConfig->load (stream);
		///By Ember end


	    // Set up the dimensions For a Map
        #if !defined(_MAPSPLITTER) && !defined(_MAPEDITOR)

	        setUint (world_height, "Height");
	        setUint (world_width, "Width");
            if (world_width == 0 || world_height == 0)
            {
                OGRE_EXCEPT(1, 
						    "Your map must at least have a width and height defined in the config file ", 
						    "PagingLandScapeOptions::loadMapOptions");
            }
        #endif // !defined(_MAPSPLITTER) && !defined(_MAPEDITOR)


	    setUint (PageSize, "PageSize");
        setUint (TileSize, "TileSize");

        invPageSizeMinusOne = 1.0 / (PageSize - 1);	
        invTileSizeMinusOne = 1.0 / (TileSize - 1);	

        NumTiles = static_cast<unsigned int> (static_cast <Real> (PageSize) / (TileSize - 1));
        NumPages = static_cast<unsigned int> (static_cast <Real> (world_height * world_width));
        const unsigned int totalNumTiles = NumPages * (NumTiles*NumTiles) + 1;
    	    
		///EMBER: added for ember, since we never want the scale to be different from the page size
        scale.x = scale.y = scale.z = PageSize - 1;
// 	    setReal (scale.x, "ScaleX");
//         setReal (scale.y, "ScaleY");
//         setReal (scale.z, "ScaleZ");

	    setReal( position.x, "PositionX");
	    setReal( position.y, "PositionY");
	    setReal( position.z, "PositionZ");
	
		// secret tweak helping joining tile using vertex compression
		// leads to texture distortion at page breaks otherwise.
		const double scalemodif = 1.0f;//static_cast <double> (PageSize - 1) / PageSize;
        ScaledPageSizeX = scale.x * scalemodif;
        ScaledPageSizeZ = scale.z * scalemodif;
        ScaledHeightY = scale.y / 65535;

        setReal (TextureStretchFactor,  "TextureStretchFactor");

#if !defined(_MAPSPLITTER) && !defined(_MAPEDITOR)

        // Scale x/z relative to page size
        scale.x /= (PageSize - 1);
        scale.z /= (PageSize - 1);    

#else// defined(_MAPSPLITTER) || defined(_MAPEDITOR)
        //scale.y = scale.y;
        scale.x /= (PageSize - 1) * TextureStretchFactor;
        scale.z /= (PageSize - 1) * TextureStretchFactor;    

#endif// defined(_MAPSPLITTER) || defined(_MAPEDITOR)

		invScale.x = 1 / scale.x;
		invScale.y = 1 / scale.y;
		invScale.z = 1 / scale.z;

	    maxUnScaledZ = world_height * (PageSize - 1) * 0.5f;
	    maxUnScaledX = world_width  * (PageSize - 1) * 0.5f;

	    maxScaledZ = scale.z * maxUnScaledZ;
	    maxScaledX = scale.x * maxUnScaledX;


	    // Set up the names For a Map

        setString (groupName, "GroupName");
        
        if (!setString (LandScape_filename, "HeightMapFileName"))
            setString (LandScape_filename, "LandScapeFileName");

        if (!setString (TerrainName, "TerrainName"))
            TerrainName = LandScape_filename;    

		///By Ember start, we don't need this for Ember
        #if 0
        //add Resources Group to Ogre if needed.
		StringVector mResourceFilesystem =  mConfig->getMultiSetting("FileSystem");
        std::vector<Ogre::String>::iterator itFileSystem = mResourceFilesystem.begin();
        for(; itFileSystem != mResourceFilesystem.end(); ++itFileSystem) 
        {
			String resourceFileSystem = *itFileSystem;
			String BasePath, FilePath;
			StringUtil::splitFilename(resourceFileSystem, BasePath, FilePath);
            if (StringUtil::endsWith  (BasePath, "terrainname", true))
            {
                BasePath = TerrainName;
            }
            else if (StringUtil::endsWith  (BasePath, "landscapefilename", true))
            {
                BasePath = LandScape_filename;
            }
            else if (StringUtil::endsWith  (BasePath, "landscapeexportfilename", true))
            {
                BasePath = LandScape_export_filename;
            }

			if (!cfgGroupName.empty() && FilePath.empty () && !mapName.empty())
			{
				FileInfoListPtr finfo = ResourceGroupManager::getSingleton().findResourceFileInfo(
                    cfgGroupName, 
                    mapName);
				FilePath = (finfo->begin() != finfo->end())? (finfo->begin())->archive->getName() : "."; 
            }
            if (!FilePath.empty ())
            {
               
                FilePath = FilePath + "/"; 
            } 

            ResourceGroupManager::getSingleton().addResourceLocation(
                 FilePath + BasePath, "FileSystem", groupName);
        }

        StringVector mResourceZip =  mConfig->getMultiSetting("Zip");
        std::vector<Ogre::String>::iterator itZip = mResourceZip.begin();
        for(; itZip != mResourceZip.end(); ++itZip) 
        {
            String resourceZip  = *itZip;

            if (resourceZip != StringUtil::BLANK)
            {
                ResourceGroupManager::getSingleton().addResourceLocation(
                    resourceZip, "Zip", groupName);
            }
        }
		#endif
		///By Ember end

        if (!setString (LandScape_filename, "HeightMapFileName"))
            setString (LandScape_filename, "LandScapeFileName");

	    if (!setString (LandScape_export_filename, "LandScapeExportFileName"))
            LandScape_export_filename = LandScape_filename;
            
        if (!setString (LandScape_extension, "HeightMapExtension"))
            setString (LandScape_extension, "LandScapeExtension");

        if (!setString (LandScape_export_extension, "HeightMapExportExtension"))
            setString (LandScape_export_extension, "LandScapeExportExtension");
                
                    
        if (!setString (TextureExtension, "ImageExtension"))
            if (!setString (TextureExtension, "ColorMapExtension"))
                setString (TextureExtension, "TextureExtension");

        if (!setString (TextureExportExtension, "ImageExportExtension"))
            if (!setString (TextureExportExtension, "ColorMapExportExtension"))
                setString (TextureExportExtension, "TextureExportExtension");

        ImageNameLoad = setString (image_filename, "ImageFilename");
        if (!ImageNameLoad)
            ImageNameLoad = setString (image_filename, "ColorMapFileName");
      
	    // Set up the data source For a Map
	    
	    setString (data2DFormat, "Data2DFormat");
            
        bool HeightField = StringUtil::endsWith(data2DFormat, "HeightField", false);
        bool HeightFieldN = StringUtil::endsWith(data2DFormat, "HeightFieldN", false);
        bool HeightFieldTC = StringUtil::endsWith(data2DFormat, "HeightFieldTC", false);
        bool HeightFieldNTC = StringUtil::endsWith(data2DFormat, "HeightFieldNTC", false);
        bool HeightFieldRawTC = StringUtil::endsWith(data2DFormat, "HeightFieldRawC", false);
        bool HeightFieldRaw = StringUtil::endsWith(data2DFormat, "HeightFieldRaw", false);

    
        if (StringUtil::endsWith(LandScape_extension, "raw", true))
        {
            isRaw = true;
            setUint (RawHeight, "RawHeight");
            setUint (RawWidth, "RawWidth");

            if (HeightField || HeightFieldN)
            {
                data2DFormat = "HeightFieldRaw";
                HeightField   = false;
                HeightFieldN = false;
                HeightFieldRaw = true;
            }
            else if (HeightFieldTC || HeightFieldNTC) 
            {
                data2DFormat = "HeightFieldRawTC";
                HeightFieldTC   = false;
                HeightFieldNTC = false;
                HeightFieldRawTC = true;
            }
        }
        else 
            isRaw = false;

	    if (HeightFieldRawTC 
            || HeightFieldTC 
            || HeightFieldNTC)
        {
            setUint (maxValue, "MaxValue");
            maxValue *= static_cast<unsigned int>(scale.y);
            setUint (minValue, "MinValue");
            minValue *= static_cast<unsigned int>(scale.y);
        }
        else
        {
            maxValue = static_cast<unsigned int> (scale.y);
            minValue = 0;
        }

		setBool (materialPerPage, "MaterialPerPage");
        setBool (textureModifiable, "TextureModifiable");
        setBool(mUseLodMapCache, "UseLodMapCache");

        setBool(roughnessLod, "RoughnessLod");



	    // Set up the splatting options For a Map
	    
		setUint (NumMatHeightSplat, "NumMatHeightSplat");
		matHeight.reserve (NumMatHeightSplat);
		matHeight.resize (NumMatHeightSplat);
		matColor.reserve (NumMatHeightSplat);
		matColor.resize (NumMatHeightSplat);
		SplatDetailMapNames.reserve (NumMatHeightSplat);
		SplatDetailMapNames.resize (NumMatHeightSplat);	
		
		// Init to 0.0f for lazy config files omitting some.
		for (i = 0; i < NumMatHeightSplat; i++)
		{			
			matHeight[i] = 0.0f;
		}
        const Ogre::Real divider = (maxValue - minValue) / (100.0f);
		for (i = 0; i < NumMatHeightSplat; i++)
		{
			setReal (matHeight[i], 
					"MaterialHeight" + StringConverter::toString(i));
			matHeight[i] *= divider;
		}

        const String baseSplatName ("SplatFilename");
        for (i = 0; i < NumMatHeightSplat; i++) 
            setString (SplatDetailMapNames[i], 
						baseSplatName + StringConverter::toString (i));
        getAvgColors ();
        
       Ogre::Real MaterialDistanceLod = 0.0f;   
        setReal (MaterialDistanceLod,"MaterialDistanceLod");
        if (MaterialDistanceLod != 0.0f)
        {
            lodMaterialDistanceList.clear();
            lodMaterialDistanceList.push_back (MaterialDistanceLod);
        }

#ifndef _MAPSPLITTER

        // PLUGIN OPTIONS ONLY

	    // Set up the vertexes options For a Map
	    
        setBool (colored, "VertexColors");
        setBool (coverage_vertex_color, "CoverageVertexColor");
        setBool (base_vertex_color, "BaseVertexColor");
        setBool (vertex_shadowed, "BaseVertexShadow");
        setBool (vertex_instant_colored, "BaseVertexInstantColor");
        
        // ensure combination are correct 
        if (vertex_shadowed)
            vertex_instant_colored = true;

        if (coverage_vertex_color || base_vertex_color
          )// || vertex_shadowed || vertex_instant_colored)
            colored = true;
           
	    // Set up the paging options For a Map
	    
	    setUint (num_renderables, "MaxNumRenderables");
        num_renderables = (totalNumTiles < num_renderables)?totalNumTiles:num_renderables;
	    setUint (num_renderables_increment, "IncrementRenderables");

	    setUint (num_tiles, "MaxNumTiles");
        num_tiles = (totalNumTiles < num_tiles)? totalNumTiles : num_tiles;
	    setUint (num_tiles_increment, "IncrementTiles");
       

	    setUint (num_renderables_loading, "NumRenderablesLoading");
		setUint (RenderableLoadInterval, "RenderableLoadInterval");

	    setUint (max_adjacent_pages, "MaxAdjacentPages");
	    setUint (max_preload_pages, "MaxPreloadedPages");

	    // Set up the LOD options For a Map
        setUint (maxRenderLevel, "MaxRenderLevel");
        if (maxRenderLevel == 100)
        {
			maxRenderLevel = 0;
			const unsigned int halftileSize = static_cast<unsigned int>(TileSize * 0.5f);
            while (static_cast<unsigned int> ((1 << maxRenderLevel)) < halftileSize )
                maxRenderLevel++;
        }


        setReal (change_factor,  "ChangeFactor");
	    change_factor *=  (static_cast <Real> (PageSize * (scale.z + scale.x) * 0.5f) / 9);

	    // Set up the distance options For a Map
	    
	    setReal (visible_renderables, "VisibleRenderables");

	    // compute the actual distance as a square
        // Factor is a Tile distance squared
        const Ogre::Real Factor = TileSize * scale.x * TileSize * scale.z;


        // max distance upon which renderables are not rendered anymore
	    renderable_factor = visible_renderables * Factor;
	    //renderable_factor *= renderable_factor;

		//setReal (cameraThreshold,  "CameraThreshold");
		// To avoid the use of a square root.
		//cameraThreshold *= cameraThreshold;
		cameraThreshold = ((scale.x < scale.z ) ? TileSize * scale.x : TileSize * scale.z) * 0.25;

	    setReal (distanceLOD, "DistanceLOD");
	    // Compute the actual distance as a square
	    LOD_factor = distanceLOD * Factor;
		loadMapInfo();

        setBool (lit, "VertexLit");
        setBool (normals, "VertexNormals");
        
        setBool (Deformable, "Deformable");
        if (!Deformable)
            saveDeformation = false;
        else
            setBool (saveDeformation, "SaveDeformation");
            
        //Morphing
        if (maxRenderLevel > 1)
        {
            setBool (lodMorph, "VertexProgramMorph");
            setReal (lodMorphStart, "LODMorphStart");
        }            
        setUint (maxPixelError, "MaxPixelError");


        setBool (BigImage, "BigImage");
        setBool (VisMap, "HorizonVisibilityComputing");
		setBool (MaxLodUnderCam, "MaxLodUnderCam");
        setBool (VertexCompression, "VertexCompression");
        VertexCompression = VertexCompression  && hasVertexShader;


        setUint (NumTextureFormatSupported, "NumTextureFormatSupported");
        TextureFormatSupported.reserve (NumTextureFormatSupported);
        TextureFormatSupported.resize (NumTextureFormatSupported);
        for (i = 0; i < NumTextureFormatSupported; i++)
        {
            setString (TextureFormatSupported[i], "TextureFormatSupported" + StringConverter::toString(i));
        }
        String tempTexformat;
        setString (tempTexformat, "TextureFormat");
        // must be after getting supported formats
        setTextureFormat (tempTexformat);

		setUint (TileInvisibleUnloadFrames, "TileInvisibleUnloadFrames");
		setUint (PageInvisibleUnloadFrames, "PageInvisibleUnloadFrames");
		
		
	    setReal (BaseCameraViewpoint.x, "BaseCameraViewpoint.x");
        setReal (BaseCameraViewpoint.y, "BaseCameraViewpoint.y");
        setReal (BaseCameraViewpoint.z, "BaseCameraViewpoint.z");
        
	    setReal (Baselookat.x, "Baselookat.x");
        setReal (Baselookat.y, "Baselookat.y");
        setReal (Baselookat.z, "Baselookat.z");


		ResourceGroupManager::getSingleton().initialiseResourceGroup (groupName);
#else 
        
        // MAP TOOL OPTIONS ONLY

        setBool (Paged, "Paged");

        setString (OutDirectory, "OutDirectory");

		String BasePath, FilePath;
		StringUtil::splitFilename(OutDirectory, BasePath, FilePath);
	    if (StringUtil::endsWith  (BasePath, "landscapefilename", true))
        {
            BasePath = LandScape_filename;
        }
	    if (StringUtil::endsWith  (BasePath, "landscapeexportfilename", true))
		{
			BasePath = LandScape_export_filename;
		}
		if (FilePath.empty())
		{
			//Get cfg current Directory

			FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
				cfgGroupName, mapName);
			FileInfoList::iterator it = finfo->begin();
			if (it != finfo->end())
			{
				FilePath = (it)->archive->getName();
			}
		}
		if (StringUtil::endsWith  (FilePath, "/", true))
		{
			FilePath.resize (FilePath.size() - 1);
		}
		OutDirectory = FilePath + "/" +  BasePath;

        setBool (MiniMap, "MiniMap");
        setBool (BaseMap, "BaseMap");


        setBool (ColorMapGenerate, "ColorMapGenerate");

        if (!setBool (ColorMapSplit, "ImageSplit"))
            setBool (ColorMapSplit, "ColorMapSplit");

        setBool (LightMap, "LightMap");
        setBool (NormalMap, "NormalMap");
        setBool (HeightMap, "HeightMap");
        setBool (AlphaMaps, "AlphaMaps");
        setBool (LitBaseMap, "LitBaseMap");
        setBool (InfiniteMap, "InfiniteMap");
        setBool (CoverageMap, "CoverageMap");
        setBool (LitColorMapGenerate, "LitColorMapGenerate");

        if (!setBool (LitColorMapSplit, "LitImageSplit"))
            setBool (LitColorMapSplit, "LitColorMapSplit");

        setBool (HeightNormalMap, "HeightNormalMap");

        if (!setString (ColorMapName, "ImageFileName"))
            if (!setString (ColorMapName, "ColorMapFileName"))
                setString (ColorMapName, "ColorMapName");

		if ((BaseMap || CoverageMap || AlphaMaps)
			&& NumMatHeightSplat == 0)
			OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS, 
			"Generating a texture set from a heightmap needs some MaterialHeights in cfg file.", 
			"PagingLandScapeData2D_HeightField::getScale");

        setReal (HeightMapBlurFactor, "HeightMapBlurFactor");

	    setReal (Sun.x, "Sunx");
        setReal (Sun.y, "Suny");
        setReal (Sun.z, "Sunz");


        setReal (Amb, "Ambient");
        setReal (Diff, "Diffuse");
        setUint (Blur, "Blur");

        setBool (Equalize, "Equalize");
        setBool (ZHorizon, "ZHorizon");

        setBool (SRTM_water, "SRTM_water");

        setUint (MiniMapHeight, "MiniMapHeight");
        setUint(MiniMapWidth, "MiniMapWidth");


		setUint (NumSplatMapToSplit, "NumSplatMapToSplit");
		SplatMapNames.reserve (NumSplatMapToSplit);
		SplatMapNames.resize (NumSplatMapToSplit);
		
		for (i = 0; i < NumSplatMapToSplit; i++)
		{
			setString (SplatMapNames[i], "SplatMapName" + StringConverter::toString(i));
		}
    #endif
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::calculateCFactor()
    {
        // ConstOgre::Real A = 1 / Math::Tan(Math::AngleUnitsToRadians(opts.primaryCamera->getFOVy()));
		// Turn off detail compression at higher FOVs
        const Ogre::Real A = 1.0f;

		assert (primaryCamera);
        const Viewport *v = primaryCamera->getViewport();
        if (v)
        {
            const int vertRes = v->getActualHeight();
            
            assert (vertRes != 0);

            const Ogre::Real T = 2 * static_cast <Ogre::Real > (maxPixelError) / vertRes;
            
			if (T != 0)
                CFactor =  A / T;
            else
                CFactor =  A * vertRes * 0.5f;
        }
    }  
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::setPrimaryCamera(PagingLandScapeCamera *cam)
    {
        primaryCamera = cam;
		if (cam && cam->getViewport())
			calculateCFactor (); 
    } 
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::setOption(const String& strKey, const void* pValue)
    {
        if (strKey == "saveDeformation")
        {
            saveDeformation = * static_cast < const bool * > (pValue); 
            return true;
        }
        if (strKey == "MaxLodUnderCam")
        {
            MaxLodUnderCam = * static_cast < const bool * > (pValue); 
            return true;
        }
// 	    if (strKey == "VisibleRenderables")
// 	    {
// 		    visible_renderables = * static_cast < const int * > (pValue);
// 		    // compute the actual distance as a square
//            Ogre::Real Factor = TileSize;
//             Factor = Factor * scale.x * Factor * scale.z;
// 
// 	        renderable_factor = visible_renderables * Factor;
//             return true;
// 	    }
	    if (strKey == "DistanceLOD")
	    {
		    distanceLOD = * static_cast < const Ogre::Real * > (pValue);
		    // Compute the actual distance as a square
           Ogre::Real Factor = TileSize;
            Factor = Factor * scale.x * Factor * scale.z;
	        LOD_factor = distanceLOD * Factor;
            return true;
	    }
	    if (strKey == "Sun")
	    {
            Sun = * static_cast < const Ogre::Vector3 * > (pValue);
		    lightmoved = true;
            return true;
	    }
	    if (strKey == "SunAngle")
	    {
            SunAngle = * static_cast < const Ogre::Real * > (pValue);
		    lightmoved = true;
            return true;
	    }
	    if (strKey == "Width")
	    {
		    world_width = * static_cast < const unsigned int * > (pValue) ;
#ifndef _MAPSPLITTER
#ifndef _MAPEDITOR
                assert (primaryCamera);
		        static_cast <PagingLandScapeSceneManager *> (primaryCamera->getSceneManager())->WorldDimensionChange();

#endif //_MAPEDITOR
#endif  //_MAPSPLITTER
		    return true;
	    }
	    if (strKey == "Height")
	    {
		    world_height = * static_cast < const unsigned int * > (pValue);
#ifndef _MAPSPLITTER
#ifndef _MAPEDITOR
                assert (primaryCamera);
		        static_cast <PagingLandScapeSceneManager *> (primaryCamera->getSceneManager())->WorldDimensionChange();
#endif //_MAPEDITOR
#endif  //_MAPSPLITTER
		    return true;
	    }
	    if (strKey == "WorldDimension")
	    {
		    Vector2 dim =  * static_cast < const Vector2 * > (pValue);
		    world_height = static_cast < unsigned int > (dim.x);
		    world_width  = static_cast < unsigned int > (dim.y);
    #ifndef _MAPSPLITTER
    #ifndef _MAPEDITOR
                assert (primaryCamera);
		        static_cast <PagingLandScapeSceneManager *> (primaryCamera->getSceneManager())->WorldDimensionChange();
    #endif //_MAPEDITOR
    #endif  //_MAPSPLITTER
		    return true;
	    }
	    if (strKey == "primaryCamera")
	    {
		    setPrimaryCamera (const_cast < PagingLandScapeCamera * > (static_cast < const PagingLandScapeCamera * > (pValue)));
		    return true;
	    }

		if (strKey == "TextureNameLayer0")
	    {
			SplatDetailMapNames[0] = * static_cast < const String * > (pValue);
		    return true;
	    }
		if (strKey == "TextureNameLayer1")
	    {
			SplatDetailMapNames[1] = * static_cast < const String * > (pValue);
		    return true;
	    }
		if (strKey == "TextureNameLayer2")
	    {
			SplatDetailMapNames[2] =  * static_cast < const String * > (pValue);
		    return true;
	    }
		if (strKey == "TextureNameLayer3")
	    {
			SplatDetailMapNames[3] =  * static_cast < const String * > (pValue);
		    return true;
	    }


        if (strKey == "MaxAdjacentPages")
        {
            max_adjacent_pages = * static_cast < const unsigned int * > (pValue);
            return true;
        }
        if (strKey == "MaxPreloadedPages")
        {
            max_preload_pages = * static_cast < const unsigned int * > (pValue);
            return true;
		}
		if (strKey == "PositionX")
		{
			position.x = * static_cast < const Ogre::Real * > (pValue);
			return true;
		}
		if (strKey == "PositionY")
		{
			position.y = * static_cast < const Ogre::Real * > (pValue);
			return true;
		}
		if (strKey == "PositionZ")
		{
			position.z = * static_cast < const Ogre::Real * > (pValue);
			return true;
		}	

		if (strKey == "ConfigGroupName")
		{
			cfgGroupName = * static_cast < const String * > (pValue);
			return true;
		}  
		if (strKey == "queryNoInterpolation")
		{
			queryNoInterpolation = * static_cast < const bool * > (pValue);
			return true;
		}  
		if (strKey == "queryResolutionFactor")
		{
			queryResolutionFactor = * static_cast < const Ogre::Real * > (pValue);
			return true;
        }  
	    return false;
    }
       
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::getOption(const String& strKey, void* pDestValue)
    {
        if (strKey == "saveDeformation")
        {
            * static_cast < bool * > (pDestValue) = saveDeformation;
            return true;
        }
	    if (strKey == "VisibleRenderables")
	    {
		    * static_cast < int * > (pDestValue) = static_cast<int> (visible_renderables);
		    return true;
	    }
	    if (strKey == "DistanceLOD")
	    {
		    * static_cast <Ogre::Real * > (pDestValue) = static_cast<Real> (distanceLOD);
		    return true;
	    }
	    if (strKey == "VisibleDistance")
	    {
		    // we need to return the square root of the distance
            * static_cast <Ogre::Real * > (pDestValue) =  Math::Sqrt (renderable_factor);
            return true;
	    }
	    if (strKey == "VisibleLOD")
	    {
		    // we need to return the square root of the distance
		    * static_cast <Ogre::Real * > (pDestValue) =  Math::Sqrt (LOD_factor);
            return true;
	    }
	    // Some options proposed by Praetor
	    if (strKey == "Width")
	    {
		    * static_cast < int * > (pDestValue) = world_width;
		    return true;
	    }
	    if (strKey == "Height")
	    {
		    * static_cast < int * > (pDestValue) = world_height;
		    return true;
		}
		if (strKey == "MaxHeight")
		{
			* static_cast <Ogre::Real * > (pDestValue) = scale.y;
			return true;
		}
	    if (strKey == "PageSize")
	    {
		    * static_cast < int * > (pDestValue) = PageSize;
		    return true;
		}
		if (strKey == "Scale")
		{
			* static_cast < Ogre::Vector3 * > (pDestValue) = scale;
			return true;
		}
	    if (strKey == "ScaleX")
	    {
		    * static_cast <Ogre::Real * > (pDestValue) = scale.x;
		    return true;
	    }
	    if (strKey == "ScaleY")
	    {
		    * static_cast <Ogre::Real * > (pDestValue) = scale.y;
		    return true;
	    }
	    if (strKey == "ScaleZ")
	    {
		    * static_cast <Ogre::Real * > (pDestValue) = scale.z;
		    return true;
	    }	
    	if (strKey == "PositionX")
	    {
		    * static_cast <Ogre::Real * > (pDestValue) = position.x;
		    return true;
	    }
	    if (strKey == "PositionY")
	    {
		    * static_cast <Ogre::Real * > (pDestValue) = position.y;
		    return true;
	    }
	    if (strKey == "PositionZ")
	    {
		    * static_cast <Ogre::Real * > (pDestValue) = position.z;
		    return true;
	    }	
    
        if (strKey == "TextureNameLayer0")
	    {
			* static_cast < String * > (pDestValue) =  SplatDetailMapNames[0];
		    return true;
	    }
		if (strKey == "TextureNameLayer1")
	    {
			* static_cast < String * > (pDestValue) =  SplatDetailMapNames[1];
		    return true;
	    }
		if (strKey == "TextureNameLayer2")
	    {
			* static_cast < String * > (pDestValue) =  SplatDetailMapNames[2];
		    return true;
	    }
		if (strKey == "TextureNameLayer3")
	    {
			* static_cast < String * > (pDestValue) =  SplatDetailMapNames[3];
		    return true;
	    }

		if (strKey == "GroupName")
	    {
			* static_cast < String * > (pDestValue) =  groupName;
		    return true;
	    }
		
		if (strKey == "BaseCameraViewpoint")
	    {
			* static_cast < Ogre::Vector3 * > (pDestValue) =  BaseCameraViewpoint;
		    return true;
	    }
	    
		if (strKey == "Baselookat")
	    {
			* static_cast < Ogre::Vector3 * > (pDestValue) =  Baselookat;
		    return true;
        }

        if (strKey == "MaxAdjacentPages")
        {
            * static_cast < unsigned int * > (pDestValue) =  max_adjacent_pages;
            return true;
        }
        if (strKey == "MaxPreloadedPages")
        {
            * static_cast < unsigned int * > (pDestValue) =  max_preload_pages;
            return true;
		}
		if (strKey == "ConfigGroupName")
		{
			* static_cast < String * > (pDestValue) = cfgGroupName;
			return true;
		}  
		if (strKey == "queryNoInterpolation")
		{
			* static_cast < bool * > (pDestValue) = queryNoInterpolation;
			return true;
		}  
		if (strKey == "queryResolutionFactor")
		{
			* static_cast <Ogre::Real * > (pDestValue) = queryResolutionFactor;
			return true;
		}  
	    return false;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::hasOption(const String& strKey) const
    {
	    if (strKey == "VisibleRenderables")
	    {
		    return true;
	    }
	    if (strKey == "DistanceLOD")
	    {
		    return true;
	    }
	    if (strKey == "VisibleDistance")
	    {
		    return true;
	    }
	    if (strKey == "VisibleLOD")
	    {
		    return true;
	    }
	    // Some options proposed by Praetor
	    if (strKey == "Width")
	    {
		    return true;
	    }
	    if (strKey == "Height")
	    {
		    return true;
	    }
	    if (strKey == "PageSize")
	    {
		    return true;
	    }
	    if (strKey == "ScaleX")
	    {
		    return true;
	    }
	    if (strKey == "ScaleY")
	    {
		    return true;
	    }
	    if (strKey == "ScaleZ")
	    {
		    return true;
	    }
	    if (strKey == "PositionX")
	    {
		    return true;
	    }
	    if (strKey == "PositionY")
	    {
		    return true;
	    }
	    if (strKey == "PositionZ")
	    {
		    return true;
	    }
	    return false;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::getOptionValues(const String & key, StringVector &refValueList)
    {
    //	if (key == "VisibleRenderables")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "DistanceLOD")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "VisibleDistance")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "VisibleLOD")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "Width")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "Height")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "PageSize")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "ScaleX")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "ScaleY")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if (key == "ScaleZ")
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
	    return false;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::getOptionKeys(StringVector &refKeys)
    {
	    refKeys.push_back("VisibleRenderables");
	    refKeys.push_back("DistanceLOD");
	    refKeys.push_back("VisibleDistance");
	    refKeys.push_back("VisibleLOD");
	    // Some options from Praetor
	    refKeys.push_back("Width");
	    refKeys.push_back("Height");
	    refKeys.push_back("PageSize");
	    refKeys.push_back("ScaleX");
	    refKeys.push_back("ScaleY");
	    refKeys.push_back("ScaleZ");
	    refKeys.push_back("PositionX");
	    refKeys.push_back("PositionY");
	    refKeys.push_back("PositionZ");
	    return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::getAvgColors()
    {
        bool AvgColorsExists = false; 
        setBool (AvgColorsExists, "AvgColorsExists");
        if (AvgColorsExists)
        {   
            const String baseName ("MaterialColor");
            for (unsigned int i = 0; i < NumMatHeightSplat; i++) 
            {   
                const String matColorString (baseName + StringConverter::toString (i));
                setColourValue (matColor[i], matColorString);
            }                  
        }
        else
		{     			
			for (unsigned int i = 0; i < NumMatHeightSplat; i++) 
                matColor[i] = _getAvgColor(SplatDetailMapNames[i]);
        }
    }
    //-----------------------------------------------------------------------
    ColourValue PagingLandScapeOptions::_getAvgColor(const String &tex) const
    {
        if (tex.empty())
            return ColourValue::White;

	    Image img;

	    img.load (tex, groupName);
	    const uchar * const ogre_restrict data = img.getData();
        if (!data)
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                "You need to define SplatFilename that has at least 3 or 4 bytes componennts (RGB or RGBA)",
                "PagingLandScapeOptions::_getAvgColor");
        }    
        size_t bpp = PixelUtil::getNumElemBytes (img.getFormat ());
        if (bpp < 3)
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                "You need to define SplatFilename that has at least 3 or 4 bytes componennts (RGB or RGBA)",
                "PagingLandScapeOptions::_getAvgColor");
        }    
	    int cr = 0, cg = 0, cb = 0, s = 0;
		const size_t imgSize = img.getSize();
	    for (size_t i = 0; i < imgSize; i += bpp)
	    {
		    cr += data[i];
		    cg += data[i+1];
		    cb += data[i+2];

		    s++;
	    }
        assert (s > 0);
       Ogre::Real divider = 1.0f /  (s * 255);
	    return ColourValue (cr * divider, cg * divider, cb * divider, 1.0f);
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeOptions::clearTileInfo()
	{
		if (!mTileInfoCache.empty())
		{
			///don't do thses things when run in Ember
			//assert (mCurrentMap != "");
			//saveMapInfo();
			std::for_each(mTileInfoCache.begin (),
						  mTileInfoCache.end (),
						delete_object());
			mTileInfoCache.clear();
		}
	}
	//-----------------------------------------------------------------------
	PagingLandScapeTileInfo *PagingLandScapeOptions::getTileInfo(const uint pageX, const uint pageZ, 
																 const uint tileX, const uint tileZ)
	{
		PagingLandScapeTileInfo *t = 0;

		std::deque<PagingLandScapeTileInfo*>::iterator q = mTileInfoCache.begin ();
		std::deque<PagingLandScapeTileInfo*>::iterator qend = mTileInfoCache.end ();
		while (q != qend)
		{
			t = *q;
			if (pageX == t->mPageX &&
				pageZ == t->mPageZ &&
				tileX == t->mTileX &&
				tileZ == t->mTileZ)
			{
				return t;
			}
			++q;
		}
        // no info in cache.
        t = new PagingLandScapeTileInfo(pageX, pageZ, tileX, tileZ);
        setTileInfo (t);
		return t;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeOptions::setTileInfo(PagingLandScapeTileInfo *t)
	{
		mTileInfoCache.push_back (t);
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeOptions::loadMapInfo()
	{
		// load terrain.info.cfg into the deque.
        if (mUseLodMapCache)
        {
            assert (!mCurrentMap.empty ());
		    const String fName (mCurrentMap + ".info.cfg");
		    if (ResourceGroupManager::getSingleton().resourceExists(groupName, fName))
		    {
			    ConfigFile config;

			    config.load (fName, cfgGroupName, String("="), true);

			    // those info are dependent of Pagesize and Tilesize.
			    // if not the same as when generated...
			    // we must recompute them.
			    const String pageSizeString = config.getSetting(String("PageSize"));
			    if (pageSizeString.empty() || PageSize != StringConverter::parseUnsignedInt (pageSizeString)) 
				    return;

			    const String tileSizeString = config.getSetting(String("TileSize"));
			    if (tileSizeString.empty() || TileSize != StringConverter::parseUnsignedInt (tileSizeString)) 
                    return;

			    ConfigFile::SettingsIterator setIt = config.getSettingsIterator();
			    const size_t numLod = maxRenderLevel;
			    PagingLandScapeTileInfo *t;
			    while (setIt.hasMoreElements())
			    {  
				    const String name = setIt.peekNextKey();
				    const String value = setIt.getNext();
				    if (name != "PageSize" && name != "TileSize")
				    {
					    // name to pageX, uint pageZ, uint tileX, uint tileZ
					    {
					    	vector< String >::type coordinates = StringUtil::split(name, "_");

						    const uint pageX = StringConverter::parseUnsignedInt(coordinates[0]);
						    const uint pageZ = StringConverter::parseUnsignedInt(coordinates[1]);
						    const uint tileX = StringConverter::parseUnsignedInt(coordinates[2]);
						    const uint tileZ = StringConverter::parseUnsignedInt(coordinates[3]);

						    t = new PagingLandScapeTileInfo(pageX, pageZ, tileX, tileZ);
					    }

					    // name to LOD roughness value.
					    {
							vector< String >::type minLevelDistSqr = StringUtil::split(value, "_");
						    assert (minLevelDistSqr.size () == numLod);

						    t->mMinLevelDistSqr =  new std::vector<Real>();

						    t->mMinLevelDistSqr->reserve(numLod);
						    t->mMinLevelDistSqr->resize(numLod);

						    for (size_t i = 0; i < numLod; i++)
						    {
							    (*(t->mMinLevelDistSqr))[i]  = StringConverter::parseReal(minLevelDistSqr[i]);
						    }
					    }
					    mTileInfoCache.push_back(t);
				    }
    				
			    }
		    }
        }
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeOptions::saveMapInfo()
	{
        if (mUseLodMapCache)
        {
		    //if(modif ||was_empty when loaded.) ??

            assert (!mCurrentMap.empty ());
		    const String fInfoName (getMapFilename(mCurrentMap) + ".cfg");

		    FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
			    groupName, fInfoName);
		    FileInfoList::iterator it = finfo->begin();	
		    if (it != finfo->end())
		    {
			    // save deque into terrain.info.cfg
			    const size_t numLod = maxRenderLevel;
			    const String eol("\n");
			    const String coordinateSeparator("_");
			    const String valueSeparator("=");
			    PagingLandScapeTileInfo *t;

			    String tilesInfo("");

			    // those info are dependent of Pagesize and Tilesize.
			    // if not the same as when generated...
			    // we must recompute them.
			    tilesInfo += String("PageSize=")+StringConverter::toString(PageSize) + eol;
			    tilesInfo += String("TileSize=")+StringConverter::toString(TileSize) + eol;

			    std::deque<PagingLandScapeTileInfo*>::iterator q = mTileInfoCache.begin ();
			    std::deque<PagingLandScapeTileInfo*>::iterator qend = mTileInfoCache.end ();
			    while (q != qend)
			    {
				    t = *q;

				    if (t->mMinLevelDistSqr)
				    {
					    bool notEmpty = false;
					    for (size_t i = 0; i < numLod; i++)
					    {
						    if ((*(t->mMinLevelDistSqr))[i] != 0.0f)
						    {
							    notEmpty = true;
							    break;
						    }
					    }
					    if (notEmpty)
					    {
						    tilesInfo += StringConverter::toString(t->mPageX) + coordinateSeparator;
						    tilesInfo += StringConverter::toString(t->mPageZ) + coordinateSeparator;
						    tilesInfo += StringConverter::toString(t->mTileX) + coordinateSeparator;
						    tilesInfo += StringConverter::toString(t->mTileZ);

						    tilesInfo += valueSeparator;

						    for (size_t i = 0; i < numLod; i++)
						    {
							    tilesInfo +=  StringConverter::toString((*(t->mMinLevelDistSqr))[i]) + coordinateSeparator;
						    }
						    tilesInfo += eol;
					    }
				    }
				    ++q;
			    }


			    const String fConfigName (mCurrentMap + ".info.cfg");

			    char *olddir = ChangeToDir (const_cast< char * > (((it)->archive->getName()).c_str()));
			    std::ofstream outfile;
			    outfile.open (const_cast< char * > (fConfigName.c_str())
							    //,std::ios:binary
							    );
			    // Write out
			    outfile << tilesInfo;
			    outfile.close ();

			    RetablishDir (olddir);
		    }
        }
	}
} //namespace
