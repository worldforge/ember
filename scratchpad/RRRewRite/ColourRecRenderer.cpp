#include "ColourRecRenderer.h"

/**
 * Constructor for a flat solid color RectangleRenderer.
 */
dime::ColourRecRenderer::ColourRecRenderer(const Rectangle &rect, Uint8 red,
	Uint8 green, Uint8 blue) :	myRect(rect), myColor(Color(red, green, blue))
{
}

/**
 * Constructor for a flat solid color RectangleRenderer.
 */
dime::ColourRecRenderer::ColourRecRenderer(const Rectangle &rect, 
	const dime::Color &color) : myRect(rect), myColor(color)
{
}

/**
 * Renders a flat color, not much error checking though.
 */
void dime::ColourRecRenderer::render(dime::DrawDevice *device)
{
    device->fillRect(&myRect.getSDL_Rect(), myColor);
	
	//TODO: What should be returned here?
}
