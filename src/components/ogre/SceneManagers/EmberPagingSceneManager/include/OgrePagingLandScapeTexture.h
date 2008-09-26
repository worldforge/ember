/***************************************************************************
	OgrePagingLandScapeTexture.h  -  description
  -------------------
  begin                : Fri Apr 16 2004
  copyright            : (C) 2002-2006 by Jose A Milan & Tuan Kuranes
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

#ifndef PAGINGLandScapeTEXTURE_H
#define PAGINGLandScapeTEXTURE_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

#		define SAND  0
#		define GRASS 1
#		define ROCK  2
#		define SNOW  3

    /**
    * A simple class for encapsulating Texture generation.
    */
    class PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture(PagingLandScapeTextureManager *pageMgr, 
				const String materialName,
				const unsigned int numTexture = 0,
				bool isSplatMode = false);

			// Method that must be overridden
			virtual String getName (void) const {return mMaterialBaseName;};
			virtual PagingLandScapeTexture *newTexture()
			{
				return new PagingLandScapeTexture(mParent, mMaterialBaseName);
			};

			// Method that can be overridden
	        virtual ~PagingLandScapeTexture(void);

			virtual void bindCompressionSettings(GpuProgramParametersSharedPtr params);
			virtual void bindCompressionSettings();

	        virtual void load(unsigned int mX, unsigned int mZ);
            virtual void update(void);
			virtual void unload(void);

			virtual bool isMaterialSupported(bool recursive = true);
            virtual void setOptions(void);


			virtual const unsigned int getNumChannels (void) const 
			{
				return mNumTexture;
			};
			virtual const unsigned int getNumChannelsperTexture (const size_t i) const 
			{
				return static_cast<unsigned int>(mNumChannelperTexture[i]);
			};

			// Real Method
            bool getCastsShadows(void) const {return false;}

	        bool isLoaded(void) const;

            void setNeedUpdate(void);
            void updated(void);
            bool needUpdate(void) const;

	        const MaterialPtr& getMaterial(void) const;

            void paint (const unsigned int x, 
						const unsigned int z, 
						const Real paintForce);


            void adjustDeformationRectangle(unsigned int x, unsigned int z);
            void adjustPaintRectangle(unsigned int x, unsigned int z);
            
            virtual void lightUpdate();

			void getCoordinates(unsigned int& X, unsigned int& Z)
			{
				X = mDataX;
				Z = mDataZ;
			};
			inline bool isCoord(const unsigned int x, const unsigned int z)
            {
                return (mDataX == x && mDataZ == z);
            };

            const String &getMaterialName();

        protected:

			void compute(PagingLandScapeData2D* data, 
				const Image::Box& dataRect,
				const Image::Box& textureRect);


			void computePointAlpha(const unsigned int imagePos,
				const Real height, 
				const Real slope) ;

			void computePointColor(const unsigned int imagePos,
				const Real height, 
				const Real slope) ;

			void paintPoint (const unsigned int imagePos,
				const Real paintForce);

			void upload(const Image::Box& textureRect);

			void setNumTexture();

			// Method that can be overridden
			virtual void _loadMaterial(void);
			// Method that can be overridden
			virtual void _unloadMaterial(void);
			// Method that can be overridden
            virtual void _save(void);



			// properties that can be accessed from children 

            bool mIsSplatMode;
			bool mIsBaseMode;


	        bool mIsLoaded;
            bool mIsModified;
	        //! Pointer to ogre material
	        MaterialPtr mMaterial;
	        //! what page it correspond to
	        unsigned int  mDataX, mDataZ;

            Image::Box  mPaintRect;
            bool        mIsPaintRectModified;
            Image::Box  mDeformRect;
            bool        mIsDeformRectModified;
            PagingLandScapeTextureManager *mParent;

			// Edit, deform and update
			unsigned int								mNumTexture;
			std::vector<unsigned int>					mNumChannelperTexture;			
			std::vector<Image>							mImages;
			std::vector<TexturePtr>						mTextures;
			std::vector<HardwarePixelBufferSharedPtr>	mBuffers;
			std::vector<bool>							doTextureNeedUpdate;
			std::vector<bool>							isTextureModified;

			// special case alpha loading Image as A8 only.
			void loadAlphaTexture(const String& filename, const unsigned int channel);
			//  loading Image, buffer and texture in channels
			void loadColorTexture(const String& TexName, const unsigned int channel);
			// loading Image, buffer and texture.
			void loadTexture(const String &filename, Image &img);
			// Dynamic Lighting
			void computeLightMap () const;
			Image mShadow;
			Image mLightImage;
			TexturePtr mLightTexture;
			HardwarePixelBufferSharedPtr mLightBuffer;
			bool mPositiveShadow;
			bool mIsShaderShadowed;
			bool mIsShadowed;

			// Name that helps building textures names on each page
			String mMaterialBaseName;
			
			private :
				void loadTexturesToModify();
    };
}

#endif
