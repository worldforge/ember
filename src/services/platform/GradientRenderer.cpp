#include "GradientRenderer.h"

/**
 * Constructor for a gradient filled RectangleRenderer
 */
dime::GradientRenderer::GradientRenderer(const Rectangle &rect, dime::Color topleft,
	dime::Color topright, dime::Color bottomleft, dime::Color bottomright) :
	RectangleRenderer(rect),
	myColor(topleft),
	myColor2(topright),
	myColor3(bottomleft),
	myColor4(bottomright)
{
}

dime::GradientRenderer::GradientRenderer(const dime::Rectangle &rect, dime::Color color1, dime::Color color2, dime::GradientRenderer::GradientStyle style)
{
	myRect = rect;
	myColor = color1;
	myColor2 = (style=HORIZONTAL)? color1 : color2;
	myColor3 = (style=HORIZONTAL)? color2 : color1;
	myColor4 = color2;
}

/**
 * Renders a gradient fill, blending a color from each corner.
 */
void dime::GradientRenderer::render(dime::DrawDevice *device)
{
    device->drawGradient(&myRect.getSDL_Rect(), myColor, myColor2, myColor3, myColor4);

	//TODO: What should be returned here?
}