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

Shows chat bubbles over npc's heads when they say something.
The bubbles will disappear after a while (actually fade out) or when the player moves away. This can be set in the config file.

If the npc has a list of suggested responses these will be shown in a list of clickable buttons to the right.

@author Erik Hjortsberg
*/
class IngameChatWidget : public Widget {


	/**
	Holds the actual chat window and keeps track of fading, catching clicks etc.
	*/
	class ActiveChatWindow
	{
		public:
			/**
			
			*/
			ActiveChatWindow(CEGUI::Window* window, EmberPhysicalEntity* entity, CEGUI::WindowManager* windowManager);
			
			~ActiveChatWindow();

			/**
			
			*/
			void updateText(const std::string& line);
		
			inline float getElapsedTimeSinceLastUpdate() { return mElapsedTimeSinceLastUpdate;}
			
			/**
			increases the elapsed time with the supplied amount
			*/
			void increaseElapsedTime(float timeSlice);
			
			/**
			
			gets the entity the window belongs to
			*/
			EmberPhysicalEntity* getEntity();
			
			CEGUI::Window* getWindow();
			
			/**
			call this each frame to update the window
			*/
			void frameStarted( const Ogre::FrameEvent & event );

			
		protected:
			/**
			positions the window on top of the entity
			*/
			void placeWindowOnEntity();
			
			CEGUI::Window* mWindow;
			float mElapsedTimeSinceLastUpdate;
			EmberPhysicalEntity* mEntity;
			std::vector<CEGUI::Window*> mResponseTextWidgets;
			CEGUI::WindowManager* mWindowManager;
			
			
			bool buttonResponse_Click(const CEGUI::EventArgs& args);
		
	};

typedef std::map<std::string, ActiveChatWindow*> ActiveChatWindowMap;
public:

    ~IngameChatWidget();
	void buildWidget();
	virtual void frameStarted(const Ogre::FrameEvent & event);
	
protected:
	void appendIGChatLine(const std::string& line, EmberEntity* entity);
	//void placeWindowOnEntity( CEGUI::Window* window, EmberPhysicalEntity* entity);

		
	
	/**
	a map of all the active windows
	*/
	ActiveChatWindowMap mActiveChatWindows;
	
	//the length in seconds a window should be shown after it has been activated
	float timeShown;
	
	//how far away, in meters, the window should be visible
	float distanceShown;
	
	


};

};

#endif
