/*
    Copyright (C) 2004  Erik Hjortsberg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include "TerrainGenerator.h"
#include "MathConverter.h"

#include "DimeTerrainRenderable.h"

using namespace Ogre;
namespace DimeOgre {

#define POSITION_BINDING 0
#define NORMAL_BINDING 1
#define TEXCOORD_BINDING 2
#define COLOUR_BINDING 3

DimeTerrainRenderable::DimeTerrainRenderable() : TerrainRenderable::TerrainRenderable()
{}
DimeTerrainRenderable::~DimeTerrainRenderable() {}

void DimeTerrainRenderable::init( TerrainOptions &options, TerrainGenerator* generator )
{
    //ensure that the size works
    if ( ! _checkSize( options.size ) )
    {
        printf( "Terrain block must size: 2^n+1 ( it's %d )\n", options.size );
        return ;
    }

    if ( options.max_mipmap != 0 )
    {
        int i = ( int ) 1 << ( options.max_mipmap - 1 ) ;

        if ( ( i + 1 ) > options.size )
        {
            printf( "Invalid maximum mipmap specifed, must be n, such that 2^(n-1)+1 < options.size \n" );
            return ;
        }
    }

    deleteGeometry();

    //calculate min and max heights;
    Real min = 256000, max = 0;

    /* Initialize the variables */
    mLit = options.lit;

    mColored = options.colored;

    mNearPlane = options.near_plane;

    mMaxPixelError = options.max_pixel_error;

    mVertResolution = options.vert_res;

    mTopCoord = options.top_coord;

    mSize = options.size;

    mWorldSize = options.world_size;

    mNumMipMaps = options.max_mipmap;

    mTerrain = new VertexData;
    mTerrain->vertexStart = 0;
    mTerrain->vertexCount = mSize * mSize;

    VertexDeclaration* decl = mTerrain->vertexDeclaration;
    VertexBufferBinding* bind = mTerrain->vertexBufferBinding;

    // positions
    size_t offset = 0;
    decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
    decl->addElement(NORMAL_BINDING, 0, VET_FLOAT3, VES_NORMAL);
    // texture coord sets
    decl->addElement(TEXCOORD_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
    offset += VertexElement::getTypeSize(VET_FLOAT2);
    decl->addElement(TEXCOORD_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 1);
    offset += VertexElement::getTypeSize(VET_FLOAT2);
    decl->addElement(COLOUR_BINDING, 0, VET_COLOUR, VES_DIFFUSE);

    HardwareVertexBufferSharedPtr vbuf =
        HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(POSITION_BINDING),
            mTerrain->vertexCount, 
            HardwareBuffer::HBU_STATIC_WRITE_ONLY, true);

    bind->setBinding(POSITION_BINDING, vbuf);

    vbuf =
        HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(NORMAL_BINDING),
            mTerrain->vertexCount, 
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    bind->setBinding(NORMAL_BINDING, vbuf);

    vbuf =
        HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            mTerrain->vertexCount, 
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    bind->setBinding(TEXCOORD_BINDING, vbuf);

    vbuf =
        HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(COLOUR_BINDING),
            mTerrain->vertexCount, 
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    bind->setBinding(COLOUR_BINDING, vbuf);

    mInit = true;

    mRenderLevel = 1;

    mMinLevelDistSqr = new Real[ mNumMipMaps ];


    mScale.x = options.scalex;

    mScale.y = options.scaley;

    mScale.z = options.scalez;


    RGBA* pCol = static_cast<RGBA*>( vbuf->lock(HardwareBuffer::HBL_DISCARD) );
    for ( int i = 0; i < mSize*mSize; i++ )
    {
        *pCol++ = 0xFFFFFFFF;
    }
    vbuf->unlock();

    int endx = options.startx + options.size;

    int endz = options.startz + options.size;

    Vector3 left, down, here;

    vbuf = bind->getBuffer(POSITION_BINDING);

    Real* pPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

    HardwareVertexBufferSharedPtr vtexbuf = bind->getBuffer(TEXCOORD_BINDING);

    Real* pTex = static_cast<Real*>(vtexbuf->lock(HardwareBuffer::HBL_DISCARD));

    for ( int j = options.startz; j < endz; j++ )
    {
        for ( int i = options.startx; i < endx; i++ )
        {
            Real height = generator->getHeight(i, j) * options.scaley ;//data[  ( j * (TerrainGenerator::segSize) ) + i  ];
            

            *pPos++ = ( Real ) i * options.scalex; //x
            *pPos++ = height; //y
            *pPos++ = ( Real ) j * options.scalez; //z

            *pTex++ = ( Real ) i / ( Real ) options.world_size ;
            *pTex++ = ( Real ) ( Real ) j / ( Real ) options.world_size;

            *pTex++ = ( ( Real ) i / ( Real ) mSize ) * options.detail_tile;
            *pTex++ = ( ( Real ) ( Real ) j / ( Real ) mSize ) * options.detail_tile;

            if ( height < min )
                min = ( Real ) height;

            if ( height > max )
                max = ( Real ) height;
        }
    }

    vtexbuf->unlock();
    vbuf->unlock();

    mBounds.setExtents( ( Real ) options.startx * options.scalex, min, ( Real ) options.startz * options.scalez,
                        ( Real ) ( endx - 1 ) * options.scalex, max, ( Real ) ( endz - 1 ) * options.scalez );


    mCenter = Vector3( ( options.startx * options.scalex + endx - 1 ) / 2,
                       ( min + max ) / 2,
                       ( options.startz * options.scalez + endz - 1 ) / 2 );


    Real C = _calculateCFactor();

    _calculateMinLevelDist2( C );
    _calculateNormals();

}
}

