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

#ifndef AUTHORINGHANDLER_H_
#define AUTHORINGHANDLER_H_

#include "components/ogre/IEntityVisitor.h"
#include <map>
#include <sigc++/trackable.h>

namespace Eris
{
class View;
class Entity;
}

namespace EmberOgre
{
class EmberEntity;

namespace Authoring
{
class AuthoringVisualization;

class AuthoringHandler : public virtual sigc::trackable, public IEntityVisitor
{
public:
	AuthoringHandler(Eris::View& view);
	virtual ~AuthoringHandler();

	void visit(EmberEntity& entity);

protected:

	typedef std::map<EmberEntity*, AuthoringVisualization*> VisualizationStore;

	VisualizationStore mVisualizations;

	void view_EntitySeen(Eris::Entity* entity);
	void view_EntityDeleted(Eris::Entity* entity);

	void createVisualizationForEntity(EmberEntity* entity);
	void createVisualizationsForExistingEntities(Eris::View& view);
};

}

}

#endif /* AUTHORINGHANDLER_H_ */
