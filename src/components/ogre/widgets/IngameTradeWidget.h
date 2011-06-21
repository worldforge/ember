//
// C++ Interface: IngameTradeWidget
//
// Description:
//
//
// Author: Martin Preisler <preisler.m@gmail.com>, (C) 2011
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
#ifndef EMBEROGREINTEGRATEDTRADEWIDGET_H
#define EMBEROGREINTEGRATEDTRADEWIDGET_H

#include "Widget.h"
#include "WidgetPool.h"
#include "services/config/ConfigListenerContainer.h"
#include <Eris/Entity.h>

namespace Ember {

class CommandHistory;

namespace OgreView {

namespace Camera
{
class MainCamera;
}

namespace Model {
class ModelRepresentation;
}
class GUIManager;
class Widget;
class EmberEntity;
class World;

namespace Gui {



/**
 * @brief Allows comfortable trading without much typing
 * 
 * The trading itself is done via natural language parsing (the interlinguish system), however typing to buy gear would be too
 * annoying and error-prone so Ember offers UI around that that "talks for you".
 */
class IngameTradeWidget : public Widget, public ConfigListenerContainer
{
public:
	/**
	 * @brief Responsible for displaying chat messages and chat related interaction
	 */
	class TradeWindow : public virtual sigc::trackable
	{
		public:
			/**
			 * @brief Ctor
			 * 
			 * @param prefix string prefix of the CEGUI windows for this chat text
			 */
			TradeWindow(const std::string& prefix);
			
			/**
			 * @brief Dtor
			 */ 
			virtual ~TradeWindow();
			
			/**
			 * @brief Sets the entity we want to trade without
			 * 
			 * The trading always takes place between current player and this target entity
			 * (The UI wouldn't make any sense between 2 NPCs or other players of course)
			 * 
			 * @param entity pointer to the target entity (can be 0 if we are stopping the trade)
			 */
			void setTargetEntity(EmberEntity* entity);

		protected:
			/// root CEGUI window of the layout
			CEGUI::Window* mWindow;
			/// prefix of all CEGUI windows
			std::string mPrefix;
	};

	class TradeWindowCreator : public WidgetPool<IngameTradeWidget::TradeWindow>::WidgetCreator
	{
		public:
			TradeWindowCreator(IngameTradeWidget& ingameTradeWidget);
			virtual ~TradeWindowCreator();
			
			virtual IngameTradeWidget::TradeWindow* createWidget(unsigned int currentPoolSize);
			
		protected:
			IngameTradeWidget& mIngameTradeWidget;
			CEGUI::Window* mLayout;
	};

public:
	IngameTradeWidget();
    virtual ~IngameTradeWidget();
	
	void buildWidget();
	
	virtual void frameStarted(const Ogre::FrameEvent & event);

	WidgetPool<TradeWindow>& getTradeWindowPool();

protected:
// 	/**
// 	 * @brief Listen to the Eris::View::EntitySeen event.
// 	 * @param entity The seen entity.
// 	 */
// 	void View_EntitySeen(Eris::Entity* entity);
// 
// 	/**
// 	 * @brief Listen to the Eris::View::EntityCreated event.
// 	 * @param entity The created entity.
// 	 */
// 	void View_EntityCreated(Eris::Entity* entity);
// 
	/**
	 * @brief Listen to the world created event, and attach listeners.
	 * @param world The new world.
	 */
	void EmberOgre_WorldCreated(World& world);
	
	void GUIManager_EventEntityAction(const std::string& action, EmberEntity* entity);
// 
// 	/**
// 	 * @brief Called whenever an entity has arrived from the server.
// 	 *
// 	 * Either through being seen or being created. This will inspect the entity and set up any observers if needed.
// 	 * @param entity A newly created or seen entity.
// 	 */
// 	void entityArrivedFromServer(EmberEntity& entity);

	TradeWindowCreator mTradeWindowCreator;
	WidgetPool<TradeWindow> mTradeWindowPool;
};

}
}

}

#endif
