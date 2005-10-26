//
// C++ Implementation: ChatWidget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
//
//
#include "Widget.h"
#include "../GUIManager.h"
#include "ChatWidget.h"
#include "../EmberEntity.h"

namespace EmberOgre {

//template<>  WidgetLoader WidgetLoaderHolder<ChatWidget>::loader("ChatWidget", &createWidgetInstance);



ChatWidget::~ChatWidget()
{
}

void ChatWidget::buildWidget()
{
	loadMainSheet("ChatWidget.xml", "Chat/");
	
	mChatTextBox = static_cast<CEGUI::MultiLineEditbox*>(getWindow("TextBox"));
	
	mGuiManager->AppendIGChatLine.connect(sigc::mem_fun(*this, &ChatWidget::appendIGChatLine));
	mGuiManager->AppendOOGChatLine.connect(sigc::mem_fun(*this, &ChatWidget::appendOOGChatLine));
	
	registerConsoleVisibilityToggleCommand("chat");
	enableCloseButton();

}

void ChatWidget::appendOOGChatLine(const std::string& line, EmberEntity* entity) 
{
	// Add the name of the entity to the appended line
	std::string message = "{";
	message.append(entity->getName());
	message.append("} ");
	message.append(line);
	
	// Add the message to the chat box
	CEGUI::String current = mChatTextBox->getText();
	message.append("\n");
	message.append(current.c_str());
	mChatTextBox->setText(CEGUI::String(message));
}

void ChatWidget::appendIGChatLine(const std::string& line, EmberEntity* entity)
{
	// Add the name of the entity to the appended line
	std::string message = "<";
	message.append(entity->getName());
	message.append("> ");
	message.append(line);

	// Add the message to the chat box
	CEGUI::String current = mChatTextBox->getText();
	message.append("\n");
	message.append(current.c_str());
	mChatTextBox->setText(CEGUI::String(message));
}

};
