#ifndef WIDGET_H
#define WIDGET_H

#include <Ogre.h>
#include <CEGUI.h>
#include <CEGUIBase.h>
#include <CEGUIExceptions.h>
#include <CEGUISystem.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUIImageset.h>
#include <elements/CEGUIStaticImage.h>
#include <elements/CEGUIPushButton.h>
#include <renderers/OgreGUIRenderer/ogrerenderer.h>

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

namespace DimeOgre {
	
class GUIManager;

class Widget : virtual public SigC::Object
{
public:

	Widget(GUIManager* guiManager);
	virtual ~Widget();
	
	virtual void frameStarted(const Ogre::FrameEvent& evt);
	virtual void buildWidget();
	
protected:
	CEGUI::Window* mMainWindow;

	GUIManager* mGuiManager;
	
	CEGUI::Window* getMainSheet();
	
	CEGUI::WindowManager* mWindowManager;
	
};

}

#endif // WIDGET_H
