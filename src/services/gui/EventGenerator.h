/*
    Copyright (C) 2002  Lakin Wecker

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

// Include project specific headers here
#include "widget/Widget.h"
#include "MouseMotionEvent.h"
#include "MouseButtonEvent.h"
#include "KeyPressEvent.h"
#include <services/logging/LoggingService.h>
#include <services/input/InputService.h>

// Include the signal system in headers file.
#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/signal.h>
#endif


namespace dime 
{
    class Widget;
    
    class EventGenerator : public SigC::Object
    {
        
    public:
        // Start all event handlers here.
        EventGenerator(Widget *rootWidget) 
            :  myRootWidget(rootWidget)
        { 
            myInputService = dime::InputService::getInstance();

            dime::InputDevice *mouse = myInputService->getInputDevice(dime::InputDevice::MOUSE);
			dime::InputDevice *keyboard = myInputService->getInputDevice(dime::InputDevice::KEYBOARD);

            myInputService->addInputMapping(new dime::InputMapping(mouse, SigC::slot(*this,&dime::EventGenerator::MouseMotion)));
            myInputService->addInputMapping(new dime::InputMapping(mouse, mouse, SDLK_LEFT_MB, SDLK_RIGHT_MB, KMOD_NONE, dime::InputMapping::InputSignalType(InputMapping::KEY_PRESSED | InputMapping::KEY_RELEASED | InputMapping::EVENT_OCCURED), SigC::slot(*this,&dime::EventGenerator::MouseClick)));

			// Add a new input mapping for the keyboard for keys going from 0 to SDLK_PAGEDOWN numbered on the keyboard, fire the event on press and release, tie to EventGenerator::KeyboardPress
			// repetition delay 500ms.
			RepetitionDevice * repDevice = new RepetitionDevice(1000, 200);
			InputMapping * keyBoardMapping = new dime::InputMapping(repDevice, keyboard, SDLK_FIRST, SDLK_PAGEDOWN, KMOD_NONE, dime::InputMapping::InputSignalType(InputMapping::KEY_PRESSED | InputMapping::KEY_RELEASED | InputMapping::EVENT_OCCURED), SigC::slot(*this, &dime::EventGenerator::KeyboardPress) );
			myInputService->addInputMapping(keyBoardMapping);
			keyBoardMapping->getSignal()->connect(SigC::slot(*repDevice, &dime::RepetitionDevice::switchOn) );
		
			// NULL out this lot
			myPointedWidget = NULL;
			myMouseCaptureWidget = NULL;
			myKeyboardCaptureWidget= NULL;
            
        };
        
    private:
        
        //---------------------------------------------------------------------------------------------------
        //  The methods that we connect to the InputService to listen to events.
        //---------------------------------------------------------------------------------------------------
        	
	/**
	 * Passes mouse motion events down to widget tree
	 */
	void MouseMotion(InputDevice *mouse, InputDevice * otherDevice, const DimeKey &key, InputMapping::InputSignalType signaltype);
	
	/**
	 * Passes mouse motion events down to widget tree
	 */
	void MouseClick(InputDevice * otherDevice, InputDevice *mouse, const DimeKey &key, InputMapping::InputSignalType signaltype);

	/**
	 * Passes keyboard events down to widget tree
	 */
	void KeyboardPress(InputDevice * otherDevice, InputDevice *keyboard, const DimeKey &key, InputMapping::InputSignalType signaltype);

        //---------------------------------------------------------------------------------------------------
        // Keyboard events: 
        //---------------------------------------------------------------------------------------------------

        /**
         * Checks the keyboard input and redirects keyboard events to the main
         * widget or the widget that has captured the keyboard.
         */
        virtual void handleKeyboardEvent();
                
        /**
         * Captures keyboard input and directs it to the specified widget until
         * releaseKeyboard is called.  Returns true on success, false if keyboard
         * already captured.
         */
        virtual bool captureKeyboard( Widget *widget );
        
        /**
         * Releases the keyboard if it is captured by widget,
         * keyboard input again goes to the widget that has the focus.
         */
        virtual void releaseKeyboard( Widget *widget );
       
	/**
	 * Gives a widget focus, calls loseFocus of oldFocusWidget,
	 * calls gainFocus on newFocusWidget
	 */
	virtual void giveWidgetFocus( Widget *widget );
	
        //---------------------------------------------------------------------------------------------------
        // Mouse events
        //---------------------------------------------------------------------------------------------------

        /**
         * Checks the mouse input queue and redirects mouse events to the main
         * widget or the widget that has captured the mouse.
         */
        virtual void handleMouseEvent(int x, int y);
        /**
         * Captures mouse input and directs it to the specified widget until
         * releaseMouse is called.  Returns true on success, false if mouse
         * already captured.
         */
        virtual bool captureMouse( Widget *widget );

        /**
         * Releases the mouse if it was captured by widget,
         * mouse input again goes to the widget the mouse currently is in.
         */
        virtual void releaseMouse( Widget *widget );

        /**
         * Checks which widget the mouse pointer currently is in and
         * sets pointedWidget to point at it.  Calls mouseExit for the previous
         * pointedWidget and mouseEnter for the new one.
         */
        virtual void updatePointedWidget( int mx, int my, InputDevice *device);
        
        //---------------------------------------------------------------------------------------------------
        // Style
        //---------------------------------------------------------------------------------------------------
        /**
         * Sets the hardwired default style for gui colors, cursorblinkrate, etc.
         */
        //virtual void setDefaultStyle();
        
        //---------------------------------------------------------------------------------------------------
        // Accessors
        //---------------------------------------------------------------------------------------------------
        virtual Widget *getMainWidget() 
	  { 
            return myRootWidget; 
	  }
        virtual Widget *getMouseCaptureWidget()    
	  { 
            return myMouseCaptureWidget; 
	  }
        virtual Widget *getKeyboardCaptureWidget() 
	  { 
            return myKeyboardCaptureWidget; 
	  }
        virtual Widget *getPointedWidget()         
	  { 
            return myPointedWidget;      
	  }
        virtual Widget *getFocusedWidget() 
	  { 
	    return myFocusedWidget;      
	  }
        virtual void setFocusedWidget(Widget *widget) 
	  { 
	    myFocusedWidget = widget;  
	  }
	
        
	
    protected:
        
        Widget *myRootWidget;
        Widget *myMouseCaptureWidget;  // NULL if mouse not captured, else all mouse
        // events are sent to the specified widget.
        Widget *myKeyboardCaptureWidget;  // NULL if keyboard not captured, else all
        // keyboard events are sent to the specified widget.
        // (Usefull for example when configuring input keys
        //  in games, pressing TAB won't shift away from the
        //  input gizmo.)
        Widget *myPointedWidget;   // The widget the mouse pointer currently is in.
        Widget *myFocusedWidget;   // The currently focused widget.
        InputService *myInputService;
        
        
    }; // end class EventGenerator
    
}// end namespace dime




#endif
