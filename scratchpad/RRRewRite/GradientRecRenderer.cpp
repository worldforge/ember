#include "GradientRecRenderer.h"

/**
 * Constructor for a gradient filled RectangleRenderer
 */
dime::GradientRecRenderer::GradientRecRenderer(const Rectangle &rect, dime::Color topleft,
	dime::Color topright, dime::Color bottomleft, dime::Color bottomright) :
	myRect(rect),
	myColor(color1),
	myColor2(color2),
	myColor3(color3),
	myColor4(color4)
{
}

GradientRecRenderer(const Rectangle &rect, Color color1, Color color2, GradientType type)
{
	myRect = rect;
	myColor = color1;
	myColor2 = (type=HORIZONTAL)? color1 : color2;
	myColor3 = (type=HORIZONTAL)? color2 : color1;
	myColor4 = color2;
}

/**
 * Renders a gradient fill, blending a color from each corner.
 */
void dime::GridRecRenderer::render(dime::DrawDevice *device)
{
    device->drawGradient(&myRect.getSDL_Rect(), myColor, myColor2, myColor3, myColor4);

	//TODO: What should be returned here?
}