//
// C++ Implementation: EntityEditor
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include "EntityEditor.h"

#include "components/ogre/MousePicker.h"
#include "components/ogre/Convert.h"
#include "components/ogre/World.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/ShapeVisual.h"
#include "components/ogre/terrain/TerrainManager.h"

#include "adapters/atlas/AdapterBase.h"
#include "adapters/atlas/MapAdapter.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "framework/LoggingInstance.h"

#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Formatter.h>

#include <wfmath/segment.h>

#include <Eris/Entity.h>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include <sigc++/trackable.h>

using namespace Atlas::Message;

namespace Ember
{
namespace OgreView
{

namespace Gui
{

/**
 * @author Erik Ogenvik
 *
 * @brief Provides a marker and a line from an entity to a point.
 *
 * This is useful for visualizing locations known by entities.
 *
 */
class EntityEditor::EntityPointMarker: public sigc::trackable
{
protected:

	/**
	 * @brief The entity to which the marker is connected.
	 */
	Eris::Entity& mEntity;

	/**
	 * @brief The entity which represents the entity.
	 *
	 * This is a large blue ball.
	 */
	Ogre::Entity* mMarkerEntity;

	/**
	 * @brief The node to which the marker entity is attached.
	 */
	Ogre::SceneNode* mMarkerNode;

	/**
	 * @brief A line drawn from the entity to the marker.
	 */
	ShapeVisual* mMarkerDirectionIndicator;

	/**
	 * @brief Provides height data.
	 */
	const Domain::IHeightProvider& mHeightProvider;

	/**
	 * @brief The location we want marked.
	 */
	const WFMath::Point<3> mPoint;

public:

	/**
	 * @brief Updates the marker, and any line drawn to it.
	 */
	void updateMarker()
	{
		//Check if we should adjust to the height of the world
		WFMath::Point<3> adjustedPoint(mPoint);

		float height = adjustedPoint.z();
		if (mHeightProvider.getHeight(Domain::TerrainPosition(mPoint.x(), mPoint.y()), height)) {
			adjustedPoint.z() = height;
		}

		mMarkerNode->setPosition(Convert::toOgre(adjustedPoint));

		WFMath::Segment<3> shape(adjustedPoint, mEntity.getViewPosition() + WFMath::Vector<3>(mEntity.getBBox().getCenter()));
		mMarkerDirectionIndicator->update(shape);

	}

	/**
	 * @brief Called when the entity moves.
	 */
	void entityMoved()
	{
		updateMarker();
	}

	/**
	 * @brief Ctor.
	 * @param entity The entity which the marker is attached to.
	 * @param sceneManager A scene manager used to create nodes and entities.
	 * @param terrainManager Provides height data.
	 * @param point The location which will be marked.
	 */
	EntityPointMarker(Eris::Entity& entity, Ogre::SceneManager& sceneManager, const Domain::IHeightProvider& heightProvider, const WFMath::Point<3>& point) :
			mEntity(entity), mMarkerEntity(0), mMarkerNode(0), mMarkerDirectionIndicator(0), mHeightProvider(heightProvider), mPoint(point)
	{
		mMarkerNode = sceneManager.getRootSceneNode()->createChildSceneNode();
		try {
			mMarkerEntity = sceneManager.createEntity("3d_objects/primitives/models/sphere.mesh");
			//start out with a normal material
			mMarkerEntity->setMaterialName("/global/authoring/point");
			mMarkerEntity->setRenderingDistance(300);
			mMarkerEntity->setQueryFlags(MousePicker::CM_NONPICKABLE);
			mMarkerNode->attachObject(mMarkerEntity);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when creating marker node." << ex);
			return;
		}
		mMarkerNode->setVisible(true);

		mMarkerDirectionIndicator = new ShapeVisual(*sceneManager.getRootSceneNode(), false);

		mEntity.Moved.connect(sigc::mem_fun(*this, &EntityPointMarker::entityMoved));
	}

	/**
	 * @brief Dtor.
	 */
	~EntityPointMarker()
	{
		if (mMarkerEntity) {
			mMarkerEntity->_getManager()->destroyEntity(mMarkerEntity);
		}
		if (mMarkerNode) {
			mMarkerNode->getCreator()->destroySceneNode(mMarkerNode);
		}

		delete mMarkerDirectionIndicator;
	}

};

EntityEditor::EntityEditor(World& world, Eris::Entity& entity, Adapters::Atlas::MapAdapter* rootAdapter) :
		mWorld(world), mRootAdapter(rootAdapter), mEntity(entity), mMarker(0)
{
}

EntityEditor::~EntityEditor()
{
	delete mRootAdapter;

	delete mMarker;
}

void EntityEditor::submitChanges()
{
	if (mRootAdapter->hasChanges()) {
		Atlas::Message::Element rootElement = mRootAdapter->getSelectedChangedElements();
		if (rootElement.isMap()) {
			std::map<std::string, ::Atlas::Message::Element> attributes(rootElement.asMap());
			if (attributes.size()) {

				std::stringstream ss;

				Atlas::Message::QueuedDecoder decoder;

				Atlas::Codecs::XML codec(ss, decoder);
				Atlas::Formatter formatter(ss, codec);
				Atlas::Message::Encoder encoder(formatter);
				formatter.streamBegin();
				encoder.streamMessageElement(attributes);
				formatter.streamEnd();
				S_LOG_VERBOSE("Sending attribute update to server:\n" << ss.str());

				EmberServices::getSingleton().getServerService().setAttributes(&mEntity, attributes);
			}
		}
	}
}

Atlas::Message::Element EntityEditor::createMapElement()
{
	return Element(MapType());
}

Atlas::Message::Element EntityEditor::createListElement()
{
	return Element(ListType());
}
Atlas::Message::Element EntityEditor::createStringElement()
{
	return Element("");
}
Atlas::Message::Element EntityEditor::createIntElement()
{
	return Element(0);
}
Atlas::Message::Element EntityEditor::createFloatElement()
{
	return Element(0.0f);
}
Atlas::Message::Element EntityEditor::createPosition2dElement()
{
	ListType list;
	list.push_back(createFloatElement());
	list.push_back(createFloatElement());
	return Element(list);
}

void EntityEditor::addGoal(const std::string& verb, const std::string& definition)
{
	std::stringstream ss;
	//TODO: we should probably have a better way to define the verbs
	ss << "learn ('" << verb << "', '#" << verb << "_verb1') " << definition;
	EmberServices::getSingleton().getServerService().adminTell(mEntity.getId(), "say", ss.str());
}

void EntityEditor::addKnowledge(const std::string& predicate, const std::string& subject, const std::string& knowledge)
{
	std::stringstream ss;
	ss << "know " << subject << " " << predicate << " " << knowledge;
	EmberServices::getSingleton().getServerService().adminTell(mEntity.getId(), "say", ss.str());
}

void EntityEditor::addMarker(const WFMath::Point<3>& point)
{
	if (point.isValid()) {

		delete mMarker;
		mMarker = new EntityPointMarker(mEntity, mWorld.getSceneManager(), mWorld.getTerrainManager(), point);
		mMarker->updateMarker();
	}
}

void EntityEditor::removeMarker()
{
	delete mMarker;
	mMarker = 0;
}

WFMath::Point<3> EntityEditor::createPoint(float x, float y, float z)
{
	return WFMath::Point<3>(x, y, z);
}

}

}
}
