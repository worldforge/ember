#include "TextBox.h"

using namespace dime;

int TextBox::draw(DrawDevice *target)
{
	// Draw the background rect
	myBackRect.render(target);

	// Use fontservice to draw the text of the widget
	Label::draw(target);

	// Now draw the caret
	drawCaret(target);

	//TODO: what should be returned here?
	return 0;
}

bool TextBox::keyPress( KeyPressEvent *event)
 { 
    if (event->getState() == KeyPressEvent::PRESSED)
        {
            int i = 0;
            int w;
            Font::FontString fittedText;
            
            switch (event->getKey().getKey())
	      {
	      case SDLK_BACKSPACE:
                {
                    // FIXME: reinventing the wheel bigtime, use std::string::erase(index,len)
                    // rather than all that stupid substringing.
		  unsigned int len = myText.length();

                    if((len > 0)&&(myCaretPos>0))
                        {
                            myText.erase(myCaretPos-1,1);
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
        // FIXME: Don't pass the end of line if we do tack a space onto itn
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

void TextBox::drawCaret(DrawDevice* target)
{
  int height = 0, width = 0;
  // --- Calculate the location where to draw the caret  ---  

  // The fun bit is we're probably using a variable width font
  // so we ask for the size of the text from that font
  // TODO: the substr bit
  myFontRenderer->getFont()->sizeText(myFontRenderer->getText(), &width, &height);

  // Work out where on screen our text is starting
  // TODO: replace the +1 with + half the width of a space
  int x = Label::getRectangle().getX()+1+width;
  int y = Label::getRectangle().getY();

  // --- Draw it at calculated spot ---
  RectangleRenderer cursor(Rectangle(x,y,1,height),dime::Color(255,0,0));
  cursor.render(target);
}
