/*
 Copyright (C) 2013 Erik Ogenvik

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

#ifndef ATLASPRESENTATIONBRIDGE_H_
#define ATLASPRESENTATIONBRIDGE_H_

#include <sstream>
#include <string>
#include <stack>
#include <Atlas/Bridge.h>

namespace Ember
{

/**
 * @brief A bridge which is meant to be used solely for presenting Element data.
 *
 */
class AtlasPresentationBridge: public Atlas::Bridge
{
public:
	explicit AtlasPresentationBridge(std::stringstream& stream);
	virtual ~AtlasPresentationBridge();

	virtual void streamBegin();
	virtual void streamMessage();
	virtual void streamEnd();

	virtual void mapMapItem(const std::string& name);
	virtual void mapListItem(const std::string& name);
	virtual void mapIntItem(const std::string& name, long);
	virtual void mapFloatItem(const std::string& name, double);
	virtual void mapStringItem(const std::string& name, const std::string&);
	virtual void mapEnd();

	virtual void listMapItem();
	virtual void listListItem();
	virtual void listIntItem(long);
	virtual void listFloatItem(double);
	virtual void listStringItem(const std::string&);
	virtual void listEnd();

private:

	void addPadding();

	void removePadding();

	std::string mPadding;

	std::stringstream& mStream;

	/**
	 * @brief Keeps track of the number of maps in lists.
	 *
	 * This is used to determine if we should print a separator to make it easier to see where a new map starts.
	 */
	std::stack<int> mMapsInList;
};

}
#endif /* ATLASPRESENTATIONBRIDGE_H_ */
