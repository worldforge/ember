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

#ifndef AUTHORINFO_H
#define AUTHORINFO_H

// Included headers from the current project

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Holds basic information about a theme author.
 *
 * Holds basic information about a theme author: username(nickname),
 * first name, last name, home page, email, and a description.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Lakin Wecker aka nikal
 *
 * 
 *
 * @see dime::Theme
 * @see dime::WidgetStyle
 *
 */

class AuthorInfo
{
    
    //======================================================================
    // Private Variables
    //======================================================================/
    private:

	std::string myContribution;
	std::string myUserName;
	std::string myFirstName;
        std::string myLastName;
        std::string myEmail;
        std::string myHomepage;
        std::string myDescription;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

	/**
	 * Creates a new AuthorInfo using default values
	 */
	AuthorInfo(std::string contribution, std::string userName,
			   std::string firstName = "", std::string lastName = "",
			   std::string email = "", std::string homepage = "",
			   std::string desc = "")
			   : myContribution(contribution), myUserName(userName),
			   	 myFirstName(firstName), myLastName(lastName),
			   	 myEmail(email), myHomepage(homepage), myDescription(desc)
    {
    }

    /**
     * Copy constructor.
     */
    AuthorInfo( const AuthorInfo &source )
    {
        // Use assignment operator to do the copy
     	*this = source;
    }


    /**
     * Assignment operator.
     */
    AuthorInfo &operator= ( const AuthorInfo &source )
    {
        // Copy fields from source class to this class here.
		myContribution = source.getContribution();
		myUserName = source.getUserName();
		myFirstName = source.getFirstName();
		myLastName = source.getLastName();
		myEmail = source.getEmail();
		myHomePage = source.getHomepage();
		myDescription = source.getDescription();
        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a AuthorInfo instance.
     */
    virtual ~AuthorInfo ()
    {
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Gets the value of Description of this AuthorInfo
     */
    std::string getDescription() const
    {
        return myDescription;
    }

    /**
     * Gets the value of FirstName of this AuthorInfo
     */
    std::string getFirstName() const
    {
        return myFirstName;
    }

    /**
     * Gets the value of UserName of this AuthorInfo
     */
    std::string getUserName() const
    {
        return myUserName;
    }

    /**
     * Gets the value of Contribution of this AuthorInfo
     */
    std::string getContribution() const
    {
        return myContribution;
    }

    /**
     * Gets the value of Email of this AuthorInfo
     */
    std::string getEmail() const
    {
        return myEmail;
    }

    /**
     * Gets the value of Homepage of this AuthorInfo
     */
    std::string getHomepage() const
    {
        return myHomepage;
    }

    /**
     * Gets the value of LastName of this AuthorInfo
     */
    std::string getLastName() const
    {
        return myLastName;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of Description of this AuthorInfo
     */
    void setDescription( std::string description )
    {
        myDescription = description ;
    }

    /**
     * Sets the value of FirstName of this AuthorInfo
     */
    void setFirstName( std::string firstName )
    {
        myFirstName = firstName ;
    }

    /**
     * Sets the value of UserName of this AuthorInfo
     */
    void setUserName( std::string userName )
    {
        myUserName = userName ;
    }

    /**
     * Sets the value of Contribution of this AuthorInfo
     */
    void setContribution( std::string contribution )
    {
        myContribution = contribution ;
    }

    /**
     * Sets the value of Email of this AuthorInfo
     */
    void setEmail( std::string email )
    {
        myEmail = email ;
    }

    /**
     * Sets the value of Homepage of this AuthorInfo
     */
    void setHomepage( std::string homepage )
    {
        myHomepage = homepage ;
    }

    /**
     * Sets the value of LastName of this AuthorInfo
     */
    void setLastName( std::string lastName )
    {
        myLastName = lastName ;
    }


    


}; // End of AuthorInfo

} // End of dime namespace

#endif
