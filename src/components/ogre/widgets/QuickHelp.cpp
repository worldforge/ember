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
	//TODO delete messages
}

void QuickHelp::nextMessage()
{
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
		mTutorialText.pop_back();
	mTutorialText.push_front(*message);
	mCurrentPosition = mTutorialText.end();
	mCurrentPosition--;
	EventUpdateText.emit(text);
}

}
}
