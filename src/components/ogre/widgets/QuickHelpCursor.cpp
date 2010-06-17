#include "QuickHelpCursor.h"
#include "QuickHelp.h"
#include "TutorialMessage.h"

using namespace EmberOgre;

namespace EmberOgre
{

namespace Gui
{

QuickHelpCursor::QuickHelpCursor() : mQuickHelp(Gui::QuickHelp::getSingleton())
{
	mQuickHelp.EventTutorialAdded.connect(sigc::mem_fun(*this, &QuickHelpCursor::getLatestTutorial));
	getLatestTutorial();
}

QuickHelpCursor::~QuickHelpCursor()
{

}

void QuickHelpCursor::getLatestTutorial()
{
	//getEnd returns one past the last element.
	mCurrentPosition = mQuickHelp.getEnd();
	mCurrentPosition--;
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
