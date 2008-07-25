/*
	Copyright (C) 2008  Alexey Torkhov <atorkhov@gmail.com>

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

#include "DetachedEntity.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/Action.h"
#include "components/ogre/terrain/TerrainArea.h"
#include "components/ogre/MousePicker.h"
#include <Eris/EntityRouter.h>

namespace EmberOgre
{

DetachedEntity::DetachedEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager)
		: EmberPhysicalEntity(id, ty, vw, sceneManager)
{
}

DetachedEntity::~DetachedEntity()
{
}

void DetachedEntity::init(const Atlas::Message::MapType& attrs)
{
	// Init part from EmberPhysicalEntity --------------------------------------
	///first we need to create the scale node
	createScaleNode();

	/// we need a model mapping
	createModelMapping();
	
	assert(mModelMapping);
	///calling this will result in a call to setModel(...)
	mModelMapping->initialize();
	
	///if the model mapping framework couldn't produce a model to use, we'll fall back to the placeholder
	if (!mModel) {
		S_LOG_WARNING("Entity of type " << getType()->getName() << " have no default model, using placeholder.");
		setModel("placeholder");
	}

	///start out with the default movement mode
	onModeChanged(EmberEntity::MM_DEFAULT);
	
	// EmberEntity init part ---------------------------------------------------
	//EmberEntity::init(ge, fromCreateOp);
	//Eris::Entity::init(ge, fromCreateOp);
	setFromMessage(attrs);
	
	synchronizeWithServer();
	
	// set the Ogre node position and orientation based on Atlas data
	std::stringstream ss;
	ss << "Entity " << getId() << "(" << getName() << ") placed at (" << getPredictedPos().x() << "," << getPredictedPos().y() << "," << getPredictedPos().x() << ")";
	S_LOG_VERBOSE( ss.str());
	
	if (hasAttr("area")) {
		mTerrainArea = std::auto_ptr<Terrain::TerrainArea>(new Terrain::TerrainArea(this));
		if (mTerrainArea->init()) {
			addArea(mTerrainArea.get());
		}
	}
		
	mIsInitialized = true;

	// EmberPhysicalEntity init continues --------------------------------------
	getModel()->setQueryFlags(MousePicker::CM_ENTITY);

/*	assert(mOgreNode);
	assert(mScaleNode);*/
	
	//if there is no bounding box, scaleNode hasn't been called, so do it here
/*	if (!hasBBox()) {
		scaleNode();
	}*/

	initFromModel();
	
	/** If there's an idle animation, we'll randomize the entry value for that so we don't end up with too many similiar entities with synched animations (such as when you enter the world at origo and have 20 settlers doing the exact same motions. */ 
	Model::Action* idleaction = mModel->getAction(ACTION_STAND);
	if (idleaction) {
		idleaction->getAnimations().addTime(Ogre::Math::RangeRandom(0, 15));
	}
	
	
	///check if we should do delayed attachment
	if (mModelMarkedToAttachTo) {
		attachToPointOnModel(mAttachPointMarkedToAttachTo, mModelMarkedToAttachTo);
		mModelMarkedToAttachTo = 0;
		mAttachPointMarkedToAttachTo = "";
	}

	///NOTE: for now, add all particle systems. we will want to add some visibility flag or something in the future
	for (Model::ParticleSystemSet::iterator I = mModel->getParticleSystems().begin(); I != mModel->getParticleSystems().end(); ++I) 
	{
		getScaleNode()->attachObject((*I)->getOgreParticleSystem());
	}
	
	///listen for reload or reset events from the model. This allows us to alter model definitions at run time and have the in game entities update.
//	getModel()->Reloaded.connect(sigc::mem_fun(*this, &EmberPhysicalEntity::Model_Reloaded));
//	getModel()->Resetting.connect(sigc::mem_fun(*this, &EmberPhysicalEntity::Model_Resetting));
}

void DetachedEntity::setFromMessage(const Atlas::Message::MapType& attrs)
{
	beginUpdate();
    
    Atlas::Message::MapType::const_iterator A;

	/*
    attrs.erase("loc");
    attrs.erase("id");
    attrs.erase("contains");
    
    if (!allowMove) filterMoveAttrs(attrs);
	*/

    for (A = attrs.begin(); A != attrs.end(); ++A) {
		if (A->first == "loc" || A->first == "id" || A->first == "contains") continue;

        // see if the value in the sight matches the exsiting value
		Eris:Entity::AttrMap::iterator I = m_attrs.find(A->first);
        if ((I != m_attrs.end()) && (I->second == A->second)) continue;

        setAttr(A->first, A->second);
    }
    
    endUpdate();
}

void DetachedEntity::shutdown()
{
	BeingDeleted.emit();
//	if (m_moving) m_view->removeFromPrediction(this);  // protected

	while (!m_contents.empty()) {
		Entity *e = m_contents.back();
		e->shutdown();
		delete e;
	}
	setLocation(NULL);

//	m_view->getConnection()->unregisterRouterForFrom(m_router, m_id);  // getConnection is protected
//  m_view->entityDeleted(this);  // avoid this because of assert
	delete m_router;

	m_initialised = false;
}

}
