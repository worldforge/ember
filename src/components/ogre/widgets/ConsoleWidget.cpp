#include "Widget.h"
#include "ConsoleWidget.h"

namespace DimeOgre {

ConsoleWidget::ConsoleWidget(GUIManager* guiManager) : Widget::Widget(guiManager), mSpeedOfWindow(0.4)
{
	myBackend = dime::ConsoleBackend::getMainConsole();
	myBackend->GotMessage.connect(SigC::slot(*this, &ConsoleWidget::pushMessage));
	mState = CS_CLOSED;

}
ConsoleWidget::~ConsoleWidget()
{}


void ConsoleWidget::buildWidget()
{
	
	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"widgets/ConsoleWidget.xml");
	mMainWindow->setAlwaysOnTop(true);
	
	mInputBox = static_cast<CEGUI::Editbox*>(mMainWindow->getChild((CEGUI::utf8*)"ConsoleInputBox"));
	mConsoleTextBox = static_cast<CEGUI::MultiLineEditbox*>(mMainWindow->getChild((CEGUI::utf8*)"ConsoleTextBox"));
	mInputBox->subscribeEvent(CEGUI::Editbox::EventCharacterKey, 
		boost::bind(&ConsoleWidget::consoleTextBox_Character, this, _1));
	mInputBox->subscribeEvent(CEGUI::Editbox::EventKeyUp, 
		boost::bind(&ConsoleWidget::consoleTextBox_KeyUp, this, _1));
	
	mInputBox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, 
		boost::bind(&ConsoleWidget::consoleInputBox_TextAcceptedEvent, this, _1));
	
/*	mConsoleTextBox = static_cast<CEGUI::MultiLineEditbox*>(mMainWindow->getChild((CEGUI::utf8*)"ConsoleTextBox"));
	mConsoleTextBox->subscribeEvent(CEGUI::MultiLineEditbox::CharacterEvent, 
		boost::bind(&ConsoleWidget::consoleTextBox_Character, this, _1));
	mConsoleTextBox->subscribeEvent(CEGUI::MultiLineEditbox::KeyUpEvent, 
		boost::bind(&ConsoleWidget::consoleTextBox_KeyUp, this, _1));
*/	
	getMainSheet()->addChildWindow(mMainWindow); 
}
	

void ConsoleWidget::frameStarted(const Ogre::FrameEvent & event)
{
	if(mState == CS_CLOSING )
	{
		mMainWindow->setYPosition(mMainWindow->getYPosition() - (mSpeedOfWindow*event.timeSinceLastFrame));
		
		if(mMainWindow->getYPosition() <= -0.475)
		{
			mMainWindow->setYPosition(-0.475);
			mState = CS_CLOSED;
		}
	}
	else if(mState == CS_OPENING)
	{
		mMainWindow->setYPosition(mMainWindow->getYPosition() + (mSpeedOfWindow*event.timeSinceLastFrame));
		
		if(mMainWindow->getYPosition() >= 0)
		{
			mMainWindow->setYPosition(0);
			mState = CS_OPEN;
		}
	}

}

void ConsoleWidget::open(void)
{
	mState = CS_OPENING;
}

void ConsoleWidget::close(void)
{
	mState = CS_CLOSING;
}

void ConsoleWidget::toggleActive()
{
	if(mState == CS_OPEN || mState == CS_OPENING)
		close();
	else
		open();

}

bool ConsoleWidget::pushMessage(const std::string& message)
{
	mConsoleTextBox->setText(message + "\n" + mConsoleTextBox->getText());
	return true;

}

bool ConsoleWidget::consoleInputBox_TextAcceptedEvent(const CEGUI::EventArgs& args)
{
	const CEGUI::String consoleText = mInputBox->getText();
	mInputBox->setText(CEGUI::String(""));

	pushMessage(("> " + consoleText).c_str());
	// run the command
	myBackend->runCommand(consoleText.c_str());
	
	return true;

}

bool ConsoleWidget::consoleTextBox_KeyUp(const CEGUI::EventArgs& args)
{
	const CEGUI::KeyEventArgs& keyargs = static_cast<const CEGUI::KeyEventArgs&>(args);
	fprintf(stderr, (std::string("CEGUI - KEY UP:") + keyargs.scancode + "\n").c_str());

/*
	//TODO: add history handling
	if(keyargs.scancode == CEGUI::Key::Up)
	{
	}
 */
	return true;
}


bool ConsoleWidget::consoleTextBox_Character(const CEGUI::EventArgs& args)
{

	const CEGUI::KeyEventArgs& keyargs = static_cast<const CEGUI::KeyEventArgs&>(args);
	std::stringstream tempString;
	tempString << std::string("CEGUI - CHARACTER:") << keyargs.codepoint;
	fprintf(stderr, tempString.str().c_str());
	return true;
}

}
