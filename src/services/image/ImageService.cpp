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

#include "ImageService.h"


namespace dime
{

    ImageService *ImageService::theInstance = NULL;

   SDL_Surface *ImageService::loadImage(std::string imageName)
    {
        SDL_SurfacePtr image;
        std::string newName;
        
		image = myImages[imageName];
		
		if (image.get() == NULL)
		{
			image.reset(IMG_Load(imageName.c_str()));

			std::list<std::string>::iterator cur = mySearchPaths.begin();
			std::list<std::string>::iterator last = mySearchPaths.end();
        
			while(image.get() == NULL && cur != last)
            {
                newName = (*cur) + imageName;
                image.reset(IMG_Load(newName.c_str()));
                ++cur;
            }
			if (image.get() != NULL) myImages[imageName] = image;
		}
        return image.get();
    }

} // namespace dime
