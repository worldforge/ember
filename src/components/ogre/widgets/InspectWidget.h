//
// C++ Interface: InspectWidget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef DIMEOGREINSPECTWIDGET_H
#define DIMEOGREINSPECTWIDGET_H


namespace EmberOgre {

class GUIManager;
class Widget;
/**
@author Erik Hjortsberg
*/
class InspectWidget : public Widget
{
public:

    InspectWidget();
	void buildWidget();
	virtual void frameStarted(const Ogre::FrameEvent & evt);
	
	const Ember::ConsoleCommandWrapper Inspect;
		
	
	/**
	 *    reimplement ConsoleObject::runCommand to catch the "inspect $entityid" command
	 * @param command 
	 * @param args 
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

protected:
	
	
	/**
	 *    Starts inspecting an entity
	 * @param entity 
	 */
	void startInspecting(EmberEntity* entity);
	
	CEGUI::Listbox* mChildList;
	CEGUI::StaticText* mInfo;
	
	/**
	 * Updated the entity information text
	 * @param entity 
	 */
	void showEntityInfo(EmberEntity* entity);
	
	void handleAction(const std::string& action, EmberEntity* entity);
	bool ShowOgreBoundingBox_Click(const CEGUI::EventArgs& args);
	bool ShowErisBoundingBox_Click(const CEGUI::EventArgs& args);
	bool MoveEntity_Click(const CEGUI::EventArgs& args);

	EmberEntity* mCurrentEntity;

	void Server_GotView(Eris::View* view);
	void View_EntityDeleted(Eris::Entity* entity);
	
	std::string mAttributeString;
	
	void updateAttributeString();
	
	void entity_Changed(const Eris::StringSet& attributes);

	std::string mAttributesString;
	
	sigc::connection mChangedConnection;
};

class AttributeTextBuilder
{
public:

	AttributeTextBuilder();
	
	std::string parseAttributes(const Eris::Entity::AttrMap& map);

	const std::stringstream& getText() const;

private:
	EmberEntity* mEntity;
		
	std::stringstream mMainText;
	int mLevel;
		
	void parseElement(const std::string& key, const Atlas::Message::Element& element);
	void parseElement(const Atlas::Message::Element& element);
		
	void parseString(const std::string& text);
	void parseNumber(float number);
	
	void parseString(const std::string& key, const std::string& text);
	void parseNumber(const std::string& key, float number);
	void parseList(const std::string& key, const Atlas::Message::ListType& list);
	void parseMap(const std::string& key, const Atlas::Message::MapType& map);
	
	void pad();
};

class IAttributeTextHandler
{
public:
	void handle(const Eris::Entity::AttrMap& attrMap);
};

};

#endif
