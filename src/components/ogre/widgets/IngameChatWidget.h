//
// C++ Interface: IntegratedChatWidget
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
#ifndef EMBEROGREINTEGRATEDCHATWIDGET_H
#define EMBEROGREINTEGRATEDCHATWIDGET_H

#include "Widget.h"
#include "WidgetPlugin.h"
#include "WidgetPool.h"
#include "services/config/ConfigListenerContainer.h"
#include "CEGUIUtils.h"
#include <Eris/Entity.h>
#include <OgreMovableObject.h>
#include <unordered_map>
#include <functional>
#include <OgreCamera.h>
#include <components/ogre/INodeProvider.h>
#include <boost/noncopyable.hpp>

namespace Ember {

class CommandHistory;

class EmberEntity;

namespace OgreView {

class Avatar;

namespace Camera {
class MainCamera;
}

namespace Model {
class Model;

class ModelRepresentation;
}
class GUIManager;

class Widget;

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
 * @author Erik Ogenvik
 */
class IngameChatWidget
		: public ConfigListenerContainer,
		  public Ogre::Camera::Listener,
		  public virtual sigc::trackable {

	class Label;

	struct EntityObserver : public virtual sigc::trackable {
		EntityObserver(IngameChatWidget& chatWidget, EmberEntity& entity);

		virtual ~EntityObserver();

		void updateLabel(const Ogre::Camera* camera);

		/**
		 * @brief Gets the id of the observed entity.
		 * @return The id of the observed entity.
		 */
		const std::string& getEntityId() const;

		/**
		 * @brief Shows the detached chat widget for the entity.
		 */
		void showDetachedChat();

		IngameChatWidget& mChatWidget;
		EmberEntity& mEntity;
		std::unique_ptr<Label> mLabel;
		Eris::Entity::PropertyChangedSlot mExternalSlot; //, mNameSlot;

		void showLabel();

		void hideLabel();

		void entity_VisibilityChanged(bool visible);

		void entity_BeingDeleted();

		void entity_Say(const Atlas::Objects::Root& talk);

		void entity_attributeChanged(const Atlas::Message::Element& attributeValue);

		void entity_GraphicalRepresentationChanged();


	};

	class ChatText;


	/**
	 * @brief Holds the actual chat window and keeps track of fading, catching clicks etc.
	 * 
	 * The chat text is contained in the mChatText member variable.
	 */
	class Label : public virtual sigc::trackable {
	public:
		/**
		 * @brief Ctor
		 */
		Label(UniqueWindowPtr<CEGUI::Window> window, IngameChatWidget& containerWidget, const std::string& prefix);

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

		void attachToEntity(EmberEntity* entity);

		void setVisible(bool visible);

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
		void frameStarted(float timeSinceLastFrame);

		/**
		 * @brief Positions the window on top of the entity
		 *
		 * Does 3D -> 2D projection and places the 2D CEGUI window so that it appears to be on top
		 * of the entity.
		 *
		 * @see IngameChatWidget::Label::getEntity()
		 */
		void placeWindowOnEntity();

		IngameChatWidget& getIngameChatWidget() { return mContainerWidget; }

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

		void objectRendering(const Ogre::Camera* camera);

	protected:
		UniqueWindowPtr<CEGUI::Window> mWindow;
		//INodeProvider* mNodeProvider;
		EmberEntity* mEntity;
		IngameChatWidget& mContainerWidget;

		/**
		 * @brief If true, the label is actively monitoring an entity.
		 */
		bool mActive;

		/**
		 * @brief If true, the label is visible to the user.
		 */
		bool mVisible;

		/**
		 * @brief If true, the label should be shown on the next frame rendering.
		 */
		bool mRenderNextFrame;
		std::unique_ptr<ChatText> mChatText;

		/**
		 * @brief Gets the attached chat text instance, or create a new one if needed.
		 * @return A chat text instance.
		 */
		ChatText& getOrCreateChatText();
	};

	class LabelCreator : public WidgetPool<IngameChatWidget::Label>::WidgetCreator {
	public:
		explicit LabelCreator(IngameChatWidget& ingameChatWidget);

		~LabelCreator() override;

		std::unique_ptr<IngameChatWidget::Label> createWidget(unsigned int currentPoolSize) override;

	protected:
		IngameChatWidget& mIngameChatWidget;
		UniqueWindowPtr<CEGUI::Window> mLayout;
	};

	/**
	 * @brief Responsible for displaying chat messages and chat related interaction
	 */
	class ChatText : public virtual sigc::trackable {
	public:
		/**
		 * @brief Ctor
		 *
		 */
		ChatText(IngameChatWidget& chatWidget, CEGUI::Window* attachedWindow, CEGUI::Window* detachedWindow);

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
		bool frameStarted(float timeSinceLastFrame);

		float getElapsedTimeSinceLastUpdate() const;

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

		IngameChatWidget& mChatWidget;

		std::vector<UniqueWindowPtr<CEGUI::Window>> mResponseTextWidgets;

		Label* mLabel;

		UniqueWindowPtr<CEGUI::Window> mAttachedWindow;
		CEGUI::Window* mAttachedTextWidget;
		CEGUI::Window* mAttachedResponseContainer;
		CEGUI::Window* mAttachedEllipsisButton;

		UniqueWindowPtr<CEGUI::Window> mDetachedWindow;
		CEGUI::Window* mDetachedChatHistory;
		CEGUI::Window* mDetachedResponseContainer;
		CEGUI::Window* mDetachedEditbox;
		CEGUI::Window* mDetachedTradeButton;

		CEGUI::Window* mResponseWidget;

		std::unique_ptr<CommandHistory> mCommandHistory;

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

	class ChatTextCreator : public WidgetPool<IngameChatWidget::ChatText>::WidgetCreator {
	public:
		explicit ChatTextCreator(IngameChatWidget& ingameChatWidget);

		~ChatTextCreator() override;

		std::unique_ptr<IngameChatWidget::ChatText> createWidget(unsigned int currentPoolSize) override;

	protected:
		IngameChatWidget& mIngameChatWidget;
		UniqueWindowPtr<CEGUI::Window> mAttachedLayout;
		UniqueWindowPtr<CEGUI::Window> mDetachedLayout;
	};

	friend struct IngameChatWidget::EntityObserver;

public:
	static WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager);

	IngameChatWidget(GUIManager& guiManager, Avatar& avatar, Camera::MainCamera& mainCamera);

	~IngameChatWidget() override;

	void frameStarted(float timeSinceLastFrame);

	void removeWidget(const std::string& windowName);

	void removeEntityObserver(EntityObserver* observer);

	WidgetPool<Label>& getLabelPool();

	WidgetPool<ChatText>& getChatTextPool();

	float getTimeShown() const;

	CEGUI::Window* getLabelSheet();

	void cameraPreRenderScene(Ogre::Camera* cam) override;

protected:

	void Config_TimeShown(const std::string& section, const std::string& key, varconf::Variable& variable);

	void Config_DistanceShown(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Called whenever labels should be enabled for an entity.
	 *
	 * @param entity An entity.
	 */
	void enableForEntity(EmberEntity& entity);

	void disableForEntity(EmberEntity& entity);

	/**
	 * @brief Listen to any "talk" entity action and respond by bringing up the detached chat widget for the entity.
	 * @param action The action.
	 * @param entity The entity it was performed on.
	 */
	void GUIManager_EntityAction(const std::string& action, EmberEntity* entity);


	GUIManager& mGuiManager;
	Avatar& mAvatar;
	/// camera used for 3D -> 2D projection (placing labels and chat texts on top of Entities)
	Camera::MainCamera& mCamera;

	/// this sheet contains all labels of the entities
	UniqueWindowPtr<CEGUI::Window> mLabelSheet;

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


	Widget* mWidget;

	std::unordered_map<std::string, std::unique_ptr<EntityObserver>> mEntityObservers;

	std::vector<EntityObserver*> mActiveObservers;

};

inline float IngameChatWidget::ChatText::getElapsedTimeSinceLastUpdate() const {
	return mElapsedTimeSinceLastUpdate;
}

inline float IngameChatWidget::getTimeShown() const {
	return mTimeShown;
}


}
}

}
#ifdef WF_USE_WIDGET_PLUGINS

BOOST_DLL_ALIAS(
		Ember::OgreView::Gui::IngameChatWidget::registerWidget,
		registerWidget
)
#endif
#endif
