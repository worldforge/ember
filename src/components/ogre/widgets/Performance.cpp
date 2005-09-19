//
// C++ Implementation: Performance
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "Performance.h"

#include "Widget.h"
#include <elements/CEGUIMultiLineEditbox.h>
#include <Ogre.h>
#include "../EmberOgre.h"
//#include <OgreRenderTarget.h> 
#include <numeric>
#include <sigc++/object_slot.h>
namespace EmberOgre {

// template<> WidgetLoader WidgetLoaderHolder<Performance>::loader("Performance", &createWidgetInstance);

Performance::Performance()
 : Widget()
{
}


Performance::~Performance()
{
}

void Performance::buildWidget()
{
	loadMainSheet("Performance.widget", "Performance/");
	
	mMainText = static_cast<CEGUI::MultiLineEditbox*>(getWindow("TextBox"));
	
	EmberOgre::getSingleton().EventStartErisPoll.connect(SigC::slot(*this, &Performance::startErisPolling));
	EmberOgre::getSingleton().EventEndErisPoll.connect(SigC::slot(*this, &Performance::endErisPolling));
	
	
	registerConsoleVisibilityToggleCommand("performance");
	enableCloseButton();
}

void Performance::frameStarted(const Ogre::FrameEvent& evt)
{

	if (mMainWindow->isVisible()) {
		std::stringstream ss;
		const Ogre::RenderTarget::FrameStats& stats = EmberOgre::getSingleton().getRenderWindow()->getStatistics();
		
		ss << "Current FPS: " << stats.lastFPS << "\n";
		ss << "Average FPS: " << stats.avgFPS << "\n";
		ss << "Triangle count: " << stats.triangleCount << "\n";
		//ss << "Time in eris: " << getAverageErisTime() * 100 << "% \n";
	
		mMainText->setText(ss.str());
	}


}

Ogre::Real Performance::getAverageErisTime() const 
{
	if (mErisTimes.size()) {
		
		long inEris = std::accumulate(mErisTimes.begin(), mErisTimes.end(), 0);
		long notInEris = std::accumulate(mNotInErisTimes.begin(), mNotInErisTimes.end(), 0);
	
		return  inEris / notInEris ;
	}
	return 0;
}

void Performance::startErisPolling()
{
	mTimer.reset();
	mNotInErisTimes.push_back(mNotInErisTimer.getMilliseconds());
	if (mNotInErisTimes.size() > 100) {
		mNotInErisTimes.pop_front();
	}
}

void Performance::endErisPolling()
{
	mNotInErisTimer.reset();
	
	mErisTimes.push_back(mTimer.getMilliseconds());
	if (mErisTimes.size() > 100) {
		mErisTimes.pop_front();
	}
}



};
