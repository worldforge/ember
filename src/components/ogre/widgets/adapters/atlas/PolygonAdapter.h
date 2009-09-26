//
// C++ Interface: PolygonAdapter
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASPOLYGONADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASPOLYGONADAPTER_H

#include "AdapterBase.h"
#include "components/ogre/authoring/Polygon.h"
#include "components/ogre/authoring/PolygonPointMovement.h"
#include "components/ogre/authoring/IPolygonPositionProvider.h"
#include "components/ogre/EmberEntity.h"

namespace CEGUI
{
class Combobox;
class PushButton;
}

namespace EmberOgre
{

class EmberEntity;
namespace Manipulation
{
class Polygon;
class PolygonPoint;
class PolygonPointPickListener;
class PolygonPointMovement;
}
namespace Gui
{

namespace Adapters
{

namespace Atlas
{

/**
 @brief Provides height positions for a polygon attached to an EmberEntity instance.
 The height will be adjusted for the terrain.
 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class EntityPolygonPositionProvider: public ::EmberOgre::Manipulation::IPolygonPositionProvider
{
public:

	/**
	 * @brief Ctor.
	 * @param entity The entity to which this instance belongs.
	 */
	EntityPolygonPositionProvider(EmberEntity& entity);

	/**
	 * @brief Gets the height for the local position.
	 * The local position will be translated to a global position, and then looked up in the terrain, so that the height of the terrain is returned.
	 * @param localPosition The local position.
	 * @return A height.
	 */
	virtual float getHeightForPosition(const WFMath::Point<2>& localPosition);

protected:
	/**
	 * @brief The entity to which this instance belongs.
	 */
	EmberEntity& mEntity;
};

/**
 @brief An adapter for polygons.
 This allows the editing of polygons through graphical drag and drop operations.
 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class PolygonAdapter: public AdapterBase, public ::EmberOgre::Manipulation::IMovementListener
{
public:
	/**
	 * @brief Ctor.
	 * @param element The Atlas element containing the initial data for the adapter.
	 * @param showButton A button which will toggle the display of the graphical representation of the area on and off.
	 * @param entity The entity to which the area belongs. This is needed to do height lookups so that the polygon points are snapped to the ground. It's optional, but the graphical representation might not work if it's omitted.
	 */
	PolygonAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity);

	/**
	 * @brief Dtor.
	 */
	virtual ~PolygonAdapter();

	/**
	 * @brief Updates the gui with new values.
	 */
	virtual void updateGui(const ::Atlas::Message::Element& element);

	/**
	 * @brief Toggles the display of the graphical representation of the polygon.
	 */
	void toggleDisplayOfPolygon();

	/**
	 * @brief Creates a new polygon.
	 */
	void createNewPolygon();

	/**
	 * @brief When movement has ended, remove the movement instance.
	 */
	virtual void endMovement();

	/**
	 * @brief When movement has been cancelled, remove the movement instance.
	 */
	virtual void cancelMovement();

	/**
	 * @brief True if there's an existing polygon instance created from which a shape can be gotten.
	 * @return True if there's an existing polygon instance.
	 */
	bool hasShape() const;

	/**
	 * @brief Gets the polygon shape.
	 * Note that there's no guarantee that the polygon shape returned will be valid. Make sure to check with WFMath::Polygon::isValid().
	 * @return A polygon shape.
	 */
	const WFMath::Polygon<2> getShape() const;

protected:

	/**
	 * @brief A button used for toggling the display of the polygon on and off.
	 */
	CEGUI::PushButton* mShowButton;

	/**
	 * @brief The polygon used for graphical representation of this area.
	 * Owned by this instance.
	 */
	Manipulation::Polygon* mPolygon;

	/**
	 * @brief Responsible for listening for pick events and initiating movements of the points.
	 */
	Manipulation::PolygonPointPickListener* mPickListener;

	/**
	 * @brief Responsible for handling the movement of a specific point.
	 */
	Manipulation::PolygonPointMovement* mPointMovement;

	/**
	 * @brief An optional entity to which the area belongs.
	 * Mainly used for height lookups, so that the polygon snaps to the ground.
	 */
	EmberEntity* mEntity;

	/**
	 * @brief A position provider, for the polygon.
	 * If an EmberEntity instance is present this will be created (and owned).
	 */
	EntityPolygonPositionProvider* mPositionProvider;

	bool showButton_Clicked(const CEGUI::EventArgs& e);

	/**
	 * @brief Listen for points being picked and initiate movement of the point.
	 * @param point The point that was picked.
	 */
	void pickListener_PickedPoint(Manipulation::PolygonPoint& point);

	virtual void fillElementFromGui();
	virtual bool _hasChanges();

	/**
	 * @brief Tries to get the scene node the entity is attached to.
	 * Not all entities are attached to scene nodes, so this might very well also return null.
	 * @return The scene node to which the entity is attached, or null if it's not attached to any scene node.
	 */
	Ogre::SceneNode* getEntitySceneNode() const;
};

}

}

}

}

#endif
