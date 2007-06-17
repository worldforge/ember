#ifndef __OgreOpcodeTerrainData_h__
#define __OgreOpcodeTerrainData_h__

#include "OgrePtrCollisionShape.h"
#include <vector>

namespace OgreOpcode
{
	/// OgreOpcode container for data related to a particular terrain.
	/// Uses data to derive PtrCollisionShape objects in the form of Tiles.
	class _OgreOpcode_Export TerrainData
	{
	public:
		// Constructor supporting Data that can be retrieved from ET::TerrainInfo derived Terrain.
		TerrainData(const std::vector<float>& HeightmapData, size_t Width, size_t Depth, Ogre::Vector3 Scale, Ogre::Vector3 Offset = Ogre::Vector3::ZERO);
		~TerrainData();

		/** Retrieves a value from the heightmap. */
		const float& at(size_t x, size_t y) { return (*mHeightmapData)[x + y*mWidth]; }

		// It is up to the user to properly destroy the created Tile Shapes, just like all other created shapes!
		std::vector<OgreOpcode::PtrCollisionShape*>* createTileShapes(unsigned int numTilesX = 2, unsigned int numTilesZ = 2);

		size_t getDepth();
		Ogre::Vector3 getOffset();
		Ogre::Vector3 getScale();
		size_t getWidth();

	protected:
	private:
		const std::vector<float>* mHeightmapData;
		size_t mWidth;
		size_t mDepth;
		// ETM does not require terrain be at position 0,0,0.  
		// In fact, you can even have multiple terrain rendered.
		Ogre::Vector3 mOffset; // origin of the terrain
		Ogre::Vector3 mScale; // ratio of vertices to 3d coordinate space, in user defined dimensions

		std::vector<OgreOpcode::PtrCollisionShape*> mTileShapes;
	};
}

#endif
