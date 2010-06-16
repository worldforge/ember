//
// C++ Implementation: QuickHelp
//
// Description:
//
//
// Author: Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
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

#include "QuickHelp.h"

template<> EmberOgre::Gui::QuickHelp* Ember::Singleton<EmberOgre::Gui::QuickHelp>::ms_Singleton = 0;

using namespace Ember;
namespace EmberOgre
{

namespace Gui
{

QuickHelp::QuickHelp()
{
	mCurrentPosition = mTutorialText.begin();
}

QuickHelp::~QuickHelp()
{
	mTutorialText.clear();
}

void QuickHelp::nextMessage()
{
	//list.end() returns an iterator that is one past the front of the list, so we have a bit of extra work
	mCurrentPosition++;
	if (mCurrentPosition != mTutorialText.end())
	{
		EventUpdateText.emit((*mCurrentPosition).getHelp());
	} else {
		mCurrentPosition--;
	}
}

void QuickHelp::previousMessage()
{
	if (mCurrentPosition != mTutorialText.begin())
	{
		mCurrentPosition--;
		EventUpdateText.emit((*mCurrentPosition).getHelp());
	}
}

void QuickHelp::updateText(const std::string& text)
{
	TutorialMessage *message = new TutorialMessage(text);
	if (mTutorialText.size() == MAXTUTORIALS)
		mTutorialText.pop_front();

	mTutorialText.push_back(*message);
	mCurrentPosition = mTutorialText.end();
	mCurrentPosition--;
	EventUpdateText.emit((*mCurrentPosition).getHelp());
}

}
}
