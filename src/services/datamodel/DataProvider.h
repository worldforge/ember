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

#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

// Include other headers of the current program here
#include "DataModel.h"

// Include library headers here

#include <string>
#include <vector>
#include <list>
#include <map>

// Include system headers here

namespace dime {

//======================================================================
// Short type macros
//======================================================================
/**
 * This class is what is used in OOP instead of  'void * cookie'
 */

class DataKey
{
public:
    DataKey() {}
	/**
	 * The 'virtual' in front of this destructor is the only
     * reason we need DataKey for (and to make a type browser
	 * becoming handy).
	 */					   
    virtual ~DataKey () {} 
};

template <class T>
class DataKeyImpl: public DataKey
{
private:
	T	myKey;
public:
	DataKeyImpl(const T & key) { myKey = key; }
	const T & getKey() { return myKey; }
	virtual ~DataKeyImpl() {} //This causes to free myKey

	static T& spec/*ialize*/(boost::shared_ptr<DataKey> ptr) 
	{		
		return static_cast<DataKeyImpl<T> *>(ptr.get())->myKey;
	}
	
	static boost::shared_ptr<DataKey> gen/*aralize*/(const T & t)
	{
		return boost::shared_ptr<DataKey>(static_cast<DataKey*>(new DataKeyImpl<T>(t))); 
	}
};

class InternDataConnection
{
public:
	DataSignal mySignal;
	DataType myTypes;
};

typedef std::list<InternDataConnection *> DataConnectionList;
typedef std::map<std::string, DataConnectionList *> DataConnectionMap;

/**
 * HINT: Not to be confused with IternDataConnection.
 * This class hides from the Data Model user what's actually going on 
 * with his connections. He can use it to remove Connections
 * once made.
 */

class DataConnection
{
private:
	std::string myPath;
	DataConnectionList::iterator myI;
	bool		myConnected;

public:
	
	DataConnection();
	DataConnection(std::string path,
				   DataConnectionList::iterator & i);

	DataConnection & operator=(const DataConnection & source);
	void disconnect();
};	


/**
 * (Nearly) abstract base class for all parts of the program hosting own subtrees of the data model.
 *
 * See http://www.worldforge.org/dev/eng/clients/dime/developer_doc/DataModelFW
 * for more info on the data model and the role of data model providers.
 * 
 * @author Tim Enderling
 */

class DataProvider
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

	DataConnectionMap	myConns;

	public: 

	DataProvider	  * myParent;
	PDataKey			myKey;
	std::string			mySubpath;
	std::string			myPath;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

	/**
	 * Returns a new DataKey to an arbitrary child of a Data Object given by its DataKey. As
	 * the ID of a child node, DataKey's cannot change till objects deletion.
	 * NULL is used for the root node of the hosted branch.
	 * The return value is NULL if the ID is unknown 
	 *
	 * If another Provider is responsible provider is filled with the responsible provider,
	 * the return value is though a valid DataKey for this provider. This special DataKey
	 * is also called AdoptionKey. If not otherwise stated the AdoptionKey can not be
	 * used for subsequent function calls to the interface.
	 *
	 * (Hence there are two Datakeys for adopted objects - since there's also NULL 
	 * for the adopted provider.)
	 */
	virtual PDataKey getChild(PDataKey parent, std::string ID, DataProvider *& provider) = 0;

	/**
	 * Returns the subpath of a given key. As an exception to the general rule
	 * AdoptionKeys are allowed to use as value for key.
	 */
	virtual std::string getSubpath(PDataKey key) = 0;


	virtual DataType getType(PDataKey key) = 0;
	virtual std::string getDescription(PDataKey key) = 0;
	virtual void setDescription(PDataKey key, std::string description) = 0;

	/**
	 * Getters and setters. Although getters and setters are only called by the DataModelService
	 * value changes are not automatically signaled from it.
	 * Setters are never called for DataObjects not having the CAN_SET_VALUE flag in it's type.
	 */
	/* only called for BOOLEAN or CUSTOM - type */
	virtual bool getBoolVal(PDataKey key) = 0;
	virtual void setBoolVal(PDataKey key, bool newValue) = 0;

	/* only called for INTEGER or CUSTOM - type */
	virtual int  getIntVal(PDataKey key) = 0;
	virtual void setIntVal(PDataKey key, int newValue) = 0;

	/* only called for FLOAT or CUSTOM - type */
	virtual float getFloatVal(PDataKey key) = 0;
	virtual void  setFloatVal(PDataKey key, float newValue) = 0;

	/* only called for STRING or CUSTOM - type */
	virtual std::string getStringVal(PDataKey key) = 0;
	virtual void        setStringVal(PDataKey key, const std::string & newValue) = 0;

	/**
	 * Adds a new child. If it should be handled by some
	 * other provider this has to be specified by a non-NULL value of DataProvider.
	 *
	 * HINT: The provider may or may not take the suggestedID as ID for the new DataObject.
	 * The caller finds the corrected ID after the call in suggestedID.
	 */
	virtual void addChild(PDataKey parent, std::string & suggestedID, 
					DataProvider * provider = NULL) = 0;
	
	/**
	 * Removes a Data Object given by it's DataKey.
	 */ 
	virtual void remove(PDataKey parent) = 0;

	/**
	 * Removes an adopted Data Provider (given by it's Adoption Key)
	 */
	virtual void removeAdopted(PDataKey adopted) = 0;

	virtual void getChilds(PDataKey parent, std::vector<std::string> & listOfChilds) = 0;

	//----------------------------------------------------------------------
    // Constructors

	DataProvider();

    //----------------------------------------------------------------------
    // Destructor

	virtual ~DataProvider();
    
	//----------------------------------------------------------------------
    // Getters

    //----------------------------------------------------------------------
    // Setters

    //----------------------------------------------------------------------
    // Other public methods
	void removeAllConnections(std::string subpath);
	void fireGeneralSignal(DataType event);
	DataConnection addConnection(std::string subpath, const DataSlot & slot, 
						DataType event);
	void removeConnection(std::string subpath, DataConnectionList::iterator & i);
	void fireSignal(std::string subpath, DataType event);

	static std::string makeSubpath(std::string parentSubpath, std::string ID);
	static std::string makePath(std::string parentPath, std::string ID);
	
    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:
							

}; // DataProvider




} // namespace dime

#endif
