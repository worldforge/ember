#include "ColorRenderer.h"

/**
 * Constructor for a flat solid color Renderer.
 */
dime::ColorRenderer::ColorRenderer(const Rectangle &rect, Uint8 red,
	Uint8 green, Uint8 blue) : Renderer(rect), myColor(Color(red, green, blue))
{
}

/**
 * Constructor for a flat solid color Renderer.
 */
dime::ColorRenderer::ColorRenderer(const Rectangle &rect, 
	const dime::Color &color) : Renderer(rect), myColor(color)
{
}

/**
 * Renders a flat color, not much error checking though.
 */
void dime::ColorRenderer::render(dime::DrawDevice *device)
{
    device->fillRect(&myRect.getSDL_Rect(), myColor);
}

dime::ColorRenderer::~ColorRenderer()
{
};
