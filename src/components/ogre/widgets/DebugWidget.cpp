#include "Widget.h"
#include "DebugWidget.h"

#include <CEGUIWindow.h>
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIComboDropList.h> 
#include <elements/CEGUICombobox.h> 
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 

#include <sigc++/object_slot.h>
namespace EmberOgre {

/*template<> WidgetLoader WidgetLoaderHolder<DebugWidget>::loader("DebugWidget", &createWidgetInstance);*/
//WidgetLoader Widget::loader("DebugWidget", &createWidgetInstance<DebugWidget>);


DebugWidget::~DebugWidget()
{}

void DebugWidget::buildWidget() 
{
	loadMainSheet("DebugWidget.xml", "Debug/");
	
	mLogTextBox = static_cast<CEGUI::MultiLineEditbox*>(getWindow("TextBox"));
	mLoglevelDroplist = static_cast<CEGUI::Combobox*>(getWindow("LogLevelDropList"));
	fillListWithLogLevels(mLoglevelDroplist);
	
	BIND_CEGUI_EVENT(mLoglevelDroplist, CEGUI::Combobox::EventListSelectionAccepted, DebugWidget::loglevel_SelectionChanged);
	
	
	Eris::Logged.connect(SigC::slot(*this, &DebugWidget::recieveLog));
	
	registerConsoleVisibilityToggleCommand("debug");
	enableCloseButton();

}

void DebugWidget::recieveLog(Eris::LogLevel level, const std::string& line)
{
	mLogTextBox->setText(line + "\n" + mLogTextBox->getText());
}

void DebugWidget::fillListWithLogLevels(CEGUI::Combobox* list) 
{

	CEGUI::ListboxItem* item = new ColoredListItem("error", (uint)Eris::LOG_ERROR, 0);
	list->addItem(item);
	item = new ColoredListItem("warning", Eris::LOG_WARNING, 0);
	list->addItem(item);
	item = new ColoredListItem("notice", Eris::LOG_NOTICE, 0);
	list->addItem(item);
	item = new ColoredListItem("verbose", Eris::LOG_VERBOSE, 0);
	list->addItem(item);
	item = new ColoredListItem("debug", Eris::LOG_DEBUG, 0);
	list->addItem(item);
	
}

bool DebugWidget::loglevel_SelectionChanged(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mLoglevelDroplist->getSelectedItem();
	if (item) {
		Eris::LogLevel level = (Eris::LogLevel)item->getID();
		Eris::setLogLevel(level);
	}
	return true;

}

// void DebugWidget::frameStarted(const Ogre::FrameEvent & evt)
// {
// 	updateStats();
// }

/*void DebugWidget::updateStats(void)
{*/
/*	static CEGUI::String currFps = (CEGUI::utf8*)"Current FPS: ";
	static CEGUI::String avgFps = (CEGUI::utf8*)"Average FPS: ";
	static CEGUI::String bestFps = (CEGUI::utf8*)"Best FPS: ";
	static CEGUI::String worstFps = (CEGUI::utf8*)"Worst FPS: ";
	static CEGUI::String tris = (CEGUI::utf8*)"Triangle Count: ";

	if (mSkipCount >= mUpdateFreq)
	{
		mSkipCount = 0;

		try 
		{
			CEGUI::Window* guiAvg = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPAverageFPS");
			CEGUI::Window* guiCurr = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPCurrentFPS");
			CEGUI::Window* guiBest = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPBestFPS");
			CEGUI::Window* guiWorst = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPWorstFPS");

			const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();

			guiAvg->setText(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
			guiCurr->setText(currFps + Ogre::StringConverter::toString(stats.lastFPS));
			guiBest->setText(bestFps + Ogre::StringConverter::toString(stats.bestFPS)
				+" "+Ogre::StringConverter::toString(stats.bestFrameTime)+" ms");
			guiWorst->setText(worstFps + Ogre::StringConverter::toString(stats.worstFPS)
				+" "+Ogre::StringConverter::toString(stats.worstFrameTime)+" ms");

			CEGUI::Window* guiTris = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPTriCount");
			guiTris->setText(tris + Ogre::StringConverter::toString(stats.triangleCount));

			CEGUI::Window* guiDbg = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPDebugMsg");
			guiDbg->setText(mWindow->getDebugText());
		}
		catch (CEGUI::UnknownObjectException x)
		{
			// just skip if windows are missing
		}

	}
	else
	{
		mSkipCount++;
	}
*/
// }


}

