//
// C++ Implementation: GrassFoliage
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "GrassFoliage.h"

#include "framework/LoggingInstance.h"

#include "FoliageLayer.h"

#include "components/ogre/EmberOgre.h"

#include "../EmberOgre.h"
#include "../Convert.h"
#include "../terrain/TerrainInfo.h"
#include "../terrain/TerrainGenerator.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/ISceneManagerAdapter.h"

#include "pagedgeometry/include/PagedGeometry.h"
#include "pagedgeometry/include/BatchPage.h"
#include "pagedgeometry/include/GrassLoader.h"

namespace EmberOgre {

namespace Environment {

GrassFoliage::GrassFoliage(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition)
: FoliageBase(terrainLayerDefinition, foliageDefinition)
, mGrass(0)
, mGrassLoader(0)
, mMinHeight(1.0f)
, mMaxHeight(1.5f)
, mMinWidth(1.0f)
, mMaxWidth(1.5f)
{
	if (mFoliageDefinition.hasParameter("minHeight")) {
		mMinHeight = atof(mFoliageDefinition.getParameter("minHeight").c_str());
	}
	if (mFoliageDefinition.hasParameter("maxHeight")) {
		mMaxHeight = atof(mFoliageDefinition.getParameter("maxHeight").c_str());
	}
	if (mFoliageDefinition.hasParameter("minWidth")) {
		mMinWidth = atof(mFoliageDefinition.getParameter("minWidth").c_str());
	}
	if (mFoliageDefinition.hasParameter("maxWidth")) {
		mMaxWidth = atof(mFoliageDefinition.getParameter("maxWidth").c_str());
	}
}

GrassFoliage::~GrassFoliage()
{
	delete mGrassLoader;
}

void GrassFoliage::initialize()
{
	Ogre::Camera* camera = EmberOgre::getSingleton().getMainOgreCamera();
	mPagedGeometry = new ::Forests::PagedGeometry(camera, EmberOgre::getSingleton().getTerrainGenerator()->getFoliageBatchSize());
	const WFMath::AxisBox<2>& worldSize = EmberOgre::getSingleton().getTerrainGenerator()->getTerrainInfo().getWorldSizeInIndices();

	::Forests::TBounds ogreBounds(Convert::toOgre(worldSize));
	if (ogreBounds.width() != ogreBounds.height()) {
		if (ogreBounds.width() > ogreBounds.height()) {
			float difference = ogreBounds.width() - ogreBounds.height();
			ogreBounds.bottom += difference;
		} else {
			float difference = ogreBounds.height() - ogreBounds.width();
			ogreBounds.right += difference;
		}
	}
	mPagedGeometry->setBounds(ogreBounds);

	mPagedGeometry->addDetailLevel<Forests::GrassPage>(96);

	//Create a GrassLoader object
	mGrassLoader = new ::Forests::GrassLoader<FoliageLayer>(mPagedGeometry);
 	mPagedGeometry->setPageLoader(mGrassLoader);	//Assign the "treeLoader" to be used to load
	mGrassLoader->setHeightFunction(&getTerrainHeight);

	//Add some grass to the scene with GrassLoader::addLayer()
	FoliageLayer *l = mGrassLoader->addLayer(mFoliageDefinition.getParameter("material"));

	l->configure(&mTerrainLayerDefinition, &mFoliageDefinition);
	//Configure the grass layer properties (size, density, animation properties, fade settings, etc.)
	l->setMinimumSize(mMinWidth, mMinHeight);
	l->setMaximumSize(mMaxWidth, mMaxHeight);
	l->setAnimationEnabled(true);		//Enable animations
	if (mFoliageDefinition.hasParameter("swayDistribution")) {
		l->setSwayDistribution(atof(mFoliageDefinition.getParameter("swayDistribution").c_str()));
	} else {
		l->setSwayDistribution(10.0f);		//Sway fairly unsynchronized
	}
	if (mFoliageDefinition.hasParameter("swayLength")) {
		l->setSwayLength(atof(mFoliageDefinition.getParameter("swayLength").c_str()));
	} else {
		l->setSwayLength(0.5f);				//Sway back and forth 0.5 units in length
	}

	if (mFoliageDefinition.hasParameter("swaySpeed")) {
		l->setSwaySpeed(atof(mFoliageDefinition.getParameter("swaySpeed").c_str()));
	} else {
		l->setSwaySpeed(0.5f);				//Sway 1/2 a cycle every second
	}

	if (mFoliageDefinition.hasParameter("fadeTech")) {
		const std::string& fadeTech(mFoliageDefinition.getParameter("fadeTech"));
		if (fadeTech == "alphagrow") {
			l->setFadeTechnique(::Forests::FADETECH_ALPHAGROW);	//Distant grass should slowly fade in
		} else if (fadeTech == "grow") {
			l->setFadeTechnique(::Forests::FADETECH_GROW);	//Distant grass should slowly fade in
		} else {
			l->setFadeTechnique(::Forests::FADETECH_ALPHA);	//Distant grass should slowly fade in
		}
	} else {
		l->setFadeTechnique(::Forests::FADETECH_ALPHA);	//Distant grass should slowly fade in
	}
// 	l->setDensity(1.5f);				//Relatively dense grass
	if (mFoliageDefinition.hasParameter("renderTech")) {
		const std::string& renderTech(mFoliageDefinition.getParameter("renderTech"));
		if (renderTech == "quad") {
			l->setRenderTechnique(::Forests::GRASSTECH_QUAD);
		} else if (renderTech == "sprite") {
			l->setRenderTechnique(::Forests::GRASSTECH_SPRITE);
		} else {
			l->setRenderTechnique(::Forests::GRASSTECH_CROSSQUADS);	//Draw grass as scattered quads
		}
	} else {
		l->setRenderTechnique(::Forests::GRASSTECH_CROSSQUADS);	//Draw grass as scattered quads
	}

	l->setMapBounds(Convert::toOgre(worldSize));

}

void GrassFoliage::frameStarted(const Ogre::FrameEvent & evt)
{
	if (mPagedGeometry) {
		try {
			mPagedGeometry->update();
		} catch (const Ogre::Exception& ex)
		{
			S_LOG_FAILURE("Error when updating grass. Will disable grass.\n"<< ex.what());
			delete mGrassLoader;
			delete mPagedGeometry;
			mGrassLoader = 0;
			mPagedGeometry = 0;
		}
	}
}

}

}
