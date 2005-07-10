/***************************************************************************
	OgrePagingLandScapeOptions.h  -  description
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

#ifndef PAGINGLANDSCAPEOPTIONS_H
#define PAGINGLANDSCAPEOPTIONS_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreStringVector.h"

namespace Ogre
{
    /**
    * A simple class for encapsulating parameters passed in when initializing *
    * a LandScapeRenderable
    */
    class PagingLandScapeOptions : public Singleton< PagingLandScapeOptions >
    {
        public:
	        PagingLandScapeOptions( void );

	        ~PagingLandScapeOptions( void );

            void init(  );

	        void load( const String& filename );
            void loadMap( const String& mapName );
            void loadMapOptions( const String& mapName );
            void insertMap( const String& mapName );

            LandscapeFileNames* getLandscapeFileNames( void )
			{
				return &mMapList;
			};

	        bool setOption( const String& strKey, const void* pValue );
	        bool getOption( const String& strKey, void* pDestValue );

	        bool hasOption( const String& strKey ) const ; 

	        bool getOptionValues( const String & key, StringVector &refValueList );
            bool getOptionKeys( StringVector &refKeys );

            void getAvgColors( void );

            LandscapeFileNames& getMapList( void );

            const String& getPreviousMapName( void ) const;
            const String& getNextMapName( void ) const;
            const String& getCurrentMapName( void ) const; 

            void setCurrentMapName( const String& mapName );

            const String& getCurrentTextureFormat( void ) const;
            void setTextureFormat( const String& format );

            void setPrimaryCamera( PagingLandScapeCamera* cam );
            void calculateCFactor( void );

	        static PagingLandScapeOptions& getSingleton( void );
	        static PagingLandScapeOptions* getSingletonPtr( void );

	        String data2DFormat;
	        String textureFormat;

	        String landscape_filename;
            String landscape_extension;
            String landscape_export_extension;

            String image_filename;
            bool ImageNameLoad;


            // MAP TOOL OPTIONS
            String Splat_Filename_0;
            String Splat_Filename_1;
            String Splat_Filename_2;
            String Splat_Filename_3;

            String OutDirectory;
            String TextureExtension;

            bool Paged;
            bool PVSMap;
            bool BaseMap;
            bool MiniMap;
            bool RGBMaps;
            bool ColorMapSplit;
            bool ColorMapGenerate;
            bool LightMap;
            bool NormalMap;
            bool HeightMap;
            bool AlphaMaps;
            bool ShadowMap;
            bool HorizonMap;
            bool LitBaseMap;
            bool LitColorMapSplit;
            bool LitColorMapGenerate;
            bool InfiniteMap;
            bool CoverageMap;
            bool ElevationMap;
            bool HeightNormalMap;
            bool AlphaSplatRGBAMaps;
            bool AlphaSplatLightMaps;

            Real HeightMapBlurFactor;
            
            String ColorMapName;

            bool    lightmoved;
            Vector3 Sun;
            Real    SunAngle;
            Real    Amb;
            Real    Diff;

            int     Blur;
            // end of MAP TOOL OPTIONS

	        uint maxValue;						//Compression range for the TC height field
	        uint minValue;

            uint TileSize;
	        uint PageSize;						//size of the page.
            uint NumTiles;
            uint NumPages;

	        uint world_height;					//world page height, from 0 to height, in page number
	        uint world_width;					//world page width, from 0 to width, in page number

	        Real maxScaledZ;					//world page height, scaled
	        Real maxScaledX;					//world page width, scaled

            Real maxUnScaledZ;					//world page height, scaled
	        Real maxUnScaledX;					//world page width, scaled


	        Real change_factor;					//Determines the value of the change factor for loading/unloading LandScape Pages
	        uint max_adjacent_pages;
	        uint max_preload_pages;
	        Real visible_renderables;			//Numbers of visible renderables surrounding the camera
	        Real renderable_factor;				//Determines the distance of loading and unloading of renderables in renderable numbers

	        Vector3 scale;

	        Real distanceLOD;					//Distance for the LOD change
	        Real LOD_factor;


	        uint num_renderables;				//Max number of renderables to use.
	        uint num_renderables_increment;		//Number of renderables to add in case we run out of renderables
	        uint num_tiles;						//Max number of tiles to use.
	        uint num_tiles_increment;			//Number of renderables to add in case we run out of renderables

	        Real cameraThreshold;				//If the last camera position is >= the the scene is transverse again.

	        uint num_renderables_loading;		//Max number of renderable to load in a single Frame.
            uint maxRenderLevel;
	        ColourValue matColor[4];
	        Real matHeight[4];

            bool VertexCompression;

            bool hasVertexShader;
            bool hasFragmentShader;
            bool hasFragmentShader2;
            uint numTextureUnits;
            bool isRenderGL;

            Real ScaledPageSizeX;
            Real ScaledPageSizeZ;
            Real ScaledHeightY;


            /// used to test all texture format without
            /// restrictions (renderer or capabilities)
            bool TextureFormatDebug;
            /// Can terrain be deformed real-time
            bool Deformable;


            //used to get screen height...
            // should find another way...
            PagingLandScapeCamera* primaryCamera;
            /// At what point (parametric) should LOD morphing start
            Real lodMorphStart;
            bool lodMorph;
            size_t maxPixelError;
            Real CFactor;
            // MapSplitter Tool

            uint RawHeight;
            uint RawWidth;
               
            bool isRaw;
            bool Equalize;
            bool ZHorizon;
            bool SRTM_water;

            uint MiniMapHeight;
            uint MiniMapWidth;

            // Generate one or all maps ?
            bool mBatchMode;

            // Both

            /// ResourceGroupName
            String groupName;
	        bool lit;
	        bool normals;
	        bool colored;

            bool coverage_vertex_color;
            bool base_vertex_color;
            bool vertex_shadowed;
            bool vertex_instant_colored;

            bool BigImage;
            bool VisMap;

        private:
	        ColourValue _getAvgColor( const String& tex ) const;
            String mCurrentMap;
            LandscapeFileNames mMapList;
            StringVector mTextureFormats;
            bool isInit;
    };

}

#endif
