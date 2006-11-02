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

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"



#include "EmberEntity.h"
#include "model/Model.h"
#include "terrain/TerrainGenerator.h"
#include "WorldEmberEntity.h"
#include "environment/Foliage.h"
#include "environment/Water.h"
#include "environment/Sun.h"
#include "environment/Sky.h"
#include "environment/Environment.h"
#include "environment/CaelumEnvironment.h"
#include "EmberOgre.h"
#include "Avatar.h"
#include "AvatarCamera.h"

//#include "components/ogre/EmberSceneManager/include/EmberTerrainSceneManager.h"
#include <Mercator/Area.h>

namespace EmberOgre {
WorldEmberEntity::WorldEmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, TerrainGenerator* terrainGenerator) : 
mTerrainGenerator(terrainGenerator),
mFoliage(0),
EmberEntity(id, ty, vw, sceneManager)
{
	//getSceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
	sceneManager->getRootSceneNode()->addChild(getSceneNode());
	//mOgreNode = mSceneManager->getRootSceneNode();
	//mSceneManager->getRootSceneNode()->addChild(mOgreNode);
//	this->mModel->setQueryFlag(EmberEntity::CM_TERRAIN);
}

WorldEmberEntity::~WorldEmberEntity()
{}

void WorldEmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	EmberEntity::init(ge, fromCreateOp);
	//mTerrainGenerator->initTerrain(this, getView());
	
	mEnvironment = new Environment::Environment(new Environment::CaelumEnvironment( EmberOgre::getSingleton().getSceneManager(), EmberOgre::getSingleton().getRenderWindow(), EmberOgre::getSingleton().getMainCamera()->getCamera()));
	
	///create the foliage
 	mFoliage = new Environment::Foliage(EmberOgre::getSingleton().getSceneManager());
	
	///prepare all the segments in advance
	mTerrainGenerator->prepareAllSegments();
	//mTerrainGenerator->prepareSegments(0,0,1,true);
	
	///set the position to always 0, 0, 0
	mOgreNode->setPosition(Ogre::Vector3(0, 0, 0));
	
	mEnvironment->initialize();
	
}

void WorldEmberEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v)
{
	///check for terrain updates
    if (str == "terrain") {
    	updateTerrain(v);
	}
	Entity::onAttrChanged(str, v);
}

void WorldEmberEntity::updateTerrain(const Atlas::Message::Element& terrain)
{
	//_fpreset();
    if (!terrain.isMap()) {
		S_LOG_FAILURE( "Terrain is not a map" );
    }
    const Atlas::Message::MapType & tmap = terrain.asMap();
    Atlas::Message::MapType::const_iterator I = tmap.find("points");
    int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
    if (I == tmap.end()) {
		S_LOG_FAILURE( "No terrain points" );
    }

	TerrainGenerator::TerrainDefPointStore pointStore;
	if (I->second.isList()) {
        // Legacy support for old list format.
        const Atlas::Message::ListType & plist = I->second.asList();
        Atlas::Message::ListType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->isList()) {
				S_LOG_INFO( "Non list in points" );
                continue;
            }
            const Atlas::Message::ListType & point = J->asList();
            if (point.size() != 3) {
				S_LOG_INFO( "point without 3 nums" );
                continue;
            }
            //int x = (int)point[0].asNum();
            //int y = (int)point[1].asNum();

			TerrainDefPoint defPoint((int)point[0].asNum(),(int)point[1].asNum(),(int)point[3].asNum());
			pointStore.push_back(defPoint);
			//mTerrain->setBasePoint(x,y,point[2].asNum());
        }
    } else if (I->second.isMap()) {

        const Atlas::Message::MapType & plist = I->second.asMap();
        Atlas::Message::MapType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->second.isList()) {
				S_LOG_INFO( "Non list in points" );
                continue;
            }
            const Atlas::Message::ListType & point = J->second.asList();
            if (point.size() != 3) {
				S_LOG_INFO( "point without 3 nums" );
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            float z = point[2].asNum();
			TerrainDefPoint defPoint(x,y,z);
			pointStore.push_back(defPoint);

			
			
/*	        Mercator::BasePoint bp;
            if (mTerrain->getBasePoint(x, y, bp) && (z == bp.height())) {
				S_LOG_INFO( "Point [" << x << "," << y << " unchanged");
                continue;
            }
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
            bp.height() = z;
            // FIXME Sort out roughness and falloff, and generally
            // verify this code is the same as that in Terrain layer
            mTerrain->setBasePoint(x, y, bp);*/
            
        }
	
	
/*        const Atlas::Message::MapType & plist = I->second.asMap();
        Atlas::Message::MapType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->second.isList()) {
                std::cout << "Non list in points" << std::endl << std::flush;
                continue;
            }
            const Atlas::Message::ListType & point = J->second.asList();
            if (point.size() != 3) {
                std::cout << "point without 3 nums" << std::endl << std::flush;
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
          //  m_terrain.setBasePoint(x, y, point[2].asNum());
			mTerrain->setBasePoint(x,y,point[2].asNum());
//System::instance()->getGraphics()->getTerrainRenderer()->m_terrain.setBasePoint(x,y,point[2].asNum());
//System::Instance()->getGraphics(x,y,point[2].asNum());
        }*/

      

    } else {
		S_LOG_FAILURE( "Terrain is the wrong type" );
        return;
    }
	mTerrainGenerator->updateTerrain(pointStore);
}

void WorldEmberEntity::adjustPositionForContainedNode(EmberEntity* const entity, const Ogre::Vector3& position)
{
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	
	if (entity->getMovementMode() == EmberEntity::MM_FLOATING) {
		sceneNode->setPosition(position.x, 0,position.z);
	} else if (entity->getMovementMode() == EmberEntity::MM_SWIMMING) {
		///if it's swimming, make sure that it's between the sea bottom and the surface
		const TerrainPosition pos = Ogre2Atlas_TerrainPosition(position);
		float height = mTerrainGenerator->getHeight(pos);
		if (position.y < height) {
			sceneNode->setPosition(position.x, height,position.z);
		} else if (position.y > 0) {
			sceneNode->setPosition(position.x, 0,position.z);
		}
		
	} else {
		///get the height from Mercator through the TerrainGenerator
/*		assert(mTerrainGenerator);
//		TerrainPosition pos(entity->getPredictedPos().x(), entity->getPredictedPos().y());
		TerrainPosition pos = Ogre2Atlas_TerrainPosition(position);
		float height = mTerrainGenerator->getHeight(pos);*/
		EmberEntity::adjustPositionForContainedNode(entity, position);
		//sceneNode->setPosition(getOffsetForContainedNode(position, entity));
	}

}

const Ogre::Vector3& WorldEmberEntity::getOffsetForContainedNode(const Ogre::Vector3& localPosition, EmberEntity* const entity)
{
	assert(mTerrainGenerator);
	///NOTE: won't work with threading!
	static Ogre::Vector3 offset = Ogre::Vector3::ZERO;
	float height = mTerrainGenerator->getHeight(Ogre2Atlas_TerrainPosition(localPosition));
	offset.y = height - localPosition.y;
	return offset;
	//return mTerrainGenerator->getHeight(position);

}


 void WorldEmberEntity::onMoved(){
 	Eris::Entity::onMoved();
 }
//  void WorldEmberEntity::onTalk(const Atlas::Objects::Operation::RootOperation& talk)
//  {
//  	Eris::Entity::onTalk(talk);
//  }
//	virtual void setContainer(Entity *pr);
 void WorldEmberEntity::onVisibilityChanged(bool vis)
 {
 	Eris::Entity::onVisibilityChanged(vis);
 }
 void WorldEmberEntity::onLocationChanged(Eris::Entity *oldLocation)
 {
 	Eris::Entity::onLocationChanged(oldLocation);
 }
 
void WorldEmberEntity::addArea(TerrainArea* area)
{
	mTerrainGenerator->addArea(area->getArea());
}



}
