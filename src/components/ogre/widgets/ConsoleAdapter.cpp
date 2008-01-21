//
// C++ Implementation: ConsoleAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#include "ConsoleAdapter.h"
#include "framework/ConsoleBackend.h"
#include <elements/CEGUIEditbox.h>

namespace EmberOgre {

namespace Gui {

ConsoleAdapter::ConsoleAdapter(CEGUI::Editbox* inputBox)
: mInputBox(inputBox), mReturnKeyDown(false)
{
	mBackend = Ember::ConsoleBackend::getMainConsole();
	mInputBox->subscribeEvent(CEGUI::Editbox::EventKeyUp, CEGUI::Event::Subscriber(&ConsoleAdapter::consoleInputBox_KeyUp, this)); 
	mInputBox->subscribeEvent(CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber(&ConsoleAdapter::consoleInputBox_KeyDown, this)); 
}


ConsoleAdapter::~ConsoleAdapter()
{
}

bool ConsoleAdapter::consoleInputBox_KeyDown(const CEGUI::EventArgs& args)
{
	const CEGUI::KeyEventArgs& keyargs = static_cast<const CEGUI::KeyEventArgs&>(args);
	if (keyargs.scancode == CEGUI::Key::Return || keyargs.scancode == CEGUI::Key::NumpadEnter) {
		mReturnKeyDown = true;
	}
	return true;
}

bool ConsoleAdapter::consoleInputBox_KeyUp(const CEGUI::EventArgs& args)
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
			if(mBackend->getHistoryPosition() == 0)
			{
				mCommandLine = mInputBox->getText().c_str();
			}
			else
			{
				// we are not at the command line but in the history
				// => write back the editing
				mBackend->changeHistory(mBackend->getHistoryPosition(), mInputBox->getText().c_str());
			}
			mBackend->moveBackwards();
			if(mBackend->getHistoryPosition() != 0)
			{
				mInputBox->setText(mBackend->getHistoryString());
			}
			
			break;
		}
		case CEGUI::Key::ArrowDown:
		{
			if(mBackend->getHistoryPosition() > 0)
			{
				mBackend->changeHistory(mBackend->getHistoryPosition(), mInputBox->getText().c_str());
				mBackend->moveForwards();
				if(mBackend->getHistoryPosition() == 0)
				{
					mInputBox->setText(mCommandLine);
				}
				else
				{
					mInputBox->setText(mBackend->getHistoryString());
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
				const std::set< std::string > commands(mBackend->getPrefixes(sCommand));
				
				//std::cout << sCommand << std::endl;
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
					mBackend->pushMessage(sMessage);
				}
			}
			else
			{
				mTabPressed = true;
				mSelected = 0;
				
				const std::set< std::string > commands(mBackend->getPrefixes(sCommand));
				
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
			if (mReturnKeyDown) {
				mReturnKeyDown = false;
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
					mBackend->pushMessage(("> " + consoleText).c_str());
					// run the command
					mBackend->runCommand(consoleText.c_str());
					EventCommandExecuted.emit(consoleText.c_str());
				}
			}
		
			break;
		}
		default:
		{
			break;
		}
	}
	
	return true;
}


}

}
