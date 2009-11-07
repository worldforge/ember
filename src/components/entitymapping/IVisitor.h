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

#ifndef IVISITOR_H_
#define IVISITOR_H_
namespace Ember
{

namespace EntityMapping
{

namespace Actions
{
class Action;
}

namespace Matches
{
class MatchBase;
}

namespace Cases
{
class CaseBase;
}

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Interface for visitor classes intended to walk the entity mapping hierarchy.
 */
class IVisitor
{
public:
	/**
	 * @brief Dtor.
	 */
	virtual ~IVisitor()	{}

	virtual void visit(Actions::Action& action) = 0;
	virtual void visit(Matches::MatchBase& match) = 0;
	virtual void visit(Cases::CaseBase& caseBase) = 0;
};

}
}

#endif /* IVISITOR_H_ */
