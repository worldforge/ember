#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H


#include <Eris/Log.h>

namespace CEGUI {
	class MultiLineEditbox;
	class ComboDropList;
	class Listbox;
	class Combobox;
	class EventArgs;
}
namespace EmberOgre {

class Widget;
class GUIManager;

class DebugWidget
: public Widget
{
public:

	virtual ~DebugWidget();
	
/*	void frameStarted(const Ogre::FrameEvent & evt);*/
	
	void buildWidget();
	
	void recieveLog(Eris::LogLevel level, const std::string&);
protected:

// 	void updateStats(void);
	
	CEGUI::MultiLineEditbox* mLogTextBox;
	CEGUI::Combobox* mLoglevelDroplist;
	
	void fillListWithLogLevels(CEGUI::Combobox* list);
	bool loglevel_SelectionChanged(const CEGUI::EventArgs& args);
	
};

}

#endif // DEBUGWIDGET_H
