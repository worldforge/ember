#ifndef EMBEROGRE_QUICKHELP_H
#define EMBEROGRE_QUICKHELP_H

#include "framework/Singleton.h"
#include <sigc++/signal.h>
#include <string>
#include <list>
#include <iterator>
#include "TutorialMessage.h"

namespace EmberOgre {

namespace Gui {

const short unsigned int MAXTUTORIALS = 5;

class QuickHelp : public Ember::Singleton<QuickHelp>
{
public:
	QuickHelp();
	virtual ~QuickHelp();
	void updateText(const std::string& text);
	void nextMessage();
	void previousMessage();

	sigc::signal <void,const std::string&> EventUpdateText;

private:
	std::list<TutorialMessage> mTutorialText;
	std::list<TutorialMessage>::iterator mCurrentPosition;

};

}
}


#endif
