/***************************************************************************
	OgrePagingLandScapeSceneManagerDll.cpp  -  description
	-------------------
	begin                : Mon May 12 2003
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
#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"

namespace Ogre
{
    PagingLandScapeSceneManager* PagingLandScapePlugin;

    extern "C" void _OgrePagingLandScapeExport dllStartPlugin( void )
    {
	    // Create new scene manager
	    PagingLandScapePlugin = new PagingLandScapeSceneManager();

	    // Register
	    Root::getSingleton().setSceneManager( ST_EXTERIOR_REAL_FAR, PagingLandScapePlugin );
    }

    extern "C" void _OgrePagingLandScapeExport dllStopPlugin( void )
    {
	    delete PagingLandScapePlugin;
    }
}
