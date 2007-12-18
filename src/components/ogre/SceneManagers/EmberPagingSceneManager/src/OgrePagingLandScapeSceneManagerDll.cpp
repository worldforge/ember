/***************************************************************************
	OgrePagingLandScapeSceneManagerDll.cpp  -  description
	-------------------
	begin                : Mon May 12 2003
	copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
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

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgreRoot.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"

namespace Ogre
{
    PagingLandScapeSceneManagerFactory* PagingLandScapePlugin;

    extern "C" void _OgrePagingLandScapeExport dllStartPlugin(void)
    {
	    // Create new scene manager  Factory
	    PagingLandScapePlugin = new PagingLandScapeSceneManagerFactory();

	    // Register  Factory
	    Root::getSingleton().addSceneManagerFactory(PagingLandScapePlugin);
    }

    extern "C" void _OgrePagingLandScapeExport dllShutdownPlugin(void)
    {	
        Root::getSingleton().removeSceneManagerFactory(PagingLandScapePlugin);
    }

    extern "C" void _OgrePagingLandScapeExport dllStopPlugin(void)
    {
	    delete PagingLandScapePlugin;
    }
}
