//
// C++ Implementation: IngameTradeWidget
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

#include "IngameTradeWidget.h"

#include "../GUIManager.h"
#include "../EmberOgre.h"
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>

using namespace CEGUI;
namespace Ember
{
namespace OgreView
{
namespace Gui
{

IngameTradeWidget::IngameTradeWidget():
	mTradeWindowCreator(*this),
	mTradeWindowPool(mTradeWindowCreator)
{
	GUIManager::getSingleton().EventEntityAction.connect(sigc::mem_fun(*this, &IngameTradeWidget::GUIManager_EventEntityAction));
}

IngameTradeWidget::~IngameTradeWidget()
{}

void IngameTradeWidget::buildWidget()
{
	mTradeWindowPool.initializePool(3);
	EmberOgre::getSingleton().EventWorldCreated.connect(sigc::mem_fun(*this, &IngameTradeWidget::EmberOgre_WorldCreated));
}

void IngameTradeWidget::EmberOgre_WorldCreated(World& world)
{
	//world.getView().EntitySeen.connect(sigc::mem_fun(*this, &IngameChatWidget::View_EntitySeen));
	//world.getView().EntityCreated.connect(sigc::mem_fun(*this, &IngameChatWidget::View_EntityCreated));
}

void IngameTradeWidget::GUIManager_EventEntityAction(const std::string& action, EmberEntity* entity)
{
	if (action == "Trade")
	{
		TradeWindow* wnd = mTradeWindowPool.checkoutWidget();
		wnd->setTargetEntity(entity);
	}
}

/*
void IngameTradeWidget::View_EntitySeen(Eris::Entity* entity)
{
	entityArrivedFromServer(static_cast<EmberEntity&> (*entity));
}

void IngameTradeWidget::View_EntityCreated(Eris::Entity* entity)
{
	entityArrivedFromServer(static_cast<EmberEntity&> (*entity));
}

void IngameTradeWidget::entityArrivedFromServer(EmberEntity& entity)
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
*/

WidgetPool<IngameTradeWidget::TradeWindow>& IngameTradeWidget::getTradeWindowPool()
{
	return mTradeWindowPool;
}

//---------------------------------------------------


void IngameTradeWidget::frameStarted(const Ogre::FrameEvent & event)
{
	/*for (WidgetPool<Label>::WidgetStore::iterator I = mLabelPool.getUsedWidgets().begin(); I != mLabelPool.getUsedWidgets().end(); ++I) {
		(*I)->frameStarted(event);
	}*/
}

IngameTradeWidget::TradeWindow::TradeWindow(const std::string& prefix) :
	mWindow(WindowManager::getSingleton().getWindow(prefix + "MainWindow")),
	mPrefix(prefix)
{
	// ensure we are hidden at the start
	mWindow->setVisible(false);
	GUIManager::getSingleton().getMainSheet()->addChildWindow(mWindow);
}

IngameTradeWidget::TradeWindow::~TradeWindow()
{
	WindowManager::getSingleton().destroyWindow(mWindow);
}

void IngameTradeWidget::TradeWindow::setTargetEntity(EmberEntity* entity)
{
	if (entity)
	{
		mWindow->setVisible(true);
	}
	else
	{
		mWindow->setVisible(false);
	}
}

IngameTradeWidget::TradeWindowCreator::TradeWindowCreator(IngameTradeWidget& ingameTradeWidget):
	mIngameTradeWidget(ingameTradeWidget),
	mLayout(WindowManager::getSingleton().loadWindowLayout(GUIManager::getSingleton().getLayoutDir() + "IngameTradeWidget.layout"))
{
}

IngameTradeWidget::TradeWindowCreator::~TradeWindowCreator()
{
	WindowManager::getSingleton().destroyWindow(mLayout);
}

IngameTradeWidget::TradeWindow* IngameTradeWidget::TradeWindowCreator::createWidget(unsigned int currentPoolSize)
{
	//there is no chat window for this entity, let's create one by cloning the existing layout
	std::stringstream ss;
	ss << "TradeWindow/" << currentPoolSize << "/";
	Window* wnd = mLayout->clone(ss.str() + "MainWindow");
	
	TradeWindow* widget = new TradeWindow(ss.str());
	return widget;
}

}

}

}
