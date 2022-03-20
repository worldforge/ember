//
// C++ Interface: PolygonAdapter
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASPOLYGONADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASPOLYGONADAPTER_H

#include "AdapterBase.h"
#include "components/ogre/authoring/Polygon.h"
#include "components/ogre/authoring/PolygonPointMovement.h"
#include "components/ogre/authoring/IPolygonPositionProvider.h"
#include "domain/EmberEntity.h"

namespace CEGUI {
class Combobox;

class PushButton;
}

namespace Ember {
class EmberEntity;
namespace OgreView {
namespace Authoring {
class Polygon;

class PolygonPoint;

class PolygonPointPickListener;

class PolygonPointMovement;
}
namespace Gui {

namespace Adapters {

namespace Atlas {

/**
 @brief Provides height positions for a polygon attached to an EmberEntity instance.
 The height will be adjusted for the terrain.
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class EntityPolygonPositionProvider : public Authoring::IPolygonPositionProvider {
public:

	/**
	 * @brief Ctor.
	 * @param entity The entity to which this instance belongs.
	 */
	explicit EntityPolygonPositionProvider(EmberEntity& entity);

	/**
	 * @brief Dtor.
	 */
	~EntityPolygonPositionProvider() override = default;

	/**
	 * @brief Gets the height for the local position.
	 * The local position will be translated to a global position, and then looked up in the terrain, so that the height of the terrain is returned.
	 * @param localPosition The local position.
	 * @return A height.
	 */
	double getHeightForPosition(const WFMath::Point<2>& localPosition) const override;

protected:
	/**
	 * @brief The entity to which this instance belongs.
	 */
	EmberEntity& mEntity;
};

/**
 @brief An adapter for polygons.
 This allows the editing of polygons through graphical drag and drop operations.
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class PolygonAdapter : public AdapterBase, public Authoring::IMovementListener, public virtual sigc::trackable {
public:
	/**
	 * @brief Ctor.
	 * @param element The Atlas element containing the initial data for the adapter.
	 * @param showButton A button which will toggle the display of the graphical representation of the area on and off.
	 * @param entity The entity to which the area belongs. This is needed to do height lookups so that the polygon points are snapped to the ground. It's optional, but the graphical representation might not work if it's omitted.
	 */
	PolygonAdapter(const ::Atlas::Message::Element& element,
				   CEGUI::PushButton* showButton,
				   EmberEntity* entity);

	/**
	 * @brief Dtor.
	 */
	~PolygonAdapter() override;

	/**
	 * @brief Updates the gui with new values.
	 */
	void updateGui(const ::Atlas::Message::Element& element) override;

	/**
	 * @brief Toggles the display of the graphical representation of the polygon.
	 */
	void toggleDisplayOfPolygon();

	/**
	 * @brief Creates a new polygon.
	 * @param existingPoly An optional existing poly definition to use as base. If null, a default shape will be used.
	 */
	void createNewPolygon(WFMath::Polygon<2>* existingPoly = nullptr);

	/**
	 * @brief When movement has ended, remove the movement instance.
	 */
	void endMovement() override;

	/**
	 * @brief When movement has been cancelled, remove the movement instance.
	 */
	void cancelMovement() override;

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
	WFMath::Polygon<2> getShape();

protected:

	/**
	 * @brief A button used for toggling the display of the polygon on and off.
	 */
	CEGUI::PushButton* mShowButton;

	/**
	 * @brief The polygon used for graphical representation of this area.
	 * Owned by this instance.
	 */
	std::unique_ptr<Authoring::Polygon> mPolygon;

	/**
	 * @brief Responsible for listening for pick events and initiating movements of the points.
	 */
	std::unique_ptr<Authoring::PolygonPointPickListener> mPickListener;

	/**
	 * @brief Responsible for handling the movement of a specific point.
	 */
	std::unique_ptr<Authoring::PolygonPointMovement> mPointMovement;

	/**
	 * @brief An optional entity to which the area belongs.
	 * Mainly used for height lookups, so that the polygon snaps to the ground.
	 */
	EmberEntity* mEntity;

	/**
	 * @brief A position provider, for the polygon.
	 * If an EmberEntity instance is present this will be created (and owned).
	 */
	std::unique_ptr<EntityPolygonPositionProvider> mPositionProvider;

	Ogre::SceneNode* mEntityNode;

	bool showButton_Clicked(const CEGUI::EventArgs& e);

	/**
	 * @brief Listen for points being picked and initiate movement of the point.
	 * @param point The point that was picked.
	 */
	void pickListener_PickedPoint(Authoring::PolygonPoint& point);

	void fillElementFromGui() override;

	bool _hasChanges() override;

	/**
	 * @brief Tries to get the scene node the entity is attached to.
	 * Not all entities are attached to scene nodes, so this might very well also return null.
	 * @return The scene node to which the entity is attached, or null if it's not attached to any scene node.
	 */
	Ogre::SceneNode* getEntitySceneNode();

	void entityMoved();

};

}

}

}

}

}

#endif
