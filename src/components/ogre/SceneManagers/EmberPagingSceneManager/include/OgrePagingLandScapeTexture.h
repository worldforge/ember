/***************************************************************************
	OgrePagingLandScapeTexture.h  -  description
  -------------------
  begin                : Fri Apr 16 2004
  copyright            : (C) 2002-2005 by Jose A Milan & Tuan Kuranes
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

#ifndef PAGINGLANDSCAPETEXTURE_H
#define PAGINGLANDSCAPETEXTURE_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    /**
    * A simple class for encapsulating Texture generation.
    */
    class PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture( void );

	        virtual ~PagingLandScapeTexture( void );

	        virtual void load( uint mX, uint mZ );
            virtual void update( void ) {};
	        virtual void unload( void );

            virtual void setPagesize( void ){};
            virtual void clearData( void ){};

	        virtual String getName ( void ) = 0;

	        virtual PagingLandScapeTexture *newTexture( ) = 0;
            virtual bool TextureRenderCapabilitesFullfilled() 
            {
                return true;
            };

            virtual const uint getNumChannels ( void ) const {return mNumChannel;};

            bool getCastsShadows( void ) const {return false;}

	        bool isLoaded( void ) const;

            void setNeedUpdate( void );
            void updated( void );
            bool needUpdate( void ) const;

	        const MaterialPtr& getMaterial( void ) const;

            virtual void paint (const uint x, const uint z, 
                        const Real paintForce, const ColourValue &mPaintColor){};

            virtual void paint (const uint x, const uint z, 
                        const Real paintForce, const uint mPaintChannel){};

            virtual void deformheight (const uint x, const uint z, 
                        const Real paintForce){};

            void adjustDeformationRectangle(uint x, uint z);
            void adjustPaintRectangle(uint x, uint z);
        protected:
	        virtual void _loadMaterial( void ) = 0;

            virtual void _unloadMaterial( void ){};

	        bool mIsLoaded;
            bool mIsModified;
	        //! Pointer to ogre material
	        MaterialPtr mMaterial;
	        //! what page it correspond to
	        uint  mDataX, mDataZ;

            uint        mNumChannel;
            Image::Box  mPaintRect;
            bool        mIsPaintRectModified;
            Image::Box  mDeformRect;
            bool        mIsDeformRectModified;
    };
}

#endif
