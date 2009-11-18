/*
 Copyright (C) 2009 Erik Hjortsberg

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

#ifndef RAWTYPEINFOREPOSITORY_H_
#define RAWTYPEINFOREPOSITORY_H_

#include <Atlas/Objects/ObjectsFwd.h>
#include <sigc++/trackable.h>
#include <map>
#include <string>

namespace Ember
{

class ServerService;

}

namespace EmberOgre
{
namespace Authoring
{

class RawTypeInfoRepository: public virtual sigc::trackable
{
public:
	RawTypeInfoRepository(Ember::ServerService& serverService);
	virtual ~RawTypeInfoRepository();

	Atlas::Objects::Root getRawTypeData(const std::string& typeName) const;

private:
	typedef std::map<std::string, Atlas::Objects::Root> RawTypeDataStore;

	RawTypeDataStore mRawTypeData;

	void setupConnections(Ember::ServerService& serverService);

	void server_ReceivedObject(const Atlas::Objects::Root& message);

	void parseAndSaveTypeData(const Atlas::Objects::Root& message);
};

}
}

#endif /* RAWTYPEINFOREPOSITORY_H_ */
