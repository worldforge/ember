
#include "EventGenerator.h"

//---------------------------------------------------------------------------------------------------
// Event slots.
//---------------------------------------------------------------------------------------------------
void dime::EventGenerator::MouseMotion(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
{
    int x, y, button, event, loop = 0;
    x = mouse->getAxisPosition(0);
    y  = mouse->getAxisPosition(1);

    // Select mouse event destination:
    dime::Widget *eventDest  = NULL;
    
    // Get the widget the mouse is over:
    updatePointedWidget( x, y );
    
    if (myMouseCaptureWidget) 
        {
            eventDest = myMouseCaptureWidget;
        }
    else
        {
            eventDest = myPointedWidget;
        }
    
    // Determine event type and redirect it:
    if(eventDest)
        {
            eventDest->mouseMove( x, y, myPointedWidget );
        }
}

void dime::EventGenerator::MouseClick(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
{

    int x, y, button, event, loop = 0;
    x = mouse->getAxisPosition(0);
    y = mouse->getAxisPosition(1);
    //  dime::LoggingService::getInstance()->slog(__FILE__, __LINE__, dime::LoggingService::INFO) << "Mouse click of button " << key << " at (" << coords[0] << ", " << coords[1] << ") received" << ENDM;
    //myRootWidget.checkMouseEvent(coords);
    
    // Checks the mouse input queue and redirects mouse events to the main
    // widget or the widget that has captured the mouse.
    
    
    // Select mouse event destination:
    dime::Widget *eventDest  = NULL;
    
    // Get the widget the mouse is over:
    updatePointedWidget( x, y );
    
    if (myMouseCaptureWidget)
        {
            eventDest = myMouseCaptureWidget;
        }
    
    else
        {
            eventDest = myPointedWidget;
        }
    
    
    //eventDest->mouseUp( x, y, button, myPointedWidget );
    //eventDest->mouseDown( x, y, button, myPointedWidget );
    
}


void dime::EventGenerator::updatePointedWidget( int mx, int my ) {
    // Checks which widget the mouse pointer currently is in and
    // sets myPointedWidget to point at it.  Calls mouseExit for the previous
    // myPointedWidget and mouseEnter for the new one.
    
    if (!myRootWidget) {
        myPointedWidget = NULL;
        return;
    }
    
    // Check if myPointedWidget changed:
    dime::Widget *p = myRootWidget->getWidgetAt( mx, my );
    if (p != myPointedWidget) {
        // Exit old widget object:
        if (myPointedWidget) myPointedWidget->mouseExit( mx, my, p );
        
        // Set default (desktop(?)) cursor if the mouse isn't captured
        // and no new widget is pointed at:
        /*if (!myMouseCaptureWidget && !p) {
        //      if (style) set_mouse_sprite( style->mouseCursorPic );
        else set_mouse_sprite( NULL );
        }*/
        
        if (p) {
            // Enter new widgets object:
            p->mouseEnter( mx, my, p );
            
            // Set mouse cursor to that of the new pointed widget
            // (if it has one and the mouse isn't captured):
            /*if (!myMouseCaptureWidget && p->getGuiStyle())
              set_mouse_sprite( p->getGuiStyle()->mouseCursorPic );
            */
        }
        
        myPointedWidget = p;
    }
    
} // updatePointedWidget

/*--------------------------------------------------------------------------*/
/*----[ Keyboard events: ]--------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void dime::EventGenerator::handleKeyboardEvent() {
// Checks the keyboard input and redirects keyboard events to the main
// widget or the widget that has captured the keyboard.
    /*if (keypressed()) {
      int k = readkey();
      if (myKeyboardCaptureWidget) myKeyboardCaptureWidget->keyPress(k);
      else if (myRootWidget) myRootWidget->keyPress(k);  // or keyPressed(k) ?
      }*/
} // checkKeyboard

/*--------------------------------------------------------------------------*/
bool dime::EventGenerator::captureKeyboard( dime::Widget *widget ) {
// Captures keyboard input and directs it to the specified widget until
// releaseKeyboard is called.  Returns true on success, false if keyboard
// already captured.
  if (myKeyboardCaptureWidget == widget) return true;  // Already has it.
  if (myKeyboardCaptureWidget)        return false; // Somebody else has it.

  myKeyboardCaptureWidget = widget;
  return true;
} // captureKeyboard

/*--------------------------------------------------------------------------*/
void dime::EventGenerator::releaseKeyboard( dime::Widget *widget ) {
// Releases the keyboard if it is captured by widget,
// keyboard input again goes to the widget that has the focus.

  if (myKeyboardCaptureWidget == widget)
    myKeyboardCaptureWidget = NULL;
} // releaseKeyboard


/*--------------------------------------------------------------------------*/
/*----[ Mouse events: ]-----------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void dime::EventGenerator::handleMouseEvent(int x, int y) {
    // Checks the mouse input queue and redirects mouse events to the main
    // widget or the widget that has captured the mouse.
    /*
      int x, y, button, event, loop = 0;
      while ( loop < TTGUI_MAX_CHECKMOUSE_LOOP &&
      ttGetMouseEvent( x, y, button, event ) ) {
      loop++;
    */
    // Select mouse event destination:
    dime::Widget *eventDest  = NULL;
    
    // Get the widget the mouse is over:
    //updatePointedWidget( x, y );
    
    if (myMouseCaptureWidget) eventDest = myMouseCaptureWidget;
    else                 eventDest = myPointedWidget;
    
    //if (!eventDest) continue; // Read away the rest of the mouse events. (?)
    
    
    // Determine event type and redirect it:
    /*switch (event) {
      case TTGUI_MOUSE_UP:
      eventDest->mouseUp( x, y, button, myPointedWidget );
      break;
      
      case TTGUI_MOUSE_DOWN:
      eventDest->mouseDown( x, y, button, myPointedWidget );
      break;
      
      case TTGUI_MOUSE_MOVE:
      eventDest->mouseMove( x, y, myPointedWidget );
      break;
      
      default:
      break;
      } // end switch.
    */
    
} // checkMouse

/*--------------------------------------------------------------------------*/
bool dime::EventGenerator::captureMouse( dime::Widget *widget ) {
    // Captures mouse input and directs it to the specified widget until
    // releaseMouse is called.  Returns true on success, false if mouse
    // already captured.
    
    if (myMouseCaptureWidget == widget) return true;  // Already has it.
    if (myMouseCaptureWidget)        return false; // Somebody else has it.
    
    myMouseCaptureWidget = widget;
    
    // Set mouse cursor to the default specified for the capturing widget,
    // or the default cursor if the widget doesn't have any.
    /*if (myMouseCaptureWidget) {
      if (myMouseCaptureWidget->getGuiStyle())
      set_mouse_sprite( myMouseCaptureWidget->getGuiStyle()->mouseCursorPic );
      else if (style)
      set_mouse_sprite( style->mouseCursorPic );
      else set_mouse_sprite( NULL );
      }
    */
    
    return true;
} // captureMouse

/*--------------------------------------------------------------------------*/
void dime::EventGenerator::releaseMouse( dime::Widget *widget ) {
    // Releases the mouse if it was captured by widget,
    // mouse input again goes to the widget the mouse currently is in.
    
    if (myMouseCaptureWidget == widget) {
        myMouseCaptureWidget = NULL;
        
        // Set mouse cursor to the default specified for the pointed widget,
        // or the default cursor if the widget doesn't have any.
        /*if (myPointedWidget && myPointedWidget->getGuiStyle())
          set_mouse_sprite( myPointedWidget->getGuiStyle()->mouseCursorPic );
          else if (style)
          set_mouse_sprite( style->mouseCursorPic );
          else set_mouse_sprite( NULL );
        */
    }
    
    
} // releaseMouse

