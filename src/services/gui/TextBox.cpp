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
            
            if(event->getKey().getKey() == SDLK_BACKSPACE)
                {
                    if(myText.length() > 1)
                        {
                            myText = myText.substr(0, myText.length()-1);
                        }
                    else if(myText.length() ==1)
                        {
                            myText = (Uint16*)"";
                        }
                    
                }
            else
                {
                    myText += event->getKey().getUnicode();
                }
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
