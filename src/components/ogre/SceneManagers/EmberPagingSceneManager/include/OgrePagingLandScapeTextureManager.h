/***************************************************************************
	OgrePagingLandScapeTextureManager.h  -  description
	-------------------
  begin                : Fri Apr 16 2004
  copyright            : (C) 2003-2005 by Jose A. Milan and Tuan Kuranes
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

#ifndef PAGINGLandScapeTEXTUREMANAGER_H
#define PAGINGLandScapeTEXTUREMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

    class PagingLandScapeTextureManager
    {
        public:

	        PagingLandScapeTextureManager(PagingLandScapeSceneManager * scnMgr);

	        virtual ~PagingLandScapeTextureManager(void);

            void load(void);
            void clear(void);
            void WorldDimensionChange(void);
            PagingLandScapeTexture* allocateTexture() const;
            void reset(void);

	        void load(const uint texX, const uint texZ);
            void reload(const uint dataX, const uint dataZ);
	        void unload(const uint texX, const uint texZ);

         	bool isLoaded(const uint texX, const uint texZ);


	        const MaterialPtr& getMaterial(const uint texX, const uint texZ);


            MaterialPtr getMapMaterial(void);
            void setMapMaterial(void);

            void setPaintChannel (const uint channel){mPaintChannel = channel;};
            void setPaintColor (const ColourValue &color){mPaintColor = color;};
            void paint (const Vector3 &currpoint, const Real paintForce, 
                        const PagingLandScapeTileInfo *info, const bool isAlpha);
            void DeformHeight (const Vector3 &currpoint,
                                const PagingLandScapeTileInfo *info);
  
            String getNextTextureFormat();
            String getCurrentTextureFormat();
            
            void registerTextureType(PagingLandScapeTexture* source)
            {
                mTextureTypeMap.push_back(source);
            }

			PagingLandScapeTexture* getTexture(const uint i, const uint j,
				const bool alwaysReturn = true);
			PagingLandScapeTexture* getNewTexture(const uint i, const uint j);
			void releaseTexture (PagingLandScapeTexture*p );

            PagingLandScapeSceneManager *getSceneManager(){return mSceneManager;}
            PagingLandScapeOptions*		getOptions(){return mOptions;}
    protected:
            PagingLandScapeSceneManager *mSceneManager;
            PagingLandScapeOptions*		mOptions;


            uint                        mTextureType;
            String                      mTextureFormat;

            uint                        mWidth;
            uint                        mHeight;

            uint                        mTexturePageSize;
     
	        //PagingLandScapeTexturePages mTexture;

            MaterialPtr                 mMapMaterial;

            uint                        mPaintChannel;
            ColourValue                 mPaintColor;

            

        PagingLandScapeTextureMap mTextureTypeMap;
        /// The currently active page Data2d source
		PagingLandScapeTexture* mActiveTextureType;

		PagingLandScapeTextureList mActiveTextures;
		PagingLandScapeTextureList mFreeTextures;
		PagingLandScapeTextureArray mTexturePool;
    };

} //namespace

#endif
