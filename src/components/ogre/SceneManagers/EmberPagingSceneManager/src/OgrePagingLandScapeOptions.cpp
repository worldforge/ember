/***************************************************************************
  OgrePagingLandScapeOptions.cpp  -  description
  -------------------
  begin                : Sun Mar 02 2003
  copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
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

namespace Ogre
{

    //-----------------------------------------------------------------------
    template<> PagingLandScapeOptions* Singleton<PagingLandScapeOptions>::ms_Singleton = 0;
    PagingLandScapeOptions* PagingLandScapeOptions::getSingletonPtr(void)
    {
	    return ms_Singleton;
    }
    PagingLandScapeOptions& PagingLandScapeOptions::getSingleton(void)
    {  
	    assert(ms_Singleton);  return (*ms_Singleton);  
    }

    //-----------------------------------------------------------------------
    PagingLandScapeOptions::PagingLandScapeOptions()
    {
      primaryCamera = 0;
      isInit = false;
	  //setDefault();
    };

    //-----------------------------------------------------------------------
    PagingLandScapeOptions::~PagingLandScapeOptions()
    {
    }
    //
    void PagingLandScapeOptions::setDefault()
    {
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
		PageLoadInterval = 30;

	    change_factor = 1;
	    distanceLOD = 4;
	    LOD_factor = 10;

        scale = Vector3::UNIT_SCALE;


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
		VertexCompression = false;

		BigImage = false;

        lodMorphStart = 0.2f;
        lodMorph = false;

        maxPixelError = 8;
       
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

		TileInvisibleUnloadFrames = 400;
		PageInvisibleUnloadFrames = 400;

        lodMaterialDistanceList.clear();
        lodMaterialDistanceList.push_back (80000.0f);

        mResourceFilesystem = StringUtil::BLANK;
        mResourceZip = StringUtil::BLANK;
#ifdef _MAPSPLITTER

        Blur = 0.0f;
        Amb = 0.5f;
        Diff = 0.5f;
        
        Blur = 0.0f;
        HeightMapBlurFactor = 0.0f;
		Paged = true;
        MiniMap = false;
        PVSMap = false;
        BaseMap = false;
        RGBMaps = false;
        ColorMapGenerate = false;
        ColorMapSplit = false;
        LightMap = false;
        NormalMap = false;
        HeightMap = false;
        AlphaMaps = false;
        ShadowMap = false;
        HorizonMap = false;  
        LitBaseMap = false;
        InfiniteMap = false;
        CoverageMap = false;
        LitColorMapGenerate = false;
        LitColorMapSplit = false;
        ElevationMap = false; 
        HeightNormalMap = false;
        AlphaSplatRGBAMaps = false;
        AlphaSplatLightMaps = false;        
        Equalize = false;
        ZHorizon = false;
        SRTM_water = false;
        
#endif _MAPSPLITTER
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
                hasVertexShader = caps->hasCapability(RSC_VERTEX_PROGRAM) && !(StringUtil::startsWith(caps->getMaxFragmentProgramVersion (), "vs_1_0", true));
                hasFragmentShader = caps->hasCapability(RSC_FRAGMENT_PROGRAM);
                const String &maxShaderVersion = caps->getMaxFragmentProgramVersion ();
                hasFragmentShader2 = hasFragmentShader && !(maxShaderVersion == "ps_1_1" || maxShaderVersion == "ps_1_0");
                numTextureUnits = caps->getNumTextureUnits ();
                isRenderGL = StringUtil::startsWith(renderer->getName(), "OpenGL", false);
                isInit = true;
            }
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::load(const String &filename)
    {
        std::ifstream fs;
 		fs.open(filename.c_str());
 		if (fs)
 		{
 			// Wrap as a stream
 			DataStreamPtr stream(
 				new FileStreamDataStream(filename, &fs, false));
 			load(stream);
 		}
		else
 		{
 			// otherwise try resource system
 			DataStreamPtr stream = 
				ResourceGroupManager::getSingleton().openResource(filename);
     			
 			load(stream);
 		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::load(DataStreamPtr& stream)
    {
        init();
	    /* Set up the options : List of map associated with map names.*/
	    ConfigFile config;
	    config.load(stream);

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

		//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups ();

        mMapList.erase (i);

        i = mMapList.find("BatchMode");
        if ((i != mMapList.end()) && (i->second == "yes"))
        {
            mBatchMode = true;
            mMapList.erase (i);
        }    
        else
            mBatchMode = false;

        i = mMapList.find("TextureFormatDebug");
        if (i != mMapList.end())
        {       
            TextureFormatDebug = config.getSetting("TextureFormatDebug") == "yes";
            mMapList.erase (i);
        }



        i = mMapList.find("DefaultMap");
        if (i == mMapList.end() && mBatchMode == false)
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                "You need to define a DefaultMap= ",
                "PagingLandScapeOptions::load");
        }
        mCurrentMap = i->second; 
   
		if (!mBatchMode)
			loadMap(mCurrentMap);
		mMapList.erase (i);
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
            return;
        }
        textureFormat = format;
        // sort of debugging hack til BigImage itself becomes a chunked Lod thing.
        BigImage = BigImage && textureFormat == "Image";
    }   
    //-----------------------------------------------------------------------
    LandScapeFileNames& PagingLandScapeOptions::getMapList() 
    {
        return mMapList;
    }
    //-----------------------------------------------------------------------
    const String& PagingLandScapeOptions::getPreviousMapName() const
    {
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
        return StringUtil::BLANK;
    }
    //-----------------------------------------------------------------------
    const String& PagingLandScapeOptions::getNextMapName() const
    {  
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
            // find filename
            LandScapeFileNames::iterator i = mMapList.find(mapName);
            mCurrentMap = mapName;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::loadMap(const String& mapName)
    {
        if (mResourceFilesystem != StringUtil::BLANK)
            ResourceGroupManager::getSingleton().removeResourceLocation(
                                            mResourceFilesystem, groupName);
        if (mResourceZip != StringUtil::BLANK)
            ResourceGroupManager::getSingleton().removeResourceLocation(
                                            mResourceZip, groupName);
    
  
        LandScapeFileNames::iterator i = mMapList.find(mapName);
        if (i == mMapList.end())
        {
            bool bfound = false;
            for (i = mMapList.begin(); i != mMapList.end(); ++i)
            {
                if (StringUtil::match(i->second, mapName, false) || 
					StringUtil::match(i->first, mapName, false))
                {
                    bfound = true;
                    break;
                }
            }
            if (!bfound)
            {
                OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                    "Cannot locate a Config File defining the map named " + mapName,
                    "PagingLandScapeOptions::loadMap");
            }
        }

	    /* Set up the options For a Map*/
	    //ConfigFile config;
	    loadMapOptions (i->second  + String(".cfg"));
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
    void PagingLandScapeOptions::setUint (uint &u, const String &ValuetoGet)
	{	
		const String valString = mConfig->getSetting(ValuetoGet);
		if (valString != StringUtil::BLANK)
		{
			const uint val = static_cast<uint> (StringConverter::parseReal(valString)); 
			if (val  != 0)
				u = val;
		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::setBool (bool &b, const String &ValuetoGet)
	{
		const String valString = mConfig->getSetting (ValuetoGet);
		if (valString != StringUtil::BLANK)
		{
			b = StringUtil::startsWith(valString, "yes", true);
		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::setReal (Real &r, const String &ValuetoGet)
	{
		const String valString = mConfig->getSetting(ValuetoGet);
		if (valString != StringUtil::BLANK)
		{
			const Real val = StringConverter::parseReal(valString);
			r = val;
		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::setString (String &s, const String &ValuetoGet)
    {
        const String val = mConfig->getSetting (ValuetoGet);
        if (val != StringUtil::BLANK)
            s = val;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::loadMapOptions(const String& mapName)
    {
		uint i;
		
        setDefault ();
        init ();

	    // Set up the options For a Map
	    ConfigFile config;
//	    config.load (mapName, cfgGroupName, String("="), true);
	    config.load (mapName);
        mConfig = &config;


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
        NumTiles = static_cast<uint> (static_cast <Real> (PageSize) / (TileSize - 1));
        NumPages = static_cast<uint> (static_cast <Real> (world_height * world_width));
        const uint totalNumTiles = NumPages * (NumTiles*NumTiles) + 1;
    	    
	    setReal (scale.x, "ScaleX");
        setReal (scale.y, "ScaleY");
        setReal (scale.z, "ScaleZ");

		// secret tweak helping joining tile using vertex compression
		// leads to texture distortion at page breaks otherwise.
		const double scalemodif = static_cast <double> (PageSize - 1) / PageSize;
        ScaledPageSizeX = scale.x * scalemodif;
        ScaledPageSizeZ = scale.z * scalemodif;
        ScaledHeightY = scale.y / 65535;

        setReal (TextureStretchFactor,  "TextureStretchFactor");

#if !defined(_MAPSPLITTER) && !defined(_MAPEDITOR)

        // Scale x/z relative to page size
        scale.x /= PageSize;
        scale.z /= PageSize;    
#else// !defined(_MAPSPLITTER) && !defined(_MAPEDITOR)
        //scale.y = scale.y;
        scale.x /= PageSize * TextureStretchFactor;
        scale.z /= PageSize * TextureStretchFactor;    

#endif// !defined(_MAPSPLITTER) && !defined(_MAPEDITOR)

	    maxUnScaledZ = world_height * (PageSize - 1) * 0.5f;
	    maxUnScaledX = world_width  * (PageSize - 1) * 0.5f;

	    maxScaledZ = scale.z * maxUnScaledZ;
	    maxScaledX = scale.x * maxUnScaledX;


	    // Set up the names For a Map

        setString (groupName, "GroupName");  
        setString (LandScape_filename, "LandScapeFileName");

        //add Resource Group to Ogre if needed.
        mResourceFilesystem = config.getSetting ("FileSystem");
        if (mResourceFilesystem != StringUtil::BLANK)
        {
            if (StringUtil::endsWith  (mResourceFilesystem, "landscapefilename", true))
            {
                String BasePath, FilePath;
                StringUtil::splitFilename(mResourceFilesystem, BasePath, FilePath);
                mResourceFilesystem = FilePath + LandScape_filename;
            }
            if (StringUtil::endsWith  (mResourceFilesystem, "landscapeexportfilename", true))
            {
                String BasePath, FilePath;
                StringUtil::splitFilename(mResourceFilesystem, BasePath, FilePath);
                mResourceFilesystem = FilePath + LandScape_export_filename;
            }
            ResourceGroupManager::getSingleton().addResourceLocation(
            mResourceFilesystem, "FileSystem", groupName);
        }
        mResourceZip =  config.getSetting ("Zip");
        if (mResourceZip != StringUtil::BLANK)
        {
            ResourceGroupManager::getSingleton().addResourceLocation(
                mResourceZip, "Zip", groupName);
        }

	    setString (LandScape_filename, "LandScapeFileName");
	    setString (LandScape_export_filename, "LandScapeExportFileName");
	    
        if (LandScape_export_filename == StringUtil::BLANK)
            LandScape_export_filename = LandScape_filename;
            
	    setString (LandScape_extension, "LandScapeExtension");
        setString (LandScape_export_extension, "LandScapeExportExtension");
        
        if (LandScape_export_extension == StringUtil::BLANK)
            LandScape_export_extension = LandScape_extension;
            
        setString (TextureExtension, "TextureExtension");

        setString (image_filename, "ImageFilename");
        if (image_filename == "")
            ImageNameLoad = false;
        else 
            ImageNameLoad = true;

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
            maxValue *= scale.y;
            setUint (minValue, "MinValue");
            minValue *= scale.y;
        }
        else
        {
            maxValue = static_cast<uint> (scale.y);
            minValue = 0;
        }



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
        const Real divider = (maxValue - minValue) / (100.0f);
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
        
        Real MaterialDistanceLod = 0.0f;   
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
		setUint (PageLoadInterval, "PageLoadInterval");

	    // Set up the LOD options For a Map
        setUint (maxRenderLevel, "MaxRenderLevel");
        if (maxRenderLevel == 100)
        {
			maxRenderLevel = 0;
			const uint halftileSize = TileSize * 0.5f;
            while (static_cast<uint> ((1 << maxRenderLevel)) < halftileSize )
                maxRenderLevel++;
        }

        setReal (change_factor,  "ChangeFactor");
	    change_factor *=  (static_cast <Real> (PageSize * (scale.z + scale.x) * 0.5f) / 9);
	    
	    // Set up the distance options For a Map
	    
	    setReal (visible_renderables, "VisibleRenderables");

	    // compute the actual distance as a square
        // Factor is a Tile distance squared
        const Real Factor = TileSize * scale.x * TileSize * scale.z;


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
	    //LOD_factor *= LOD_factor;

        setBool (lit, "VertexLit");
        setBool (normals, "VertexNormals");
        
        setBool (Deformable, "Deformable");

        //Morphing
        setBool (lodMorph, "VertexProgramMorph");
        setReal (lodMorphStart, "LODMorphStart");
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
		
#else 
        
        // MAP TOOL OPTIONS ONLY

        setBool (Paged, "Paged");

        setString (OutDirectory, "OutDirectory");
	    if (StringUtil::endsWith  (OutDirectory, "landscapefilename", true))
        {
            String BasePath, FilePath;
            StringUtil::splitFilename(OutDirectory, BasePath, FilePath);
            OutDirectory = FilePath + LandScape_filename;
        }
	    if (StringUtil::endsWith  (OutDirectory, "landscapeexportfilename", true))
        {
            String BasePath, FilePath;
            StringUtil::splitFilename(OutDirectory, BasePath, FilePath);
            OutDirectory = FilePath + LandScape_export_filename;
        }
        
        setBool (MiniMap, "MiniMap");
        setBool (PVSMap, "PVSMap");
        setBool (BaseMap, "BaseMap");
        setBool (RGBMaps, "RGBMaps");
        setBool (ColorMapGenerate, "ColorMapGenerate");
        setBool (ColorMapSplit, "ColorMapSplit");
        setBool (LightMap, "LightMap");
        setBool (NormalMap, "NormalMap");
        setBool (HeightMap, "HeightMap");
        setBool (AlphaMaps, "AlphaMaps");
        setBool (ShadowMap, "ShadowMap");
        setBool (HorizonMap, "HorizonMap");  
        setBool (LitBaseMap, "LitBaseMap");
        setBool (InfiniteMap, "InfiniteMap");
        setBool (CoverageMap, "CoverageMap");
        setBool (LitColorMapGenerate, "LitColorMapGenerate");
        setBool (LitColorMapSplit, "LitColorMapSplit");
        setBool (ElevationMap, "ElevationMap"); 
        setBool (HeightNormalMap, "HeightNormalMap");
        setBool (AlphaSplatRGBAMaps, "AlphaSplatRGBAMaps");
        setBool (AlphaSplatLightMaps, "AlphaSplatLightMaps");

        setString (ColorMapName, "ColorMapName");

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
        // Const Real A = 1 / Math::Tan(Math::AngleUnitsToRadians(opts.primaryCamera->getFOVy()));
		// Turn off detail compression at higher FOVs
        const Real A = 1.0f;

		assert (primaryCamera);
        const Viewport *v = primaryCamera->getViewport();
        if (v)
        {
            const int vertRes = v->getActualHeight();
            
            assert (vertRes != 0);
            const Real T = 2 * static_cast < Real >(maxPixelError) / vertRes;
            
			assert (T != 0);
            CFactor =  A / T;
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
	    if (strKey == "VisibleRenderables")
	    {
		    visible_renderables = * static_cast < const int * > (pValue);
		    // compute the actual distance as a square
            Real Factor = TileSize;
            Factor = Factor * scale.x * Factor * scale.z;

	        renderable_factor = visible_renderables * Factor;
            return true;
	    }
	    if (strKey == "DistanceLOD")
	    {
		    distanceLOD = * static_cast < const Real * > (pValue);
		    // Compute the actual distance as a square
            Real Factor = TileSize;
            Factor = Factor * scale.x * Factor * scale.z;
	        LOD_factor = distanceLOD * Factor;
            return true;
	    }
	    if (strKey == "Sun")
	    {
            Sun = * static_cast < const Vector3 * > (pValue);
		    lightmoved = true;
            return true;
	    }
	    if (strKey == "SunAngle")
	    {
            SunAngle = * static_cast < const Real * > (pValue);
		    lightmoved = true;
            return true;
	    }
	    if (strKey == "Width")
	    {
		    world_width = * static_cast < const uint * > (pValue) ;
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
		    world_height = * static_cast < const uint * > (pValue);
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
		    world_height = static_cast < uint > (dim.x);
		    world_width  = static_cast < uint > (dim.y);
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


	    return false;
    }
       
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::getOption(const String& strKey, void* pDestValue)
    {
	    if (strKey == "VisibleRenderables")
	    {
		    * static_cast < int * > (pDestValue) = static_cast<int> (visible_renderables);
		    return true;
	    }
	    if (strKey == "DistanceLOD")
	    {
		    * static_cast < Real * > (pDestValue) = static_cast<Real> (distanceLOD);
		    return true;
	    }
	    if (strKey == "VisibleDistance")
	    {
		    // we need to return the square root of the distance
            * static_cast < Real * > (pDestValue) =  Math::Sqrt (renderable_factor);
            return true;
	    }
	    if (strKey == "VisibleLOD")
	    {
		    // we need to return the square root of the distance
		    * static_cast < Real * > (pDestValue) =  Math::Sqrt (LOD_factor);
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
	    if (strKey == "PageSize")
	    {
		    * static_cast < int * > (pDestValue) = PageSize;
		    return true;
	    }
	    if (strKey == "ScaleX")
	    {
		    * static_cast < Real * > (pDestValue) = scale.x;
		    return true;
	    }
	    if (strKey == "ScaleY")
	    {
		    * static_cast < Real * > (pDestValue) = scale.y;
		    return true;
	    }
	    if (strKey == "ScaleZ")
	    {
		    * static_cast < Real * > (pDestValue) = scale.z;
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
			* static_cast < Vector3 * > (pDestValue) =  BaseCameraViewpoint;
		    return true;
	    }
	    
		if (strKey == "Baselookat")
	    {
			* static_cast < Vector3 * > (pDestValue) =  Baselookat;
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
            for (uint i = 0; i < NumMatHeightSplat; i++) 
            {   
                const String matColorString (baseName + StringConverter::toString (i));
                setReal (matColor[i].r, matColorString + ".r");
                setReal (matColor[i].g, matColorString + ".g");
                setReal (matColor[i].b, matColorString + ".b");
            }                  
        }
        else
		{     			
			for (uint i = 0; i < NumMatHeightSplat; i++) 
                matColor[i] = _getAvgColor(SplatDetailMapNames[i]);
        }
    }
    //-----------------------------------------------------------------------
    ColourValue PagingLandScapeOptions::_getAvgColor(const String &tex) const
    {
        if (tex == "")
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
        Real divider = 1.0f /  (s * 255);
	    ColourValue c (cr * divider, cg * divider, cb * divider, 1.0f);
	    return c;
    }

} //namespace
