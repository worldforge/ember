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


#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"

#include <framework/Service.h>
#include <services/logging/LoggingService.h>
#include <SDL/SDL.h>
#include <list>
#include <map>
#include <string>

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
public:
     
    //======================================================================
    // Private Variables
    //======================================================================
private:
    dime::LoggingService *myLog;
    std::list<std::string> mySearchPaths;
    std::map<std::string/*name*/, int/*size*/>myFonts;
    static FontService *theInstance;
    /* The FreeType font engine/library */
    FT_Library library;
    bool myInitialized;
    
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
    
    Service::Status start()
    {
		//TODO: What should be returned here?
        return Service::OK;
    }
    
    //----------------------------------------------------------------------
    // Other public methods
    Font *loadFontIndex(const std::string& fontName, int pointSize, int index);
    Font *loadFont(const std::string& fontName, int pointSize);
    void closeFont( dime::Font* font );
    void flushGlyph( dime::Glyph* glyph );
    void flushCache( Font* font );
    

private:
    
    /** Creates a new FontService using default values. */
    FontService() 
    {
        setName("Font Service");
        setDescription("Service for loading and caching images");
        addPath("./");
        addPath("../");
        addPath("./data/");
        FT_Error error;
        error = FT_Init_FreeType( &library );
        if ( error ) 
            {
                myInitialized = false;
            } 
        else 
            {
                myInitialized = true;
            }
        myLog = dime::LoggingService::getInstance();
        myLog->slog(__FILE__, __LINE__, LoggingService::INFO) 
            << "Started Font Service"<<ENDM;
    }

}; //FontService

} // namespace dime

#endif

