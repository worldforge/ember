/***************************************************************************
OgrePagingLandScapeRenderableManager.h  -  description
-------------------
begin                : Mon Jun 16 2003
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLandScapeTextureCoordinatesMANAGER_H
#define PAGINGLandScapeTextureCoordinatesMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
	/** Class to manage the creation, destruction and use of PagingLandScapeRenderables.
	@remarks
	This class is used as a store of PagingLandScapeTexturecoordinatesBuffer shared between Page 
	*/
	class PagingLandScapeTextureCoordinatesManager 
	{
	public:
		/** Initializes the PagingLandScapeTextureCoordinatesManager with the 
		* given options and allocate the necessary memory.
		*/
		PagingLandScapeTextureCoordinatesManager(PagingLandScapeSceneManager * scnMgr);
		virtual ~PagingLandScapeTextureCoordinatesManager(void);
		void load(void);
		void clear(void);

		HardwareVertexBufferSharedPtr getBuffer(const unsigned int tilex, const unsigned int tilez);

        PagingLandScapeOptions*		getOptions(){return mOptions;}
    protected:

        PagingLandScapeOptions*		mOptions;
       
		unsigned int mPageSize; 
		unsigned int mTileSize;
		
		HardwareTextureBuffersCol mTexBuffs; 
	};

}

#endif //PAGINGLandScapeTextureCoordinatesMANAGER_H
