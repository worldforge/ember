#include "BorderRenderer.h"
#include "ColorRenderer.h"

/**
 * Constructor for a BorderRenderer
 */
dime::BorderRenderer::BorderRenderer(const Rectangle &rect,
	unsigned int width, const Renderer *prototype) : Renderer(rect),
	myLines(8), myWidth(width)
{
	setBorderContents(prototype);
	generateBorder();
}


/**
 * Generates the border's Renderers based on a prototype.
 */
void dime::BorderRenderer::setBorderContents(const Renderer *prototype)
{
  const ColorRenderer* colorprototype = dynamic_cast<const ColorRenderer *> (prototype);
  if (colorprototype != NULL)
    {
      //it's a ColorRenderer
      // TODO: Optional 3d Bevel effect. Rounded corners - tricky, May need custom bitmaprenderers.
      for (unsigned int i = 0; i < 8; i++)
	{
	  if (myLines[i] != NULL)	delete myLines[i];
	  myLines[i] = new ColorRenderer();
	  ColorRenderer* colorline = static_cast<ColorRenderer *>(myLines[i]);
	  colorline->setColor(colorprototype->getColor());
	}
    }
	
  /*GradientRenderer *gradientprototype = dynamic_cast<GradientRenderer *> (prototype);
    if (gradientprototype != NULL)
    {
    //it's a GradientRenderer
    for (unsigned int i = 0; i < 8; i++)
    {
    if (myLines[i] != NULL)	delete myLines[i];
    myLines[i] = new GradientRenderer();
    }
		
    GradientRenderer* gradientline;
    gradientline = static_cast<GradientRenderer *>(myLines[0]);
    gradientline->Gradient(gradientprototype->);

    }*/
	
}

void dime::BorderRenderer::generateBorder()
{
  if (!checkLines()) return;
	
  int x = myRect.getX();
  int y = myRect.getY();
  int w = myRect.getWidth();
  int h = myRect.getHeight();
	
  myLines[0]->setRect(Rectangle(x - myWidth, y - myWidth, myWidth, myWidth));
  myLines[1]->setRect(Rectangle(x, y - myWidth, w, myWidth));
  myLines[2]->setRect(Rectangle(x + w, y - myWidth, myWidth, myWidth));
  myLines[3]->setRect(Rectangle(x + w, y, myWidth, h));
  myLines[4]->setRect(Rectangle(x + w, y + h, myWidth, myWidth));
  myLines[5]->setRect(Rectangle(x, y + h, w, myWidth));
  myLines[6]->setRect(Rectangle(x - myWidth, y + h, myWidth, myWidth));
  myLines[7]->setRect(Rectangle(x - myWidth, y, myWidth, h));
}

/**
 * Changes the width of the border.
 */
void dime::BorderRenderer::setWidth(unsigned int width)
{
  myWidth = width;
  generateBorder();
}

/**
 * Resizes the border.
 */
void dime::BorderRenderer::setRect(const Rectangle &rect)
{
  myRect = rect;
  generateBorder();
}

/**
 * Renders the border.
 */
void dime::BorderRenderer::render(dime::DrawDevice *device)
{
  for (unsigned int i = 0; i < 8; i++)
    {
      myLines[i]->render(device);
    }
}
