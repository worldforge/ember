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

#ifndef IMAGESERVICE_H
#define IMAGESERVICE_H

#include <framework/Service.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <list>
#include <map>
#include <string>

namespace dime {

/**
 * Image service
 *
 * @author Lakin Wecker [nikal]
 *
 * @see dime::Service
 */
class ImageService : public Service
{
    //======================================================================
    // Private Variables
    //======================================================================
private:

    std::list<std::string> mySearchPaths;
    std::map<std::string, SDL_Surface *> myImages;
    
    

    //----------------------------------------------------------------------
    // Constructors & Destructor
    
public:    
    
    /** Deletes a ConfigService instance. */
    ~ImageService()
    {
    }
    
    //----------------------------------------------------------------------
    // Getters & Setters
    
    void addPath(std::string newPath)
    {
        mySearchPaths.push_back(newPath);
    }
    
    /*
     * because this class is a singleton you must use this method to obtain a copy.
     */
    static ImageService *getInstance()
    {
        if(theInstance == NULL)
            {
                theInstance = new ImageService();
            }
        
        return theInstance;
    }
    
    int start()
    {
        
    }
    
    //----------------------------------------------------------------------
    // Methods
	
	SDL_Surface *loadImage(std::string imageName);
    

private:
    
    /** Creates a new ConfigService using default values. */
    ImageService() 
    {
        setName("Image Service");
        setDescription("Service for loading and caching images");
        addPath("./");
        addPath("../");
        addPath("./data/");
    }
    
    
    static ImageService *theInstance;

}; //ImageService

} // namespace dime

#endif

