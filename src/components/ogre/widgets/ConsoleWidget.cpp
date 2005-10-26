#include "Widget.h"
#include "ConsoleWidget.h"

namespace EmberOgre {

// template<> WidgetLoader WidgetLoaderHolder<ConsoleWidget>::loader("ConsoleWidget", &createWidgetInstance);

ConsoleWidget::ConsoleWidget() : mSpeedOfWindow(0.4)
{

}
ConsoleWidget::~ConsoleWidget()
{}


void ConsoleWidget::buildWidget()
{
	myBackend = Ember::ConsoleBackend::getMainConsole();
	myBackend->GotMessage.connect(sigc::mem_fun(*this, &ConsoleWidget::pushMessage));
	mState = CS_CLOSED;
	
	loadMainSheet("ConsoleWidget.xml", "Console/");
	mMainWindow->setAlwaysOnTop(true);
	
	mInputBox = static_cast<CEGUI::Editbox*>(getWindow("InputBox"));
	mConsoleTextBox = static_cast<CEGUI::MultiLineEditbox*>(getWindow("TextBox"));
	BIND_CEGUI_EVENT(mInputBox, CEGUI::Editbox::EventCharacterKey, ConsoleWidget::consoleTextBox_Character);
	BIND_CEGUI_EVENT(mInputBox, CEGUI::Editbox::EventKeyUp, ConsoleWidget::consoleTextBox_KeyUp);
	
	
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

// bool ConsoleWidget::consoleInputBox_TextAcceptedEvent(const CEGUI::EventArgs& args)
// {
// 	
// 	return true;
// 
// }

bool ConsoleWidget::consoleTextBox_KeyUp(const CEGUI::EventArgs& args)
{
	const CEGUI::KeyEventArgs& keyargs = static_cast<const CEGUI::KeyEventArgs&>(args);
//	fprintf(stderr, (std::string("CEGUI - KEY UP:") + keyargs.scancode + "\n").c_str());
	
	if(keyargs.scancode != CEGUI::Key::Tab)
	{
		mTabPressed = false;
	}
	switch(keyargs.scancode)
	{
	case CEGUI::Key::ArrowUp:
		{
			if(myBackend->getHistoryPosition() == 0)
			{
				mCommandLine = mInputBox->getText().c_str();
			}
			else
			{
				// we are not at the command line but in the history
				// => write back the editing
				myBackend->changeHistory(myBackend->getHistoryPosition(), mInputBox->getText().c_str());
			}
			myBackend->moveBackwards();
			if(myBackend->getHistoryPosition() != 0)
			{
				mInputBox->setText(myBackend->getHistoryString());
			}
			
			break;
		}
	case CEGUI::Key::ArrowDown:
		{
			if(myBackend->getHistoryPosition() > 0)
			{
				myBackend->changeHistory(myBackend->getHistoryPosition(), mInputBox->getText().c_str());
				myBackend->moveForwards();
				if(myBackend->getHistoryPosition() == 0)
				{
					mInputBox->setText(mCommandLine);
				}
				else
				{
					mInputBox->setText(myBackend->getHistoryString());
				}
			}
			
			break;
		}
	case CEGUI::Key::Tab:
		{
			std::string sCommand(mInputBox->getText().c_str());
			
			// only process commands
			if(sCommand[0] != '/')
			{
				break;
			}
			sCommand = sCommand.substr(1, mInputBox->getCaratIndex() - 1);
			if(mTabPressed == true)
			{
				const std::set< std::string > commands(myBackend->getPrefixes(sCommand));
				
				std::cout << sCommand << std::endl;
				if(commands.size() > 0)
				{
					std::set< std::string >::const_iterator iCommand(commands.begin());
					std::string sMessage("");
				
					mSelected = (mSelected + 1) % commands.size();
				
					int select(0);
				
					while(iCommand != commands.end())
					{
						if(select == mSelected)
						{
							std::string sCommandLine(mInputBox->getText().c_str());
						
							// compose the new command line: old text before the caret + selected command
							mInputBox->setText(sCommandLine.substr(0, mInputBox->getCaratIndex()) + iCommand->substr(mInputBox->getCaratIndex() - 1));
							mInputBox->setSelection(mInputBox->getCaratIndex(), 0xFFFFFFFF);
						}
						sMessage += *iCommand + ' ';
						++iCommand;
						++select;
					}
					myBackend->pushMessage(sMessage);
				}
			}
			else
			{
				mTabPressed = true;
				mSelected = 0;
				
				const std::set< std::string > commands(myBackend->getPrefixes(sCommand));
				
				if(commands.size() == 0)
				{
					// TODO: Error reporting?
				}
				else
				{
					// if any command starts with the current prefix
					if(commands.size() == 1)
					{
						mInputBox->setText(std::string("/") + *(commands.begin()) + ' ');
						// this will be at the end of the text
						mInputBox->setCaratIndex(0xFFFFFFFF);
					}
					else
					{
						std::set< std::string >::const_iterator iSelected(commands.begin());
						std::set< std::string >::const_iterator iCommand(commands.begin());
						std::string sCommonPrefix(*iCommand);
						int select = 1;
						
						++iCommand;
						while(iCommand != commands.end())
						{
							if(select == mSelected)
							{
								iSelected = iCommand;
							}
							
							std::string::size_type i(0);
							
							while((i < sCommonPrefix.length()) && (i < (*iCommand).length()))
							{
								if(sCommonPrefix[i] != (*iCommand)[i])
								{
									break;
								}
								++i;
							}
							if(i < sCommonPrefix.length())
							{
								sCommonPrefix = sCommonPrefix.substr(0, i);
							}
							++select;
							++iCommand;
						}
						mInputBox->setText(std::string("/") + sCommonPrefix + iSelected->substr(sCommonPrefix.length()));
						mInputBox->setCaratIndex(sCommonPrefix.length() + 1);
						mInputBox->setSelection(sCommonPrefix.length() + 1, 0xFFFFFFFF);
					}
				}
			}
				
			break;
		}
	case CEGUI::Key::Return:
	case CEGUI::Key::NumpadEnter:
		{
			if(mInputBox->getSelectionLength() > 0)
			{
				unsigned long ulSelectionEnd(mInputBox->getSelectionEndIndex());
				
				mInputBox->setText(mInputBox->getText() + ' ');
				mInputBox->setCaratIndex(ulSelectionEnd + 1);
				mInputBox->setSelection(mInputBox->getCaratIndex(), mInputBox->getCaratIndex());
			}
			else
			{
				const CEGUI::String consoleText(mInputBox->getText());
				
				mInputBox->setText(CEGUI::String(""));	
				pushMessage(("> " + consoleText).c_str());
				// run the command
				myBackend->runCommand(consoleText.c_str());
			}
		
			break;
		}
	}
	
	return true;
}


bool ConsoleWidget::consoleTextBox_Character(const CEGUI::EventArgs& args)
{

	const CEGUI::KeyEventArgs& keyargs = static_cast<const CEGUI::KeyEventArgs&>(args);
/*	std::stringstream tempString;
	tempString << std::string("CEGUI - CHARACTER:") << keyargs.codepoint;
	fprintf(stderr, tempString.str().c_str());*/
	return true;
}

}
