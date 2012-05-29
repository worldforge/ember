/***************************************************************************
OgrePagingLandScapePrecompiledHeaders.cpp  -  description
-------------------
copyright            : (C) 2006  Tuan Kuranes
email                : tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/
#ifndef _PRECOMP_HEADERS
#define _PRECOMP_HEADERS

#ifdef _PRECOMPILED_HEADERS

#include "OgrePagingLandScapePrerequisites.h"

// -------------------------------------------------------
// Octree Scene Manager (  occlusion management inside )
// -------------------------------------------------------

#include "OgrePagingLandScapeOctreeSceneManager.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeCamera.h"

#include "OgrePagingLandScapeOctreeRaySceneQuery.h"

//hardware occlusion management


#include "OgreOcclusionBoundingBox.h"
#include "OgrePagingLandScapeOcclusionQuerySet.h"
#include "OgrePagingLandScapeOcclusionSorter.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"

#include "OgrePagingLandScapeOcclusion.h"
#include "OgrePagingLandScapeOcclusionElement.h"


// -------------------------------------------------------
// Paging Scene Manager
// -------------------------------------------------------
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeRaySceneQuery.h"

// Horizon Culling 
#include "OgrePagingLandScapeHorizon.h"

// Queues


// pages
#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapePageRenderable.h"
#include "OgrePagingLandScapePageManager.h"


// Tile Management (page is constituted of tiles.)
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"
#include "OgrePagingLandScapeTileManager.h"

// IndexBuffer Caching to share it across tiles and LOD
#include "OgrePagingLandScapeIndexBuffer.h"

// Renderable that constitutes tiles
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeRenderableManager.h"


// Terrain Data Source management
#include "OgrePagingLandScapeData2D.h"

#include "OgrePagingLandScapeData2DManager.h"


// Terrain Texture Source management
#include "OgrePagingLandScapeTexture.h"

#include "OgrePagingLandScapeTextureManager.h"

// User Call back system using listener pattern
#include "OgrePagingLandScapeListenerManager.h"
#include "OgrePagingLandScapeListener.h"
#include "OgrePagingLandScapeCallbackEvent.h"

//decals 
#include "OgrePagingLandScapeMeshDecal.h"

//utils 
#include "OgrePagingLandscapePoolSet.h"
#include "OgrePagingLandscapeQueue.h"

#include "OgreDebugRectangle2D.h"
#include "fileutils.h"

#endif
#endif  //_PRECOMP_HEADERS
