/*
    Copyright (C) 2004  Erik Hjortsberg
    Some parts Copyright © 2000-2003 The OGRE Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "DebugListener.h"
using namespace Ogre;
namespace EmberOgre {

DebugListener* DebugListener::_instance = 0;

DebugListener& DebugListener::getSingleton(void)
{
	if(_instance == 0)
		_instance = new DebugListener;
	return *_instance;
}


DebugListener::DebugListener()
{
	Ogre::Overlay* debugOverlay = (Ogre::Overlay *)Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");  
	debugOverlay->show();  
    mDebugElement = Ogre::GuiManager::getSingleton().getGuiElement("Core/DebugText");
}
DebugListener::~DebugListener()
{}


bool DebugListener::frameStarted(const Ogre::FrameEvent & event)
{
	
 	//mDebugElement->setCaption(mString);
 	updateStats();

	return true;
}
bool DebugListener::frameEnded(const Ogre::FrameEvent & event) { return true;}

 
void DebugListener::setString(const Ogre::String string) {
	mString = string;
}

/*
 * 
 * This method is taken from the class ExampleListener in Ogre's samples
 * Copyright © 2000-2003 The OGRE Team
 */
void DebugListener::updateStats(void)
{
    static String currFps = "Current FPS: ";
    static String avgFps = "Average FPS: ";
    static String bestFps = "Best FPS: ";
    static String worstFps = "Worst FPS: ";
    static String tris = "Triangle Count: ";

    // update stats when necessary
    GuiElement* guiAvg = GuiManager::getSingleton().getGuiElement("Core/AverageFps");
    GuiElement* guiCurr = GuiManager::getSingleton().getGuiElement("Core/CurrFps");
    GuiElement* guiBest = GuiManager::getSingleton().getGuiElement("Core/BestFps");
    GuiElement* guiWorst = GuiManager::getSingleton().getGuiElement("Core/WorstFps");

    const RenderTarget::FrameStats& stats = Ogre::Root::getSingleton().getAutoCreatedWindow()->getStatistics();

    guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
    guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
    guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
        +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
    guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
        +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

    GuiElement* guiTris = GuiManager::getSingleton().getGuiElement("Core/NumTris");
    guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

    GuiElement* guiDbg = GuiManager::getSingleton().getGuiElement("Core/DebugText");
    guiDbg->setCaption(Ogre::Root::getSingleton().getAutoCreatedWindow()->getDebugText());
};
}


