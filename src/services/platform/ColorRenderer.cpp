#include "ColorRenderer.h"

/**
 * Constructor for a flat solid color RectangleRenderer.
 */
dime::ColorRenderer::ColorRenderer(const Rectangle &rect, Uint8 red,
	Uint8 green, Uint8 blue) : RectangleRenderer(rect), myColor(Color(red, green, blue))
{
}

/**
 * Constructor for a flat solid color RectangleRenderer.
 */
dime::ColorRenderer::ColorRenderer(const Rectangle &rect, 
	const dime::Color &color) : RectangleRenderer(rect), myColor(color)
{
}

/**
 * Renders a flat color, not much error checking though.
 */
void dime::ColorRenderer::render(dime::DrawDevice *device)
{
    device->fillRect(&myRect.getSDL_Rect(), myColor);
	
	//TODO: What should be returned here?
}

dime::ColorRenderer::~ColorRenderer()
{
};