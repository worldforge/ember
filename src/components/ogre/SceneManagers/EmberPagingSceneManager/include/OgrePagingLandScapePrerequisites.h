/***************************************************************************
  OgrePagingLandScapePrerequisites.h  -  description
  -------------------
	begin                : Sun Oct 26 2003
	copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
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

#ifndef __PagingLandScapePrerequisites_H__
#define __PagingLandScapePrerequisites_H__

#include "OgrePrerequisites.h"

#include "OgreHardwareBufferManager.h"
#include "OgreTextureManager.h"
#include "OgreHardwarePixelBuffer.h"


//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

#include <vector>
#include <map>
#include <queue>
#include <list>

namespace Ogre
{
    class PagingLandScapeSceneManager;
    class PagingLandScapeOptions;
    class PagingLandScapeCamera;
    typedef std::list< PagingLandScapeCamera* > PagingLandscapeCameraList;

    class PagingLandScapeRaySceneQuery;
    class PagingLandScapeIntersectionSceneQuery;
    class PagingLandScapeHorizon;

    class PagingLandScapePage;
    class PagingLandScapePageRenderable;
    class PagingLandScapePageManager;
    typedef std::vector< PagingLandScapePage* > PagingLandScapePageRow;
    typedef std::vector< PagingLandScapePageRow > PagingLandScapePages;

    typedef std::list< PagingLandScapePage* > PagingLandScapePagelist;

    class PagingLandScapeTile;
    class PagingLandScapeTileInfo;
    class PagingLandScapeTileManager;
    typedef std::vector< PagingLandScapeTile* > PagingLandScapeTileRow;
    typedef std::vector< PagingLandScapeTileRow > PagingLandScapeTiles;

    class PagingLandScapeIndexBuffer;
    typedef std::map< uint, IndexData* > IndexMap;
    typedef std::vector< IndexData* > IndexArray;
    typedef std::vector< IndexMap* > LevelArray;

    class PagingLandScapeRenderable;
    class PagingLandScapeRenderableManager;
    typedef std::vector< PagingLandScapeRenderable* > PagingLandScapeRenderableVector;


    class PagingLandScapeData2D;
    class PagingLandScapeData2DManager;
    typedef std::vector< PagingLandScapeData2D* > PagingLandScapeData2DRow;
    typedef std::vector< PagingLandScapeData2DRow > PagingLandScapeData2DPages;

    typedef std::vector<PagingLandScapeData2D*> PagingLandScapeData2DMap;

    class PagingLandScapeTexture;
    class PagingLandScapeTextureManager;
    typedef std::vector< PagingLandScapeTexture* > PagingLandScapeTextureRow;
    typedef std::vector< PagingLandScapeTextureRow > PagingLandScapeTexturePages;

    typedef std::vector<PagingLandScapeTexture*> PagingLandScapeTextureMap;


    typedef std::vector< HardwareVertexBufferSharedPtr> HardwareTextureBuffersRow;
    typedef std::vector< HardwareTextureBuffersRow> HardwareTextureBuffersCol;

    class PagingLandscapeListenerManager;
    class PagingLandscapeListener;

    typedef std::map< String, String > LandscapeFileNames;

    #define MORPH_CUSTOM_PARAM_ID 77

    enum Neighbor
    {
        NORTH = 0,
        SOUTH,
        EAST,
        WEST
    };

    #define STITCH_NORTH_SHIFT 0
    #define STITCH_SOUTH_SHIFT 8
    #define STITCH_WEST_SHIFT  16
    #define STITCH_EAST_SHIFT  24

    #define STITCH_NORTH  128 << STITCH_NORTH_SHIFT
    #define STITCH_SOUTH  128 << STITCH_SOUTH_SHIFT
    #define STITCH_WEST   128 << STITCH_WEST_SHIFT
    #define STITCH_EAST   128 << STITCH_EAST_SHIFT

    
    typedef struct _RGBPixel               
    {
        uchar r;
        uchar g;
        uchar b;
    } RGBPixel;    

}
//-----------------------------------------------------------------------
// Windows Settings
//-----------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
#   ifdef PLUGIN_PAGINGLANDSCAPE2_EXPORTS
#       define _OgrePagingLandScapeExport __declspec(dllexport) 
#   else 
#       define _OgrePagingLandScapeExport __declspec(dllimport) 
#   endif 
#else 
#   define _OgrePagingLandScapeExport 
#endif 

//STL trick to call delete in a for_each
struct delete_object
{
    template <typename T>
    void operator()(T *ptr){ delete ptr; ptr = 0;}
};

#ifdef _PLSM2_RELEASE_ASSERT
#undef  assert
extern "C"
{
	_CRTIMP void __cdecl _assert( const char*, const char*, unsigned );
}
#define assert( exp ) ( void )( ( exp ) || ( _assert( #exp, __FILE__, __LINE__ ), 0 ) )
#endif //_PLSM2_RELEASE_ASSERT

#endif //__PagingLandScapePrerequisites_H__
