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
    

    ImageService::~ImageService()
    {
    }

    void ImageService::addPath(const std::string& newPath) 
    {
        mySearchPaths.push_back(newPath);    
    }

    ImageService *ImageService::getInstance()
    {
      if(theInstance == NULL)
	{
	  theInstance = new ImageService();
	}
        
      return theInstance;
    }
     
    Service::Status ImageService::start()
    {
        setStatus(Service::OK);
        return Service::OK;
    }


    ImageService *ImageService::theInstance = NULL;

    SDL_Surface *ImageService::loadImage(const std::string& imageName)
    {
        SDL_Surface *image;
        std::string newName;

        image = myImages[imageName];

        if (!image)
        {
            image = IMG_Load( imageName.c_str() );

            std::list<std::string>::iterator cur = mySearchPaths.begin();
            std::list<std::string>::iterator last = mySearchPaths.end();

            while( !image && cur != last )
            {
                newName = (*cur) + imageName;
                image = IMG_Load(newName.c_str());
                ++cur;
            }
            if ( image ) myImages[imageName] = image;
        }
        return image;
    }

    //Private methods
    ImageService::ImageService()
    {
        setName("Image Service");
        setDescription("Service for loading and caching images");
        addPath("./");
        addPath("../");
        addPath("./data/");
    }

} // namespace dime
