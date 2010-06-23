//
// C++ Implementation: QuickHelpCursor
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
//
#include "QuickHelpCursor.h"
#include "QuickHelp.h"
#include "HelpMessage.h"

using namespace EmberOgre;

namespace EmberOgre
{

namespace Gui
{

QuickHelpCursor::QuickHelpCursor() : mQuickHelp(Gui::QuickHelp::getSingleton())
{
	mQuickHelp.EventTutorialAdded.connect(sigc::mem_fun(*this, &QuickHelpCursor::getLatestTutorial));

	mCurrentPosition = mQuickHelp.getEnd();
	mCurrentPosition--;
	updateMessage();
}

QuickHelpCursor::~QuickHelpCursor()
{

}

void QuickHelpCursor::getLatestTutorial(std::list<HelpMessage>::const_iterator position)
{
	mCurrentPosition = position;
	updateMessage();
}

void QuickHelpCursor::updateMessage()
{
	EventUpdateText.emit((*mCurrentPosition).getHelp());
}

void QuickHelpCursor::nextMessage()
{
	//list.end() returns an iterator that is one past the front of the list, so we have a bit of extra work
	mCurrentPosition++;
	if (mCurrentPosition != mQuickHelp.getEnd())
	{
		updateMessage();
	} else {
		mCurrentPosition--;
	}
}

void QuickHelpCursor::previousMessage()
{
	if (mCurrentPosition != mQuickHelp.getBeginning())
	{
		mCurrentPosition--;
		updateMessage();
	}
}


}

}
