/*
  Copyright (C) 2002  Lakin Wecker [nikal]

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "FontService.h"


namespace dime
{

    FontService *FontService::theInstance = NULL;
    
    TTF_Font *FontService::loadFont(std::string fontName, int pointSize)
    {
        std::string newName;
        return TTF_OpenFont(newName.c_str(), pointSize);
    }

    SDL_Surface *FontService::renderFont(TTF_Font *font, char *text, Color fontColor)
    {
        SDL_Color myColor;
        myColor.r = fontColor.getR();
        myColor.g = fontColor.getG();
        myColor.b = fontColor.getB();
        
        assert(font);
        assert(text);
        return TTF_RenderText_Solid(font, text, myColor);
    }

    
    SDL_Surface *FontService::renderFont(TTF_Font *font, std::string text, Color fontColor)
    {
        SDL_Color myColor;
        myColor.r = fontColor.getR();
        myColor.g = fontColor.getG();
        myColor.b = fontColor.getB();
        
        assert(font);
        return TTF_RenderText_Solid(font, text.c_str(), myColor);
    }

} // namespace dime
