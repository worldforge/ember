/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
 
Copyright  2000-2006 The OGRE Team
Also see acknowledgements in Readme.html
 
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
 
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
/***************************************************************************
OgrePagingLandScapeAxisAlignedBoxSceneQuery.cpp  -  description
-------------------
begin                : Sat Oct 15, 2006
copyright            : (C) 2006 by Steven Klug
email                : stevenklug@san.rr.com
 
 
***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeAxisAlignedBoxSceneQuery.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgreEntity.h"

#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapePageManager.h"

using namespace Ogre;

namespace Ogre
{
	// NOTE:  Creating some of my own conversion functions here even though
	// these are somewhat duplicating code in the page manager.  It was
	// more for the exercise of getting clear in my head the differences
	// between world coords, page local coords, page numbers, tiles, etc.
	// May convert back to the ones in the page manager eventually, though
	// these may offer some speed advantages as some redundant calculations
	// (invPageSize, etc.) are only done once in the main query.

    //-----------------------------------------------------------------------
	// Scale a coordinate from world space to page number and coord for local
	// unscaled page space.
	inline
	bool WorldToPage( Real coord, Real scale, Real maxUnScaled, uint pageSize, Real invPageSize, uint worldExtent,
					  uint& pageNumber, Real& localPageCoord  )
	{
        // scale position from world to page scale
		Real unscaled = (coord / scale) + maxUnScaled;
        
        // get Page 
		Real value = unscaled * invPageSize;
		uint maxPage = static_cast<int>( worldExtent );
		// Set output params.
		pageNumber = static_cast<int>( value );
		if( value < 0.0 )
		{
			pageNumber = 0;
			localPageCoord = 0.0;
			return false;
		}
		else if( pageNumber >= maxPage )
		{
			pageNumber = maxPage - 1;
			localPageCoord = pageNumber * pageSize + pageSize - 1;
			return false;
		}
		else
		{
			localPageCoord = unscaled - ( pageNumber * pageSize );
			return true;
		}
	}

    //-----------------------------------------------------------------------
	inline
	Real PageToWorld( uint pageNumber, Real localPageCoord, Real scale, Real maxUnScaled, uint pageSize )
	{
		Real unscaled = localPageCoord + pageNumber * pageSize;
		return ( unscaled - maxUnScaled ) * scale;
	}

    //-----------------------------------------------------------------------
	// Scale a coordinate from local page space to tile number.
	inline
	uint PageToTile( int localPageCoord, Real invTileSize, uint maxNumTiles )
	{
        // get Tile
		uint value = static_cast< uint > (localPageCoord * invTileSize);
        return ( value < 0 ) ? 0 : (value >=  maxNumTiles ? maxNumTiles - 1: value); 
	}

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
	// Operations and data for render levels (lod levels).
	class RenderLevel
	{
		int renderLevel_;
		int lodStep_;
		uint lodMask_;
    public:
        //-----------------------------------------------------------------------
		RenderLevel()
		{
			SetRenderLevel( 0 );
        }
        //-----------------------------------------------------------------------
		RenderLevel( int renderLevel )
		{
			SetRenderLevel( renderLevel );
        }
        //-----------------------------------------------------------------------
		void SetRenderLevel( int renderLevel )
		{
			renderLevel_ = renderLevel;
			lodStep_ = 1 << renderLevel;
			lodMask_ = ~( lodStep_ - 1 );
        }
        //-----------------------------------------------------------------------
		int GetRenderLevel() const
		{
			return renderLevel_;
        }
        //-----------------------------------------------------------------------
		int GetLodStep() const
		{
			return lodStep_;
        }
        //-----------------------------------------------------------------------
		uint GetLodMask() const
		{
			return lodMask_;
        }
        //-----------------------------------------------------------------------
		bool operator < ( const RenderLevel& rhs ) const
		{
			return renderLevel_ < rhs.renderLevel_;
        }
        //-----------------------------------------------------------------------
		// Step the input coordinate by the lodStep.
		int LodStep( int localPageCoord ) const
		{
			return localPageCoord + lodStep_;
        }
        //-----------------------------------------------------------------------
		// Return the floor of the passed in coord based on valid LOD.
		int Floor( int localPageCoord ) const
		{
			return localPageCoord & lodMask_;
        }
        //-----------------------------------------------------------------------
		// Return the ceiling of the passed in coord based on valid LOD.
		int Ceil( int localPageCoord ) const
		{
			if( Floor( localPageCoord ) == localPageCoord )
				return localPageCoord;	// Already at ceiling.
			else
				return Floor( localPageCoord + lodStep_ );
        }
        //-----------------------------------------------------------------------
		int GetLodLevel() const
		{
			return lodStep_;
		}
    };

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
	// Object responsible for caching tile changes specific to lod stepping.
	class LodTracker
	{
		const Vector3 scale_;
		const Real maxUnScaledX_;
		const Real maxUnScaledZ_;
		const uint pageSize_;
		const Real invPageSize_;
		const uint tileSize_;
		const Real invTileSize_;
		const uint maxNumTiles_;
		PagingLandScapePageManager* pageMgr_;
		PagingLandScapeData2DManager* dataMgr_;

		const PagingLandScapePage* page_;
		const PagingLandScapeData2D* pageData_;
		PagingLandScapeTile* tile_;
		RenderLevel renderLevel_;
		uint pageX_;
		uint pageZ_;
    public:
        //-----------------------------------------------------------------------
		LodTracker( Vector3 scale,
					Real maxUnScaledX,
					Real maxUnScaledZ,
					uint pageSize,
					uint tileSize, 
					uint maxNumTiles,
					PagingLandScapePageManager* pageMgr,
					PagingLandScapeData2DManager* dataMgr ) 
			: scale_( scale )
			, maxUnScaledX_( maxUnScaledX )
			, maxUnScaledZ_( maxUnScaledZ )
			, pageSize_( pageSize )
			, invPageSize_( 1.0f / pageSize )
			, tileSize_( tileSize )
			, invTileSize_( 1.0f / tileSize )
			, maxNumTiles_( maxNumTiles )
			, pageMgr_( pageMgr ) 
			, dataMgr_( dataMgr )
			, page_( 0 )
			, pageData_( 0 )
			, tile_( 0 )
			, pageX_( 0 )
			, pageZ_( 0 ) {}

        //-----------------------------------------------------------------------
		uint GetTileSize() const
		{
			return tileSize_;
        }
        //-----------------------------------------------------------------------
		Real GetInvTileSize() const
		{
			return invTileSize_;
        }
        //-----------------------------------------------------------------------
		uint GetMaxNumTiles() const
		{
			return maxNumTiles_;
        }
        //-----------------------------------------------------------------------
		const RenderLevel& GetRenderLevel() const
		{
			return renderLevel_;
        }
        //-----------------------------------------------------------------------
		const PagingLandScapePage* GetPage() const
		{
			return page_;
        }
        //-----------------------------------------------------------------------
		uint GetPageX() const
		{
			return pageX_;
        }
        //-----------------------------------------------------------------------
		uint GetPageZ() const
		{
			return pageZ_;
        }
        //-----------------------------------------------------------------------
		// Returns true if the vertex is included, or false if it is removed
		// by LOD. Requires local page coordinates.
		bool GetWorldVertex( int localPageX, int localPageZ, Vector3& vertex )
		{
			UpdateWithLocalPage( localPageX, localPageZ );
			bool included = ( renderLevel_.Floor( localPageX ) == localPageX &&
							  renderLevel_.Floor( localPageZ ) == localPageZ );
			if( page_ && page_->isLoaded() && pageData_ )
			{
				// TODO:  Do we really need to include the real data when the vertex
				// has been removed by lod? Or can we just stuff a dummy result in
				// here to fill out the array?
				vertex.y = pageData_->getHeight( localPageX, localPageZ );
				page_->getCoordinates( pageX_, pageZ_ );	
				vertex.x = PageToWorld( pageX_, localPageX, scale_.x, maxUnScaledX_, pageSize_ );
				vertex.z = PageToWorld( pageZ_, localPageZ, scale_.z, maxUnScaledZ_, pageSize_ );
			}
			return included;
        }
        //-----------------------------------------------------------------------
		// Set the current page.  Must be called before Update().
		void SetPage( const PagingLandScapePage* page )
		{
			page_ = page;
			if( page_ && page_->isLoaded() )
			{
				page_->getCoordinates( pageX_, pageZ_ );
				pageData_ = dataMgr_->getData2D( pageX_, pageZ_, false );
			}
        }
        //-----------------------------------------------------------------------
		// Sets the page using the page coordinates.
		void SetPage( int pageX, int pageZ )
		{
			const PagingLandScapePage* page = pageMgr_->getPage( pageX, pageZ, false );
			if( !page || !page->isLoaded() )
			{
				assert( false );
				return; // TODO:  Currently the AABB must be on the currently loaded landscape.
			}
			SetPage( page );
        }
        //-----------------------------------------------------------------------
		// Update using page local tile coordinates.
		void UpdateTile( int tileX, int tileZ )
		{
			PagingLandScapeTile* tile = page_->getTile( tileX, tileZ );
			if( tile != tile_ )
			{
				tile_ = tile;
				bool tileLoaded = tile_ && tile_->isLoaded() && tile_->getRenderable();
				renderLevel_.SetRenderLevel( (tileLoaded) ? tile_->getRenderable()->getRenderLevel() : 0 );
			}
        }
        //-----------------------------------------------------------------------
		// Update our lodStep and lodMask for the current tile, given the page
		// local coordinates of a vertex.
		void UpdateWithLocalPage( int localPageX, int localPageZ )
		{
			int tileX =	PageToTile( localPageX, invTileSize_, maxNumTiles_ );
			int tileZ = PageToTile( localPageZ, invTileSize_, maxNumTiles_ );
			
			UpdateTile( tileX, tileZ );
		}
	};

    //-----------------------------------------------------------------------
	// Loop over every edge tile.  Determine which the lowest lod on each edge
	// and use that to adjust the extents.
	// TODO:  This routine is expensive, and is only needed when LOD is involved.
	// Optimize?  Ironic since LOD is supposed to make things faster.
	void AdjustEdgesForLod( LodTracker& lodTracker,
							int minPageX, int minPageZ, int maxPageX, int maxPageZ,
							int& localPageMinX, int& localPageMinZ, int& localPageMaxX, int& localPageMaxZ )
	{
		int minTileZ = PageToTile( localPageMinZ, 
								   lodTracker.GetInvTileSize(),
								   lodTracker.GetMaxNumTiles() );
		int maxTileZ = PageToTile( localPageMaxZ, 
								   lodTracker.GetInvTileSize(),
								   lodTracker.GetMaxNumTiles() );
		int minTileX = PageToTile( localPageMinX,
								   lodTracker.GetInvTileSize(),
								   lodTracker.GetMaxNumTiles() );
		int maxTileX = PageToTile( localPageMaxX, 
								   lodTracker.GetInvTileSize(),
								   lodTracker.GetMaxNumTiles() );
		int westRenderLevel = 0;
		int eastRenderLevel = 0;
		for( int pageZ = minPageZ; pageZ <= maxPageZ; ++pageZ )
		{
			int endTileZ = ( pageZ == maxPageZ ) ? maxTileZ : lodTracker.GetMaxNumTiles() - 1;
			for( int tileZ = ( pageZ == minPageZ ) ? minTileZ : 0;
				 tileZ <= endTileZ;
				 ++tileZ )
			{
				// West edge
				lodTracker.SetPage( minPageX, pageZ );
				lodTracker.UpdateTile( minTileX, tileZ );
				int renderLevel = lodTracker.GetRenderLevel().GetRenderLevel();
				if( renderLevel > westRenderLevel )
					westRenderLevel = renderLevel;
				// East edge
				lodTracker.SetPage( maxPageX, pageZ );
				lodTracker.UpdateTile( maxTileX, tileZ );
				renderLevel = lodTracker.GetRenderLevel().GetRenderLevel();
				if( renderLevel > eastRenderLevel )
					eastRenderLevel = renderLevel;
			}
		}

		int northRenderLevel = 0;
		int southRenderLevel = 0;
		for( int pageX = minPageX; pageX <= maxPageX; ++pageX )
		{
			int endTileX = ( pageX == maxPageX ) ? maxTileX : lodTracker.GetMaxNumTiles() - 1;
			for( int tileX = ( pageX == minPageX ) ? minTileX : 0;
				 tileX <= endTileX;
				 ++tileX )
			{
				// North edge
				lodTracker.SetPage( pageX, minPageZ );
				lodTracker.UpdateTile( tileX, minTileZ );
				int renderLevel = lodTracker.GetRenderLevel().GetRenderLevel();
				if( renderLevel > northRenderLevel )
					northRenderLevel = renderLevel;
				// South edge
				lodTracker.SetPage( pageX, maxPageZ );
				lodTracker.UpdateTile( tileX, maxTileZ );
				renderLevel = lodTracker.GetRenderLevel().GetRenderLevel();
				if( renderLevel > southRenderLevel )
					southRenderLevel = renderLevel;
			}
		}
		// Use the lods we found to adjust the min and max local coords.
		RenderLevel renderLevel( westRenderLevel );
		localPageMinX = renderLevel.Floor( localPageMinX );
		renderLevel.SetRenderLevel( eastRenderLevel );
		localPageMaxX = renderLevel.Ceil( localPageMaxX );
		renderLevel.SetRenderLevel( northRenderLevel );
		localPageMinZ = renderLevel.Floor( localPageMinZ );
		renderLevel.SetRenderLevel( southRenderLevel );
		localPageMaxZ = renderLevel.Ceil( localPageMaxZ );
	}

    //-----------------------------------------------------------------------
	void DoTileSubSection( LodTracker& lodTracker,
						   SceneQuery::WorldFragment& worldFragment, 
						   PagingLandScapeAxisAlignedBoxSceneQuery::CustomQueryResult& queryResult, 
						   SceneQueryListener* listener,
						   int localPageMinX, int localPageMinZ, int localPageMaxX, int localPageMaxZ,
						   uint subX, uint subZ )
	{
		lodTracker.UpdateWithLocalPage( localPageMinX, localPageMinZ );

		// Send the width and height of this sub-section.
		queryResult.SetSubsectionExtents( localPageMaxX - localPageMinX + 1, 
										  localPageMaxZ - localPageMinZ + 1,
										  lodTracker.GetRenderLevel().GetRenderLevel(),
										  subX, subZ );
		listener->queryResult( &worldFragment );

		for( int z = localPageMinZ; z <= localPageMaxZ; ++z )
		{
			for( int x = localPageMinX; x <= localPageMaxX; ++x )
			{
				Vector3 vertex;
				bool included = lodTracker.GetWorldVertex( x, z, vertex );
				queryResult.SetVertex( vertex, included );
				listener->queryResult( &worldFragment );
			}
		}
	}
}

namespace Ogre
{

//---------------------------------------------------------------------
PagingLandScapeAxisAlignedBoxSceneQuery::PagingLandScapeAxisAlignedBoxSceneQuery(SceneManager* creator)
	: DefaultAxisAlignedBoxSceneQuery(creator)
{
}

//---------------------------------------------------------------------
PagingLandScapeAxisAlignedBoxSceneQuery::~PagingLandScapeAxisAlignedBoxSceneQuery(void)
{
}

//---------------------------------------------------------------------
// Well return via callbacks a set of rectangular tile sub-sections.
// All fragments returned via callback use a WFT_CUSTOM_GEOMETRY type.
// The CustomQueryResult structure defined above is what is passed
// in the void* geometry field.  The type_ field of the CustomQuery
// result determins what is being sent in the fragment as follows:
//
// QUERY_EXTENTS_TYPE:  Sent as the first result, this gives the width
// and height (in tile subsections) of the entire query result.  The
// render level in this case is not defined.
//
// SUBSECTION_EXTENTS_TYPE:  Sent before each sub-section's vertex
// data.  This contains the height, width, and render level of the
// subsequent vertex data.
//
// VERTEX_TYPE:  This is the actual vertex data, and whether the vertex
// is included when displayed at the tile's current render level.
//
// The seemingly complex way of returning the data is so that the same
// stitching algorithm used by PLSM can be used by the client in
// creating a mesh out of the data.
//---------------------------------------------------------------------
void PagingLandScapeAxisAlignedBoxSceneQuery::execute(SceneQueryListener* listener)
{
	if( !( getQueryTypeMask() & SceneManager::WORLD_GEOMETRY_TYPE_MASK ) )
	{
        OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, 
					 "PagingLandScapeSceneManager only supports WORLD_GEOMETRY_TYPE_MASK for AxisAlignedBoxSceneQueries", 
					 "PagingLandScapeAxisAlignedBoxSceneQuery::execute");
	}
	PagingLandScapeSceneManager* mgr = static_cast< PagingLandScapeSceneManager* >(mParentSceneMgr);

	// Currently we ignore the y coords of the bounding box and assume they
	// want all the vertices in the xz square.
	
	// Determine page that each corner of AABB is on.
	Vector3 min = mAABB.getMinimum();
	Vector3 max = mAABB.getMaximum();

	// Pre-gather some constants.
	const PagingLandScapeOptions* options = mgr->getOptions();
	const Vector3& scale = options->scale;
	const Real maxUnScaledX = options->maxUnScaledX;
	const Real maxUnScaledZ = options->maxUnScaledZ;
	const uint pageSize = options->PageSize - 1;
	const Real invPageSize = 1.0f / pageSize;
	const uint tileSize = options->TileSize - 1;
    const Real invTileSize = 1.0f / tileSize;
	const uint maxNumTiles = options->NumTiles;
	const uint worldWidth = options->world_width;
	const uint worldHeight = options->world_height;
	// Note that for efficiency here, I'm not allocating a new WorldFragment 
	// every time, so clients will have to use the subscribtion mechanism to
	// get the results, as the execute() that returns the SceneQueryResult
	// will contain a collection of only the last result.  TODO:  The ogre
	// RegionSceneQuery could be modified to only do heap allocation when
	// the subscription mechanism isn't being used by the client.
	WorldFragment worldFragment;
	worldFragment.fragmentType = SceneQuery::WFT_CUSTOM_GEOMETRY;
	CustomQueryResult result;
	worldFragment.geometry = &result;

	// Calculate page min/max.
	uint minPageX;
	Real localPageMinXReal;
	bool minXInWorld;
	uint minPageZ;
	Real localPageMinZReal;
	bool minZInWorld;
	uint maxPageX;
	Real localPageMaxXReal;
	bool maxXInWorld;
	uint maxPageZ;
	Real localPageMaxZReal;
	bool maxZInWorld;

	minXInWorld = WorldToPage( min.x, scale.x, maxUnScaledX, pageSize, invPageSize, worldWidth, minPageX, localPageMinXReal );
	minZInWorld = WorldToPage( min.z, scale.z, maxUnScaledZ, pageSize, invPageSize, worldHeight, minPageZ, localPageMinZReal );
	maxXInWorld = WorldToPage( max.x, scale.x, maxUnScaledX, pageSize, invPageSize, worldWidth, maxPageX, localPageMaxXReal );
	maxZInWorld = WorldToPage( max.z, scale.z, maxUnScaledZ, pageSize, invPageSize, worldHeight, maxPageZ, localPageMaxZReal );

	// At least one corner of the bounding box must be inside the world, or
	// we return no results.
	if( !( minXInWorld && minZInWorld ) || !( maxXInWorld && maxZInWorld ) ||
		!( minXInWorld && maxZInWorld ) || !( maxXInWorld && minZInWorld ) )
		return;

	// Determine the width and height of the query in terms of tile sub-sections.
	// We need to make sure we return enough vertices so the entire bounding box
	// fits within.  That means for lodded tiles we may need to move to the 
	// next valid vertex, however, satisfying this requirement should never change
	// the query extents because every tile has valid vertices at its edges.
	int localPageMinX = static_cast<int>( localPageMinXReal );
	int localPageMinZ = static_cast<int>( localPageMinZReal );
	int localPageMaxX = static_cast<int>( Math::Ceil( localPageMaxXReal ) );
	int localPageMaxZ = static_cast<int>( Math::Ceil( localPageMaxZReal ) );

	LodTracker lodTracker( scale, 
						   maxUnScaledX,
						   maxUnScaledZ,
						   pageSize,
						   tileSize,
						   maxNumTiles,
						   mgr->getPageManager(),
						   mgr->getData2DManager() );

	// TODO:  Expensive routine.  Can we do away with it or optimze it somehow?
	AdjustEdgesForLod( lodTracker, 
					   minPageX, minPageZ, maxPageX, maxPageZ,
					   localPageMinX, localPageMinZ, localPageMaxX, localPageMaxZ );

	// Pre-calculate the width and height of the whole query in terms of how many
	// tile sub-sections we'll have.
	int middleTiles = ( static_cast<int>( maxPageX ) - static_cast<int>( minPageX ) - 1 ) * 
					  ( pageSize / tileSize );							// Get the contribution from the middle pages.
	int leftTiles = ( maxNumTiles - PageToTile( localPageMinX, 
												invTileSize, 
												maxNumTiles ) );		// Get contribution from the left side.
	int rightTiles = PageToTile( localPageMaxX - 1, 
								 invTileSize, 
								 maxNumTiles ) + 1;						// Get contribution from the right side.
	int calcWidth = leftTiles + middleTiles + rightTiles;

	middleTiles = ( static_cast<int>( maxPageZ ) - static_cast<int>( minPageZ ) - 1 ) * 
				  ( pageSize / tileSize );								// Get the contribution from the middle pages.
	leftTiles = ( maxNumTiles - PageToTile( localPageMinZ, 
											invTileSize, 
											maxNumTiles ) );			// Get contribution from the left side.
	rightTiles = PageToTile( localPageMaxZ - 1, 
							 invTileSize,
							 maxNumTiles ) + 1;							// Get contribution from the right side.
	int calcHeight = leftTiles + middleTiles + rightTiles;

	//Real calcWidth = ( maxPageX == minPageX ) ? localPageMaxX - localPageMinX + 1 :
	//										    ( pageSize - localPageMinX ) + localPageMaxX + 1 +
	//										    ( maxPageX - minPageX - 1 ) * pageSize;
	//Real calcHeight = ( maxPageZ == minPageZ ) ? localPageMaxZ - localPageMinZ + 1 :
	//										     ( pageSize - localPageMinZ ) + localPageMaxZ + 1 +
	//											 ( maxPageZ - minPageZ - 1 ) * pageSize;
	// Notify our caller about the width and height, using custom geometry type.
	result.SetQueryExtents( calcWidth, calcHeight );
	listener->queryResult( &worldFragment );

	uint subZ = 0;
	for( uint pageZ = minPageZ; pageZ <= maxPageZ; ++pageZ )
	{
		// Determine Z tile range on this page.
		uint tileBeginZ = ( pageZ == minPageZ ) ?
									PageToTile( localPageMinZ, invTileSize, maxNumTiles ) : 0;
		uint tileEndZ = ( pageZ == maxPageZ ) ? 
									PageToTile( localPageMaxZ - 1, invTileSize, maxNumTiles ) : maxNumTiles - 1;
		for( uint tileZ = tileBeginZ; tileZ <= tileEndZ; ++tileZ, ++subZ )
		{
			uint subX = 0;
			for( uint pageX = minPageX; pageX <= maxPageX; ++pageX )
			{
				lodTracker.SetPage( pageX, pageZ );
			
				// Determine X tile range on this page.
				uint tileBeginX = ( pageX == minPageX ) ? PageToTile( localPageMinX, invTileSize, maxNumTiles ) : 0;
				uint tileEndX = ( pageX == maxPageX ) ? PageToTile( localPageMaxX - 1, invTileSize, maxNumTiles ) : maxNumTiles - 1;

				for( uint tileX = tileBeginX; tileX <= tileEndX; ++tileX, ++subX )
				{
					// Determine the Z local page coord range for this tile.
					uint localPageBeginZ = ( pageZ == minPageZ ) && ( tileZ == tileBeginZ ) ? 
													localPageMinZ : tileZ * tileSize;
					uint localPageEndZ = ( pageZ == maxPageZ ) && ( tileZ == tileEndZ ) ? 
													localPageMaxZ : ( tileZ + 1 ) * tileSize;

					// Determine the X local page coord range for this tile.
					uint localPageBeginX = ( pageX == minPageX ) && ( tileX == tileBeginX ) ? 
													localPageMinX : tileX * tileSize;
					uint localPageEndX = ( pageX == maxPageX ) && ( tileX == tileEndX ) ? 
													localPageMaxX : ( tileX + 1 ) * tileSize;

					DoTileSubSection( lodTracker, worldFragment, result, listener,
									  localPageBeginX, localPageBeginZ, localPageEndX, localPageEndZ,
									  subX, subZ );
				}
			}
		}
	}
}

}
