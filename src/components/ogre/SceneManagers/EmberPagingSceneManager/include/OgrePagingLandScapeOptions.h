/***************************************************************************
	OgrePagingLandScapeOptions.h  -  description
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

#ifndef PAGINGLandScapeOPTIONS_H
#define PAGINGLandScapeOPTIONS_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgreStringVector.h"
#include "OgreConfigFile.h"
#include "OgreMaterial.h"

namespace Ogre
{
    /**
    * A simple class for encapsulating parameters passed in when initializing *
    * a LandScapeRenderable
    */
    class PagingLandScapeOptions
    {
        public:
	        PagingLandScapeOptions(PagingLandScapeSceneManager * scnMgr);

	        ~PagingLandScapeOptions(void);

            void init();
            void setDefault();

            bool load(DataStreamPtr& stream);
            bool load(const String &filename, ConfigFile& config);
            bool load(const String &filename);

            void loadMap(const String& mapName);
            void loadMapOptions(DataStreamPtr &stream);
            void insertMap(const String& mapName);

            LandScapeFileNames* getLandScapeFileNames(void)
			{
				return &mMapList;
			};

	        bool setOption(const String& strKey, const void* pValue);
	        bool getOption(const String& strKey, void* pDestValue);

	        bool hasOption(const String& strKey) const ; 

	        bool getOptionValues(const String & key, StringVector &refValueList);
            bool getOptionKeys(StringVector &refKeys);

            void getAvgColors(void);

            LandScapeFileNames& getMapList(void);

			const String& getMapFilename(const String &currMapName) const;
            const String& getPreviousMapName(void) const;
            const String& getNextMapName(void) const;
            const String& getCurrentMapName(void) const; 

            void setCurrentMapName(const String& mapName);

            const String& getCurrentTextureFormat(void) const;
			void setTextureFormat(const String& format);
			void insertTextureFormat (const String &format) ;

            void setPrimaryCamera(PagingLandScapeCamera* cam);
            void calculateCFactor(void);

	        String data2DFormat;
	        String textureFormat;

            String TerrainName;

	        String LandScape_filename;
            String LandScape_extension;
	        String LandScape_export_filename;
            String LandScape_export_extension;

            String image_filename;
            bool ImageNameLoad;


            // MAP TOOL OPTIONS
            String OutDirectory;
            String TextureExtension;
            String TextureExportExtension;

            bool Paged;
            bool BaseMap;
            bool MiniMap;
            bool ColorMapSplit;
            bool ColorMapGenerate;
            bool LightMap;
            bool NormalMap;
            bool HeightMap;
            bool AlphaMaps;
            bool LitBaseMap;
            bool LitColorMapSplit;
            bool LitColorMapGenerate;
            bool InfiniteMap;
            bool CoverageMap;
            bool HeightNormalMap;

            Real HeightMapBlurFactor;
            
            String ColorMapName;

            bool    lightmoved;
            Vector3 Sun;
            Real    SunAngle;
            Real    Amb;
            Real    Diff;

            bool mUseLodMapCache;

            unsigned int     Blur;
            // end of MAP TOOL OPTIONS

	        unsigned int maxValue;						//Compression range for the TC height field
	        unsigned int minValue;

            unsigned int TileSize;
            unsigned int PageSize;						//size of the page.
            Real invTileSizeMinusOne;
            Real invPageSizeMinusOne;					
            unsigned int NumTiles;
            unsigned int NumPages;

	        unsigned int world_height;					//world page height, from 0 to height, in page number
	        unsigned int world_width;					//world page width, from 0 to width, in page number

	        Real maxScaledZ;					//world page height, scaled
	        Real maxScaledX;					//world page width, scaled

            Real maxUnScaledZ;					//world page height, scaled
	        Real maxUnScaledX;					//world page width, scaled


	        Real change_factor;					//Determines the value of the change factor for loading/unloading LandScape Pages
	        unsigned int max_adjacent_pages;
	        unsigned int max_preload_pages;
	        Real visible_renderables;			//Numbers of visible renderables surrounding the camera
	        Real renderable_factor;				//Determines the distance of loading and unloading of renderables in renderable numbers

			Vector3 position;				//Startup position of the terrain surface			
			Vector3 scale;
			Vector3	invScale;

	        Material::LodValueList lodMaterialDistanceList; //Distance for the material LOD change
	        Real distanceLOD;					//Distance for the LOD change
	        Real LOD_factor;
            bool roughnessLod;

	        unsigned int num_renderables;				//Max number of renderables to use.
	        unsigned int num_renderables_increment;		//Number of renderables to add in case we run out of renderables
	        unsigned int num_tiles;						//Max number of tiles to use.
	        unsigned int num_tiles_increment;			//Number of renderables to add in case we run out of renderables

	        Real cameraThreshold;				//If the last camera position is >= the the scene is transverse again.

	        unsigned int num_renderables_loading;		//Max number of renderable to load in a single Frame.
            unsigned int maxRenderLevel;

			unsigned int NumMatHeightSplat;
			std::vector <ColourValue>	matColor;
			std::vector <Real>			matHeight;
			std::vector<String>			SplatDetailMapNames;

            bool VertexCompression;

            bool hasVertexShader;
            bool hasFragmentShader;
            bool hasFragmentShader2;
            unsigned int numTextureUnits;
            bool isRenderGL;

            Real ScaledPageSizeX;
            Real ScaledPageSizeZ;
            Real ScaledHeightY;


            /// used to test all texture format without
            /// restrictions (renderer or capabilities)
            bool TextureFormatDebug;
            /// Can terrain be deformed real-time
            bool Deformable;
            /// are deformation saved?
            bool saveDeformation;


            //used to get screen height...
            // should find another way...
            PagingLandScapeCamera* primaryCamera;

            /// At what point (parametric) should LOD morphing start
            Real lodMorphStart;
            bool lodMorph;
            unsigned int maxPixelError;
            Real CFactor;

            // MapSplitter Tool
            unsigned int RawHeight;
            unsigned int RawWidth;
               
            bool isRaw;
            bool Equalize;
            bool ZHorizon;
            bool SRTM_water;

            unsigned int MiniMapHeight;
            unsigned int MiniMapWidth;

            // Generate one or all maps ?
            bool mBatchMode;

            // Both

            /// ResourceGroupName
            String groupName;
            String cfgGroupName;
            
	        bool lit;
	        bool normals;
	        bool colored;

            bool coverage_vertex_color;
            bool base_vertex_color;
            bool vertex_shadowed;
            bool vertex_instant_colored;

            bool BigImage;
            bool VisMap;
			bool MaxLodUnderCam;

            Real TextureStretchFactor;

			unsigned int RenderableLoadInterval;

			unsigned int TileInvisibleUnloadFrames;
			unsigned int PageInvisibleUnloadFrames;

			unsigned int NumSplatMapToSplit;
			std::vector<String> SplatMapNames;

            unsigned int NumTextureFormatSupported;
            std::vector<String> TextureFormatSupported;

			bool  queryNoInterpolation;
			Real  queryResolutionFactor;

			bool materialPerPage;
			bool textureModifiable;

			Vector3 BaseCameraViewpoint;
			Vector3 Baselookat;
			
            bool setUint (unsigned int &u, const String &ValuetoGet);
            bool setBool (bool &b, const String &ValuetoGet);
            bool setReal (Real &r,const String &ValuetoGet);
			bool setColourValue(ColourValue &r,const String &ValuetoGet);
            bool setString (String &s, const String &ValuetoGet);

            StringVector mResourceFilesystem;
            StringVector mResourceZip;

            PagingLandScapeSceneManager *mScnMgr;

			void setTileInfo(PagingLandScapeTileInfo *t);
			PagingLandScapeTileInfo *getTileInfo(const uint pageX, const uint pageZ, 
												const uint tileX, const uint tileZ);
			void loadMapInfo();
			void clearTileInfo();
			void saveMapInfo();

			/**
			 * @brief The default height of any uninitialized terrain.
			 * Any terrain at exactly this height should be removed from the LOD calculation, as it's most often "surrounding" terrain.
			 */
			float uninitializedHeight;

        private:

            void loadcfg (const String &filename, ConfigFile& config);
	        ColourValue _getAvgColor(const String& tex) const;
            String mCurrentMap;
            LandScapeFileNames mMapList;
            StringVector mTextureFormats;
            bool isInit;
			ConfigFile *mConfig;

			std::deque<PagingLandScapeTileInfo*> mTileInfoCache;
    };

}

#endif
