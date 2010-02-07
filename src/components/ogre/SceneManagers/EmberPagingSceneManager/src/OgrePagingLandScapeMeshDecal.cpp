/*-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright © 2000-2006 The OGRE Team
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
-----------------------------------------------------------------------------*/
/***************************************************************************
OgrePagingLandScapeMeshDecal.cpp  -  description
-------------------
begin                : Sat Oct 15, 2006
copyright            : (C) 2006 by Steven Klug
email                : stevenklug@san.rr.com
 
 
***************************************************************************/
#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "Ogre.h"

#include "OgrePagingLandScapeMeshDecal.h"
#include "OgrePagingLandScapeAABSQ.h"

#include <numeric>

using namespace Ogre;
using namespace std;

namespace
{
	typedef PagingLandScapeAxisAlignedBoxSceneQuery::CustomQueryResult CustomQueryResult;

	template<class T>
	class IndexCalculator
	{
		int width_;
		int offset_;
	public:
		IndexCalculator( int width, int offset )
			: width_( width )
			, offset_( offset ) {}
		
		T GetIndex( int x, int z ) const
		{
			return offset_ + x + ( z * width_ );
		}
	};
}

PagingLandScapeMeshDecal::PagingLandScapeMeshDecal( const String& name,
													const String& materialName,
													const Vector2& size,
													const String& sceneMgrInstanceName )
	: MovableObject( name )
	, material_( MaterialManager::getSingleton().getByName( materialName ) )
	, size_( size )
	, sceneMgr_( Root::getSingleton().getSceneManager( sceneMgrInstanceName ) )
	, position_( Vector3::ZERO )
	, mDirty(true)
{
	if( material_.isNull() )
	{
		OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS,
					 "Invalid material name for mesh decal.",
					 "PagingLandScapeMeshDecal::PagingLandScapeMeshDecal");
	}

	// Create a dummy rayQuery to avoid heap allocation every frame.
	rayQuery_ = sceneMgr_->createRayQuery( Ray( Vector3::ZERO, Vector3::NEGATIVE_UNIT_Y ) );
	rayQuery_->setQueryTypeMask( SceneManager::WORLD_GEOMETRY_TYPE_MASK );
	rayQuery_->setWorldFragmentType( SceneQuery::WFT_SINGLE_INTERSECTION ); 

	AABBquery_ = sceneMgr_->createAABBQuery( AxisAlignedBox() );

	// Allocate objects for render op.
	renderOp_.vertexData = new VertexData();
	renderOp_.indexData = new IndexData();

	// Create declaration (memory format) of vertex data
	VertexDeclaration* decl = renderOp_.vertexData->vertexDeclaration;
	decl->addElement( 0, 0, VET_FLOAT3, VES_POSITION );
	declSize_ = sizeof( Vector3 );
	decl->addElement( 0, declSize_, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
	declSize_ += sizeof( Vector2 );

	// Set bounding information
	AABB_.setMinimum( Vector3( -size_.x / 2, 1, -size_.y / 2 ) );
	AABB_.setMaximum( Vector3( size_.x / 2, 1, size_.y / 2 ) );

// 	CreateGeometry();
}

PagingLandScapeMeshDecal::~PagingLandScapeMeshDecal()
{
	sceneMgr_->destroyQuery( rayQuery_ );
	sceneMgr_->destroyQuery( AABBquery_ );
	delete renderOp_.vertexData;
	delete renderOp_.indexData;
}

void PagingLandScapeMeshDecal::_notifyAttached(Node* parent, bool isTagPoint)
{
	MovableObject::_notifyAttached(parent, isTagPoint);
	CreateGeometry();
}


void PagingLandScapeMeshDecal::CreateGeometry()
{
	Matrix4 xlate;
	if( mParentNode )
		getWorldTransforms( &xlate );
	else
		xlate = Matrix4::IDENTITY;
	Matrix4 invXlate = xlate.inverse();

	// Deterimine approximate size of vertex data from query.
	// Get the scale.
	Real scaleX;
	Real scaleZ;
	sceneMgr_->getOption( "ScaleX", &scaleX );
	sceneMgr_->getOption( "ScaleZ", &scaleZ );

	const AxisAlignedBox& box = getWorldBoundingBox( true );
	subSections_.clear();
	subExtents_.clear();
	height_ = 0;
	width_ = 0;

	AABBquery_->setBox( box );
	AABBquery_->execute( this );

	if( !height_ || !width_ )
		return;

	//only continue if it's dirty
	if (!mDirty) {
		return;
	}
	//make a copy of the subextends for lookup
	mOldSubExtents = subExtents_;
	mDirty = false;

	Real xOffset = -size_.x / 2;
	Real zOffset = -size_.y / 2;

	// Create vertex data structure for 4 vertices shared between submeshes
	VertexData* vertexData = renderOp_.vertexData;

	// Allocate vertex buffer of the requested number of vertices (vertexCount) 
	// and bytes per vertex (offset)
	HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().
		createVertexBuffer( declSize_,
							GetNumVertices(),
							HardwareBuffer::HBU_STATIC_WRITE_ONLY );
//							HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE );

	// Set vertex buffer binding so buffer 0 is bound to our vertex buffer
	vertexData->vertexBufferBinding->setBinding( 0, vbuf );

	Real minY = Math::POS_INFINITY;
	Real maxY = Math::NEG_INFINITY;
	float* buffer = (float*)vbuf->lock( HardwareBuffer::HBL_DISCARD );
	int vertexCount = 0;

	SubSectionContainer::const_iterator it = subSections_.begin();
	SubSectionContainer::const_iterator itEnd = subSections_.end();
	for( ; it != itEnd; ++it )
	{
		VertexContainer::const_iterator vertIt = it->begin();
		VertexContainer::const_iterator vertItEnd = it->end();
		for( ; vertIt != vertItEnd; ++vertIt )
		{
			// Copy our vertices into the hardware buffer and calculate
			// texture coords.
			// TODO:  Don't transform vertices that aren't included due to LoD.
			Vector3 localVertex = invXlate * vertIt->vertex_;		// Convert back to local coordinates.
			*buffer++ = localVertex.x;
			*buffer++ = localVertex.y;
			*buffer++ = localVertex.z;
			// Keep track of height bounds for final AABB.
			if( localVertex.y < minY )
				minY = localVertex.y;
			if( localVertex.y > maxY )
				maxY = localVertex.y;
			// Calculate texture coordinates based on distances from the
			// edges of our rectangle.
			*buffer++ = ( localVertex.x - xOffset ) / size_.x;
			*buffer++ = 1.0 - ( ( localVertex.z - zOffset ) / size_.y );
			++vertexCount;
		}
	}
	vbuf->unlock();

	vertexData->vertexCount = vertexCount;

	// Create index data.
	IndexData* indexData = renderOp_.indexData;
	indexData->indexStart = 0;
	size_t maxNumIndices = GetNumIndices();
	bool is32bits = maxNumIndices >= 0x10000;
	// Allocate index buffer of the requested number of vertices (ibufCount) 
	HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().
		createIndexBuffer( (is32bits) ? HardwareIndexBuffer::IT_32BIT : HardwareIndexBuffer::IT_16BIT, 
						   maxNumIndices,
						   HardwareBuffer::HBU_STATIC_WRITE_ONLY );
	if( is32bits )
		indexData->indexCount = CreateIndices<uint>( ibuf );
	else
		indexData->indexCount = CreateIndices<ushort>( ibuf );

	// Put the hardware index buffer into our render operation object.
	indexData->indexBuffer = ibuf;
	indexData->optimiseVertexCacheTriList();

	subSections_.clear();
	subExtents_.clear();

	AABB_.setMinimum( Vector3( -size_.x / 2, minY, -size_.y / 2 ) );
	AABB_.setMaximum( Vector3( size_.x / 2, maxY, size_.y / 2) );
	radius_ = std::max( size_.x / 2, size_.y / 2);
	radius_ = std::max( radius_, maxY );
}

size_t PagingLandScapeMeshDecal::GetNumVertices() const
{
	size_t vertices = 0;
	SubSectionContainer::const_iterator it = subSections_.begin();
	for( ; it != subSections_.end(); ++it )
		vertices += it->size();
	return vertices;
}

size_t PagingLandScapeMeshDecal::GetNumIndices() const
{
	size_t indices = 0;
	SubExtentContainer::const_iterator it = subExtents_.begin();
	for( ; it != subExtents_.end(); ++it )
	{
		int step = 1 << it->renderLevel_;
		indices += ( it->width_ / step ) * ( it->height_ / step ) * 6;
	}
	return indices;
}

template<class T> 
int PagingLandScapeMeshDecal::CreateIndices( Ogre::HardwareIndexBufferSharedPtr ibuf )
{
	T* indices = (T*)ibuf->lock( HardwareBuffer::HBL_DISCARD );
	int indexCount = 0;
	int indexOffset = 0;
	for( int z = 0; z < height_; ++z )
	{
		for( int x = 0; x < width_; ++x )
		{
			indexCount += MeshSubSection<T>( x, z, indices, indexOffset );
		}
	}
	ibuf->unlock();
	return indexCount;
}

template<class T>
int PagingLandScapeMeshDecal::MeshSubSection( int x, int z, T*& indices, int& indexOffset )
{
	int indexCount = 0;
	int offset = indexOffset;
	const SubExtent& extents = subExtents_[z * width_ + x];
	int rl = extents.renderLevel_;
	// Get render levels of neighbors so we can stitch them properly.
	int northRL = z > 0 ? GetRenderLevel( x, z - 1 ) : rl;
	int westRL = x > 0 ? GetRenderLevel( x - 1, z ) : rl;
	int southRL = z < height_ - 1 ? GetRenderLevel( x, z + 1 ) : rl;
	int eastRL = x < width_ - 1 ? GetRenderLevel( x + 1, z ) : rl;
	// We only stitch if the neighbor's render level is greater than ours.
	bool northStitch = northRL > rl;
	bool westStitch = westRL > rl;
	bool southStitch = southRL > rl;
	bool eastStitch = eastRL > rl;
	// Get extents.
	int height = extents.height_;
	int width = extents.width_;
	int step = 1 << rl;
	int lineStepOffset = width * step;
	int iEnd = height - 1 - ( southStitch ? step : 0 );
	int i = 0;
	if( northStitch )
	{
		i = step;
		offset += lineStepOffset;
	}
	for( ; i < iEnd; i += step )
	{
		int nextLineOffset = offset + lineStepOffset;
		int jEnd = width - 1 - ( eastStitch ? step : 0 );
		for( int j = westStitch ? step : 0; j < jEnd; j += step )
		{
			*indices++ = j +		offset;				// up-left
			*indices++ = j +		nextLineOffset;		// low-left
			*indices++ = j + step + offset;				// up-right

			*indices++ = j + step + offset;				// up-right
			*indices++ = j +		nextLineOffset;		// low-left
			*indices++ = j + step + nextLineOffset;		// low-right
			
			indexCount += 6;
		}
		offset = nextLineOffset;
	}
	// Stich any sides that need stiching.
	if( northStitch )
		indexCount += StitchEdge( indices, extents, rl, northRL, true, true, westStitch, eastStitch, indexOffset );
	if( westStitch )
		indexCount += StitchEdge( indices, extents, rl, westRL, false, false, southStitch, northStitch, indexOffset );
	if( southStitch )
		indexCount += StitchEdge( indices, extents, rl, southRL, true, false, eastStitch, westStitch, indexOffset );
	if( eastStitch )
		indexCount += StitchEdge( indices, extents, rl, eastRL, false, true, northStitch, southStitch, indexOffset );

	indexOffset += ( width * height );
	return indexCount;
}

// Use the same stiching algorithm the landscape renderer does or
// the decal won't look right.  TODO:  At some point it would be nice
// to just use the actual code from the renderer, but it's made to
// work specifically with tiles, not sub-sections of tiles.
template<class T>
int PagingLandScapeMeshDecal::StitchEdge( T*& indices,
								  const SubExtent& extents,
								  const int hiLOD,
								  const int loLOD, 
								  const bool horizontal,
								  const bool forward,
								  const bool omitFirstTri,
								  const bool omitLastTri, 
								  const int indexOffset )
{
    // Work out the steps ie how to increment indexes
    // Step from one vertex to another in the high detail version
    int step = 1 << hiLOD;
    // Step from one vertex to another in the low detail version
    int superstep = 1 << loLOD;
    // Step half way between low detail steps
    int halfsuperstep = superstep >> 1;

    // Work out the starting points and sign of increments
    // We always work the strip clockwise
    int startx, startz, endx, rowstep;
	if( horizontal )
	{
		if( forward )
		{
            startx = startz = 0;
            endx =  extents.width_ - 1;
            rowstep = step;
		}
		else
		{
            startx = extents.width_ - 1;
			startz = extents.height_ - 1;
            endx = 0;
            rowstep = -step;
            step = -step;
            superstep = -superstep;
            halfsuperstep = -halfsuperstep;
		}
	}
	else
	{
		if( forward )
		{
            startx = 0;
            endx = extents.height_ - 1;
            startz = extents.width_ - 1;
            rowstep = -step;
		}
		else
		{
            startx = extents.height_ - 1;
            endx = 0;
            startz = 0;
            rowstep = step;
            step = -step;
            superstep = -superstep;
            halfsuperstep = -halfsuperstep;
		}
	}
 
	int indexCount = 0;
	IndexCalculator<T> calc( extents.width_, indexOffset );
    for( int j = startx; j != endx; j += superstep )
    {
		int k; 
        for( k = 0; k != halfsuperstep; k += step )
        {
            int jk = j + k;
            //skip the first bit of the corner?
            if( j != startx || k != 0 || !omitFirstTri )
            {
                if( horizontal )
                {
                    *indices++ = calc.GetIndex( j , startz );
                    *indices++ = calc.GetIndex( jk, startz + rowstep );
                    *indices++ = calc.GetIndex( jk + step, startz + rowstep );
                }
                else
                {
                    *indices++ = calc.GetIndex( startz, j );
                    *indices++ = calc.GetIndex( startz + rowstep, jk );
                    *indices++ = calc.GetIndex( startz + rowstep, jk + step );
                }
				indexCount += 3;
            }
        }

        // Middle tri
        if( horizontal )
        {
            *indices++ = calc.GetIndex( j, startz );
            *indices++ = calc.GetIndex( j + halfsuperstep, startz + rowstep );
            *indices++ = calc.GetIndex( j + superstep, startz );
        }
        else
        {
            *indices++ = calc.GetIndex( startz, j );
            *indices++ = calc.GetIndex( startz + rowstep, j + halfsuperstep );
            *indices++ = calc.GetIndex( startz, j + superstep );
        }
		indexCount += 3;

        for( k = halfsuperstep; k != superstep; k += step )
        {
            int jk = j + k;
            if( j != endx - superstep || k != superstep - step || !omitLastTri )
            {
                if( horizontal )
                {
                    *indices++ = calc.GetIndex( j + superstep, startz );
                    *indices++ = calc.GetIndex( jk, startz + rowstep );
                    *indices++ = calc.GetIndex( jk + step, startz + rowstep );
                }
                else
                {
                    *indices++ = calc.GetIndex( startz, j + superstep );
                    *indices++ = calc.GetIndex( startz + rowstep, jk );
                    *indices++ = calc.GetIndex( startz + rowstep, jk + step );
                }
				indexCount += 3;
            }
        }
    }
	return indexCount;
}

// From Ogre::MovableObject
void PagingLandScapeMeshDecal::_updateRenderQueue( RenderQueue* queue )
{
	queue->addRenderable( this, mRenderQueueID, OGRE_RENDERABLE_DEFAULT_PRIORITY + 1 );
}

// From Ogre::Renderable
void PagingLandScapeMeshDecal::getRenderOperation( Ogre::RenderOperation& op )
{
	// Update our RenderOperation if necessary.
	const Vector3& position = mParentNode->_getDerivedPosition();
	const Quaternion& orientation = mParentNode->getOrientation();
	if( position != position_ ||
		orientation != orientation_ )
	{
		position_ = position;
		orientation_ = orientation;
		//force a full geometry rebuild
		mDirty = true;
	}
	//calling this every frame will only trigger a full rebuild when the LOD has changed
	CreateGeometry();
	op = renderOp_;
}

void PagingLandScapeMeshDecal::getWorldTransforms( Ogre::Matrix4* xform ) const
{
	*xform = mParentNode->_getFullTransform();
}

Real PagingLandScapeMeshDecal::getSquaredViewDepth( const Camera* cam ) const
{
    return ( mParentNode->_getDerivedPosition() - cam->getDerivedPosition() ).squaredLength();
}
 
// Receive terrain vertices from our query.
bool PagingLandScapeMeshDecal::queryResult(SceneQuery::WorldFragment* fragment)
{
	if( fragment->fragmentType != SceneQuery::WFT_CUSTOM_GEOMETRY )
	{
		OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR,
					 "SceneQuery flags invalid.",
					 "PagingLandScapeMeshDecal::queryResult");
	}

	CustomQueryResult* result = reinterpret_cast<CustomQueryResult*>( fragment->geometry );
	switch( result->type_ )
	{
	case CustomQueryResult::QUERY_EXTENTS_TYPE:
		{
			// Reserve space for our sub-sections.
			width_ = result->data_.queryExtents_.width_;
			height_ = result->data_.queryExtents_.height_;
			subSections_.resize( width_ * height_ );
			subExtents_.resize( width_ * height_ );
			break;
		}

	case CustomQueryResult::SUBSECTION_EXTENTS_TYPE:
		{
			currentSubX_ = result->data_.subExtents_.subX_;
			currentSubZ_ = result->data_.subExtents_.subZ_;
			int offset = currentSubX_ + currentSubZ_ * width_;
			SubExtent& subExtent = subExtents_[offset];
			subExtent.width_ = result->data_.subExtents_.width_;
			subExtent.height_ = result->data_.subExtents_.height_;
			subExtent.renderLevel_ = result->data_.subExtents_.renderLevel_;
			// TODO:  Optimize!  We wouldn't need an intermediate array at all
			// if the scene query gave us all the sub-section extents up front
			// so we could allocate the correct size fo the vertex buffer.  It's
			// a bit tricky in the scene query, so leaving it this way for now.
			// Another option could be to overallocate the vertex buffer assuming
			// that each sub-section was the full tile size, but that's also
			// pretty wasteful (though possibly faster, since no heap allocation).
			subSections_[offset].reserve( subExtent.width_ * subExtent.height_ );
			
			//compare with earlier result and see if any LOD has changed
			if (!mDirty) {
				if (mOldSubExtents.size() >= offset) {
					SubExtent& oldSubExtent = mOldSubExtents[offset];
					if (oldSubExtent.renderLevel_ != subExtent.renderLevel_) {
						mDirty = true;
					}
				} else {
					mDirty = true;
				}
			}
			break;
		}
		
	case CustomQueryResult::VERTEX_TYPE:
		{
			VertexContainer& subSection = subSections_[currentSubX_ + currentSubZ_ * width_];
			Vertex vertex( result->data_.vertexData_.x_,
						   result->data_.vertexData_.y_,
						   result->data_.vertexData_.z_,
						   result->data_.vertexData_.included_ );
			subSection.push_back( vertex );
			break;
		}

	default:
		OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR,
					 "Result type invalid.",
					 "PagingLandScapeMeshDecal::queryResult");
	}
	return true;
}


void PagingLandScapeMeshDecal::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
{
	visitor->visit(this, 0, debugRenderables);
}
//-----------------------------------------------------------------------
String PagingLandScapeMeshDecalFactory::FACTORY_TYPE_NAME = "PagingLandScapeMeshDecal";

#if 0
//-----------------------------------------------------------------------
PagingLandScapeMeshDecal* PagingLandScapeMeshDecalFactory::create( SceneManager& sceneMgr, 
																   const String& name,
																   const String& materialName,
																   const Vector2& size,
																   const String& sceneMgrInstance )
{
	NameValuePairList params;
	params["materialName"] = materialName;
	params["width"] = StringConverter::toString( size.x );
	params["height"] = StringConverter::toString( size.y );
	params["sceneMgrInstance"] = sceneMgrInstance;

	return static_cast<PagingLandScapeMeshDecal*>( 
		sceneMgr.createMovableObject( name, 
									  PagingLandScapeMeshDecalFactory::FACTORY_TYPE_NAME,
									  &params ) );
}
#endif
//-----------------------------------------------------------------------
const String& PagingLandScapeMeshDecalFactory::getType(void) const
{
	return FACTORY_TYPE_NAME;
}
//-----------------------------------------------------------------------
MovableObject* PagingLandScapeMeshDecalFactory::createInstanceImpl
(
	const String& name,
	const NameValuePairList* params
)
{
	String materialName;
	Real width = 0.0;
	Real height = 0.0;
	String sceneMgrInstance;
	if( params != 0 )
	{
		NameValuePairList::const_iterator it = params->find( "materialName" );
		if( it != params->end() )
		{
			materialName = it->second;
		}
		it = params->find( "width" );
		if( it != params->end() )
		{
			width = StringConverter::parseReal( it->second );
		}
		it = params->find( "height" );
		if( it != params->end() )
		{
			height = StringConverter::parseReal( it->second );
		}
		it = params->find( "sceneMgrInstance" );
		if( it != params->end() )
		{
			sceneMgrInstance = it->second;
		}
	}

	return new PagingLandScapeMeshDecal( name, materialName, Vector2( width, height ), sceneMgrInstance );
}
//-----------------------------------------------------------------------
void PagingLandScapeMeshDecalFactory::destroyInstance( MovableObject* obj )
{
	delete obj;
}

