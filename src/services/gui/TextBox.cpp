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
            switch (event->getKey().getKey())
	      {
	      case SDLK_BACKSPACE:
                {
		  unsigned int len = myText.length();
		  assert(len>=myCaretPos);

                    if((len > 0)&&(myCaretPos>0))
                        {
                            myText.erase(myCaretPos-1,1);
                            myCaretPos--;
			}
		    if ((myRight==myLeft)&&(myLeft>0))
		      myLeft--;
		    rebuildLeft();
                }
		break;
	      case SDLK_DELETE:
		{
		  unsigned int len = myText.length();
		  assert(len>=myCaretPos);

                    if((len > 0)&&(myCaretPos<len))
                        {
                            myText.erase(myCaretPos,1);
                        }
		    rebuildLeft();
		}
		break;
	      case SDLK_RETURN:
	      {
		// TODO: Accept input and either call default widget pressed
		//       or jump to next textbox in sequence
		onEnter.emit(myText);
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
		  if (myCaretPos != 0)
		    myCaretPos--;
		  if (myCaretPos<myLeft)
		    {
          myLeft = myCaretPos;
		      rebuildLeft();
		    }
		}
	      break;
	      case SDLK_RIGHT:
		{
		  // Move caret right
		  if (myCaretPos!=myText.length())
		    myCaretPos++;
		  if (myCaretPos>myRight)
		    {
          myRight=myCaretPos;
		      rebuildRight();
		    }
		}
	      break;
	      case SDLK_HOME:
	      {
		// Move caret to beginning
		myCaretPos = 0;
    myLeft = 0;
		rebuildLeft();
	      }
	      break;
	      case SDLK_END:
	      {
		// Move caret to end
		myCaretPos = myText.length();
    myRight= myText.length()-1;
		rebuildRight();
	      }
	      break;
	      default:
		{
		  Uint16 keyIn = event->getKey().getUnicode();
		  myText.insert( myCaretPos, 1, keyIn );
		  myCaretPos++;
		  if (myCaretPos>myRight)
		    {
          myRight=myCaretPos;
		      rebuildRight();
		    }
      else
       rebuildLeft();
		}
	      }
        }
    return true;
}

// TODO: REVIEW THIS CODE AND CRITIC THE ALGO
void TextBox::rebuildLeft()
{
  cout<<"RebuildLeft"<<endl;
  /* Start with zero chars and keep on expanding till we get too big.
     Then when we get too big go back to the one before that wasn't too
     big.  That's our target!
  */
  Font::FontString fittedText;
  int i = 0;
  int w = 0;

  Font *font = myFontRenderer->getFont();
            
  while(w <= Label::getRectangle().getWidth())
    {
      ++i;
      if (myLeft+i>myText.length()) break;
      font->sizeText(myText.substr(myLeft, myLeft+i), &w, NULL);
    }
  fittedText = myText.substr(myLeft, myLeft+i-1);
            
  myFontRenderer->setText(fittedText);
  //CHECKME: is this correct?
  myRight = myLeft+i-1;
  cout<<"myLeft:"<<myLeft<<" myRight:"<<myRight<<endl;
}

// TODO: REVIEW THIS CODE AND CRITIC THE ALGO
void TextBox::rebuildRight()
{
  cout<<"Rebuildright"<<endl;
  /* Start with zero chars and keep on expanding till we get too big.
     Then when we get too big go back to the one before that wasn't too
     big.  That's our target!
  */
  Font::FontString fittedText;
  int i = 0;
  int w = 0;

  Font *font = myFontRenderer->getFont();
  while(w <= Label::getRectangle().getWidth())
    {
      ++i;
      // Why this? Because we are using unsigned ints
      if ( myRight-i+1 == 0 ) break;
      font->sizeText(myText.substr(myRight-i, myRight), &w, NULL);
    }
  fittedText = myText.substr(myRight-i+1, myRight);
            
  myFontRenderer->setText(fittedText);
  myLeft = myRight-i+1;
  cout<<"myLeft:"<<myLeft<<" myRight:"<<myRight<<endl;
}

void TextBox::drawCaret(DrawDevice* target)
{
  int height = 0, width = 0;
  // --- Calculate the location where to draw the caret  ---  

  // The fun bit is we're probably using a variable width font
  // so we ask for the size of the text from that font
  Font::FontString pretext; // Hmm next substr don't work
  pretext=myFontRenderer->getText()/*.substr(myLeft,myCaretPos)*/;
  myFontRenderer->getFont()->sizeText(pretext, &width, &height);

  // Work out where on screen our text is starting
  // TODO: replace the +1 with + half the width of a space
  int x = Label::getRectangle().getX()+1+width;
  int y = Label::getRectangle().getY();

  // --- Draw it at calculated spot ---
  RectangleRenderer cursor(Rectangle(x,y,1,height),dime::Color(255,0,0));
  cursor.render(target);
}
