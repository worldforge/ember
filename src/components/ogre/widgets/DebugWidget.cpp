#include "Widget.h"
#include "DebugWidget.h"

#include <CEGUIWindow.h>

namespace EmberOgre {

DebugWidget::DebugWidget(GUIManager* guiManager) 
: Widget::Widget(guiManager)
{
	
}

DebugWidget::~DebugWidget()
{}

void DebugWidget::buildWidget()
{
	mFwnd = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"Taharez Frame Window", (CEGUI::utf8*)"DebugWindow");
	mFwnd->setPosition(CEGUI::Point(0.01f, 0.01f));
	mFwnd->setSize(CEGUI::Size(0.43f, 0.1f));
	mFwnd->setText((CEGUI::utf8*)"Debug");
	
	
	mSheet->addChildWindow(mFwnd); 
}

void DebugWidget::frameStarted(const Ogre::FrameEvent & evt)
{
	updateStats();
}

void DebugWidget::updateStats(void)
{
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
}


}

