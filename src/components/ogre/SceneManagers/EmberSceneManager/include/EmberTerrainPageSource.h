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

#ifndef DIMETERRAINPAGESOURCE_H
#define DIMETERRAINPAGESOURCE_H

#include <Ogre.h>
#include "OgreTerrainPageSource.h"
#include "OgreTerrainSceneManager.h"

namespace EmberOgre {

class TerrainGenerator;
class EmberTerrainSceneManager;
class EmberTerrainPageSource : public Ogre::TerrainPageSource {
	
	
	
protected:
 
     /** Utility method for building a page of tiles based on some source
    data, wherever that may have come from.
    @remarks
        It is expected that this height data is represented in the range
        [0..1], which will be duly scaled by the TerrainRenderables it
        creates.
    */
//    virtual TerrainPage* buildPage(Real* heightData, Material* pMaterial);

	TerrainGenerator* mGenerator;
	Ogre::ushort mX;
	Ogre::ushort mZ;
	bool mHasTerrain;
	void generatePage(int x, int y);
	EmberTerrainSceneManager* getEmberTerrainSceneManager() const;

public:
    virtual void requestPage (Ogre::ushort x, Ogre::ushort z);
    virtual void expirePage (Ogre::ushort x, Ogre::ushort z);	
 
	EmberTerrainPageSource(TerrainGenerator* const generator);
	virtual ~EmberTerrainPageSource();
	void setHasTerrain(bool hasTerrain);
	
	/*
	 * Pushes the page at the specified indices onto the terrain.
	 * Returns false if no page at that place has been initialized through Mercator
	 * Don't forget to call resizeTerrain() after adding pages.
	 */
	bool pushPage(int x, int y);
	
	/*
	 * Resizes the octree in the scenemanager.
	 * Don't forget to do this after adding pages.
	 */
	void resizeTerrain();

    /** Initialise this tile source based on a series of options as
        dictated by the scene manager. 
    @param tsm The TerrainSceneManager doing the initialising. This should be 
        allowed NULL, for use by external tools if they want to read data 
        generically without necessarily having a real scene manager involved
    @param tileSize The number of horizontal (and hence also vertical) 
        vertices in a single tile (which is a TerrainRenderable). This will
        always be (2^n)+1.
    @param pageSize The number of horizontal (and hence also vertical) 
        vertices in a single page. This will always be (2^n)+1.
    @param asyncLoading
        True if the scene manager would like the tile source to load tiles
        asynchronously. It does not have to do this, although if it does not
        when requested, it will likely result in stalls in the terrain rendering.
    @param optionList 
        A list of name/value pairs describing custom options for this particular
        page source. The expected convention for option names is
        "TypeName.OptionName", where TypeName is the type under which this
        page source has been registered.
    
    virtual void initialise(TerrainSceneManager* tsm, 
        ushort tileSize, ushort pageSize, bool asyncLoading, 
        TerrainPageSourceOptionList& optionList)
    {
        mSceneManager = tsm;
        mTileSize = tileSize;
        mPageSize = pageSize;
        mAsyncLoading = asyncLoading;
    }
    /** Shut down this tile source, freeing all it's memory ready for 
        decommissioning.
    @remarks
        This method will normally just be called on destruction; however
        it may also be called by the TerrainSceneManager if another source
        is provided for the same type of tile source.
    
    virtual void shutdown(void) {}

    /** Requests a new page of tiles from the source.
    @remarks
        The TerrainSceneManager will call this method when it needs new tiles.
        In response, this class must prepare TerrainRenderable instances for
        the page requested and attach the entire page when ready using 
        TerrainSceneManager::attachTerrainPage.
    @par
        Now, the tile source does not necessarily need to do all that before the
        return of this method. If it likes, and particularly if asynchronous 
        loading is enabled, it can merely queue this request, and process it
        either in another thread, or over a series of frames. The key thing
        is that attaching the new page has to be done synchronously with 
        the main rendering loop in order to avoid concurrency issues; 
        other than that, you are free to load and prepare new tiles in 
        a concurrent fashion if you like.
    @par
        Typically the scene manager will request at least one page up-front,
        with the possibility of requesting more if paging is enabled.
    @param x The x index of the page requested
    @param z The z index of the page requested
    */
    /** This notifies the tile source that the specified page of tiles 
        has been automatically detached.
    @remarks
        When paging is enabled, tiles go out of scope and the TerrainSceneManager
        detaches them automatically, notifying the TerrainPageSource that 
        this has happened. The tile source can choose to either keep these
        tiles in memory (incase they are requested again) or can delete them
        if it wishes to free memory. This freeing does not need to be done
        before the return of this method - like requesting tiles, the
        freeing of them can be done in another thread or across many frames
        if required, since the shared data in TerrainSceneManager has already
        been updated synchronously when the page was detached.
    @param x The x index of the page expired
    @param z The z index of the page expired
    */
	
};
}
#endif // DIMETERRAINPAGESOURCE_H
