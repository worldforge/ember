/***************************************************************************
	OgrePagingLandScapeTextureManager.h  -  description
	-------------------
  begin                : Fri Apr 16 2004
  copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as        *
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

	        void load(const unsigned int texX, const unsigned int texZ);
            void reload(const unsigned int dataX, const unsigned int dataZ);
	        void unload(const unsigned int texX, const unsigned int texZ);

         	bool isLoaded(const unsigned int texX, const unsigned int texZ);


	        const MaterialPtr& getMaterial(const unsigned int texX, const unsigned int texZ);


            MaterialPtr getMapMaterial(void);
            void setMapMaterial(void);

            void setPaintChannelValues (const std::vector<Real>  *theChannelModifList)
			{
				channelModifList = theChannelModifList;
			};
			void paint (const Vector3 &currpoint, 
				const Real paintForce, 
				const PagingLandScapeTileInfo *info);

            void deformHeight (const Vector3 &currpoint,
                                const PagingLandScapeTileInfo *info);
  
            String getNextTextureFormat();
            String getCurrentTextureFormat();

			void registerTextureFormats();
			void clearTextureFormats ();
            void registerTextureType(PagingLandScapeTexture* source)
            {
                mTextureTypeMap.push_back(source);
            }

			PagingLandScapeTexture* getTexture(const unsigned int i, const unsigned int j,
				const bool alwaysReturn = true);
			PagingLandScapeTexture* getNewTexture(const unsigned int i, const unsigned int j);
			void releaseTexture (PagingLandScapeTexture*p );

			unsigned int getNumChannels();
			unsigned int getNumChannelsperTexture(const size_t i);


            inline PagingLandScapeSceneManager *getSceneManager(){return mSceneManager;}
            inline PagingLandScapeOptions*		getOptions(){return mOptions;}
    

			unsigned int mPageSize;
			Image mImage;
			std::vector<Real> heights;
			std::vector<Real> dividers;
			std::vector<ColourValue> colors;

			const std::vector<Real> *channelModifList;
	protected:

			// Common var for all textures
			void setPageSize();
			void clearData ();

			//

            PagingLandScapeSceneManager *mSceneManager;
            PagingLandScapeOptions*		mOptions;


            unsigned int                        mTextureType;
            String                      mTextureFormat;

            unsigned int                        mWidth;
            unsigned int                        mHeight;

            unsigned int                        mTexturePageSize;
     
	        //PagingLandScapeTexturePages mTexture;

            MaterialPtr                 mMapMaterial;

            unsigned int                        mPaintChannel;
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
