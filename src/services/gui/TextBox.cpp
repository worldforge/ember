#include "TextBox.h"

using namespace dime;

int TextBox::draw(DrawDevice *target)
{
	// Draw the background rect
	myBackRect.render(target);

	// Use fontservice to draw the text of the widget
	Label::draw(target);

	//TODO: what should be returned here?
	return 0;
}

bool TextBox::keyPress( KeyPressEvent *event)
 { 
    if (event->getState() == KeyPressEvent::PRESSED)
        {
            Font::FontString myText = Label::getText();
            int i = 0;
            int w;
            Font::FontString fittedText;
            
            switch (event->getKey().getKey())
	      {
	      case SDLK_BACKSPACE:
                {
		  unsigned int len = myText.length();

                    if(len > 1)
                        {
                            myText = myText.substr(0, myText.length()-1);
			    myCaretPos--;
                        }
                    else if(len ==1)
                        {
			  myText = (Uint16*)"";
			  //myText.clear(); Backed out because of lackings of
			  // certain versions of gcc's STL
			  myCaretPos--;
                        }
                    
                }
		break;
	      case SDLK_RETURN:
	      {
		// TODO: Accept input and either call default widget pressed
		//       or jump to next textbox in sequence
	      }
	      break;
	      case SDLK_TAB:
	      {
		// TODO: Pass focus to next control
	      }
	      break;
	      case SDLK_LEFT:
	      {
		// Move caret left
		myCaretPos--;
	      }
	      break;
	      case SDLK_RIGHT:
	      {
		// Move caret right
		myCaretPos++;
	      }
	      break;
	      case SDLK_HOME:
	      {
		// Move caret to beginning
		myCaretPos = 0;
	      }
	      break;
	      case SDLK_END:
	      {
		// Move caret to end
		// TODO: Verify if I got this right or if it should
		//       be length()-1
		myCaretPos = myText.length();
	      }
	      break;
	      default:
		{
		  // TODO: make this insert at Caret position
		  myText += event->getKey().getUnicode();
		  myCaretPos++;
		}
	      }

	    // TODO: redraw caret
            Label::setText(myText);
            Font *font = myFontRenderer->getFont();
            font->sizeText(myText.substr(i, myText.length()-i), &w, NULL);
            
            while(w > Widget::getRectangle().getWidth())
                {
                    ++i;
                    font->sizeText(myText.substr(i, myText.length()-i), &w, NULL);
                }
            fittedText = myText.substr(i, myText.length()-i);
            
            myFontRenderer->setText(fittedText);
        }
    return true;        
}
