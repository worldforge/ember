#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include "framework/ConsoleBackend.h"
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIEditbox.h>
namespace EmberOgre {
	
class Widget;
	
/**
* The main console. This behaves like most game consoles such as quake etc.
* Switch between opened and closed through the method toggleActive()
*/
class ConsoleWidget : public Widget, virtual public SigC::Object
{
public:

	ConsoleWidget();
	virtual ~ConsoleWidget();
	void buildWidget();
	virtual void frameStarted(const Ogre::FrameEvent & event);
	
	/**
	*toggles the console on and off
	*/
	void toggleActive();
	
	/**
	* Starts to hide the console.
	*/
	void close();
	
	/**
	* Starts to show the console.
	*/
	void open();

	bool pushMessage(const std::string& message);
	
		
protected:
	CEGUI::MultiLineEditbox* mConsoleTextBox;
	CEGUI::Editbox* mInputBox;
	
        Ember::ConsoleBackend *myBackend;
        enum State { CS_OPEN, CS_CLOSED, CS_OPENING, CS_CLOSING };
        State mState;
	
	
	bool consoleTextBox_Character(const CEGUI::EventArgs& args);
	bool consoleTextBox_KeyUp(const CEGUI::EventArgs& args);
	bool consoleInputBox_TextAcceptedEvent(const CEGUI::EventArgs& args);
	
	//the speed of the window when swithing between open and closed, 0.25 by default
	Ogre::Real mSpeedOfWindow;
	
};

}
#endif // CONSOLEWIDGET_H
