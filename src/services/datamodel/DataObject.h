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

#ifndef DATAOBJECT_H
#define DATAOBJECT_H

// Include other headers of the current program here
#include "DataProvider.h"
#include "DataModel.h"

// Include library headers here

#include <boost/shared_ptr.hpp>

// Include system headers here
#include <string>
#include <list>
#include <vector>

namespace dime {
//@todo: Needs to use smartpointers

//======================================================================
// Short type macros
//======================================================================

const int CONVERT_BUFFER_SIZE  = 24;

class DataProvider;
class DataKey;
class DataConnection;

typedef boost::shared_ptr<DataObject> PDataObject;

/**
 * Class representing a node in the tree of the data model.
 *
 * This one and the DataService-class are the only classes used by data model users.
 * DataProvider should not be used by them!
 *
 * See http://www.worldforge.org/dev/eng/clients/dime/developer_doc/DataModelFW
 * for more info on the data model.
 * 
 * @author Tim Enderling
 */

class DataObject: public SigC::Object
{
    //======================================================================
    // Public Constants and enums
    //======================================================================
    public:	
 

	//======================================================================
    // Inner Classes and typedefs
    //======================================================================
	 
	public:

	/**
	 * List used for holding data object's childs.
	 */
	typedef std::vector<PDataObject> ChildList;

    //======================================================================
    // Private Constants
    //======================================================================
    private:

    //======================================================================
    // Private Variables
    //======================================================================
    private:
	DataProvider	  * myProvider;
	PDataKey			myKey;
	std::string			mySubpath;
	std::string			myPath;
	DataConnection		myConnection;


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    /**
     * returns the ID of the node.
     */
    std::string getID();
	
    /**
     * returns the destination of the link.
     */
    PDataObject getLinkDest();
	
    /**
     * returns the path to the node from the root node.
     */
    std::string getPath();
	
	/**
	 * returns the subpath within it's data model provider. Usually not needed by 
	 * Data Model users.
	 */
	std::string getSubpath();
	PDataKey getKey();

	/**
	 * returns the Data Object's data model provider. Usually not needed by 
	 * Data Model users.
	 */
	DataProvider * getProvider();

    /**
     * returns the type of the Data Object consisting of different flags described above
     */
    DataType getType();

    /**
     * returns the description of a Data Object.
     */
    std::string getDescription();
	void		setDescription(std::string description);

    /**
     * All of the following value getting and setting methods can also be applied
     * to Data Objects of other types. In this case the values are converted in some default way. Some
     * of the conversions don't work for the setting method.
     *
     * If the value of a Data Object is 0 or "" or it has no child nodes, it's boolean value is false,
     * otherwise true.
     *
     * Booleans are presented as -1 for TRUE and 0 for FALSE, if asked for integers. Floats
     * are rounded to get an integer and strings are parsed (some kind of atoi). Folders return the
     * number of child nodes as integer.
     *
     * Float convertions are performed in the same way as integer conversions.
     *
     * String conversion of boolean values are "TRUE" and "FALSE". Other string values are
     * some printf-like version of integers and floats and the ID of a folder's childs.
     */

    bool getBoolVal();
	int getIntVal();
	float  getFloatVal();
	std::string getStringVal();
	
	void setBoolVal(bool newValue);
	void setIntVal(int newValue);
    void  setFloatVal(float newValue);
    void  setStringVal(std::string newValue);

    /**
     * returns a direct/indirect child specified by the subpath.
     */
	PDataObject getChild(std::string subpath);

    /**
     * Fills a list with the subpath of all direct childs.
     */
	void getChildList(ChildList & childList);

    /**
     * returns a new Data Object added as child node. You can specify the new data object
	 * to be another provider's top node.
	 *
	 * If no suggestedID (= "") is given, an unused ID is generated automatically. 
	 * ATTENTION: Don't rely on the child to have the suggested ID. (That's why it's called
	 * suggested!)
     */
	PDataObject addChild(std::string suggestedID, DataProvider * provider);

    /**
     * returns a new Data Object added as child node. If linkTo is specified (non ""), 
	 * a link is added as new child Data Object. 
	 *
	 * If no suggestedID (= "") is given, an unused ID is generated automatically. 
	 * ATTENTION: Don't rely on the child to have the suggested ID. (That's why it's called
	 * suggested!)
     */
	PDataObject addChild(std::string suggestedID = "", std::string linkTo = "");

	/**
	 * removes a Data Object. If the object is a link, only the link will be removed.
	 *
	 * All links pointing to this child Data Object will be set to VOID-type. The same
	 * happens on the DataObject itself and all other DataObjects using it.
	 */
	void remove();
	
    /**
     * Adds a new connection to the signal. The returned DataConnection can be saved to
	 * remove the connection later. To disconnect just hold the DataConnection and call
	 * disconnect to it.
	 *
     * types are all type of events the connection should be used for,
	 * or'd together. This can also include the FIRE_ON_CHILD_EVENT. If so, all event of specified
	 * type concerning direct OR indirect child nodes of the node are forwarded to this
	 * connection, too.
	 */
	DataConnection addConnection(const DataSlot & slot, DataType types);

	/**
	 * Returns a new data object for the specified path. 
	 */
	static PDataObject getByPath(std::string path);

	/**
	 * Returns a new root data object for the specified path.
	 */
	static PDataObject getRoot(std::string path = "/");

	static void findObject(std::string & subpath,
						   DataProvider *& provider,
						   PDataKey & key);												

    //----------------------------------------------------------------------
    // Constructors
	DataObject(DataProvider * provider, 
						   PDataKey key,
						   std::string subpath,
						   std::string path);

	DataObject(std::string path);

	//----------------------------------------------------------------------
    // Destructor

	~DataObject();
	
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
	void onObjectDeletion(PDataObject object, DataType event);
	
    //======================================================================
    // Private Methods
    //======================================================================
    private:
	
	
}; // DataObject

} // namespace dime

#endif
