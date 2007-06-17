#include "OgreOpcodeTerrainData.h"
#include "OgreCollisionManager.h"

namespace OgreOpcode
{
	TerrainData::TerrainData(const std::vector<float>& HeightmapData, size_t Width, size_t Depth, Ogre::Vector3 Scale, Ogre::Vector3 Offset) :
		mWidth(Width),
		mDepth(Depth),
		mScale(Scale),
		mOffset(Offset)
	{
		mHeightmapData = &HeightmapData;
		mTileShapes.clear();
	}

	TerrainData::~TerrainData()
	{
	}

	std::vector<OgreOpcode::PtrCollisionShape*>* TerrainData::createTileShapes(unsigned int numTilesX, unsigned int numTilesZ)
	{
		if(!mTileShapes.empty()) return NULL;
		// enforcing number of x and z tiles to be greater than zero, and also a power of 2: 2, 4, 8, 16, etc.
		if( (numTilesX <= 0) || !Details::powerOf2(numTilesX) ) return NULL;
		if( (numTilesZ <= 0) || !Details::powerOf2(numTilesZ) ) return NULL;

		// calculate the number of vertices for each tile, 
		// for both x direction (in 3d space) verts, and z direction (in 3d space) verts
		unsigned int xTileSize = ((mWidth - 1) / numTilesX) + 1;
		unsigned int zTileSize = ((mDepth - 1) / numTilesZ) + 1;

		//Create indices - ever 3 indices represent a triangle. 6 indices represent a quad.
		size_t index_size = (xTileSize - 1) * (zTileSize -1) * 6; 
		// create buffer to store computed indices
		size_t* indices = new size_t[index_size*3]; 
		for( size_t x = 0; x < xTileSize - 1; x++) 
		{ 
			for( size_t y=0; y < zTileSize - 1; y++) 
			{ 
				// Define/Store indices that are used to create triangles.
				// The indices are used along with the list of vertices to define triangles.
				// Indices 0, 1, 2, for example, would create a line from vert[0] -> vert[1], vert[1] -> vert[2], vert[2] -> vert[0].
				// The example triangle would look like a line. (not your typical triangle)
				// xTileSize comes in because each horizontal strip of vertices consists of exactly xTileSize vertices.
				// In this way, you can add a z element to the triangle. (actually looks like a triangle in 3 dimensions)

				// The idea here is to go across each row and build up each quad. (2 triangles) 
				// Each quad is a square that gets split into two polygons. 
				// Below is a diagram of one tile where pX represents a vertex. 
				// 
				//   p0 --- p1 
				//   .   /  . 
				//   p2 --- p3 
				// 
				// An example of a counter-clockwise polygon would be: p0 -> p1 -> p2 
				// An example of a clockwise polygon would be: p1 -> p3 -> p2 
				// Note1: note that I said "An example".  
				//	**** You can use any order, but the indexing needs to match your vert list!!! ****
				// Note2: depending on how you define your indices, you can control how the quad looks! (Important)
				//   p0 --- p1 
				//   .   \  . 
				//   p2 --- p3 

				// triangle1: counter clockwise winding. p3 -> p1 -> p0
				indices[(x+y*(xTileSize-1))*6] = (x+1)+(y+1) * xTileSize; 
				indices[(x+y*(xTileSize-1))*6+1] = (x+1)+y * xTileSize; 
				indices[(x+y*(xTileSize-1))*6+2] = x+y * xTileSize; 

				// triangle2: counter clockwise winding. p3 -> p0 -> p2
				indices[(x+y*(xTileSize-1))*6+3] = (x+1)+(y+1) * xTileSize; 
				indices[(x+y*(xTileSize-1))*6+4] = x+y * xTileSize; 
				indices[(x+y*(xTileSize-1))*6+5] = x+(y+1) * xTileSize; 
			} 
		} 

		int tileCounter = 0;
		// Loop through all tiles 
		for ( unsigned int zIndex = 0; zIndex < numTilesZ; ++zIndex ) 
		{ 
			for ( unsigned int xIndex = 0; xIndex < numTilesX; ++xIndex ) 
			{ 
				int numVertsPerTile = xTileSize * zTileSize; 
				float* OOData = new float[numVertsPerTile * 3]; 
				int OODataCounter = 0; 

				int zStart = zIndex * (zTileSize - 1); 
				int xStart = xIndex * (xTileSize - 1); 
				for( unsigned int col = zStart; col < (zStart + zTileSize); ++col) 
				{ 
					for( unsigned int row = xStart; row < (xStart + xTileSize); ++row) 
					{ 
						Ogre::Vector3 v3( 
							mOffset.x + (row * mScale.x), 
							mOffset.y + (at(row,col) * mScale.y), 
							mOffset.z + (col * mScale.z) 
							); 
						OOData[OODataCounter + 0] = v3.x; 
						OOData[OODataCounter + 1] = v3.y; 
						OOData[OODataCounter + 2] = v3.z; 
						OODataCounter += 3; 
					}
				}

				OgreOpcode::PtrCollisionShape* tempTerrainShape; 
				Ogre::String shapeName = "terrainShape" + Ogre::StringConverter::toString(tileCounter++);
				tempTerrainShape = CollisionManager::getSingletonPtr()->createPtrCollisionShape(shapeName); 
				tempTerrainShape->load(numVertsPerTile*3, index_size, OOData, indices); 
				mTileShapes.push_back(tempTerrainShape);
	
				delete[] OOData; 
			} 
		} 

		delete[] indices; 
		indices = NULL;

		return &mTileShapes;
	}

	size_t TerrainData::getDepth()
	{
		return mDepth;
	}

	Ogre::Vector3 TerrainData::getOffset()
	{
		return mOffset;
	}

	Ogre::Vector3 TerrainData::getScale()
	{
		return mScale;
	}

	size_t TerrainData::getWidth()
	{
		return mWidth;
	}
}
