
#include "EventGenerator.h"

//---------------------------------------------------------------------------------------------------
// Event slots.
//---------------------------------------------------------------------------------------------------
void dime::EventGenerator::MouseMotion(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
{
    int x, y, button, event, loop = 0;
    x = mouse->getAxisPosition(0);
    y  = mouse->getAxisPosition(1);
    //myRootWidget.checkMouseEvent(coords);
    

    // Select mouse event destination:
    dime::Widget *eventDest  = NULL;
    
    // Get the widget the mouse is over:
    updatePointedWidget( x, y );
    
    if (mouseCaptureWidget) 
        {
            eventDest = mouseCaptureWidget;
        }
    else
        {
            eventDest = pointedWidget;
        }
    
    // Determine event type and redirect it:
    eventDest->mouseMove( x, y, pointedWidget );
    
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
    
    if (mouseCaptureWidget)
        {
            eventDest = mouseCaptureWidget;
        }
    
    else
        {
            eventDest = pointedWidget;
        }
    
    
    //eventDest->mouseUp( x, y, button, pointedWidget );
    //eventDest->mouseDown( x, y, button, pointedWidget );
    
}


void dime::EventGenerator::updatePointedWidget( int mx, int my ) {
    // Checks which widget the mouse pointer currently is in and
    // sets pointedWidget to point at it.  Calls mouseExit for the previous
    // pointedWidget and mouseEnter for the new one.
    
    if (!mainWidget) {
        pointedWidget = NULL;
        return;
    }
    
    // Check if pointedWidget changed:
    dime::Widget *p;// = mainWidget->getWidgetAt( mx, my );
    if (p != pointedWidget) {
        
        // Exit old widget object:
        if (pointedWidget) pointedWidget->mouseExit( mx, my, p );
        
        // Set default (desktop(?)) cursor if the mouse isn't captured
        // and no new widget is pointed at:
        /*if (!mouseCaptureWidget && !p) {
        //      if (style) set_mouse_sprite( style->mouseCursorPic );
        else set_mouse_sprite( NULL );
        }*/
        
        if (p) {
            // Enter new widgets object:
            p->mouseEnter( mx, my, p );
            
            // Set mouse cursor to that of the new pointed widget
            // (if it has one and the mouse isn't captured):
            /*if (!mouseCaptureWidget && p->getGuiStyle())
              set_mouse_sprite( p->getGuiStyle()->mouseCursorPic );
            */
        }
        
        pointedWidget = p;
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
      if (keyboardCaptureWidget) keyboardCaptureWidget->keyPress(k);
      else if (mainWidget) mainWidget->keyPress(k);  // or keyPressed(k) ?
      }*/
} // checkKeyboard

/*--------------------------------------------------------------------------*/
bool dime::EventGenerator::captureKeyboard( dime::Widget *widget ) {
// Captures keyboard input and directs it to the specified widget until
// releaseKeyboard is called.  Returns true on success, false if keyboard
// already captured.
  if (keyboardCaptureWidget == widget) return true;  // Already has it.
  if (keyboardCaptureWidget)        return false; // Somebody else has it.

  keyboardCaptureWidget = widget;
  return true;
} // captureKeyboard

/*--------------------------------------------------------------------------*/
void dime::EventGenerator::releaseKeyboard( dime::Widget *widget ) {
// Releases the keyboard if it is captured by widget,
// keyboard input again goes to the widget that has the focus.

  if (keyboardCaptureWidget == widget)
    keyboardCaptureWidget = NULL;
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
    
    if (mouseCaptureWidget) eventDest = mouseCaptureWidget;
    else                 eventDest = pointedWidget;
    
    //if (!eventDest) continue; // Read away the rest of the mouse events. (?)
    
    
    // Determine event type and redirect it:
    /*switch (event) {
      case TTGUI_MOUSE_UP:
      eventDest->mouseUp( x, y, button, pointedWidget );
      break;
      
      case TTGUI_MOUSE_DOWN:
      eventDest->mouseDown( x, y, button, pointedWidget );
      break;
      
      case TTGUI_MOUSE_MOVE:
      eventDest->mouseMove( x, y, pointedWidget );
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
    
    if (mouseCaptureWidget == widget) return true;  // Already has it.
    if (mouseCaptureWidget)        return false; // Somebody else has it.
    
    mouseCaptureWidget = widget;
    
    // Set mouse cursor to the default specified for the capturing widget,
    // or the default cursor if the widget doesn't have any.
    /*if (mouseCaptureWidget) {
      if (mouseCaptureWidget->getGuiStyle())
      set_mouse_sprite( mouseCaptureWidget->getGuiStyle()->mouseCursorPic );
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
    
    if (mouseCaptureWidget == widget) {
        mouseCaptureWidget = NULL;
        
        // Set mouse cursor to the default specified for the pointed widget,
        // or the default cursor if the widget doesn't have any.
        /*if (pointedWidget && pointedWidget->getGuiStyle())
          set_mouse_sprite( pointedWidget->getGuiStyle()->mouseCursorPic );
          else if (style)
          set_mouse_sprite( style->mouseCursorPic );
          else set_mouse_sprite( NULL );
        */
    }
    
    
} // releaseMouse

