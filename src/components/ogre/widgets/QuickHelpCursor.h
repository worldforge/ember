#ifndef EMBEROGRE_QUICKHELP_CURSOR_H
#define EMBEROGRE_QUICKHELP_CURSOR_H

#include <list>
#include <sigc++/signal.h>
#include <string>

namespace EmberOgre {

namespace Gui {

class QuickHelp;
class TutorialMessage;

class QuickHelpCursor
{
public:
	QuickHelpCursor();
	~QuickHelpCursor();

	/**
	 * @brief Increments the current position iterator.
	 */
	void nextMessage();

	/**
	 * @brief Decrements the current position iterator.
	 */
	void previousMessage();

	void updateMessage();

	void getLatestTutorial();

	/**
	 * @brief Called when position iterator changes
	 */
	sigc::signal <void,const std::string&> EventUpdateText;
private:

	/**
	 * @brief Holds the position of the current message displayed in the widget.
	 */
	std::list<TutorialMessage>::const_iterator mCurrentPosition;

	QuickHelp& mQuickHelp;

};

}

}

#endif
