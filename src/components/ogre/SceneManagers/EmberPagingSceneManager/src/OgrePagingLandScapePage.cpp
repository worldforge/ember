/***************************************************************************
	OgrePagingLandScapePage.cpp  -  description
	-------------------
	begin                : Sat Mar 08 2003
	copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
	email                : spoke2@supercable.es && tuan.kuranes@free.fr

  * Copyright (C) 2009 Manuel A. Fernandez Montecelo <manuel.montezelo@gmail.com>

***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include "OgrePagingLandScapePage.h"

#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeListenerManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapePageRenderable.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileManager.h"

#include "framework/LoggingInstance.h"

#include <OgreAxisAlignedBox.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreVector3.h>


namespace Ogre
{
	PagingLandScapePage::PagingLandScapePage(PagingLandScapePageManager& pageMgr) :
		mPageMgr(pageMgr),
		mPageState(STATE_UNINITED),
		mPageInQueue(QUEUE_NONE),
		mTableX(0),
		mTableZ(0),
		mWorldPosition(Ogre::Vector3::ZERO),
		mVisible(false),
		mPageNode(0),
		mRenderable(0),
		mNumTiles(0)
	{
		touch();

		// initialize own neighbors
		mNeighbors[NORTH] = 0;
		mNeighbors[SOUTH] = 0;
		mNeighbors[EAST] = 0;
		mNeighbors[WEST] = 0;
	}
	//-----------------------------------------------------------------------
	PagingLandScapePage::~PagingLandScapePage()
	{
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::init(unsigned int tableX, unsigned int tableZ)
	{
		if (mPageState != STATE_UNINITED) {
			S_LOG_WARNING("PagingLandScapePage at (" << tableX << ", " << tableZ << ") already inited, ignoring request");
			return;
		}

		S_LOG_VERBOSE("Initing PagingLandScapePage at (" << tableX << ", " << tableZ << ")");

		mTableX = tableX;
		mTableZ = tableZ;

		// create a LandScape node that handles pages and tiles
		const String NodeName = "PagingLandScapePage."
			+ StringConverter::toString(mTableX) + "."
			+ StringConverter::toString(mTableZ) + ".Node";
		mPageNode = mPageMgr.getSceneManager()->createSceneNode(NodeName);

		// boundaries of this page, the middle page is at world coordinates 0,0
		const PagingLandScapeOptions* options = mPageMgr.getOptions();
		mNumTiles = options->NumTiles;

		const unsigned int size = options->PageSize - 1;
		const Real factorX = size * options->scale.x;
		const Real factorZ = size * options->scale.z;

		const Real iniX = static_cast<Real>(static_cast<int>(mTableX + mTableX - options->world_width)) * 0.5f * factorX + options->position.x;		
		const Real iniZ = static_cast<Real>(static_cast<int>(mTableZ + mTableZ - options->world_height)) * 0.5f * factorZ + options->position.z;	
		const Real endX = iniX + factorX;
		const Real endZ = iniZ + factorZ;
		const Real minHeight = 0.0f;
		const Real maxHeight = mPageMgr.getSceneManager()->getData2DManager()->getMaxHeight(mTableX, mTableZ);

		// set node position
		mPageNode->setPosition(iniX, options->position.y, iniZ);

		// set bounds
		mBounds.setExtents(iniX, minHeight, iniZ,
				   endX, maxHeight, endZ);
		mWorldPosition = mBounds.getCenter();

		// create renderable
		if (options->BigImage) {
			mRenderable = new PagingLandScapePageRenderable(&mPageMgr,
									mPageNode->getName() + "rend", 
									mTableX, mTableZ,
									mBounds);
			mPageNode->attachObject(mRenderable);
			mRenderable->load();
		}

		touch();

		// set new state
		mPageState = STATE_INITED;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::load()
	{
		touch();

		if (isLoaded()) {
			S_LOG_WARNING("PagingLandScapePage at (" << mTableX << ", " << mTableZ << ") already loaded, ignoring request");
			return;
		}

		S_LOG_VERBOSE("Loading PagingLandScapePage at (" << mTableX << ", " << mTableZ << ")");

		if (mPageState == STATE_INITED) {
			preloadInBackground();
			return;
		}

		if (mPageState == STATE_PRELOADED) {

			// load texture
			S_LOG_VERBOSE("Loading texture for PagingLandScapePage at (" << mTableX << ", " << mTableZ << ")");
			mPageMgr.getSceneManager()->getTextureManager()->load(mTableX, mTableZ);

			// initialize tile containers
			mTiles.reserve(mNumTiles);
			mTiles.resize(mNumTiles);
			for (size_t i = 0; i < mNumTiles; ++i) {
				mTiles[i].reserve(mNumTiles);
				mTiles[i].resize(mNumTiles);
			}

			// initialize tiles
			PagingLandScapeTileManager* tileMgr = mPageMgr.getSceneManager()->getTileManager();
			for (size_t i = 0; i < mTiles.size(); ++i) {
				for (size_t j = 0; j < mTiles[i].size(); ++j) {
					PagingLandScapeTile* tile = tileMgr->getTile();
					assert (tile);
					mTiles[i][j] = tile;
					tile->init(mPageNode, mTableX, mTableZ, i, j);

					// set up tile neighbors within page
					if (j > 0) {
						PagingLandScapeTile* northTile = mTiles[i][j-1];
						tile->_setNeighbor(NORTH, northTile);
						northTile->_setNeighbor(SOUTH, tile);
					}
					if (i > 0) {
						PagingLandScapeTile* westTile = mTiles[i-1][j];
						tile->_setNeighbor(WEST, westTile);
						westTile->_setNeighbor(EAST, tile);    
					}
				}
			}

			// set page neighbors
			setNeighbors();

			// set new state
			mPageState = STATE_LOADED;

			// fire event
			fireEvent(EVENT_LOADED);

		} else {
			S_LOG_WARNING("PagingLandScapePage at (" << mTableX << ", " << mTableZ << ") already loaded, proceeding to show it");
		}

		show(true);
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::preloadInBackground()
	{
		touch();

		if (mPageState == STATE_INITED) {
			// set new state
			mPageState = STATE_PRELOADING;

			S_LOG_VERBOSE("Preloading PagingLandScapePage at (" << mTableX << ", " << mTableZ << ")");
			mPageMgr.getSceneManager()->getData2DManager()->load(mTableX, mTableZ);

		} else {
			S_LOG_WARNING("PagingLandScapePage at (" << mTableX << ", " << mTableZ << ") already preloaded (or not inited), ignoring request");
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::uninit()
	{
		if (mPageState == STATE_UNINITED) {
			S_LOG_WARNING("PagingLandScapePage at (" << mTableX << ", " << mTableZ << ") already uninited, ignoring request");
			return;
		}

		S_LOG_VERBOSE("Uniniting PagingLandScapePage at (" << mTableX << ", " << mTableZ << ")");

		// unload, if loaded
		if (isLoaded()) {
			// hide
			show(false);

			// unload the tiles
			for (size_t i = 0; i < mTiles.size(); ++i) {
				for (size_t j = 0; j < mTiles[i].size(); ++j) {
					mTiles[i][j]->uninit();
				}
			}
			mTiles.clear();

			// fire event
			fireEvent(EVENT_UNLOADED);

			// unload texture
			mPageMgr.getSceneManager()->getTextureManager()->unload(mTableX, mTableZ);

			// post unload
			mPageMgr.getSceneManager()->getData2DManager()->unload(mTableX, mTableZ);
			fireEvent(EVENT_POSTUNLOADED);
		}

		// destroy renderable
		if (mPageMgr.getOptions()->BigImage) {
			mPageNode->detachObject(mRenderable->getName());
			delete mRenderable;
			mRenderable = 0;
		}	

		// destroy page node
		if (mPageNode) {
			mPageNode->removeAndDestroyAllChildren();
			mPageMgr.getSceneManager()->destroySceneNode(mPageNode->getName());
			mPageNode = 0;
		}

		// update neighbors (tell them that we're gone)
		resetNeighbors();

		// restore state
		mBounds.setNull();
		mWorldPosition = Ogre::Vector3::ZERO;

		// set new state
		mPageState = STATE_UNINITED;
	}

	//-----------------------------------------------------------------------
	void PagingLandScapePage::setNeighbors()
	{
		PagingLandScapePage* neighbor = 0;
		// south
		neighbor = mPageMgr.getPage(mTableX, mTableZ + 1, false);
		setNeighbor(SOUTH, neighbor);
		if (neighbor) {
			neighbor->setNeighbor(NORTH, this);
		}
		// north
		neighbor = mPageMgr.getPage(mTableX, mTableZ - 1, false);
		setNeighbor(NORTH, neighbor);
		if (neighbor) {
			neighbor->setNeighbor(SOUTH, this);
		}
		// east
		neighbor = mPageMgr.getPage(mTableX + 1, mTableZ, false);
		setNeighbor(EAST, neighbor);
		if (neighbor) {
			neighbor->setNeighbor(WEST, this);
		}
		// west
		neighbor = mPageMgr.getPage(mTableX - 1, mTableZ, false);
		setNeighbor(WEST, neighbor);
		if (neighbor) {
			neighbor->setNeighbor(EAST, this);
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::resetNeighbors()
	{
		// update neighbors (tell them that we're gone)
		if (mNeighbors[NORTH]) {
			mNeighbors[NORTH]->setNeighbor(SOUTH, 0);
		}
		if (mNeighbors[SOUTH]) {
			mNeighbors[SOUTH]->setNeighbor(NORTH, 0);
		}
		if (mNeighbors[EAST]) {
			mNeighbors[EAST]->setNeighbor(WEST, 0);
		}
		if (mNeighbors[WEST]) {
			mNeighbors[WEST]->setNeighbor(EAST, 0);
		}

		// reset own neighbors
		mNeighbors[NORTH] = 0;
		mNeighbors[SOUTH] = 0;
		mNeighbors[EAST] = 0;
		mNeighbors[WEST] = 0;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::setNeighbor(Neighbor n, PagingLandScapePage* p)
	{
		// set page neighbor
		mNeighbors[n] = p;
	 
		// if neigther are loaded, nothing else to do
		const bool thisLoaded = isLoaded();
		const bool neighLoaded = p && p->isLoaded();
		if (!thisLoaded && !neighLoaded) {
			return;
		}

		assert (!thisLoaded || (thisLoaded && !mTiles.empty()));

		// tiles, 0 if not loaded
		PagingLandScapeTile* tileInCurrPage = 0;
		PagingLandScapeTile* tileInNeighborPage = 0;

		// indices for borders
		const unsigned int iNorthBorder = 0;
		const unsigned int iSouthBorder = mNumTiles - 1;
		const unsigned int iEastBorder = mNumTiles - 1;
		const unsigned int iWestBorder = 0;

		// walk the tiles in each border, and set roles accordingly
		switch (n) {
		case EAST:
		{
			for (unsigned int j = 0; j < mNumTiles; ++j) {
				if (thisLoaded) {
					// tiles of this page along east border
					tileInCurrPage = mTiles[iEastBorder][j];
				}

				if (neighLoaded) {
					// tiles of neighbor page along west border
					tileInNeighborPage = p->getTile(iWestBorder, j);
				}

				// set neighbor tiles
				if (tileInCurrPage) {
					tileInCurrPage->_setNeighbor(EAST, tileInNeighborPage);
				}
				if (tileInNeighborPage) {
					tileInNeighborPage->_setNeighbor(WEST, tileInCurrPage);
				}
			}
		}
		break;
		case WEST:
		{
			for (unsigned int j = 0; j < mNumTiles; ++j) {
				if (thisLoaded) {
					// tiles of this page along west border
					tileInCurrPage = mTiles[iWestBorder][j];
				}

				if (neighLoaded) {
					// tiles of neighbor page along east border
					tileInNeighborPage = p->getTile(iEastBorder, j);
				}

				// set neighbor tiles
				if (tileInCurrPage) {
					tileInCurrPage->_setNeighbor(WEST, tileInNeighborPage);
				}
				if (tileInNeighborPage) {
					tileInNeighborPage->_setNeighbor(EAST, tileInCurrPage);
				}
			}
		}
		break;
		case NORTH:
		{
			for (unsigned int i = 0; i < mNumTiles; ++i) {
				if (thisLoaded) {
					// tiles of this page along north border
					tileInCurrPage = mTiles[i][iNorthBorder];
				}

				if (neighLoaded) {
					// tiles of neighbor page along south border
					tileInNeighborPage = p->getTile(i, iSouthBorder);
				}

				// set neighbor tiles
				if (tileInCurrPage) {
					tileInCurrPage->_setNeighbor(NORTH, tileInNeighborPage);
				}
				if (tileInNeighborPage) {
					tileInNeighborPage->_setNeighbor(SOUTH, tileInCurrPage);
				}
			}
		}
		break;
		case SOUTH:
		{
			for (unsigned int i = 0; i < mNumTiles; ++i) {
				if (thisLoaded) {
					// tiles of this page along south border
					tileInCurrPage = mTiles[i][iSouthBorder];
				}

				if (neighLoaded) {
					// tiles of neighbor page along north border
					tileInNeighborPage = p->getTile(i, iNorthBorder);
				}

				// set neighbor tiles
				if (tileInCurrPage) {
					tileInCurrPage->_setNeighbor(SOUTH, tileInNeighborPage);
				}
				if (tileInNeighborPage) {
					tileInNeighborPage->_setNeighbor(NORTH, tileInCurrPage);
				}
			}
		}
		break;
		default:
			break;
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::setMapMaterial()
	{    
		if (mPageMgr.getOptions()->BigImage) {
			mRenderable->setMaterial(mPageMgr.getSceneManager()->getTextureManager()->getMapMaterial());
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::touch()
	{ 
		mTimeUntouched = mPageMgr.getOptions()->PageInvisibleUnloadFrames;
	}
	//-----------------------------------------------------------------------
	bool PagingLandScapePage::unloadUntouched()
	{
		if (mTimeUntouched == 0) {
			return true;
		} else {
			--mTimeUntouched; 
			return false;
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::updateTerrain()
	{
		const unsigned int size = mPageMgr.getOptions()->TileSize - 1;
		for (size_t i = 0; i < mTiles.size(); ++i) {
			for (size_t j = 0; j < mTiles[i].size(); ++j) {
				PagingLandScapeTile* tile = mTiles[i][j];
				/// we need to adjust the deformation rectangle for the whole tile/renderable so to force a recalculation of the bounding box and the LOD calculation
				if (tile->getRenderable()) {
					tile->getRenderable()->adjustDeformationRectangle(0, 0);
					tile->getRenderable()->adjustDeformationRectangle(size, size);
				}
				tile->updateTerrain();
			}
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::show(bool do_show)
	{
		if (mPageState != STATE_LOADED) {
			S_LOG_WARNING("PagingLandScapePage at (" << mTableX << ", " << mTableZ << ") not loaded, ignoring show() request");
			return;
		}

		S_LOG_VERBOSE("Showing/hiding PagingLandScapePage at (" << mTableX << ", " << mTableZ << ")");

		// set tiles in use or not according with visibility
		for (size_t i = 0; i < mTiles.size(); ++i) {
			for (size_t j = 0; j < mTiles[i].size(); ++j) {
				mTiles[i][j]->setInUse(do_show);
			}
		}

		if (do_show) {
			assert (!mVisible);

			// add node to the scene
			if (!mPageNode->getParent()) {
				mPageMgr.getSceneManager()->getRootSceneNode()->addChild(mPageNode);
			}

			mVisible = true;

			// fire event
			fireEvent(EVENT_SHOW);

		} else {
			assert (mVisible);

			// remove node from the scene
			if (mPageNode->getParent()) {
				mPageMgr.getSceneManager()->getRootSceneNode()->removeChild(mPageNode->getName());
			}

			mVisible = false;

			// fire event
			fireEvent(EVENT_HIDE);
		}
	}
	//-----------------------------------------------------------------------
	bool PagingLandScapePage::notify(const Vector3& pos, const PagingLandScapeCamera* cam)
	{
		if (mPageState == STATE_LOADED)	{
			touch();

			for (size_t i = 0; i < mTiles.size(); ++i) {
				for (size_t j = 0; j < mTiles[i].size(); ++j) {
					mTiles[i][j]->_Notify(pos, cam);
				}
			}

			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------
	PagingLandScapeTile* PagingLandScapePage::getTile(const Vector3& pos) const
	{
		const size_t x = static_cast<unsigned int>(pos.x / mPageMgr.getOptions()->scale.x / (mPageMgr.getOptions()->TileSize));
		const size_t z = static_cast<unsigned int>(pos.z / mPageMgr.getOptions()->scale.z / (mPageMgr.getOptions()->TileSize));

		PagingLandScapeTile* tile = getTile(x, z);
		if (tile && tile->isLoaded()) {
			return tile;
		} else {
			return 0;
		}
	}
	//-------------------------------------------------------------------------
	PagingLandScapeTile* PagingLandScapePage::getTile(unsigned int i, unsigned int j) const
	{
		if (mPageState == STATE_LOADED && (i < mTiles.size()) && (j < mTiles[i].size())) {
			return mTiles[i][j];
		} else {
			return 0;
		}
	}
	//-------------------------------------------------------------------------
	void PagingLandScapePage::updateLod()
	{
		if (mPageState == STATE_LOADED) {
			for (size_t i = 0; i < mTiles.size(); ++i) {
				for (size_t j = 0; j < mTiles[i].size(); ++j) {
					PagingLandScapeTile* t = mTiles[i][j];
					if (t->isVisible()) {
						t->_updateLod();
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------------
	void PagingLandScapePage::setRenderQueue(uint8 qid)
	{
		if (mPageState == STATE_LOADED && mVisible) {
			for (size_t i = 0; i < mTiles.size(); ++i) {
				for (size_t j = 0; j < mTiles[i].size(); ++j) {
					PagingLandScapeTile* t = mTiles[i][j];
					if (t->isVisible()) {
						t->setRenderQueueGroup(qid);
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePage::fireEvent(EventType type)
	{
		Ogre::PagingLandScapeListenerManager* listenerMgr = mPageMgr.getSceneManager()->getListenerManager();
		Ogre::PagingLandScapeData2DManager* data2dMgr = mPageMgr.getSceneManager()->getData2DManager();

		switch (type) {
		case EVENT_PRELOADED:
		{
			listenerMgr->firePagePreloaded(mTableX, mTableZ, 
						       data2dMgr->getData2D(mTableX, mTableZ)->getHeightData(),
						       mBounds);
		}
		break;
		case EVENT_LOADED:
		{
			listenerMgr->firePageLoaded(mTableX, mTableZ, 
						    data2dMgr->getData2D(mTableX, mTableZ)->getHeightData(),
						    mBounds);
			mPageMgr.eventPageLoaded(this);
		}
		break;
		case EVENT_UNLOADED:
		{
			listenerMgr->firePageUnloaded(mTableX, mTableZ, 
						      data2dMgr->getData2D(mTableX, mTableZ)->getHeightData(),
						      mBounds);
		}
		break;
		case EVENT_POSTUNLOADED:
		{
			listenerMgr->firePagePostunloaded(mTableX, mTableZ);
		}
		break;
		case EVENT_SHOW:
		{
			listenerMgr->firePageShow(mTableX, mTableZ, 
						  data2dMgr->getData2D(mTableX, mTableZ)->getHeightData(),
						  mBounds);
		}
		break;
		case EVENT_HIDE:
		{
			listenerMgr->firePageHide(mTableX, mTableZ, 
						  data2dMgr->getData2D(mTableX, mTableZ)->getHeightData(),
						  mBounds);
		}
		break;
		default:
			// nothing, shouldn't happen
			break;
		}
	}
	//-------------------------------------------------------------------------
	void PagingLandScapePage::eventData2DLoaded(bool operationOK)
	{
		if (mPageState == STATE_PRELOADING) {
			S_LOG_VERBOSE("PagingLandScapePage at (" << mTableX << ", " << mTableZ << ") got Data2D loaded");
			if (!operationOK) {
				// set new state
				mPageState = STATE_NOTLOADABLE;

				S_LOG_WARNING("PagingLandScapePage at (" << mTableX << ", " << mTableZ << ") is not loadable");
			} else {
				// set new state
				mPageState = STATE_PRELOADED;

				// fire event
				fireEvent(EVENT_PRELOADED);

				// load
				load();
			}
		} else {
			S_LOG_WARNING("PagingLandScapePage at (" << mTableX << ", " << mTableZ << ") already preloaded (or not in preloading state), ignoring Data2DLoaded event");
		}
	}

} //namespace
