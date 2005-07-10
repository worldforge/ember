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
	    assert( ms_Singleton );  return ( *ms_Singleton );  
    }

    //-----------------------------------------------------------------------
    PagingLandScapeOptions::PagingLandScapeOptions()
    {
	    data2DFormat = "HeightField";
	    textureFormat = "Image";

	    landscape_filename = "";
	    landscape_extension = "";

	    for (uint i = 0; i < 4; i++)
	    {
		    matColor[i] = ColourValue();
	    }
	    for (uint i = 0; i < 4; i++)
	    {
		    matHeight[i] = 0.0f;
	    }
	    maxValue = 5000;
	    minValue = 0;

	    PageSize = 257;
        TileSize = 65;
	    world_width = 0;
	    world_height = 0;

	    maxScaledZ = 0;
	    maxScaledX = 0;
	    maxUnScaledZ = 0;
	    maxUnScaledX = 0;

	    change_factor = 1;
	    max_adjacent_pages = 1;
	    max_preload_pages = 2;
	    renderable_factor = 10;

        scale = Vector3::UNIT_SCALE;

	    distanceLOD = 4;
	    LOD_factor = 10;

	    num_renderables = 1000;
	    num_renderables_increment = 16;
	    num_tiles = 1000;
	    num_tiles_increment = 16;

	    cameraThreshold = 5;

	    num_renderables_loading = 10;

	    lit = false;
        colored = false;

        coverage_vertex_color= false;
        base_vertex_color = false;
        vertex_shadowed = false;
        vertex_instant_colored = false;

        RawHeight = 0;
        RawWidth = 0;

        lightmoved = false;

        lodMorphStart = 0.5f;
        lodMorph = false;
        maxPixelError = 4;

        primaryCamera = 0;

       isInit = false;

        
        matColor[0] = ColourValue( 1.0f, 1.0f, 0.0f); //Sand
        matColor[1] = ColourValue( 0.13f, 0.545f, 0.13f); //Grass
        //MakeRGBPixel (0.7450f, 0.745f, 0.745f); //Rock
        matColor[2] = ColourValue( 0.411764f, 0.411764f, 0.411764f); //Rock
        matColor[3] = ColourValue( 1.0f, 0.98f, 0.98f); //Snow

        matHeight[0] = 0.0f;
        matHeight[1] = 0.0f;
        //MakeRGBPixel (0.7450f, 0.745f, 0.745f); //Rock
        matHeight[2] = 0.0f;
        matHeight[3] = 0.0f;
    };

    //-----------------------------------------------------------------------
    PagingLandScapeOptions::~PagingLandScapeOptions()
    {
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::init(  )
    {
        // sets options that doesn't depend on maps.
        if (!isInit )
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
    void PagingLandScapeOptions::load( const String& filename )
    {
        init();
	    /* Set up the options : List of map associated with map names.*/
	    ConfigFile config;
	    config.load( filename );

        // Go through all sections & settings in the file

        ConfigFile::SettingsIterator setIt = config.getSettingsIterator();
        while (setIt.hasMoreElements())
        {  
            const String name = setIt.peekNextKey();
            const String value = setIt.getNext();
            mMapList.insert(LandscapeFileNames::value_type(name, value));
        }

        LandscapeFileNames::iterator i = mMapList.find("GroupName");
        if (i == mMapList.end())
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                "You need to define a GroupName where to find the map definition file ",
                "PagingLandScapeOptions::load");
        }    
        groupName = i->second;

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
            TextureFormatDebug = config.getSetting( "TextureFormatDebug" ) == "yes";
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
        textureFormat = format;
        BigImage = BigImage && textureFormat == "Image";
    }   
    //-----------------------------------------------------------------------
    LandscapeFileNames& PagingLandScapeOptions::getMapList(  ) 
    {
        return mMapList;
    }
    //-----------------------------------------------------------------------
    const String& PagingLandScapeOptions::getPreviousMapName(  ) const
    {
        LandscapeFileNames::const_iterator iend = mMapList.end();
        LandscapeFileNames::const_iterator i = mMapList.begin();
        LandscapeFileNames::const_iterator iprev = mMapList.end();
        --iprev;    
        for (; i != iend;++i)
        {
            if (i->first == mCurrentMap)
                return iprev->first;
            iprev = i;
        }
        if (i == iend)
            return mMapList.begin()->first;
        return StringUtil::BLANK;
    }
    //-----------------------------------------------------------------------
    const String& PagingLandScapeOptions::getNextMapName(  ) const
    {  
        LandscapeFileNames::const_iterator iend = mMapList.end();
        LandscapeFileNames::const_iterator i = mMapList.begin();
        for (; i != iend; ++i)
        {
            if (i->first == mCurrentMap)
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
    void PagingLandScapeOptions::setCurrentMapName( const String& mapName )
    {
        if (StringUtil::BLANK != mapName)
        {
            // find filename
            LandscapeFileNames::iterator i = mMapList.find(mapName);
            mCurrentMap = mapName;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::loadMap( const String& mapName )
    {
        // find filename
        LandscapeFileNames::iterator i = mMapList.find(mapName);
        if (i == mMapList.end())
        {
            for (i = mMapList.begin(); i  != mMapList.end(); ++i)
            {
                if (i->second == mapName)  
                    break;
            }

            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                "Cannot locate a Config File defining the map named " + mapName,
                "PagingLandScapeOptions::loadMap");
        }

	    /* Set up the options For a Map*/
	    ConfigFile config;
	    loadMapOptions (i->second  + String(".cfg"));
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::insertMap( const String& mapName )
    {
        
        LandscapeFileNames::iterator ifind = mMapList.find (mapName);
        if (ifind == mMapList.end())
        {
            mMapList.insert(LandscapeFileNames::value_type(mapName, mapName));
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::loadMapOptions( const String& mapName )
    {
        init();

	    /* Set up the options For a Map*/
	    ConfigFile config;
	    config.load( mapName, groupName, String("="), true);

        getAvgColors ();

	    world_height = static_cast<uint> (StringUtil::toReal( config.getSetting( "Height" ) ));
	    world_width = static_cast<uint> (StringUtil::toReal( config.getSetting( "Width" ) ));
        
	    PageSize = static_cast<uint> (StringUtil::toReal( config.getSetting( "PageSize" ) ));
        TileSize = static_cast<uint> (StringUtil::toReal( config.getSetting( "TileSize" ) ));

        NumTiles = static_cast<uint> (static_cast <Real> (PageSize) / (TileSize - 1));
        NumPages = static_cast<uint> (static_cast <Real> (world_height * world_width));
    	    
	    scale.x = StringUtil::toReal( config.getSetting( "ScaleX" ) );
        scale.y = StringUtil::toReal( config.getSetting( "ScaleY" ) );
        scale.z = StringUtil::toReal( config.getSetting( "ScaleZ" ) );
        
        ScaledPageSizeX = scale.x;
        ScaledPageSizeZ = scale.z;
        ScaledHeightY = scale.y / 65535;

        // Scale x/z relative to page size
        scale.x /= PageSize;
        scale.z /= PageSize;    


	    maxUnScaledZ = world_height * (PageSize - 1) * 0.5f;
	    maxUnScaledX = world_width  * (PageSize - 1) * 0.5f;

	    maxScaledZ = scale.z * maxUnScaledZ;
	    maxScaledX = scale.x * maxUnScaledX;


        groupName = config.getSetting( "GroupName" );
	    landscape_filename = config.getSetting( "LandScapeFileName" );
	    landscape_extension = config.getSetting( "LandScapeExtension" );
        landscape_export_extension = config.getSetting( "LandScapeExportExtension" );
        if (landscape_export_extension == StringUtil::BLANK)
            landscape_export_extension = landscape_extension;

	    data2DFormat = config.getSetting( "Data2DFormat" );
        TextureExtension =  config.getSetting( "TextureExtension" );
            
        bool HeightField = StringUtil::endsWith(data2DFormat, "HeightField", false);
        bool HeightFieldN = StringUtil::endsWith(data2DFormat, "HeightFieldN", false);
        bool HeightFieldTC = StringUtil::endsWith(data2DFormat, "HeightFieldTC", false);
        bool HeightFieldNTC = StringUtil::endsWith(data2DFormat, "HeightFieldNTC", false);
        bool HeightFieldRawTC = StringUtil::endsWith(data2DFormat, "HeightFieldRawC", false);
        bool HeightFieldRaw = StringUtil::endsWith(data2DFormat, "HeightFieldRaw", false);

    
        if (StringUtil::endsWith(landscape_extension, "raw", true))
        {
            isRaw = true;
            RawHeight = static_cast<uint> (StringUtil::toReal (config.getSetting ("RawHeight")));
            RawWidth = static_cast<uint> (StringUtil::toReal (config.getSetting ("RawWidth" )));
            

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

	    if ( HeightFieldRawTC 
            || HeightFieldTC 
            || HeightFieldNTC )
        {
            maxValue = static_cast<uint> (StringUtil::toReal( config.getSetting( "MaxValue" ) ) * scale.y );
            minValue = static_cast<uint> (StringUtil::toReal( config.getSetting( "MinValue" ) ) * scale.y );
        }
        else
        {
            maxValue = static_cast<uint> ( scale.y );
            minValue = 0;
        }

        const Real divider = (maxValue - minValue) / (100.0f);

	    matHeight[1] = StringUtil::toReal( config.getSetting( "MaterialHeight1" ) );
	    matHeight[1] = matHeight[1] * divider;

	    matHeight[2] = StringUtil::toReal( config.getSetting( "MaterialHeight2" ) );
	    matHeight[2] = matHeight[2] * divider;

        image_filename = config.getSetting( "ImageFilename" );
        if (image_filename == "")
            ImageNameLoad = false;
        else 
            ImageNameLoad = true;

        Splat_Filename_0 = config.getSetting( "SplatFilename0" );
        Splat_Filename_1 = config.getSetting( "SplatFilename1" );
        Splat_Filename_2 = config.getSetting( "SplatFilename2" );
        Splat_Filename_3 = config.getSetting( "SplatFilename3" );   

#ifndef _MAPSPLITTER

        // PLUGIN OPTIONS ONLY

        colored = ( config.getSetting( "VertexColors" ) == "yes" );
        coverage_vertex_color = (config.getSetting( "CoverageVertexColor" ) == "yes" );
        base_vertex_color = (config.getSetting( "BaseVertexColor" ) == "yes" );
        vertex_shadowed = (config.getSetting( "BaseVertexShadow" ) == "yes" );
        vertex_instant_colored = (config.getSetting( "BaseVertexInstantColor" ) == "yes" );
        
        // ensure combination are correct 
        if (vertex_shadowed)
            vertex_instant_colored = true;

        if (coverage_vertex_color || base_vertex_color
            )// || vertex_shadowed || vertex_instant_colored)
            colored = true;
           
	    num_renderables = static_cast<uint> ( StringUtil::toReal( config.getSetting( "MaxNumRenderables" ) ));
	    num_renderables_increment =  static_cast<uint> ( StringUtil::toReal( config.getSetting( "IncrementRenderables" )));
	    num_tiles = static_cast<uint> ( StringUtil::toReal( config.getSetting( "MaxNumTiles" )) );
	    num_tiles_increment =  static_cast<uint> ( StringUtil::toReal( config.getSetting( "IncrementTiles" )));
       
        cameraThreshold = StringUtil::toReal( config.getSetting( "CameraThreshold" ) );
	    // To avoid the use of a square root.
	    cameraThreshold *= cameraThreshold;

	    num_renderables_loading = static_cast<uint> ( StringUtil::toReal( config.getSetting( "NumRenderablesLoading" ) ));
	    max_adjacent_pages = static_cast<uint> ( StringUtil::toReal( config.getSetting( "MaxAdjacentPages" ) ));
	    max_preload_pages = static_cast<uint> ( StringUtil::toReal( config.getSetting( "MaxPreloadedPages" ) ));
        maxRenderLevel = static_cast<uint> ( StringUtil::toReal( config.getSetting( "MaxRenderLevel" ) ));
        if (maxRenderLevel == 0)
        {
            while (static_cast<uint> ((1 << maxRenderLevel)) < TileSize)
                maxRenderLevel++;
        }

	    change_factor = StringUtil::toReal( config.getSetting( "ChangeFactor" ) ) * 
                                                ( static_cast <Real> ( PageSize * ( scale.z + scale.x ) * 0.5f ) / 9 );
	    visible_renderables = StringUtil::toReal( config.getSetting( "VisibleRenderables" ) );

        //Real Factor = TileSize * scale.x + TileSize * scale.z;
	    // compute the actual distance as a square

        // Factor is a Tile distance squared
        Real Factor = TileSize;
        Factor = Factor * scale.x * Factor * scale.z;

	    renderable_factor = visible_renderables * Factor;
	    //renderable_factor *= renderable_factor;

	    distanceLOD = StringUtil::toReal( config.getSetting( "DistanceLOD" ) );
	    // Compute the actual distance as a square
	    LOD_factor = distanceLOD * Factor;
	    //LOD_factor *= LOD_factor;

        lit = ( config.getSetting( "VertexLit" ) == "yes" );
        normals = ( config.getSetting( "VertexNormals" ) == "yes" );
        
        Deformable = config.getSetting( "Deformable" ) == "yes";

        //Morphing
        lodMorph = (config.getSetting( "VertexProgramMorph" ) == "yes" );
        lodMorphStart = StringUtil::toReal( config.getSetting( "LODMorphStart" ) ) ;
        maxPixelError = static_cast <size_t> (StringUtil::toReal( config.getSetting( "MaxPixelError" ) ) ) ;


        BigImage = (config.getSetting( "BigImage" ) == "yes" );
        VisMap = (config.getSetting( "HorizonVisibilityComputing" ) == "yes" );
        VertexCompression = (config.getSetting( "VertexCompression" ) == "yes" ) && hasVertexShader;

        setTextureFormat (config.getSetting( "TextureFormat" ));

#else 
        
        // MAP TOOL OPTIONS ONLY

        Paged = (config.getSetting( "Paged" ) == "yes" );

        OutDirectory = config.getSetting( "OutDirectory" );
	    if (StringUtil::endsWith  (OutDirectory, "landscapefilename", true) )
        {
            String BasePath, FilePath;
            StringUtil::splitFilename(OutDirectory, BasePath, FilePath);
            OutDirectory = FilePath + landscape_filename;
        }

        MiniMap = (config.getSetting( "MiniMap" ) == "yes" );
        PVSMap = (config.getSetting( "PVSMap" ) == "yes" );
        BaseMap = (config.getSetting( "BaseMap" ) == "yes" );
        RGBMaps = (config.getSetting( "RGBMaps" ) == "yes" );
        ColorMapGenerate = (config.getSetting( "ColorMapGenerate" ) == "yes" );
        ColorMapSplit = (config.getSetting( "ColorMapSplit" ) == "yes" );
        LightMap = (config.getSetting( "LightMap" ) == "yes" );
        NormalMap = (config.getSetting( "NormalMap" ) == "yes" );
        HeightMap = (config.getSetting( "HeightMap" ) == "yes" );
        AlphaMaps = (config.getSetting( "AlphaMaps" ) == "yes" );
        ShadowMap = (config.getSetting( "ShadowMap" ) == "yes" );
        HorizonMap = (config.getSetting( "HorizonMap" ) == "yes" );  
        LitBaseMap = (config.getSetting( "LitBaseMap" ) == "yes" );
        InfiniteMap = (config.getSetting( "InfiniteMap" ) == "yes" );
        CoverageMap = (config.getSetting( "CoverageMap" ) == "yes" );
        LitColorMapGenerate = (config.getSetting( "LitColorMapGenerate" ) == "yes" );
        LitColorMapSplit = (config.getSetting( "LitColorMapSplit" ) == "yes" );
        ElevationMap= (config.getSetting( "ElevationMap" ) == "yes" ); 
        HeightNormalMap = (config.getSetting( "HeightNormalMap" ) == "yes" );
        AlphaSplatRGBAMaps =  (config.getSetting( "AlphaSplatRGBAMaps" ) == "yes" );
        AlphaSplatLightMaps = (config.getSetting( "AlphaSplatLightMaps" ) == "yes" );

        ColorMapName  = config.getSetting( "ColorMapName" );

        HeightMapBlurFactor = StringUtil::toReal( config.getSetting( "HeightMapBlurFactor" ) );
        Sun = Vector3( StringUtil::toReal( config.getSetting( "Sunx" ) ),
                        StringUtil::toReal( config.getSetting( "Suny" ) ),
                        StringUtil::toReal( config.getSetting( "Sunz" ) ));
        Amb = StringUtil::toReal( config.getSetting( "Ambient" ) );
        Diff = StringUtil::toReal( config.getSetting( "Diffuse" ) );
        Blur = static_cast<int> (StringUtil::toReal( config.getSetting( "Blur" ) ));

        Equalize = (config.getSetting( "Equalize" ) == "yes" );
        ZHorizon = (config.getSetting( "ZHorizon" ) == "yes" );

        SRTM_water = (config.getSetting( "SRTM_water" ) == "yes" );

        MiniMapHeight = static_cast<uint> (StringUtil::toReal( config.getSetting( "MiniMapHeight" )));
        MiniMapWidth = static_cast<uint> (StringUtil::toReal( config.getSetting( "MiniMapWidth" )));

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

            CFactor =  A / T;
        }
    }  
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::setPrimaryCamera(PagingLandScapeCamera *cam)
    {
        primaryCamera = cam;
        calculateCFactor (); 
    } 
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::setOption( const String& strKey, const void* pValue )
    {
	    if ( strKey == "VisibleRenderables" )
	    {
		    visible_renderables = * static_cast < const int * > ( pValue );
		    // compute the actual distance as a square
            Real Factor = TileSize;
            Factor = Factor * scale.x * Factor * scale.z;

	        renderable_factor = visible_renderables * Factor;
            return true;
	    }
	    if ( strKey == "DistanceLOD" )
	    {
		    distanceLOD = * static_cast < const Real * > ( pValue );
		    // Compute the actual distance as a square
            Real Factor = TileSize;
            Factor = Factor * scale.x * Factor * scale.z;
	        LOD_factor = distanceLOD * Factor;
            return true;
	    }
	    if ( strKey == "Sun" )
	    {
            Sun = * static_cast < const Vector3 * > ( pValue );
		    lightmoved = true;
            return true;
	    }
	    if ( strKey == "SunAngle" )
	    {
            SunAngle = * static_cast < const Real * > ( pValue );
		    lightmoved = true;
            return true;
	    }
	    if ( strKey == "Width" )
	    {
		    world_width = * static_cast < const uint * > ( pValue ) ;
#ifndef _MAPSPLITTER
#ifndef _MAPEDITOR
                assert (primaryCamera);
		        static_cast <PagingLandScapeSceneManager *> (primaryCamera->getSceneManager())->WorldDimensionChange();

#endif //_MAPEDITOR
#endif  //_MAPSPLITTER
		    return true;
	    }
	    if ( strKey == "Height" )
	    {
		    world_height = * static_cast < const uint * > ( pValue );
#ifndef _MAPSPLITTER
#ifndef _MAPEDITOR
                assert (primaryCamera);
		        static_cast <PagingLandScapeSceneManager *> (primaryCamera->getSceneManager())->WorldDimensionChange();
#endif //_MAPEDITOR
#endif  //_MAPSPLITTER
		    return true;
	    }
	    if ( strKey == "WorldDimension" )
	    {
		    Vector2 dim =  * static_cast < const Vector2 * > ( pValue );
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
	    if ( strKey == "primaryCamera" )
	    {
		    setPrimaryCamera (const_cast < PagingLandScapeCamera * > (static_cast < const PagingLandScapeCamera * > ( pValue )));
		    return true;
	    }

		if ( strKey == "TextureNameLayer0" )
	    {
			Splat_Filename_0 = * static_cast < const String * > ( pValue );
		    return true;
	    }
		if ( strKey == "TextureNameLayer1" )
	    {
			Splat_Filename_1 = * static_cast < const String * > ( pValue );
		    return true;
	    }
		if ( strKey == "TextureNameLayer2" )
	    {
			Splat_Filename_2 =  * static_cast < const String * > ( pValue );
		    return true;
	    }
		if ( strKey == "TextureNameLayer3" )
	    {
			Splat_Filename_2 =  * static_cast < const String * > ( pValue );
		    return true;
	    }


	    return false;
    }
       
    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::getOption( const String& strKey, void* pDestValue )
    {
	    if ( strKey == "VisibleRenderables" )
	    {
		    * static_cast < int * > ( pDestValue ) = static_cast<int> (visible_renderables);
		    return true;
	    }
	    if ( strKey == "DistanceLOD" )
	    {
		    * static_cast < Real * > ( pDestValue ) = static_cast<Real> (distanceLOD);
		    return true;
	    }
	    if ( strKey == "VisibleDistance" )
	    {
		    // we need to return the square root of the distance
            * static_cast < Real * > ( pDestValue ) =  Math::Sqrt (renderable_factor);
            return true;
	    }
	    if ( strKey == "VisibleLOD" )
	    {
		    // we need to return the square root of the distance
		    * static_cast < Real * > ( pDestValue ) =  Math::Sqrt (LOD_factor);
            return true;
	    }
	    // Some options proposed by Praetor
	    if ( strKey == "Width" )
	    {
		    * static_cast < int * > ( pDestValue ) = world_width;
		    return true;
	    }
	    if ( strKey == "Height" )
	    {
		    * static_cast < int * > ( pDestValue ) = world_height;
		    return true;
	    }
	    if ( strKey == "PageSize" )
	    {
		    * static_cast < int * > ( pDestValue ) = PageSize;
		    return true;
	    }
	    if ( strKey == "ScaleX" )
	    {
		    * static_cast < Real * > ( pDestValue ) = scale.x;
		    return true;
	    }
	    if ( strKey == "ScaleY" )
	    {
		    * static_cast < Real * > ( pDestValue ) = scale.y;
		    return true;
	    }
	    if ( strKey == "ScaleZ" )
	    {
		    * static_cast < Real * > ( pDestValue ) = scale.z;
		    return true;
	    }	
    
        if ( strKey == "TextureNameLayer0" )
	    {
			* static_cast < String * > ( pDestValue ) =  Splat_Filename_0;
		    return true;
	    }
		if ( strKey == "TextureNameLayer1" )
	    {
			* static_cast < String * > ( pDestValue ) =  Splat_Filename_1;
		    return true;
	    }
		if ( strKey == "TextureNameLayer2" )
	    {
			* static_cast < String * > ( pDestValue ) =  Splat_Filename_2;
		    return true;
	    }
		if ( strKey == "TextureNameLayer3" )
	    {
			* static_cast < String * > ( pDestValue ) =  Splat_Filename_3;
		    return true;
	    }

		if ( strKey == "GroupName" )
	    {
			* static_cast < String * > ( pDestValue ) =  groupName;
		    return true;
	    }
	    return false;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::hasOption( const String& strKey ) const
    {
	    if ( strKey == "VisibleRenderables" )
	    {
		    return true;
	    }
	    if ( strKey == "DistanceLOD" )
	    {
		    return true;
	    }
	    if ( strKey == "VisibleDistance" )
	    {
		    return true;
	    }
	    if ( strKey == "VisibleLOD" )
	    {
		    return true;
	    }
	    // Some options proposed by Praetor
	    if ( strKey == "Width" )
	    {
		    return true;
	    }
	    if ( strKey == "Height" )
	    {
		    return true;
	    }
	    if ( strKey == "PageSize" )
	    {
		    return true;
	    }
	    if ( strKey == "ScaleX" )
	    {
		    return true;
	    }
	    if ( strKey == "ScaleY" )
	    {
		    return true;
	    }
	    if ( strKey == "ScaleZ" )
	    {
		    return true;
	    }
	    return false;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::getOptionValues( const String & key, StringVector &refValueList )
    {
    //	if ( key == "VisibleRenderables" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "DistanceLOD" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "VisibleDistance" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "VisibleLOD" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "Width" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "Height" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "PageSize" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "ScaleX" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "ScaleY" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
    //	if ( key == "ScaleZ" )
    //	{
    //		refValueList.push_back(DataStreamPtr());
    //		return true;
    //	}
	    return false;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeOptions::getOptionKeys( StringVector &refKeys )
    {
	    refKeys.push_back( "VisibleRenderables" );
	    refKeys.push_back( "DistanceLOD" );
	    refKeys.push_back( "VisibleDistance" );
	    refKeys.push_back( "VisibleLOD" );
	    // Some options from Praetor
	    refKeys.push_back( "Width" );
	    refKeys.push_back( "Height" );
	    refKeys.push_back( "PageSize" );
	    refKeys.push_back( "ScaleX" );
	    refKeys.push_back( "ScaleY" );
	    refKeys.push_back( "ScaleZ" );
	    return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOptions::getAvgColors()
    {
        matColor[0] = _getAvgColor(Splat_Filename_0);
        matColor[1] = _getAvgColor(Splat_Filename_1);
        matColor[2] = _getAvgColor(Splat_Filename_2);
        matColor[3] = _getAvgColor(Splat_Filename_3);
    }
    //-----------------------------------------------------------------------
    ColourValue PagingLandScapeOptions::_getAvgColor(const String &tex) const
    {
        if (tex == "")
            return ColourValue ( 1.0f, 1.0f, 1.0f, 1.0f );

	    Image img;

	    img.load (tex, PagingLandScapeOptions::getSingleton().groupName);
	    const uchar* data = img.getData();
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

	    for (size_t i = 0; i < img.getSize(); i += bpp)
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
