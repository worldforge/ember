#include "Widget.h"

#include "GUIManager.h"

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif


namespace DimeOgre
{
	
	Widget::Widget(GUIManager* guiManager)
	: mGuiManager(guiManager)
	{
		mWindowManager = &CEGUI::WindowManager::getSingleton();
	}
	
	Widget::~Widget()
	{
		mGuiManager->removeWidget(this);
	}
	
	void Widget::frameStarted(const Ogre::FrameEvent& evt)
	{
	}
	
	void Widget::buildWidget()
	{}

	CEGUI::Window* Widget::getMainSheet() { 
		return mGuiManager->getMainSheet(); 
	}
	
}
