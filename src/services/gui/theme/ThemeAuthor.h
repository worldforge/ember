/*
    Copyright (C) 2002  Lakin Wecker

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

#ifndef THEMEAUTHOR_H
#define THEMEAUTHOR_H

// Included headers from the current project

// Included custom library headers

// Included system headers


namespace dime {

/**
 * A class to hold the information about the author of a theme.
 *
 * @author Lakin Wecker
 *
 */
    class ThemeAuthor
    {
    private:
        std::string myUserName;
        std::string myFirstName;
        std::string myLastName;
        std::string myEmail;
        std::string myHomepage;
        std::string myDescription;
    public:
        ThemeAuthor( std::string userName, std::string firstName, 
                     std::string lastName, std::string email, 
                     std::string homepage, std::string description)
            : myUserName(userName), myFirstName(firstName),
              myLastName(lastName), myEmail(email),
              myHomepage(homepage), myDescription(description)
        {
        }
        
        ThemeAuthor()
            : myUserName(""), myFirstName(""),
              myLastName(""), myEmail(""),
              myHomepage(""), myDescription("")
        {
        }
                
        ~ThemeAuthor()
        {   
        }
        
        std::string getUserName()
        {
            return myUserName;
        }
        
        std::string getFirstName()
        {
            return myFirstName;
        }
        
        std::string getLastName()
        {
            return myLastName;
        }
        
        std::string getEmail()
        {
            return myEmail;
        }
        
        std::string getHomepage()
        {
            return myHomepage;
        }
        
        std::string getDescription()
        {
            return myDescription;
        }
         
        std::string setUserName(std::string userName)
        {
            myUserName = userName;
        }
           
        void setFirstName(std::string firstName)
        {
            myFirstName = firstName;
        }
        
        void setLastName(std::string lastName)
        {
            myLastName = lastName;
        }
        
        void setEmail(std::string email)
        {
            myEmail = email;
        }
        
        void setHomepage(std::string homepage)
        {
            myHomepage = homepage;
        }
        
        void setDescription(std::string description)
        {
            myDescription = description;
        }
        
        
    };//class ThemeAuthor
}//namespace dime


#endif
