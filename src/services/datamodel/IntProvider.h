/*
    Copyright (C) 2002  Tim Enderling

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

#ifndef INTPROVIDER_H
#define INTPROVIDER_H

// Include other headers of the current program here
#include "DataProvider.h"

// Include library headers here

// Include system headers here

namespace dime {

/**
 * Provider for a holding a single int.
 * 
 * @author Tim Enderling
 */

class IntProvider: public DataProvider, virtual public SigC::Object
{
    //======================================================================
    // Public Constants and enums
    //======================================================================
    public:	
  
	
	//======================================================================
    // Inner Classes and typedefs
    //======================================================================	 
	public:
	
    //======================================================================
    // Private Constants
    //======================================================================
    private:

    //======================================================================
    // Private Variables
    //======================================================================
	int		 myValue;
	DataType myPermissions; //determines whether the data model users
						    //can change the value or not/remove it our not, etc.
	std::string myDescription;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

	/**
	 * All documentation from the functions removed here to remove redundancy. 
	 * Look into DataProvider class instead.
	 */

	virtual PDataKey getChild(PDataKey parent, std::string ID, DataProvider *& provider);
	virtual std::string getSubpath(PDataKey key);
	virtual DataType getType(PDataKey key);
	virtual std::string getDescription(PDataKey key);
	virtual void setDescription(PDataKey key, std::string description);
	
	virtual bool getBoolVal(PDataKey key);
	virtual void setBoolVal(PDataKey key, bool newValue);
	virtual int  getIntVal(PDataKey key);
	virtual void setIntVal(PDataKey key, int newValue);
	virtual float getFloatVal(PDataKey key);
	virtual void  setFloatVal(PDataKey key, float newValue);
	virtual std::string getStringVal(PDataKey key);
	virtual void        setStringVal(PDataKey key, const std::string & newValue);

	virtual void addChild(PDataKey parent, std::string & suggestedID, 
					DataProvider * provider = NULL);
	virtual void remove(PDataKey child);
	virtual void removeAdopted(PDataKey adopted);
	virtual void getChilds(PDataKey parent, std::vector<std::string> & listOfChilds);

	int getValue();
	void setValue(int newValue);
	void setPermissions(DataType permissions);

	//----------------------------------------------------------------------
    // Constructors

	IntProvider(int initialValue, std::string description,
		DataType permissions = static_cast<DataType>(0));

    //----------------------------------------------------------------------
    // Destructor

	virtual ~IntProvider();
    
	//----------------------------------------------------------------------
    // Getters

    //----------------------------------------------------------------------
    // Setters

    //----------------------------------------------------------------------
    // Other public methods

    //======================================================================
    // Protected Methods
    //======================================================================
    protected:

    //======================================================================
    // Private Methods
    //======================================================================
    private:
							

}; // IntProvider


} // namespace dime

#endif
