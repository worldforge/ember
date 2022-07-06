//
// C++ Implementation: ConsoleAdapter
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ConsoleAdapter.h"
#include "framework/ConsoleBackend.h"
#include "framework/CommandHistory.h"
#include <CEGUI/widgets/Editbox.h>

namespace Ember {
namespace OgreView {

namespace Gui {

ConsoleAdapter::ConsoleAdapter(CEGUI::Editbox* inputBox)
: mInputBox(inputBox), mBackend(ConsoleBackend::getSingletonPtr()), mTabPressed(false), mSelected(0), mReturnKeyDown(false)
{
	mInputBox->subscribeEvent(CEGUI::Editbox::EventKeyUp, CEGUI::Event::Subscriber(&ConsoleAdapter::consoleInputBox_KeyUp, this)); 
	mInputBox->subscribeEvent(CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber(&ConsoleAdapter::consoleInputBox_KeyDown, this)); 
}


ConsoleAdapter::~ConsoleAdapter() = default;

bool ConsoleAdapter::consoleInputBox_KeyDown(const CEGUI::EventArgs& args)
{
	const auto& keyargs = dynamic_cast<const CEGUI::KeyEventArgs&>(args);
	if (keyargs.scancode == CEGUI::Key::Return || keyargs.scancode == CEGUI::Key::NumpadEnter) {
		mReturnKeyDown = true;
		return true;
	}
	return false;
}

bool ConsoleAdapter::consoleInputBox_KeyUp(const CEGUI::EventArgs& args)
{
	const auto& keyargs = dynamic_cast<const CEGUI::KeyEventArgs&>(args);
	
	if(keyargs.scancode != CEGUI::Key::Tab)
	{
		mTabPressed = false;
	}
	switch(keyargs.scancode)
	{
		case CEGUI::Key::ArrowUp:
		{
			if(mBackend->getHistory().getHistoryPosition() == 0)
			{
				mCommandLine = mInputBox->getText().c_str();
			}
			else
			{
				// we are not at the command line but in the history
				// => write back the editing
				mBackend->getHistory().changeHistory(mBackend->getHistory().getHistoryPosition(), mInputBox->getText().c_str());
			}
			mBackend->getHistory().moveBackwards();
			if(mBackend->getHistory().getHistoryPosition() != 0)
			{
				mInputBox->setText(mBackend->getHistory().getHistoryString());
			}
			
			return true;
		}
		case CEGUI::Key::ArrowDown:
		{
			if(mBackend->getHistory().getHistoryPosition() > 0)
			{
				mBackend->getHistory().changeHistory(mBackend->getHistory().getHistoryPosition(), mInputBox->getText().c_str());
				mBackend->getHistory().moveForwards();
				if(mBackend->getHistory().getHistoryPosition() == 0)
				{
					mInputBox->setText(mCommandLine);
				}
				else
				{
					mInputBox->setText(mBackend->getHistory().getHistoryString());
				}
			}
			
			return true;
		}
		case CEGUI::Key::Tab:
		{
			std::string sCommand(mInputBox->getText().c_str());
			
			// only process commands
			if(sCommand[0] != '/')
			{
				return true;
			}
			sCommand = sCommand.substr(1, mInputBox->getCaretIndex() - 1);
			if(mTabPressed)
			{
				const std::set< std::string > commands(mBackend->getPrefixes(sCommand));
				
				if(!commands.empty())
				{
					auto iCommand(commands.begin());
					std::string sMessage;
				
					mSelected = (mSelected + 1) % commands.size();
				
					int select(0);
				
					while(iCommand != commands.end())
					{
						if(select == mSelected)
						{
							std::string sCommandLine(mInputBox->getText().c_str());
						
							// compose the new command line: old text before the caret + selected command
							mInputBox->setText(sCommandLine.substr(0, mInputBox->getCaretIndex()) + iCommand->substr(mInputBox->getCaretIndex() - 1));
							mInputBox->setSelection(mInputBox->getCaretIndex(), 0xFFFFFFFF);
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
				
				if(commands.empty())
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
						mInputBox->setCaretIndex(0xFFFFFFFF);
					}
					else
					{
						//If there are multiple matches we need to find the lowest common denominator. We'll do this by iterating through all characters and then checking with all the possible commands if they match that prefix, until we get a false.
						auto iSelected(commands.begin());
						auto iCommand(commands.begin());
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
						mInputBox->setCaretIndex(sCommonPrefix.length() + 1);
						mInputBox->setSelection(sCommonPrefix.length() + 1, 0xFFFFFFFF);
					}
				}
			}
				
			return true;
		}
		case CEGUI::Key::Return:
		case CEGUI::Key::NumpadEnter:
		{
			if (mReturnKeyDown) {
				mReturnKeyDown = false;
				if(mInputBox->getSelectionLength() > 0)
				{
					auto ulSelectionEnd = mInputBox->getSelectionEndIndex();
					
					mInputBox->setText(mInputBox->getText() + ' ');
					mInputBox->setCaretIndex(ulSelectionEnd + 1);
					mInputBox->setSelection(mInputBox->getCaretIndex(), mInputBox->getCaretIndex());
				}
				else
				{
					const CEGUI::String consoleText(mInputBox->getText());
					
					mInputBox->setText("");
					// run the command
					mBackend->runCommand(consoleText.c_str());
					EventCommandExecuted.emit(consoleText.c_str());
				}
			}
		
			return true;
		}
		default:
		{
			break;
		}
	}
	
	return false;
}


}

}
}
