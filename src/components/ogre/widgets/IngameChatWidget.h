//
// C++ Interface: IntegratedChatWidget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBEROGREINTEGRATEDCHATWIDGET_H
#define EMBEROGREINTEGRATEDCHATWIDGET_H

namespace EmberOgre {

class GUIManager;
class Widget;
class EmberPhysicalEntity;

/**
@author Erik Hjortsberg
*/
class IngameChatWidget : public Widget {


	class ActiveChatWindow
	{
		public:
			CEGUI::Window* window;
			float elapsedTimeSinceLastUpdate;
			EmberPhysicalEntity* entity;
			std::vector<CEGUI::Window*> responseTextWidgets;
			
			void appendIGChatLine(const std::string& line);
			CEGUI::WindowManager* windowManager;
			bool buttonResponse_Click(const CEGUI::EventArgs& args);
		
	};

typedef std::map<std::string, ActiveChatWindow> ActiveChatWindowMap;
public:
    IngameChatWidget(GUIManager* guiManager);

    ~IngameChatWidget();
	void buildWidget();
	virtual void frameStarted(const Ogre::FrameEvent & event);
	
protected:
	void appendIGChatLine(const std::string& line, EmberEntity* entity);
	void placeWindowOnEntity( CEGUI::Window* window, EmberPhysicalEntity* entity);

		
	
	ActiveChatWindowMap mActiveChatWindows;
	
	//the length in seconds a window should be shown after it has been activated
	float activeTime;
	
	


};

};

#endif
