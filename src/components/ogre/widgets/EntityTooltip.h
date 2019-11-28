/*
 Copyright (C) 2010  Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EMBEROGRE_GUI_ENTITYTOOLTIP_H_
#define EMBEROGRE_GUI_ENTITYTOOLTIP_H_

#include <string>
#include <CEGUI/widgets/Tooltip.h>
#include "CEGUIUtils.h"

namespace CEGUI {
class Tooltip;

class EventArgs;

class Window;
}

namespace Ember {
class EmberEntity;
namespace OgreView {
class World;
namespace Gui {

namespace Icons {
class IconManager;
}

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A Tooltip subclass which allows for an alternative way to calculate the size of the tooltip.
 *
 * This is used to show entity bound tooltip information.
 *
 */
class EmberEntityTooltipWidget : public CEGUI::Tooltip {

public:
	static const CEGUI::String WidgetTypeName;

	EmberEntityTooltipWidget(const CEGUI::String& type, const CEGUI::String& name);

	CEGUI::Sizef getTextSize_impl() const override;
};

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Allows advanced entity specific tooltips to be used.
 *
 * This class provides mechanism through which a CEGUI::Tooltip can be setup to show information for a certain entity. This is done by setting the tooltip text of the window for which you want this functionality to the entity id, and then tell the window to use the specific EmberEntityTooltipWidget instance wrapped by this class.
 * This will set everything up so that when the tooltip is shown, the entity id will be used to fetch the correct entity from the World, and information about it will be shown.
 */
class EntityTooltip {
public:

	/**
	 * @brief Ctor.
	 * @param world The world, from which entities will be extracted.
	 * @param tooltip A tooltip instance. Ownership will not be passed. This window will be altered.
	 * @param iconManager The icon manager, from which icons for the entities will be extracted.
	 */
	EntityTooltip(World& world, UniqueWindowPtr<EmberEntityTooltipWidget> tooltip, Icons::IconManager& iconManager);

	/**
	 * @brief Dtor.
	 */
	virtual ~EntityTooltip();

	/**
	 * @brief Accessor for the tooltip window.
	 *
	 * @returns The tooltip window.
	 */
	CEGUI::Tooltip& getTooltipWindow() const;

	/**
	 * @brief Registers the EmberEntityTooltipWidget widget factory, which allows widgets of type "Ember/EntityTooltip" to be created.
	 * @note Call this as soon as you can, after CEGUI has been initialized.
	 */
	static void registerFactory();
	static void deregisterFactory();

protected:

	/**
	 * @brief The world, used for querying for entities.
	 */
	World& mWorld;

	/**
	 * @brief The tooltip widget instance, not owned by this class.
	 */
	UniqueWindowPtr<EmberEntityTooltipWidget> mTooltip;

	/**
	 * @brief The icon manager, from which we'll extract entity icons.
	 */
	Icons::IconManager& mIconManager;

	/**
	 * @brief A guard to prevent infinite loops when both listening to the TextChanged event of the tooltip and in that method also updating the text itself.
	 */
	bool isUpdatingOurselves;

	/**
	 * @brief A window containing an image of the entity.
	 */
	CEGUI::Window* mImageWindow;

	/**
	 * @brief A window containing text about the entity.
	 */
	CEGUI::Window* mTextWindow;

	/**
	 * @brief Catch the text being changed for the tooltip, as this indicates that the target window has changed.
	 *
	 * Preferably we would like to listen to a signal emitted when the target window changes, but alas there is no such signal.
	 * @param e Events args.
	 */
	bool tooltip_TextChanged(const CEGUI::EventArgs& e);

	/**
	 * @brief Tries to extract the active entity of the tooltip target window.
	 *
	 * This is done by using the text of the tooltip as the entity key, and querying the World instance for it.
	 * If no entity could be found (it might have been deleted) null will be returned.
	 *
	 * @returns The active entity, or null if no entity could be found.
	 */
	EmberEntity* getActiveEntity();

	/**
	 * @brief Composes an informative text about the entity.
	 *
	 * @param The entity for which we want a descriptive text.
	 * @returns A descriptive text of the entity.
	 */
	std::string composeEntityInfoText(EmberEntity& entity);

};

}
}

}

#endif /* ENTITYTOOLTIP_H_ */
