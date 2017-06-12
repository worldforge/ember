//
// C++ Implementation: IngameChatWidget
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "IngameChatWidget.h"
#include "QuickHelp.h"

#include "../GUIManager.h"
#include "domain/EmberEntity.h"

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

#include <CEGUI/WindowManager.h>
#include <CEGUI/Exceptions.h>
#include <CEGUI/Font.h>
#include <CEGUI/Window.h>
#include <CEGUI/widgets/Listbox.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/widgets/FrameWindow.h>
#include <CEGUI/widgets/LayoutContainer.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>
#include <Eris/Connection.h>
#include <Eris/Avatar.h>

#include <OgreCamera.h>
#include <OgreNode.h>

using namespace CEGUI;
namespace Ember
{
namespace OgreView
{
namespace Gui
{

std::function<void(EmberEntity&)> IngameChatWidget::sEnableForEntity;
std::function<void(EmberEntity&)> IngameChatWidget::sDisableForEntity;


IngameChatWidget::IngameChatWidget() :
		mTimeShown(0), mDistanceShown(100), mLabelCreator(*this), mLabelPool(mLabelCreator), mChatTextCreator(*this), mChatTextPool(mChatTextCreator), mAvatarEntityId(""), mCamera(0)
{

	registerConfigListener("ingamechatwidget", "timeshown", sigc::mem_fun(*this, &IngameChatWidget::Config_TimeShown));
	registerConfigListener("ingamechatwidget", "distanceshown", sigc::mem_fun(*this, &IngameChatWidget::Config_DistanceShown));
	assert(!sEnableForEntity);
	assert(!sDisableForEntity);
	sEnableForEntity = [&](EmberEntity& entity) {
		this->enableForEntity(entity);
	};
	sDisableForEntity = [&](EmberEntity& entity) {
		this->disableForEntity(entity);
	};
}

IngameChatWidget::~IngameChatWidget()
{
	sEnableForEntity = nullptr;
	sDisableForEntity = nullptr;
}

void IngameChatWidget::buildWidget()
{

	mLabelSheet = mWindowManager->createWindow("DefaultWindow", "IngameChatWidget/LabelSheet");
	mLabelSheet->setMousePassThroughEnabled(true);
	mLabelSheet->setRiseOnClickEnabled(false);
	getMainSheet()->addChild(mLabelSheet);

	mLabelPool.initializePool(15);
	mChatTextPool.initializePool(5);
	EmberOgre::getSingleton().EventWorldCreated.connect(sigc::mem_fun(*this, &IngameChatWidget::EmberOgre_WorldCreated));
	mGuiManager->EventEntityAction.connect(sigc::mem_fun(*this, &IngameChatWidget::GUIManager_EntityAction));
}

void IngameChatWidget::Config_TimeShown(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	mTimeShown = static_cast<double>(variable);
}

void IngameChatWidget::Config_DistanceShown(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	mDistanceShown = static_cast<double>(variable);
}

Window* IngameChatWidget::getLabelSheet()
{
	return mLabelSheet;
}

void IngameChatWidget::EmberOgre_WorldCreated(World& world)
{
	mCamera = &world.getMainCamera();

	mAvatarEntityId = world.getView().getAvatar()->getId();
}

void IngameChatWidget::GUIManager_EntityAction(const std::string& action, EmberEntity* entity)
{
	if (action == "talk") {
		EntityObserverStore::const_iterator I = mEntityObservers.find(entity->getId());
		EntityObserver* observer = 0;
		if (I != mEntityObservers.end()) {
			observer = I->second;
		} else {
			Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(*entity);
			if (modelRepresentation) {
				observer = new EntityObserver(*this, *entity);
				mEntityObservers.insert(std::make_pair(entity->getId(), observer));
			}
		}
		if (observer) {
			observer->showDetachedChat();
		}
	}
}

void IngameChatWidget::enableForEntity(EmberEntity& entity)
{
	if (mAvatarEntityId != entity.getId()) {
		EntityObserver* observer = new EntityObserver(*this, entity);
		mEntityObservers.insert(std::make_pair(entity.getId(), observer));
	}
}

void IngameChatWidget::disableForEntity(EmberEntity& entity)
{
	auto I = mEntityObservers.find(entity.getId());
	if (I != mEntityObservers.end()) {
		removeEntityObserver(I->second);
	}
}

void IngameChatWidget::removeEntityObserver(EntityObserver* observer)
{
	EntityObserverStore::iterator I = mEntityObservers.find(observer->getEntityId());
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
	for (auto& label : mLabelPool.getUsedWidgets()) {
		label->frameStarted(event);
	}
}

void IngameChatWidget::removeWidget(const std::string& windowName)
{
}

void IngameChatWidget::cameraPreRenderScene(Ogre::Camera* cam) {
	for (auto& label : mLabelPool.getUsedWidgets()) {
		label->objectRendering(cam);
	}
}

IngameChatWidget::EntityObserver::EntityObserver(IngameChatWidget& chatWidget, EmberEntity& entity) :
		mChatWidget(chatWidget), mEntity(entity), mLabel(nullptr)
{
	entity.VisibilityChanged.connect(sigc::mem_fun(*this, &EntityObserver::entity_VisibilityChanged));
	entity.BeingDeleted.connect(sigc::mem_fun(*this, &EntityObserver::entity_BeingDeleted));
	entity.Say.connect(sigc::mem_fun(*this, &EntityObserver::entity_Say));
	entity.EventChangedGraphicalRepresentation.connect(sigc::mem_fun(*this, &EntityObserver::entity_GraphicalRepresentationChanged));

	mExternalSlot = sigc::mem_fun(*this, &IngameChatWidget::EntityObserver::entity_attributeChanged);
	entity.observe("external", mExternalSlot);
	entity.observe("name", mExternalSlot);

	if (mEntity.isVisible()) {
		showLabel();
	}
}

IngameChatWidget::EntityObserver::~EntityObserver()
{
	hideLabel();
}

void IngameChatWidget::EntityObserver::entity_GraphicalRepresentationChanged()
{
	if (mLabel) {
		auto model = Model::ModelRepresentationManager::getSingleton().getModelForEntity(mEntity);
		if (model) {
			mLabel->attachToEntity(&mEntity, model);
		} else {
			hideLabel();
		}
	} else {
		if (mEntity.isVisible()) {
			showLabel();
		}
	}
}

const std::string& IngameChatWidget::EntityObserver::getEntityId() const
{
	return mEntity.getId();
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
	mLabel->objectRendering(camera);
}

void IngameChatWidget::EntityObserver::showLabel()
{
	if (!mLabel) {
		auto model = Model::ModelRepresentationManager::getSingleton().getModelForEntity(mEntity);
		if (model) {
			mLabel = mChatWidget.getLabelPool().checkoutWidget();
			mLabel->attachToEntity(&mEntity, model);
		}
	}
}

void IngameChatWidget::EntityObserver::hideLabel()
{
	if (mLabel) {
		mLabel->setActive(false);
		mLabel->setVisible(false);
		mChatWidget.getLabelPool().returnWidget(mLabel);
		mLabel = nullptr;
	}
}

void IngameChatWidget::EntityObserver::showDetachedChat()
{
	//First make sure that the label is created.
	showLabel();
	mLabel->showDetachedChatText();
}

IngameChatWidget::Label::Label(Window * window, WindowManager * windowManager, IngameChatWidget& containerWidget, const std::string& prefix) :
		mWindow(window), mModel(nullptr), mEntity(nullptr), mWindowManager(windowManager), mContainerWidget(containerWidget), mActive(false), mVisible(false), mRenderNextFrame(false), mChatText(nullptr)
{
}

IngameChatWidget::Label::~Label()
{
	mWindowManager->destroyWindow(mWindow);
}

void IngameChatWidget::Label::markForRender()
{
	mRenderNextFrame = true;
	setVisible(true);
}

void IngameChatWidget::Label::objectRendering(const Ogre::Camera * camera)
{
	//only update when being rendered by the main camera
	if (camera == &mContainerWidget.mCamera->getCamera()) {
		// 	const Ogre::Vector3& entityWorldCoords = mEntity->getDerivedPosition();
		//	Ogre::Vector3 entityWorldCoords = mEntity->getWorldBoundingBox(true).getCenter();
		//	Ogre::Vector3 entityWorldCoords = window->getEntity()->getSceneNode()->_getWorldAABB().getCenter();
		// 	const Ogre::Vector3& cameraCoords = camera->getDerivedPosition();
		//getWorldPosition is faster than getting the center of the boundingbox...

		const INodeProvider* nodeProvider = mModel->getNodeProvider();
		if (nodeProvider) {
			Ogre::Node* node = nodeProvider->getNode();
			Ogre::Vector3 diff = node->_getDerivedPosition() - camera->getDerivedPosition();

			//remove the window if it's either too far away
			if (diff.length() > mContainerWidget.mDistanceShown) {
				// 		mLabel->setActive(false);
			} else {
				markForRender();
				placeWindowOnEntity();
				/*		mLabel->setActive(true);
				 mLabel->placeWindowOnEntity();*/
			}
		}
	}
}


void IngameChatWidget::Label::placeWindowOnEntity()
{
	const INodeProvider* nodeProvider = mModel->getNodeProvider();
	if (nodeProvider) {
		//make sure that the window stays on the entity
		Ogre::Vector2 screenCoords;

		bool result = false;
//		Ogre::Vector3 entityWorldCoords = mMovableObject->getWorldBoundingSphere().getCenter();
//		entityWorldCoords.y = mMovableObject->getWorldBoundingBox().getMaximum().y;
		Ogre::Vector3 entityWorldCoords = nodeProvider->getNode()->convertLocalToWorldPosition(Ogre::Vector3::ZERO);
		entityWorldCoords.y = mModel->getCombinedBoundingBox().getMaximum().y;
		//check what the new position is in screen coords
		result = mContainerWidget.mCamera->worldToScreen(entityWorldCoords, screenCoords);

		if (result) {
			mWindow->setVisible(true);
			mWindow->setPosition(UVector2(UDim(screenCoords.x, -(mWindow->getPixelSize().d_width * 0.5)), UDim(screenCoords.y, -(mWindow->getPixelSize().d_height * 0.5))));
		} else {
			mWindow->setVisible(false);
		}
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
		setVisible(false);
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

void IngameChatWidget::Label::attachToEntity(EmberEntity* entity, Model::Model* model)
{
	mEntity = entity;
	mModel = model;
	reset();
	try {
		updateEntityName();
	} catch (const Exception& ex) {
		S_LOG_FAILURE("Could not get widget EntityName." << ex);
	}
	setActive(true);
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
		if (externalAttr.isInt() && externalAttr.asInt() == 1) {
			entityName = "!" + getEntity()->getName() + "!";
		}
	}
	// mWindow is the label itself (a StaticText)
	mWindow->setText(entityName);
}

EmberEntity * IngameChatWidget::Label::getEntity()
{
	return mEntity;
}

void IngameChatWidget::Label::setVisible(bool visible)
{
	if (mVisible == visible) {
		return;
	}

	if (visible) {
		mContainerWidget.getLabelSheet()->addChild(mWindow);
	} else {
		mContainerWidget.getLabelSheet()->removeChild(mWindow);
	}

	mVisible = visible;
}


void IngameChatWidget::Label::setActive(bool active)
{
	if (mActive == active) {
		return;
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
	getOrCreateChatText()->updateText(line);
	//mWindow->moveToFront();
}

IngameChatWidget::ChatText* IngameChatWidget::Label::getOrCreateChatText()
{
	if (!mChatText) {
		mChatText = mContainerWidget.getChatTextPool().checkoutWidget();
		mChatText->attachToLabel(this);
	}
	return mChatText;
}

void IngameChatWidget::Label::showDetachedChatText()
{
	placeWindowOnEntity();
	getOrCreateChatText()->switchToDetachedMode();
}

IngameChatWidget::LabelCreator::LabelCreator(IngameChatWidget& ingameChatWidget) :
		mIngameChatWidget(ingameChatWidget), mLayout(WindowManager::getSingleton().loadLayoutFromFile(GUIManager::getSingleton().getLayoutDir() + "Label.layout"))
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
	Window* window = mLayout->clone(true);
	window->setName(ss.str() + "EntityName");
	//We don't want the labels to prevent the user from picking entities in the world.
	window->setMousePassThroughEnabled(true);
	window->setRiseOnClickEnabled(false);

	Label* label = new Label(window, CEGUI::WindowManager::getSingletonPtr(), mIngameChatWidget, ss.str());
	return label;
}

IngameChatWidget::ChatText::ChatText(CEGUI::Window* attachedWindow, CEGUI::Window* detachedWindow) :
	mLabel(0),
	mAttachedWindow(attachedWindow),
	mAttachedTextWidget(mAttachedWindow->getChild("Text")),
	mAttachedResponseContainer(mAttachedWindow->getChild("ResponseContainer")),
	mAttachedEllipsisButton(mAttachedWindow->getChild("ResponseContainer/EllipsisButton")),

	mDetachedWindow(detachedWindow),
	mDetachedChatHistory(mDetachedWindow->getChild("ChatHistory")),
	mDetachedResponseContainer(mDetachedWindow->getChild("ResponseContainer")),
	mDetachedEditbox(mDetachedWindow->getChild("Editbox")),
	mDetachedTradeButton(mDetachedWindow->getChild("TradeButton")),

	mResponseWidget(mAttachedWindow->getChild("ResponseContainer/ResponseList")),

	mCommandHistory(new CommandHistory()),

	mElapsedTimeSinceLastUpdate(0.0f)
{
	mDetachedWindow->setVisible(false);
	GUIManager::getSingleton().getMainSheet()->addChild(mDetachedWindow);

	BIND_CEGUI_EVENT(mAttachedTextWidget, PushButton::EventClicked, IngameChatWidget::ChatText::buttonAttachedText_Click);
	// clicking the ellipsis button should do exactly the same as clicking the attached text
	BIND_CEGUI_EVENT(mAttachedEllipsisButton, PushButton::EventClicked, IngameChatWidget::ChatText::buttonAttachedText_Click);

	BIND_CEGUI_EVENT(mDetachedWindow, FrameWindow::EventCloseClicked, IngameChatWidget::ChatText::buttonDetachedClose_Click);
	BIND_CEGUI_EVENT(mDetachedEditbox, Window::EventKeyDown, IngameChatWidget::ChatText::editboxDetachedKey_Event);
	BIND_CEGUI_EVENT(mDetachedTradeButton, PushButton::EventClicked, IngameChatWidget::ChatText::buttonDetachedTrade_Click);

	// workaround, see: http://www.cegui.org.uk/mantis/view.php?id=464
	BIND_CEGUI_EVENT(mDetachedWindow, Window::EventSized, IngameChatWidget::ChatText::detachedSized_Event);
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

inline void inPlaceReplace(std::string& s, const std::string& sub, const std::string& replace)
{
	assert(!sub.empty());
	size_t pos = 0;
	while (true) {
		pos = s.find(sub, pos);
		if (pos == s.npos)
			break;

		s.replace(pos, sub.size(), replace);
		pos += replace.size();
	}
}

inline std::string escapeForCEGUI(const std::string& str)
{
	std::string ret = str;
	inPlaceReplace(ret, "[", "\\[");

	return ret;
}

void IngameChatWidget::ChatText::updateText(const std::string & line)
{
	mAttachedTextWidget->setText(escapeForCEGUI(line));

	mDetachedChatHistory->setText(mDetachedChatHistory->getText() + "\n[colour='00000000']-\n[colour='FF000000']" + escapeForCEGUI(mLabel->getEntity()->getName()) + ": " + escapeForCEGUI(line));
	mDetachedChatHistory->setProperty("VertScrollPosition", mDetachedChatHistory->getProperty("VertExtent"));

	mElapsedTimeSinceLastUpdate = 0;

	if (mLabel->getEntity()->hasSuggestedResponses()) {
		clearResponses();

		//for each response, create a button
		const std::vector<std::string>& responses = mLabel->getEntity()->getSuggestedResponses();

		std::vector<std::string>::const_iterator I = responses.begin();
		std::vector<std::string>::const_iterator I_end = responses.end();
		int i = 0;
		//std::stringstream ss;

		for (; I != I_end; ++I) {
			std::stringstream ss_;
			ss_ << i;
			PushButton* responseTextButton = static_cast<PushButton*>(WindowManager::getSingleton().createWindow(GUIManager::getSingleton().getDefaultScheme() + "/IngameChatResponseButton", "Response/" + ss_.str()));

			BIND_CEGUI_EVENT(responseTextButton, PushButton::EventClicked, IngameChatWidget::ChatText::buttonResponse_Click);

			responseTextButton->setInheritsAlpha(true);
			responseTextButton->setText(*I);
			responseTextButton->setTextParsingEnabled(false);
			responseTextButton->setTooltipText(*I);
			mResponseWidget->addChild(responseTextButton);
			mResponseTextWidgets.push_back(responseTextButton);

			++i;
		}

		// FIXME: Query the threshold from the looknfeel
		mAttachedEllipsisButton->setVisible(i >= 3);
	} else {
		mAttachedEllipsisButton->setVisible(false);
	}
}

void IngameChatWidget::ChatText::respondWithMessage(const std::string& message)
{
	EmberEntity* entity = mLabel->getEntity();
	EmberServices::getSingleton().getServerService().sayTo(message, *entity);

	mDetachedChatHistory->setText(mDetachedChatHistory->getText() + "\n[colour='00000000']-\n[colour='FF0000FF']You: " + message);
	mDetachedChatHistory->setProperty("VertScrollPosition", mDetachedChatHistory->getProperty("VertExtent"));

	mCommandHistory->addToHistory(message);

	clearResponses();
}

void IngameChatWidget::ChatText::switchToAttachedMode(bool updateHelpMessage)
{
	// be robust, if we are in detached mode, do nothing
	if (mAttachedWindow->isVisible()) {
		return;
	}

	mDetachedResponseContainer->removeChild(mResponseWidget);
	mAttachedResponseContainer->addChild(mResponseWidget);

	mAttachedWindow->setVisible(true);
	mDetachedWindow->setVisible(false);

	// workaround, see http://www.cegui.org.uk/mantis/view.php?id=464
	static_cast<LayoutContainer*>(mResponseWidget)->markNeedsLayouting();

	// reset the fade timer
	mElapsedTimeSinceLastUpdate = 0;

	if (updateHelpMessage) {
		QuickHelp::getSingleton().updateText(HelpMessage("You have switched to attached chat mode", "The messages will appear next to the entities and will slowly fade away over time", "ingame chat widget", "ingameChatWidgetAttached"));
	}
}

void IngameChatWidget::ChatText::switchToDetachedMode()
{
	// be robust, if we are in detached mode, do nothing
	if (mDetachedWindow->isVisible()) {
		return;
	}

	const auto rect = mAttachedWindow->getUnclippedOuterRect().get();
	if (rect.left() >= 0 && rect.top() >= 0) {
		mDetachedWindow->setPosition(UVector2(UDim(0, rect.left()), UDim(0, rect.top())));
	} else {
		mDetachedWindow->setPosition(UVector2(UDim(0.5, -(mDetachedWindow->getPixelSize().d_width * 0.5)), UDim(0.5, 0)));
	}

	//Make sure the widget is contained within the main window.
	auto rootSize = mAttachedWindow->getGUIContext().getRootWindow()->getPixelSize();
	float width = mDetachedWindow->getPixelSize().d_width;
	float height = mDetachedWindow->getPixelSize().d_height;
	if (mDetachedWindow->getPixelPosition().d_x + width > rootSize.d_width) {
		mDetachedWindow->setXPosition(CEGUI::UDim(0, rootSize.d_width - width));
	}
	if (mDetachedWindow->getPixelPosition().d_y + height > rootSize.d_height) {
		mDetachedWindow->setYPosition(CEGUI::UDim(0, rootSize.d_height - height));
	}


	mAttachedResponseContainer->removeChild(mResponseWidget);
	mDetachedResponseContainer->addChild(mResponseWidget);

	mAttachedWindow->setVisible(false);
	mDetachedWindow->setVisible(true);

	// workaround, see http://www.cegui.org.uk/mantis/view.php?id=464
	static_cast<LayoutContainer*>(mResponseWidget)->markNeedsLayouting();

	// reset the fade timer
	mElapsedTimeSinceLastUpdate = 0;

	QuickHelp::getSingleton().updateText(HelpMessage("You have switched to detached chat mode", "This allows you to concentrate on dialog from this particular person/entity as well as drag the chat history wherever you please. To switch back to attached mode, close the frame window with the dialog", "ingame chat widget", "ingameChatWidgetDetached"));
}

bool IngameChatWidget::ChatText::buttonResponse_Click(const EventArgs& args)
{
	const MouseEventArgs *mouseArgs = static_cast<const MouseEventArgs*>(&args);
	if (mouseArgs) {
		//each button contains a static text window, which is the one containg the actual text
		respondWithMessage(mouseArgs->window->getText().c_str());
	}
	return true;
}

bool IngameChatWidget::ChatText::buttonAttachedText_Click(const EventArgs& args)
{
	switchToDetachedMode();

	return true;
}

bool IngameChatWidget::ChatText::buttonDetachedClose_Click(const EventArgs& args)
{
	switchToAttachedMode();

	return true;
}

bool IngameChatWidget::ChatText::editboxDetachedKey_Event(const EventArgs& args)
{
	const KeyEventArgs& kargs = static_cast<const KeyEventArgs&>(args);

	if (kargs.scancode == Key::Return || kargs.scancode == Key::NumpadEnter) {
		// do not say empty messages
		if (mDetachedEditbox->getText().length() > 0) {
			respondWithMessage(mDetachedEditbox->getText().c_str());
			// clear the editbox (this message has been sent by now)
			mDetachedEditbox->setText("");
		}

		// but always eat the return key presses
		return true;
	} else if (kargs.scancode == Key::ArrowUp) {
		mCommandHistory->moveBackwards();
		mDetachedEditbox->setText(mCommandHistory->getHistoryString());

		return true;
	} else if (kargs.scancode == Key::ArrowDown) {
		mCommandHistory->moveForwards();
		mDetachedEditbox->setText(mCommandHistory->getHistoryString());

		return true;
	}

	// unless the key was a Enter/Return key, ArrowUp or ArrowDownwe have to tell CEGUI we haven't
	// handled it! otherwise backspace et al stop working
	return false;
}

bool IngameChatWidget::ChatText::buttonDetachedTrade_Click(const EventArgs& args)
{
	// FIXME: We should query if the entity is an NPC with merchanting capabilities
	//        if it is not, a normal trade window should pop instead of the merchant window
	GUIManager::getSingleton().EmitEntityAction("merchant", mLabel->getEntity());

	// also switch to attached mode to avoid having a superfluous window opened
	switchToAttachedMode(false);

	return true;
}

bool IngameChatWidget::ChatText::detachedSized_Event(const EventArgs& args)
{
	// workaround, see http://www.cegui.org.uk/mantis/view.php?id=464
	static_cast<LayoutContainer*>(mResponseWidget)->markNeedsLayouting();

	return true;
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
		mLabel->getWindow()->addChild(mAttachedWindow);
	} else {
		if (mAttachedWindow->getParent()) {
			mAttachedWindow->getParent()->removeChild(mAttachedWindow);
		}
	}
}

IngameChatWidget::ChatTextCreator::ChatTextCreator(IngameChatWidget& ingameChatWidget):
	mIngameChatWidget(ingameChatWidget),
	mAttachedLayout(WindowManager::getSingleton().loadLayoutFromFile(GUIManager::getSingleton().getLayoutDir() + "IngameChatWidgetAttached.layout")),
	mDetachedLayout(WindowManager::getSingleton().loadLayoutFromFile(GUIManager::getSingleton().getLayoutDir() + "IngameChatWidgetDetached.layout"))
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
	auto newAttached = mAttachedLayout->clone();
	newAttached->setName("MainWindow/Attached");
	auto newDetached = mDetachedLayout->clone();
	newDetached->setName("MainWindow/Detached");

	ChatText* widget = new ChatText(newAttached, newDetached);
	return widget;
}

}

}

}
