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
#include "DimeTerrainRenderable.h"
#include "services/logging/LoggingService.h"
#include "MathConverter.h"

#include "DimeTerrainSceneManager.h"

DimeTerrainSceneManager* DimeTerrainSceneManager::_instance = 0;


DimeTerrainSceneManager & DimeTerrainSceneManager::getSingleton(void)
{
	//fprintf(stderr, "TRACE - ENTITY LISTENER - SINGLETON ENTERING\n");
	if(_instance == 0)
		_instance = new DimeTerrainSceneManager;
	return *_instance;
}

DimeTerrainSceneManager::DimeTerrainSceneManager() : mGenerator(0)
{}
DimeTerrainSceneManager::~DimeTerrainSceneManager()
{}

void DimeTerrainSceneManager::buildTerrainAroundAvatar()
{
    long lowXBound = lrintf(OGRE2WF(mPositionOfAvatar.x) / TerrainGenerator::segSize) - 2,
         lowZBound = lrintf(OGRE2WF(mPositionOfAvatar.z) / TerrainGenerator::segSize) - 2;
	buildTerrain(lowXBound, lowZBound, 5);

}


void DimeTerrainSceneManager::buildTerrain(long segmentXStart, long segmentZStart, long numberOfSegments)
{
	int i,j;

	mGenerator->prepareSegments(segmentXStart, segmentZStart, numberOfSegments);
	
    Ogre::TerrainOptions* options = new Ogre::TerrainOptions();
    
    //mTerrain = terrain;
    
    options->max_mipmap = 5;

    options->scalex = WF2OGRE(1); //100/64;

    options->scaley = WF2OGRE(1);

    options->scalez = WF2OGRE(1);//100/64;

    options->max_pixel_error = 8;

    options->size = 64+1 ;
//    options->world_size = (TerrainGenerator::segSize * numberOfSegments) + 1;
    options->world_size = (TerrainGenerator::segSize * numberOfSegments) + 1;


//    options->colored = true;

//    options->lit = true;

    mScale = Ogre::Vector3( options->scalex, options->scaley, options->scalez );

    mTileSize = options->size;

	Ogre::String detail_texture = "terrain_detail.jpg";

    Ogre::String world_texture = "terrain_texture.jpg";
	
    // set up the octree size.
    float max_x = options->scalex * options->world_size;

    float max_y = WF2OGRE(255) * options->scaley;

    float max_z = options->scalez * options->world_size;

    resize( Ogre::AxisAlignedBox( 0, 0, 0, max_x, max_y, max_z ) );




    mTerrainMaterial = createMaterial( "Terrain" );


    mTerrainMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( world_texture, 0 );



    mTerrainMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( detail_texture, 1 );


    mTerrainMaterial -> setLightingEnabled( options->lit );

    mTerrainMaterial->load();


    //create a root terrain node.
    mTerrainRoot = getRootSceneNode() -> createChildSceneNode( "Terrain" );
    //mTerrainRoot->rotate(Ogre::Vector3::UNIT_Y,180);
	//mTerrainRoot->showBoundingBox(true);
    //setup the tile array.
    int num_tiles = ( options->world_size - 1 ) / ( options->size - 1 );
    
    for (i = 0; i < num_tiles; i++ )
    {
        mTiles.push_back( Ogre::TerrainRow() );

        for (j = 0; j < num_tiles; j++ )
        {
            mTiles[ i ].push_back( 0 );
        }
    }

    char name[ 24 ];
    int p = 0;
    int q = 0;
    
    int worldXOffset = segmentXStart * TerrainGenerator::segSize;
    int worldZOffset = segmentZStart * TerrainGenerator::segSize;

    for ( j = worldZOffset; j < (worldZOffset + options->world_size) - 1; j += ( options->size - 1 ) )
    {
        p = 0;

        for ( i = worldXOffset; i < (worldXOffset + options->world_size) - 1; i += ( options->size - 1 ) )
        {
  
            options->startx = i;
            options->startz = j;
            sprintf( name, "tile[%d,%d]", p, q );

            Ogre::SceneNode *c = mTerrainRoot -> createChildSceneNode( name );
            DimeTerrainRenderable *tile = new DimeTerrainRenderable();
            //c->rotate(Ogre::Vector3::UNIT_Y,-90);
            

            tile -> setMaterial( mTerrainMaterial );
            tile -> init( *options, mGenerator );

            mTiles[ p ][ q ] = tile;

            c -> attachObject( tile );
            p++;
        }
        std::string output = std::string("TRACE - TERRAIN SCENE MANAGER - CREATED TILEROW: ");
        output += q;
//        output += name;
		dime::LoggingService::getInstance()->slog(__FILE__, __LINE__, dime::LoggingService::INFO) << output  << dime::ENDM;

        q++;

    }


    //setup the neighbor links.
    int size = ( int ) mTiles.size();

    for ( j = 0; j < size; j++ )
    {
        for ( i = 0; i < size; i++ )
        {
            if ( j != size - 1 )
            {
                mTiles[ i ][ j ] -> _setNeighbor( Ogre::TerrainRenderable::SOUTH, mTiles[ i ][ j + 1 ] );
                mTiles[ i ][ j + 1 ] -> _setNeighbor( Ogre::TerrainRenderable::NORTH, mTiles[ i ][ j ] );
            }

            if ( i != size - 1 )
            {
                mTiles[ i ][ j ] -> _setNeighbor( Ogre::TerrainRenderable::EAST, mTiles[ i + 1 ][ j ] );
                mTiles[ i + 1 ][ j ] -> _setNeighbor( Ogre::TerrainRenderable::WEST, mTiles[ i ][ j ] );
            }

        }
    }

    if(options->lit)
    {
        for ( j = 0; j < size; j++ )
        {
            for ( i = 0; i < size; i++ )
            {
                mTiles[ i ][ j ] -> _calculateNormals( );
                //  mTiles[ i ][ j ] -> _generateVertexLighting( Vector3( 255, 100, 255 ), ColourValue(.25,.25,.25) );
            }
        }
    }
	
	//mTerrainRoot->rotate(Ogre::Vector3::UNIT_Y,180);

    /*
     for ( j = 0; j < size; j++ )
     {
         for ( i = 0; i < size; i++ )
         {
             mTiles[ i ][ j ] -> _generateVertexLighting( Vector3( 255, 50, 255 ), ColourValue( .25, .25, .25 ) );
         }
     }
     */



}


