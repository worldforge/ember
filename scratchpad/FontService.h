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

#ifndef FONTSERVICE_H
#define FONTSERVICE_H

#include <framework/Service.h>
#include <services/platform/Color.h>
#include <services/logging/LoggingService.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <list>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

namespace dime {

/**
 * Font service
 *
 * @author Lakin Wecker [nikal]
 *
 * @see dime::Service
 */
class FontService : public Service
{
	//======================================================================
	// Inner Classes, Typedefs, and Enums
	//======================================================================
public:

	

    //======================================================================
    // Private Variables
    //======================================================================
private:

    std::list<std::string> mySearchPaths;
    std::map<std::string, TTF_Font *> myFonts;
	static FontService *theInstance;

    //----------------------------------------------------------------------
    // Constructors & Destructor
    
public:    
    
    /** Deletes a ConfigService instance. */
    ~FontService()
    {
    }
    
    //----------------------------------------------------------------------
    // Getters & Setters
    
    void addPath(std::string newPath)
    {
        mySearchPaths.push_back(newPath);
    }
    
    /**
    * because this class is a singleton you must use this method to obtain a copy.
    */
    static FontService *getInstance()
    {
        if(theInstance == NULL)
            {
                theInstance = new FontService();
            }
        
        return theInstance;
    }
    
    int start()
    {
		//TODO: What should be returned here?
        return 0;
    }
    
    //----------------------------------------------------------------------
    // Other public methods
    TTF_Font *loadFont(std::string fontName, int pointSize);

    SDL_Surface *renderFont(TTF_Font *font, char *text, Color fontColor);
    SDL_Surface *renderFont(TTF_Font *font, std::string text, Color fontColor);
    
                            

private:
    
    /** my instance of the LogginService */
    LoggingService *myLoggingService;
    
    
    /** Creates a new ImageService using default values. */
    FontService() 
    {
        setName("Font Service");
        setDescription("Service for easy loading and caching of fonts");
        addPath("./");
        addPath("../");
        addPath("./data/fonts");
        myLoggingService = LoggingService::getInstance();
        if(TTF_Init()) {
             myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::ERROR, 
                                   "Couldn't initialize the SDL_ttf library!\n");
             assert(-1);
        }
        
    }

}; //FontService

} // namespace dime

#endif

