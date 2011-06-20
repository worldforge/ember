//
// C++ Implementation: IngameChatWidget
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "IngameChatWidget.h"

#include "../GUIManager.h"
#include "../EmberEntity.h"

#include "../EmberOgre.h"
#include "../World.h"
#include "../Avatar.h"

#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/camera/MainCamera.h"
#include "framework/CommandHistory.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"

#include <CEGUIWindowManager.h>
#include <CEGUIExceptions.h>
#include <CEGUIFont.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIGUISheet.h>
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUILayoutContainer.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>
#include <Eris/TypeService.h>
#include <Eris/Connection.h>
#include <Eris/Avatar.h>

#include <sigc++/slot.h>

#include <OgreCamera.h>
#include <OgreNode.h>

using namespace CEGUI;
namespace Ember
{
namespace OgreView
{
namespace Gui
{

IngameChatWidget::IngameChatWidget() :
	mTimeShown(0), mDistanceShown(100), mLabelCreator(*this), mLabelPool(mLabelCreator), mChatTextCreator(*this), mChatTextPool(mChatTextCreator), mAvatarEntityId(""), mCamera(0)
{

	registerConfigListener("ingamechatwidget", "timeshown", sigc::mem_fun(*this, &IngameChatWidget::Config_TimeShown));
	registerConfigListener("ingamechatwidget", "distanceshown", sigc::mem_fun(*this, &IngameChatWidget::Config_DistanceShown));

}

IngameChatWidget::~IngameChatWidget()
{
}

void IngameChatWidget::buildWidget()
{

	mLabelSheet = mWindowManager->createWindow("DefaultGUISheet", "IngameChatWidget/LabelSheet");
	mLabelSheet->setMousePassThroughEnabled(true);
	mLabelSheet->setRiseOnClickEnabled(false);
	getMainSheet()->addChildWindow(mLabelSheet);

	mLabelPool.initializePool(15);
	mChatTextPool.initializePool(5);
	EmberOgre::getSingleton().EventWorldCreated.connect(sigc::mem_fun(*this, &IngameChatWidget::EmberOgre_WorldCreated));

}

void IngameChatWidget::Config_TimeShown(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	mTimeShown = static_cast<double> (variable);
}

void IngameChatWidget::Config_DistanceShown(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	mDistanceShown = static_cast<double> (variable);
}

Window* IngameChatWidget::getLabelSheet()
{
	return mLabelSheet;
}

void IngameChatWidget::EmberOgre_WorldCreated(World& world)
{
	mCamera = &world.getMainCamera();
	Eris::TypeService* typeService = world.getView().getAvatar()->getConnection()->getTypeService();
	mLabelTypes.push_back(typeService->getTypeByName("character"));

	world.getView().EntitySeen.connect(sigc::mem_fun(*this, &IngameChatWidget::View_EntitySeen));
	world.getView().EntityCreated.connect(sigc::mem_fun(*this, &IngameChatWidget::View_EntityCreated));

	mAvatarEntityId = world.getView().getAvatar()->getId();
}

void IngameChatWidget::View_EntitySeen(Eris::Entity* entity)
{
	entityArrivedFromServer(static_cast<EmberEntity&> (*entity));
}

void IngameChatWidget::View_EntityCreated(Eris::Entity* entity)
{
	entityArrivedFromServer(static_cast<EmberEntity&> (*entity));
}

void IngameChatWidget::entityArrivedFromServer(EmberEntity& entity)
{
	Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity);
	if (modelRepresentation) {
		for (TypeInfoStore::iterator I = mLabelTypes.begin(); I != mLabelTypes.end(); ++I) {
			if (entity.getType()->isA(*I)) {
				if (mAvatarEntityId != entity.getId()) {
					EntityObserver* observer = new EntityObserver(*this, *modelRepresentation);
					mEntityObservers.push_back(observer);
				}
				break;
			}
		}
	}
}


void IngameChatWidget::removeEntityObserver(EntityObserver* observer)
{
	EntityObserverStore::iterator I = std::find(mEntityObservers.begin(), mEntityObservers.end(), observer);
	if (I != mEntityObservers.end()) {
		mEntityObservers.erase(I);
	}
	delete observer;
}

WidgetPool<IngameChatWidget::Label>& IngameChatWidget::getLabelPool()
{
	return mLabelPool;
}
WidgetPool<IngameChatWidget::ChatText>& IngameChatWidget::getChatTextPool()
{
	return mChatTextPool;
}

//---------------------------------------------------


void IngameChatWidget::frameStarted(const Ogre::FrameEvent & event)
{
	for (WidgetPool<Label>::WidgetStore::iterator I = mLabelPool.getUsedWidgets().begin(); I != mLabelPool.getUsedWidgets().end(); ++I) {
		(*I)->frameStarted(event);
	}

}

void IngameChatWidget::removeWidget(const std::string& windowName)
{
}

IngameChatWidget::MovableObjectListener::MovableObjectListener(EntityObserver& entityObserver, Model::ModelRepresentation& modelRepresentation) :
	mEntityObserver(entityObserver), mModelRepresentation(modelRepresentation), mIsObserving(false)
{
}

IngameChatWidget::MovableObjectListener::~MovableObjectListener()
{
	setObserving(false);
}

void IngameChatWidget::MovableObjectListener::setObserving(bool isObserving)
{
	if (isObserving) {
		//TODO: make sure that this doesn't interfere with other listeners
		mModelRepresentation.getModel().setListener(this);
	} else {
		mModelRepresentation.getModel().setListener(0);
	}
}

bool IngameChatWidget::MovableObjectListener::objectRendering(const Ogre::MovableObject * movableObject, const Ogre::Camera * camera)
{
	mEntityObserver.updateLabel(camera);
	return true;
}

IngameChatWidget::EntityObserver::EntityObserver(IngameChatWidget& chatWidget, Model::ModelRepresentation& modelRepresentation) :
	mChatWidget(chatWidget), mModelRepresentation(modelRepresentation), mLabel(0), mMovableObjectListener(*this, modelRepresentation)
{
	EmberEntity& entity(modelRepresentation.getEntity());
	entity.VisibilityChanged.connect(sigc::mem_fun(*this, &EntityObserver::entity_VisibilityChanged));
	entity.BeingDeleted.connect(sigc::mem_fun(*this, &EntityObserver::entity_BeingDeleted));
	entity.Say.connect(sigc::mem_fun(*this, &EntityObserver::entity_Say));

	mExternalSlot = sigc::mem_fun(*this, &IngameChatWidget::EntityObserver::entity_attributeChanged);
	entity.observe("external", mExternalSlot);
	entity.observe("name", mExternalSlot);
	entity_VisibilityChanged(entity.isVisible());
}

IngameChatWidget::EntityObserver::~EntityObserver()
{
	hideLabel();
}

void IngameChatWidget::EntityObserver::entity_VisibilityChanged(bool visible)
{
	if (visible) {
		showLabel();
	} else {
		hideLabel();
	}
}

void IngameChatWidget::EntityObserver::entity_BeingDeleted()
{
	mChatWidget.removeEntityObserver(this);
}

void IngameChatWidget::EntityObserver::entity_Say(const Atlas::Objects::Root& talk)
{
	if (mLabel) {

		if (!talk->hasAttr("say")) {
			return;
		}
		const std::string& msg = talk->getAttr("say").asString();

		mLabel->updateText(msg);
	}

}

void IngameChatWidget::EntityObserver::entity_attributeChanged(const Atlas::Message::Element& attributeValue)
{
	if (mLabel) {
		mLabel->updateEntityName();
	}
}

void IngameChatWidget::EntityObserver::updateLabel(const Ogre::Camera * camera)
{
	//only update when being rendered by the main camera
	if (camera == &mChatWidget.mCamera->getCamera()) {
		// 	const Ogre::Vector3& entityWorldCoords = mEntity->getDerivedPosition();
		//	Ogre::Vector3 entityWorldCoords = mEntity->getWorldBoundingBox(true).getCenter();
		//	Ogre::Vector3 entityWorldCoords = window->getEntity()->getSceneNode()->_getWorldAABB().getCenter();
		// 	const Ogre::Vector3& cameraCoords = camera->getDerivedPosition();
		//getWorldPosition is faster than getting the center of the boundingbox...
		Ogre::Vector3 diff = mModelRepresentation.getModel().getParentNode()->_getDerivedPosition() - camera->getDerivedPosition();

		//remove the window if it's either too far away
		if (diff.length() > mChatWidget.mDistanceShown) {
			// 		mLabel->setActive(false);
		} else {
			mLabel->markForRender();
			mLabel->placeWindowOnEntity();
			/*		mLabel->setActive(true);
			 mLabel->placeWindowOnEntity();*/
		}
	}

}

void IngameChatWidget::EntityObserver::showLabel()
{
	if (!mLabel) {
		mLabel = mChatWidget.getLabelPool().checkoutWidget();
		mLabel->setModelRepresentation(&mModelRepresentation);
	}
	mMovableObjectListener.setObserving(true);
}
void IngameChatWidget::EntityObserver::hideLabel()
{
	if (mLabel) {
		mLabel->setActive(false);
		mChatWidget.getLabelPool().returnWidget(mLabel);
		mLabel = 0;
	}
	mMovableObjectListener.setObserving(false);
}

IngameChatWidget::Label::Label(Window * window, WindowManager * windowManager, IngameChatWidget& containerWidget, const std::string& prefix) :
	mWindow(window), mModelRepresentation(0), mWindowManager(windowManager), mContainerWidget(containerWidget), mActive(false), mPrefix(prefix), mRenderNextFrame(false), mChatText(0)
{
	// 	mNameWidget = mWindow->getChild(mPrefix + "EntityName");
}

IngameChatWidget::Label::~Label()
{
	mWindowManager->destroyWindow(mWindow);
}

void IngameChatWidget::Label::markForRender()
{
	mRenderNextFrame = true;
	setActive(true);
}

void IngameChatWidget::Label::placeWindowOnEntity()
{
	//make sure that the window stays on the entity
	Ogre::Vector2 screenCoords;

	bool result = false;
	Ogre::Vector3 entityWorldCoords = mModelRepresentation->getWorldBoundingSphere().getCenter();
	entityWorldCoords.y = mModelRepresentation->getWorldBoundingBox().getMaximum().y;
	//check what the new position is in screen coords
	result = mContainerWidget.mCamera->worldToScreen(entityWorldCoords, screenCoords);

	if (result) {
		mWindow->setVisible(true);
		mWindow->setPosition(UVector2(UDim(screenCoords.x, -(mWindow->getWidth().asAbsolute(0) * 0.5)), UDim(screenCoords.y, -(mWindow->getHeight().asAbsolute(0) * 0.5))));
	} else {
		mWindow->setVisible(false);
	}
}

void IngameChatWidget::Label::frameStarted(const Ogre::FrameEvent & event)
{
	if (mRenderNextFrame) {
		mRenderNextFrame = false;
		if (mChatText) {
			bool keep = mChatText->frameStarted(event);
			if (!keep) {
				removeChatText();
			}
		}
		// 		placeWindowOnEntity();

		// 		increaseElapsedTime(event.timeSinceLastFrame);
	} else {
		setActive(false);
	}
}

void IngameChatWidget::Label::removeChatText()
{
	if (mChatText) {
		mChatText->attachToLabel(0);
		mContainerWidget.getChatTextPool().returnWidget(mChatText);
		mChatText = 0;
	}
}


void IngameChatWidget::Label::setModelRepresentation(Model::ModelRepresentation* modelRepresentation)
{
	mModelRepresentation = modelRepresentation;
	reset();
	try {
		updateEntityName();
	} catch (const Exception& ex) {
		S_LOG_FAILURE("Could not get widget EntityName." << ex);
	}
}

void IngameChatWidget::Label::reset()
{
	removeChatText();
}

void IngameChatWidget::Label::updateEntityName()
{
	std::string entityName(getEntity()->getName());
	
	//if the entity is controlled by a player, mark that
	if (getEntity()->hasAttr("external")) {
		const Atlas::Message::Element& externalAttr = getEntity()->valueOfAttr("external");
		if (externalAttr.isNum() && externalAttr.asNum() == 1) {
			entityName = "!" + getEntity()->getName() + "!";
		}
	}
	// mWindow is the label itself (a StaticText)
	mWindow->setText(entityName);
}

EmberEntity * IngameChatWidget::Label::getEntity()
{
	return &mModelRepresentation->getEntity();
}

void IngameChatWidget::Label::setActive(bool active)
{
	if (mActive == active) {
		return;
	}
	
	if (active) {
		mContainerWidget.getLabelSheet()->addChildWindow(mWindow);
	} else {
		mContainerWidget.getLabelSheet()->removeChildWindow(mWindow);
	}
	
	mActive = active;
}

bool IngameChatWidget::Label::getActive() const
{
	return mActive;
}

Window * IngameChatWidget::Label::getWindow()
{
	return mWindow;
}

void IngameChatWidget::Label::updateText(const std::string & line)
{
	if (!mChatText) {
		mChatText = mContainerWidget.getChatTextPool().checkoutWidget();
		mChatText->attachToLabel(this);
	}
	mChatText->updateText(line);
	//mWindow->moveToFront();

}

IngameChatWidget::LabelCreator::LabelCreator(IngameChatWidget& ingameChatWidget) :
	mIngameChatWidget(ingameChatWidget),
	mLayout(WindowManager::getSingleton().loadWindowLayout(GUIManager::getSingleton().getLayoutDir() + "Label.layout"))
{
}

IngameChatWidget::LabelCreator::~LabelCreator()
{
	WindowManager::getSingleton().destroyWindow(mLayout);
}

IngameChatWidget::Label* IngameChatWidget::LabelCreator::createWidget(unsigned int currentPoolSize)
{
	//there is no chat window for this entity, let's create one from the loaded layout (by cloning)
	std::stringstream ss;
	ss << "Label/" << currentPoolSize << "/";
	//clones the entire layout hierarchy (even though it's just one window for now)
	Window* window = mLayout->clone(ss.str() + "EntityName", true);
	//We don't want the labels to prevent the user from picking entities in the world.
	window->setMousePassThroughEnabled(true);
	window->setRiseOnClickEnabled(false);

	Label* label = new Label(window, CEGUI::WindowManager::getSingletonPtr(), mIngameChatWidget, ss.str());
	return label;
}

IngameChatWidget::ChatText::ChatText(const std::string& prefix) :
	mLabel(0),
	mAttachedWindow(WindowManager::getSingleton().getWindow(prefix + "MainWindow/Attached")),
	mAttachedTextWidget(WindowManager::getSingleton().getWindow(prefix + "MainWindow/Attached/Text")),
	mAttachedResponseContainer(WindowManager::getSingleton().getWindow(prefix + "MainWindow/Attached/ResponseContainer")),

	mDetachedWindow(WindowManager::getSingleton().getWindow(prefix + "MainWindow/Detached")),
	mDetachedChatHistory(WindowManager::getSingleton().getWindow(prefix + "MainWindow/Detached/ChatHistory")),
	mDetachedResponseContainer(WindowManager::getSingleton().getWindow(prefix + "MainWindow/Detached/ResponseContainer")),
	mDetachedEditbox(WindowManager::getSingleton().getWindow(prefix + "MainWindow/Detached/Editbox")),

	mResponseWidget(WindowManager::getSingleton().getWindow(prefix + "MainWindow/Attached/ResponseContainer/ResponseList")),
	
	mCommandHistory(new CommandHistory()),
	
	mElapsedTimeSinceLastUpdate(0.0f),
	mPrefix(prefix)
{
	mDetachedWindow->setVisible(false);
	GUIManager::getSingleton().getMainSheet()->addChildWindow(mDetachedWindow);
	
	BIND_CEGUI_EVENT(mAttachedTextWidget, PushButton::EventClicked, IngameChatWidget::ChatText::buttonAttachedText_Click );
	BIND_CEGUI_EVENT(mDetachedWindow, FrameWindow::EventCloseClicked, IngameChatWidget::ChatText::buttonDetachedClose_Click );
	BIND_CEGUI_EVENT(mDetachedEditbox, Window::EventKeyDown, IngameChatWidget::ChatText::editboxDetachedKey_Event );
}

IngameChatWidget::ChatText::~ChatText()
{
	delete mCommandHistory;
	
	WindowManager::getSingleton().destroyWindow(mAttachedWindow);
	WindowManager::getSingleton().destroyWindow(mDetachedWindow);
}

bool IngameChatWidget::ChatText::frameStarted(const Ogre::FrameEvent & event)
{
	increaseElapsedTime(event.timeSinceLastFrame);
	//unless timeShown is 0 windows will fade over time
	float timeShown = mLabel->getIngameChatWidget().getTimeShown();
	if (timeShown > 0) {
		//make the windows fade over time
		mAttachedWindow->setAlpha(1 - (getElapsedTimeSinceLastUpdate() / timeShown));
		// if detached window is not visible, user is not interested in the history, so we may "nuke" it.
		// note: when history is visible and user closes it, we immediately return false here.
		if (!mDetachedWindow->isVisible() && getElapsedTimeSinceLastUpdate() >= timeShown) {
			return false;
		}
	}
	return true;
}

void IngameChatWidget::ChatText::increaseElapsedTime(float timeSlice)
{
	mElapsedTimeSinceLastUpdate += timeSlice;
}

void IngameChatWidget::ChatText::updateText(const std::string & line)
{
	mAttachedTextWidget->setText(line);
	
	mDetachedChatHistory->setText(mDetachedChatHistory->getText() + "\n[colour='00000000']-\n[colour='FF000000']< " + line);
	mDetachedChatHistory->setProperty("VertScrollPosition", mDetachedChatHistory->getProperty("VertExtent"));
	
	mElapsedTimeSinceLastUpdate = 0;

	if (mLabel->getEntity()->hasSuggestedResponses()) {
		clearResponses();

		//for each response, create a button
		const std::vector<std::string>& responses = mLabel->getEntity()->getSuggestedResponses();

		std::vector<std::string>::const_iterator I = responses.begin();
		std::vector<std::string>::const_iterator I_end = responses.end();
		int i = 0;
		std::stringstream ss;
		
		for (; I != I_end; ++I) {
			std::stringstream ss_;
			ss_ << i;
			PushButton* responseTextButton = static_cast<PushButton*> (WindowManager::getSingleton().createWindow(GUIManager::getSingleton().getDefaultScheme() + "/IngameChatResponseButton", mPrefix + "Response/" + ss_.str()));

			BIND_CEGUI_EVENT(responseTextButton, PushButton::EventClicked, IngameChatWidget::ChatText::buttonResponse_Click );

			responseTextButton->setInheritsAlpha(true);
			responseTextButton->setText(*I);
			responseTextButton->setTooltipText(*I);
			mResponseWidget->addChildWindow(responseTextButton);
			mResponseTextWidgets.push_back(responseTextButton);

			++i;
		}
	}
}

void IngameChatWidget::ChatText::respondWithMessage(const std::string& message)
{
	EmberServices::getSingleton().getServerService().say(message);
	
	mDetachedChatHistory->setText(mDetachedChatHistory->getText() + "\n[colour='00000000']-\n[colour='FF0000FF']> " + message);
	mDetachedChatHistory->setProperty("VertScrollPosition", mDetachedChatHistory->getProperty("VertExtent"));
	
	mCommandHistory->addToHistory(message);
	
	clearResponses();
}

bool IngameChatWidget::ChatText::buttonResponse_Click(const EventArgs& args)
{
	const MouseEventArgs *mouseArgs = static_cast<const MouseEventArgs*> (&args);
	if (mouseArgs) {
		//each button contains a static text window, which is the one containg the actual text
		respondWithMessage(mouseArgs->window->getText().c_str());
	}
	return true;
}

bool IngameChatWidget::ChatText::buttonAttachedText_Click(const EventArgs& args)
{
	const Rect rect = mAttachedWindow->getUnclippedOuterRect();
	mDetachedWindow->setPosition(UVector2(UDim(0, rect.d_left), UDim(0, rect.d_top)));
	mAttachedResponseContainer->removeChildWindow(mResponseWidget);
	mDetachedResponseContainer->addChildWindow(mResponseWidget);
	
	mAttachedWindow->setVisible(false);
	mDetachedWindow->setVisible(true);
	
	// workaround, see http://www.cegui.org.uk/mantis/view.php?id=464
	static_cast<LayoutContainer*>(mResponseWidget)->markNeedsLayouting();
	
	// reset the fade timer
	mElapsedTimeSinceLastUpdate = 0;
	
	return true;
}

bool IngameChatWidget::ChatText::buttonDetachedClose_Click(const EventArgs& args)
{
	mDetachedResponseContainer->removeChildWindow(mResponseWidget);
	mAttachedResponseContainer->addChildWindow(mResponseWidget);
	
	mAttachedWindow->setVisible(true);
	mDetachedWindow->setVisible(false);
	
	// workaround, see http://www.cegui.org.uk/mantis/view.php?id=464
	static_cast<LayoutContainer*>(mResponseWidget)->markNeedsLayouting();
	
	return true;
}

bool IngameChatWidget::ChatText::editboxDetachedKey_Event(const EventArgs& args)
{
	const KeyEventArgs& kargs = static_cast<const KeyEventArgs&>(args);
	
	if (kargs.scancode == Key::Return)
	{
		// do not say empty messages
		if (mDetachedEditbox->getText().length() > 0)
		{
			respondWithMessage(mDetachedEditbox->getText().c_str());
			// clear the editbox (this message has been sent by now)
			mDetachedEditbox->setText("");
		}
		
		// but always eat the return key presses
		return true;
	}
	else if (kargs.scancode == Key::ArrowUp)
	{
		mCommandHistory->moveBackwards();
		mDetachedEditbox->setText(mCommandHistory->getHistoryString());
		
		return true;
	}
	else if (kargs.scancode == Key::ArrowDown)
	{
		mCommandHistory->moveForwards();
		mDetachedEditbox->setText(mCommandHistory->getHistoryString());
		
		return true;
	}
	
	// unless the key was a Enter/Return key, ArrowUp or ArrowDownwe have to tell CEGUI we haven't
	// handled it! otherwise backspace et al stop working
	return false;
}

void IngameChatWidget::ChatText::clearResponses()
{
	//remove all existing response windows
	std::vector<Window*>::const_iterator responses_I = mResponseTextWidgets.begin();
	std::vector<Window*>::const_iterator responses_I_end = mResponseTextWidgets.end();
	for (; responses_I != responses_I_end; ++responses_I) {
		WindowManager::getSingleton().destroyWindow(*responses_I);
	}
	mResponseTextWidgets.clear();
}


void IngameChatWidget::ChatText::attachToLabel(Label* label)
{
	clearResponses();
	
	mAttachedTextWidget->setText("");
	mDetachedChatHistory->setText("");
	
	mLabel = label;
	if (label) {
		mDetachedWindow->setText("Dialog with " + label->getEntity()->getName());
		mLabel->getWindow()->addChildWindow(mAttachedWindow);
	} else {
		if (mAttachedWindow->getParent()) {
			mAttachedWindow->getParent()->removeChildWindow(mAttachedWindow);
		}
	}
}

IngameChatWidget::ChatTextCreator::ChatTextCreator(IngameChatWidget& ingameChatWidget):
	mIngameChatWidget(ingameChatWidget),
	mAttachedLayout(WindowManager::getSingleton().loadWindowLayout(GUIManager::getSingleton().getLayoutDir() + "IngameChatWidgetAttached.layout")),
	mDetachedLayout(WindowManager::getSingleton().loadWindowLayout(GUIManager::getSingleton().getLayoutDir() + "IngameChatWidgetDetached.layout"))
{
}

IngameChatWidget::ChatTextCreator::~ChatTextCreator()
{
	WindowManager::getSingleton().destroyWindow(mAttachedLayout);
	WindowManager::getSingleton().destroyWindow(mDetachedLayout);
}

IngameChatWidget::ChatText* IngameChatWidget::ChatTextCreator::createWidget(unsigned int currentPoolSize)
{
	//there is no chat window for this entity, let's create one by cloning the existing layout
	std::stringstream ss;
	ss << "ChatText/" << currentPoolSize << "/";
	mAttachedLayout->clone(ss.str() + "MainWindow/Attached");
	mDetachedLayout->clone(ss.str() + "MainWindow/Detached");
	
	ChatText* widget = new ChatText(ss.str());
	return widget;
}

}

}

}
