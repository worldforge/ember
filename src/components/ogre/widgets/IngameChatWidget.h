//
// C++ Interface: IntegratedChatWidget
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef EMBEROGREINTEGRATEDCHATWIDGET_H
#define EMBEROGREINTEGRATEDCHATWIDGET_H

#include "Widget.h"
#include "WidgetPool.h"
#include "services/config/ConfigListenerContainer.h"
#include <Eris/Entity.h>
#include <OgreMovableObject.h>
#include <unordered_map>

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
 * @brief Shows chat bubbles over npc's heads when they say something.
 * 
 * The bubbles will disappear after a while (actually fade out) or when the player moves away. This can be set in the config file.
 * 
 * If the npc has a list of suggested responses these will be shown in a list of clickable buttons to the right.
 * 
 * Also allows switching the chat modes between attached to the entity (the text is projected according to the entity's 3D position
 * and detached mode where the chat window is detached and you can move it around (in this mode chat history and further options are shown)
 * 
 * @author Erik Hjortsberg
 */
class IngameChatWidget : public Widget, public ConfigListenerContainer
{
	class EntityObserver;
	class Label;

	class MovableObjectListener : public Ogre::MovableObject::Listener
	{
		public:
			MovableObjectListener(EntityObserver& entityObserver, Model::ModelRepresentation& modelRepresentation);
			virtual ~MovableObjectListener();

			virtual bool objectRendering (const Ogre::MovableObject * movableObject, const Ogre::Camera * camera);
			void setObserving(bool isObserving);

		private:
			EntityObserver& mEntityObserver;
			Model::ModelRepresentation& mModelRepresentation;
			bool mIsObserving;
	};

	class EntityObserver : public virtual sigc::trackable
	{
		public:
			EntityObserver(IngameChatWidget& chatWidget, Model::ModelRepresentation& modelRepresentation);
			virtual ~EntityObserver();
			void updateLabel(const Ogre::Camera * camera);

			/**
			 * @brief Gets the id of the observed entity.
			 * @return The id of the observed entity.
			 */
			const std::string& getEntityId() const;

			/**
			 * @brief Shows the detached chat widget for the entity.
			 */
			void showDetachedChat();

		protected:
			IngameChatWidget& mChatWidget;
			Model::ModelRepresentation& mModelRepresentation;
			Label* mLabel;
			MovableObjectListener mMovableObjectListener;
			Eris::Entity::AttrChangedSlot mExternalSlot; //, mNameSlot;

			void showLabel();
			void hideLabel();

			void entity_VisibilityChanged(bool visible);
			void entity_BeingDeleted();
			void entity_Say(const Atlas::Objects::Root& talk);
			void entity_attributeChanged(const Atlas::Message::Element& attributeValue);


	};

	class ChatText;
	
	/**
	 * @brief Holds the actual chat window and keeps track of fading, catching clicks etc.
	 * 
	 * The chat text is contained in the mChatText member variable.
	 */
	class Label : public virtual sigc::trackable
	{
		public:
			/**
			 * @brief Ctor
			 */
			Label(CEGUI::Window* window, CEGUI::WindowManager* windowManager, IngameChatWidget& containerWidget, const std::string& prefix);

			/**
			 * @brief Dtor
			 */
			virtual ~Label();

			/**
			 * @brief Called when Entity says a new message
			 * 
			 * @param line the new message (this will show up next to the entity and in chat history)
			 */
			void updateText(const std::string& line);

			/**
			 * @brief Gets the entity the window belongs to
			 */
			EmberEntity* getEntity();

			void setModelRepresentation(Model::ModelRepresentation* modelRepresentation);

			void setActive(bool active);
			bool getActive() const;
			void markForRender();

			CEGUI::Window* getWindow();
			
			/**
			 * @brief Called each frame to update the window
			 * 
			 * This mostly takes care of fading and wiping the history if the entity hasn't seen any
			 * activity for prolonged periods of time (this is configurable)
			 */
			void frameStarted( const Ogre::FrameEvent & event );

			/**
			 * @brief Positions the window on top of the entity
			 * 
			 * Does 3D -> 2D projection and places the 2D CEGUI window so that it appears to be on top
			 * of the entity.
			 * 
			 * @see IngameChatWidget::Label::getEntity()
			 */
			void placeWindowOnEntity();

			IngameChatWidget& getIngameChatWidget() { return mContainerWidget;}

			void updateEntityName();

			/**
			 * @brief Removes the attached chat text widget (i.e. the widget which shows what an entity says, and any optional response buttons).
			 */
			void removeChatText();

			/**
			 * @brief Shows the detached chat text widget.
			 */
			void showDetachedChatText();

			/**
			 * @brief Resets the label.
			 *
			 * This involves removing any chat texts.
			 */
			void reset();

		protected:
			CEGUI::Window* mWindow;
			Model::ModelRepresentation* mModelRepresentation;
			CEGUI::WindowManager* mWindowManager;
			IngameChatWidget& mContainerWidget;

			bool mActive;
			/// CEGUI window name prefix
			const std::string mPrefix;
			bool mRenderNextFrame;
			ChatText* mChatText;

			/**
			 * @brief Gets the attached chat text instance, or create a new one if needed.
			 * @return A chat text instance.
			 */
			ChatText* getOrCreateChatText();
	};

	class LabelCreator : public WidgetPool<IngameChatWidget::Label>::WidgetCreator
	{
		public:
			LabelCreator(IngameChatWidget& ingameChatWidget);
			virtual ~LabelCreator();
			virtual IngameChatWidget::Label* createWidget(unsigned int currentPoolSize);
			
		protected:
			IngameChatWidget& mIngameChatWidget;
			CEGUI::Window* mLayout;
	};

	/**
	 * @brief Responsible for displaying chat messages and chat related interaction
	 */
	class ChatText : public virtual sigc::trackable
	{
		public:
			/**
			 * @brief Ctor
			 * 
			 */
			ChatText(CEGUI::Window* attachedWindow, CEGUI::Window* detachedWindow);
			
			/**
			 * @brief Dtor
			 */ 
			virtual ~ChatText();

			/**
			 * @brief Called when the entity says something new
			 * 
			 * This adds given message to chat history and displays it next to the entity (if in attached mode)
			 */
			void updateText(const std::string& line);

			/**
			 * @brief Called each frame to update the window
			 */
			bool frameStarted( const Ogre::FrameEvent & event );

			float getElapsedTimeSinceLastUpdate();

			/**
			 * @brief Increases the elapsed time with the supplied amount
			 */
			void increaseElapsedTime(float timeSlice);

			void attachToLabel(Label* label);

			/**
			 * @brief switches from detached to attached mode
			 */
			void switchToAttachedMode(bool updateHelpMessage = true);

			/**
			 * @brief switches from attached to detached mode
			 */
			void switchToDetachedMode();

		protected:
			std::vector<CEGUI::Window*> mResponseTextWidgets;
			
			Label* mLabel;
			
			CEGUI::Window* mAttachedWindow;
			CEGUI::Window* mAttachedTextWidget;
			CEGUI::Window* mAttachedResponseContainer;
			CEGUI::Window* mAttachedEllipsisButton;
			
			CEGUI::Window* mDetachedWindow;
			CEGUI::Window* mDetachedChatHistory;
			CEGUI::Window* mDetachedResponseContainer;
			CEGUI::Window* mDetachedEditbox;
			CEGUI::Window* mDetachedTradeButton;
			
			CEGUI::Window* mResponseWidget;
			
			CommandHistory* mCommandHistory;
			
			float mElapsedTimeSinceLastUpdate;
			
			/**
			 * @brief respond to the entity with given message
			 * 
			 * This method clears all suggested responses and adds this message to the chat history
			 */
			void respondWithMessage(const std::string& message);

			bool buttonResponse_Click(const CEGUI::EventArgs& args);
			bool buttonAttachedText_Click(const CEGUI::EventArgs& args);
			bool buttonDetachedClose_Click(const CEGUI::EventArgs& args);
			bool editboxDetachedKey_Event(const CEGUI::EventArgs& args);
			bool buttonDetachedTrade_Click(const CEGUI::EventArgs& args);
			bool detachedSized_Event(const CEGUI::EventArgs& args);
			
			/**
			 * @brief Removes all response buttons.
			 */
			void clearResponses();
	};

	class ChatTextCreator : public WidgetPool<IngameChatWidget::ChatText>::WidgetCreator
	{
		public:
			ChatTextCreator(IngameChatWidget& ingameChatWidget);
			virtual ~ChatTextCreator();
			virtual IngameChatWidget::ChatText* createWidget(unsigned int currentPoolSize);
			
		protected:
			IngameChatWidget& mIngameChatWidget;
			CEGUI::Window* mAttachedLayout;
			CEGUI::Window* mDetachedLayout;
	};

typedef std::unordered_map<std::string, Label*> LabelMap;
typedef std::vector<Label*> LabelStore;
typedef std::stack<Label*> LabelStack;
typedef std::unordered_map<std::string, EntityObserver*> EntityObserverStore;
typedef std::vector<Eris::TypeInfo*> TypeInfoStore;
friend class IngameChatWidget::EntityObserver;

public:
	IngameChatWidget();
    virtual ~IngameChatWidget();
	void buildWidget();
	virtual void frameStarted(const Ogre::FrameEvent & event);

	void removeWidget(const std::string& windowName);

	void removeEntityObserver(EntityObserver* observer);

	WidgetPool<Label>& getLabelPool();
	WidgetPool<ChatText>& getChatTextPool();

	float getTimeShown();

	CEGUI::Window* getLabelSheet();

protected:

	/**
	 * @brief Listen to the Eris::View::EntitySeen event.
	 * @param entity The seen entity.
	 */
	void View_EntitySeen(Eris::Entity* entity);

	/**
	 * @brief Listen to the Eris::View::EntityCreated event.
	 * @param entity The created entity.
	 */
	void View_EntityCreated(Eris::Entity* entity);

	/**
	 * @brief Listen to the world created event, and attach listeners.
	 * @param world The new world.
	 */
	void EmberOgre_WorldCreated(World& world);

	void Config_TimeShown(const std::string& section, const std::string& key, varconf::Variable& variable);
	void Config_DistanceShown(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Called whenever an entity has arrived from the server.
	 *
	 * Either through being seen or being created. This will inspect the entity and set up any observers if needed.
	 * @param entity A newly created or seen entity.
	 */
	void entityArrivedFromServer(EmberEntity& entity);

	/**
	 * @brief Listen to any "talk" entity action and respond by bringing up the detached chat widget for the entity.
	 * @param action The action.
	 * @param entity The entity it was performed on.
	 */
	void GUIManager_EntityAction(const std::string& action, EmberEntity* entity);

	EntityObserverStore mEntityObservers;

	TypeInfoStore mLabelTypes;

	/**
	 * @brief The length in seconds a window should be shown after it has been activated.
	 */
	float mTimeShown;

	/**
	 * @brief How far away, in meters, the window should be visible.
	 */
	float mDistanceShown;

	LabelCreator mLabelCreator;
	WidgetPool<Label> mLabelPool;

	ChatTextCreator mChatTextCreator;
	WidgetPool<ChatText> mChatTextPool;

	/// this sheet contains all labels of the entities
	CEGUI::Window* mLabelSheet;

	std::string mAvatarEntityId;

	/// camera used for 3D -> 2D projection (placing labels and chat texts on top of Entities)
	Camera::MainCamera* mCamera;
};

inline float IngameChatWidget::ChatText::getElapsedTimeSinceLastUpdate() {
	return mElapsedTimeSinceLastUpdate;
}

inline float IngameChatWidget::getTimeShown() {
	return mTimeShown;
}


}
}

}

#endif
